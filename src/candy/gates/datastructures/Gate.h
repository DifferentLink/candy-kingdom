#ifndef CANDY_GATE_H
#define CANDY_GATE_H

#include <vector>
#include "candy/core/SolverTypes.h"
#include "TupleNotation.h"

using namespace std;

class Gate {
private:
    vector<unsigned int> inVariables;
    TupleNotation formula;
    int outLiteral;

public:
    Gate(const vector<unsigned int> &inVariables, const TupleNotation &formula, int outLiteral) : inVariables(
            inVariables), formula(formula), outLiteral(outLiteral) {};

    static Gate getNullGate() {
        return Gate({ }, TupleNotation::getEmptyFormula(), 0);
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
#endif //CANDY_GATE_H
