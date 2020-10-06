#ifndef CANDY_GATE_H
#define CANDY_GATE_H

#include <vector>
#include <set>
#include "candy/core/SolverTypes.h"
#include "TupleNotation.h"

using namespace std;

namespace Recognition {
class Gate {
private:
    vector<unsigned int> inVariables;
    TupleNotation formula;
    int outLiteral;

public:
    Gate(const TupleNotation &formula, int outLiteral) {
        set<unsigned int, greater<unsigned int>> inVars;
        for (const auto& clause : formula.getFormula()) {
            for (const auto& lit : clause) inVars.insert(abs(lit));
        }
        inVariables = vector<unsigned int>(inVars.begin(), inVars.end());
    }

    static Gate getNullGate() {
        return Recognition::Gate(TupleNotation::getEmptyFormula(), 0);
    }

    static bool isNullGate(const Gate& gate) {
        return gate.inVariables.empty() && TupleNotation::isEmpty(gate.formula) && gate.outLiteral == 0;
    }

    const vector<unsigned int> &getInVariables() const {
        return inVariables;
    }

    const TupleNotation &getFormula() const {
        return formula;
    }

    int getOutLiteral() const {
        return outLiteral;
    }
};
}
#endif //CANDY_GATE_H
