#include <unordered_map>
#include <vector>

#include "candy/core/clauses/Clause.h"
#include "candy/core/clauses/ClauseDatabase.h"
#include "candy/core/Trail.h"
#include "candy/core/propagate/Propagate.h"
#include "candy/core/Certificate.h"
#include "candy/mtl/Stamp.h"
#include "candy/frontend/CLIOptions.h"

namespace Candy {

template <class TPropagate> 
class Subsumption { 
private:
    ClauseDatabase& clause_db;
    Trail& trail;
    TPropagate& propagator;
    Certificate& certificate;

    const uint16_t subsumption_lim;   // Do not check if subsumption against a clause larger than this. 0 means no limit.

public:         
    Subsumption(ClauseDatabase& clause_db_, Trail& trail_, TPropagate& propagator_, Certificate& certificate_) : 
        clause_db(clause_db_),
        trail(trail_),
        propagator(propagator_),
        certificate(certificate_),
        subsumption_lim(SubsumptionOptions::opt_subsumption_lim),
        queue(),
        abstractions(),
        nSubsumed(0),
        nStrengthened(0)
    {}

    std::vector<const Clause*> queue;
    std::unordered_map<const Clause*, uint64_t> abstractions;

    unsigned int nSubsumed;
    unsigned int nStrengthened;

    void attach(const Clause* clause) {
        if (abstractions.count(clause) == 0) {
            uint64_t abstraction = 0;
            for (Lit lit : *clause) {
                abstraction |= 1ull << (var(lit) % 64);
            }
            abstractions[clause] = abstraction;
            if (subsumption_lim == 0 || clause->size() < subsumption_lim) {
                queue.push_back(clause);
            }
        }
    }

    void clear() {
        queue.clear();
        abstractions.clear();
    }

    bool subsume();
    
}; 

template <class TPropagate> 
bool Subsumption<TPropagate>::subsume() {
    assert(trail.decisionLevel() == 0);
    
    nSubsumed = 0;
    nStrengthened = 0;
    
    for (const Clause* clause : clause_db) {
        attach(clause);
    }

    sort(queue.begin(), queue.end(), [](const Clause* c1, const Clause* c2) { return c1->size() > c2->size(); });
    
    while (queue.size() > 0) {
        const Clause* clause = queue.back();
        queue.pop_back();

        if (clause->isDeleted()) continue;
        
        // Find best variable to scan:
        Var best = var(*std::min_element(clause->begin(), clause->end(), [this] (Lit l1, Lit l2) {
            return clause_db.numOccurences(var(l1)) < clause_db.numOccurences(var(l2));
        }));

        // Search all candidates:
        const uint64_t clause_abstraction = abstractions[clause];
        const std::vector<Clause*> occurences = clause_db.copyOccurences(best);
        for (const Clause* occurence : occurences) {
            if (occurence != clause && ((clause_abstraction & ~abstractions[occurence]) == 0)) {
                Lit l = clause->subsumes(*occurence);

                if (l != lit_Error) {
                    if (clause->isLearnt() && !occurence->isLearnt()) {// in case of inprocessing: recreate persistent
                        Clause* persistent = clause_db.persistClause((Clause*)clause);
                        abstractions[persistent]=clause_abstraction;
                        abstractions.erase(clause);
                    }

                    if (l == lit_Undef) { // remove:
                        Statistics::getInstance().solverSubsumedInc();
                        nSubsumed++;
                        certificate.removed(occurence->begin(), occurence->end());
                        clause_db.removeClause((Clause*)occurence);
                    }
                    else { // strengthen:
                        Statistics::getInstance().solverDeletedInc();
                        nStrengthened++;
                        certificate.strengthened(occurence->begin(), occurence->end(), ~l);
                        
                        Clause* new_clause = clause_db.strengthenClause((Clause*)occurence, ~l);
                        if (new_clause->size() == 0) {
                            return false;
                        }
                        else {
                            attach(new_clause);
                            if (new_clause->size() == 1) {
                                if (!trail.newFact(new_clause->first()) || propagator.propagate() != nullptr) {
                                    return false; 
                                }
                            } 
                        }
                    }
                    abstractions.erase(occurence);
                }
            }
        }
    }

    propagator.clear();
    for (Clause* clause : clause_db) {
        if (clause->size() > 2) {
            propagator.attachClause(clause);
        }
    }
    clear();

    return true;
}

}