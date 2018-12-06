#include <deque>
#include <unordered_map>
#include <vector>

#include "candy/core/Clause.h"
#include "candy/core/ClauseDatabase.h"
#include "candy/core/Trail.h"
#include "candy/core/propagate/Propagate.h"
#include "candy/core/Certificate.h"
#include "candy/mtl/Stamp.h"
#include "candy/frontend/CLIOptions.h"

namespace Candy {

template <class TPropagate> class Subsumption { 

private:
    ClauseDatabase& clause_db;
    Trail& trail;
    TPropagate& propagator;
    Certificate& certificate;

    Stamp<uint8_t> touched;
    uint32_t n_touched;

public:         
    Subsumption(ClauseDatabase& clause_db_, Trail& trail_, TPropagate& propagator_, Certificate& certificate_) : 
        clause_db(clause_db_),
        trail(trail_),
        propagator(propagator_),
        certificate(certificate_),
        touched(),
        n_touched(0),
        reduced_literals(),
        subsumption_lim(SubsumptionOptions::opt_subsumption_lim),
        subsumption_queue(),
        subsumption_queue_contains(),
        abstraction(),
        bwdsub_assigns(0)
    {}

    std::vector<Lit> reduced_literals;

    uint16_t subsumption_lim;   // Do not check if subsumption against a clause larger than this. 0 means no limit.

    std::deque<Clause*> subsumption_queue;
    std::unordered_map<Clause*, char> subsumption_queue_contains;
    std::unordered_map<const Clause*, uint64_t> abstraction;
    uint32_t bwdsub_assigns;

    void subsumptionQueueProtectedPush(Clause* clause) {
        if (!subsumption_queue_contains[clause]) {
            subsumption_queue.push_back(clause);
            subsumption_queue_contains[clause] = true;
        }
    }

    Clause* subsumptionQueueProtectedPop() {
        Clause* clause = subsumption_queue.front();
        subsumption_queue.pop_front();
        subsumption_queue_contains[clause] = false;
        return clause;
    }

    void gatherTouchedClauses() {
        if (n_touched == 0) {
            return;
        }
        
        for (unsigned int i = 0; i < touched.size(); i++) {
            if (touched[i]) {
                const std::vector<Clause*>& cs = clause_db.getOccurenceList(i);
                for (Clause* c : cs) {
                    if (!c->isDeleted()) {
                        subsumptionQueueProtectedPush(c);
                    }
                }
            }
        }

        touched.clear();
        n_touched = 0;
    }

    inline void touch(Var v) {
        n_touched++;
        touched.set(v);
    }

    inline bool hasTouchedClauses() {
        return n_touched > 0;
    }

    void attach(Clause* clause) {
        subsumption_queue.push_back(clause);
        subsumption_queue_contains[clause] = true;
        calcAbstraction(clause);
    }

    void init(size_t nVars) {
        touched.grow(nVars);
        touched.clear();
        for (Clause* clause : clause_db) {
            attach(clause);
        }
    }

    void clear() {
        touched.clear();
        n_touched = 0;
        subsumption_queue.clear();
        abstraction.clear();
    }

    void calcAbstraction(const Clause* clause) {
        uint64_t clause_abstraction = 0;
        for (Lit lit : *clause) {
            clause_abstraction |= 1ull << (var(lit) % 64);
        }
        abstraction[clause] = clause_abstraction;
    }

    bool strengthenClause(Clause* cr, Lit l);
    bool backwardSubsumptionCheck();

};

template <class TPropagate> bool Subsumption<TPropagate>::strengthenClause(Clause* clause, Lit l) { 
    assert(trail.decisionLevel() == 0);
    
    subsumptionQueueProtectedPush(clause);

    reduced_literals.push_back(l);
    propagator.detachClause(clause);
    clause_db.strengthenClause(clause, l);
    certificate.added(((const Clause*)clause)->begin(), ((const Clause*)clause)->end());
    
    if (clause->size() == 1) {
        reduced_literals.push_back(clause->first());
        return trail.newFact(clause->first()) && propagator.propagate() == nullptr;
    }
    else {
        propagator.attachClause(clause);
        calcAbstraction(clause);
        return true;
    }
}

template <class TPropagate> bool Subsumption<TPropagate>::backwardSubsumptionCheck() {
    assert(trail.decisionLevel() == 0);

    Clause bwdsub_tmpunit({ lit_Undef });
    
    while (subsumption_queue.size() > 0 || bwdsub_assigns < trail.size()) {
        // Check top-level assignments by creating a dummy clause and placing it in the queue:
        if (subsumption_queue.size() == 0 && bwdsub_assigns < trail.size()) {
            Lit l = trail[bwdsub_assigns++];
            bwdsub_tmpunit = Clause({l});
            abstraction[&bwdsub_tmpunit] = 1ull << (var(l) % 64);
            subsumption_queue.push_back(&bwdsub_tmpunit);
        }

        const Clause* clause = subsumptionQueueProtectedPop();
        
        if (clause->isDeleted()) {
            continue;
        }
        
        assert(clause->size() > 1 || trail.value(clause->first()) == l_True); // Unit-clauses should have been propagated before this point.
        
        // Find best variable to scan:
        Var best = var(*std::min_element(clause->begin(), clause->end(), [this] (Lit l1, Lit l2) {
            return clause_db.getOccurenceList(var(l1)).size() < clause_db.getOccurenceList(var(l2)).size();
        }));

        // Search all candidates:
        const std::vector<Clause*>& cs = clause_db.getOccurenceList(best);
        for (unsigned int i = 0; i < cs.size(); i++) {
            const Clause* csi = cs[i];
            if (csi != clause && (subsumption_lim == 0 || csi->size() < subsumption_lim)) {
                if ((abstraction[clause] & ~abstraction[csi]) != 0) continue;

                Lit l = clause->subsumes(*csi);

                if (l == lit_Undef) {
                    Statistics::getInstance().solverSubsumedInc();
                    certificate.removed(csi->begin(), csi->end());
                    propagator.detachClause(csi);
                    if (trail.locked(csi)) {
                        trail.vardata[var(csi->first())].reason = nullptr;
                    }
                    clause_db.removeClause((Clause*)csi);
                    reduced_literals.insert(reduced_literals.end(), csi->begin(), csi->end());
                }
                else if (l != lit_Error) {
                    Statistics::getInstance().solverDeletedInc();
                    // this might modifiy occurs ...
                    if (!strengthenClause((Clause*)csi, ~l)) {
                        return false;
                    }
                    // ... occurs modified, so check candidate at index i again:
                    if (var(l) == best) {
                        i--;
                    }
                }
            }
        }
    }

    return true;
}

}