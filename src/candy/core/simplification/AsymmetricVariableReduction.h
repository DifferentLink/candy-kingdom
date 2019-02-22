/*************************************************************************************************
Candy -- Copyright (c) 2015-2019, Markus Iser, KIT - Karlsruhe Institute of Technology

Candy sources are based on Glucose which is based on MiniSat (see former copyrights below). 
Permissions and copyrights of Candy are exactly the same as Glucose and Minisat (see below).


--------------- Former Glucose Copyrights

 Glucose -- Copyright (c) 2009-2014, Gilles Audemard, Laurent Simon
                                CRIL - Univ. Artois, France
                                LRI  - Univ. Paris Sud, France (2009-2013)
                                Labri - Univ. Bordeaux, France

Glucose sources are based on MiniSat (see below MiniSat copyrights). Permissions and copyrights of
Glucose (sources until 2013, Glucose 3.0, single core) are exactly the same as Minisat on which it 
is based on. (see below).


--------------- Original Minisat Copyrights

Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************/

#ifndef _VARIABLE_REDUCTION_H
#define _VARIABLE_REDUCTION_H

#include <vector> 

#include "candy/core/clauses/ClauseDatabase.h"
#include "candy/core/clauses/Clause.h"
#include "candy/core/Trail.h"
#include "candy/utils/Options.h"

namespace Candy {
  
template <class TPropagate> 
class AsymmetricVariableReduction {
private:
    ClauseDatabase& clause_db;
    Trail& trail;
    TPropagate& propagator;

    const bool active;

    unsigned int nVars;
    
    bool reduce(Var variable) {
        const std::vector<Clause*> occurences = clause_db.copyOccurences(variable);
        for (const Clause* clause : occurences) if (!trail.satisfies(*clause)) {
            trail.newDecisionLevel();

            Lit l = lit_Undef;
            for (Lit lit : *clause) {
                if (var(lit) != variable && !trail.defines(lit)) {
                    trail.decide(~lit);
                } else {
                    l = lit;
                }
            }
            assert(l != lit_Undef);

            bool conflict = (propagator.propagate() != nullptr);
            trail.backtrack(0);
            
            if (conflict) {
                nStrengthened++;
                Clause* new_clause = clause_db.strengthenClause((Clause*)clause, l);

                if (clause->size() > 2) {
                    propagator.detachClause(clause);
                }
                if (new_clause->size() > 2) {
                    propagator.attachClause(new_clause);
                }
                else {
                    if (new_clause->size() == 0) {
                        return false;
                    }
                    if (new_clause->size() == 1) {
                        if (!trail.fact(new_clause->first()) || propagator.propagate() != nullptr) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

public:
    unsigned int nStrengthened;

    AsymmetricVariableReduction(ClauseDatabase& clause_db_, Trail& trail_, TPropagate& propagator_) : 
        clause_db(clause_db_),
        trail(trail_),
        propagator(propagator_),
        active(VariableEliminationOptions::opt_use_asymm), 
        nVars(0),
        nStrengthened(0)
    { }

    unsigned int nTouched() {
        return nStrengthened; 
    }

    inline void grow(size_t size) {
        if (size > nVars) {
            nVars = size;
        }
    }

    bool reduce() {
        nStrengthened = 0;

        if (!active) {
            return true;
        }

        for (Var variable = 0; variable < (Var)nVars; variable++) {
            if (!trail.defines(mkLit(variable, false))) {
                assert(trail.decisionLevel() == 0);
                if (!reduce(variable)) {
                    return false;
                }
            }
        }

        return true;
    }

};

}

#endif