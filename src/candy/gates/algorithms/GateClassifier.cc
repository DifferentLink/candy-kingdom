
#include "GateClassifier.h"

bool hasFixedClausesSize(const TupleNotation& formula, const unsigned int size) {
    for (const auto& clause : formula.getFormula()) if (clause.size() != size) return false;
    return true;
}

GateClassifier::characteristicFunction GateClassifier::patternCheck(const TupleGate &gate) {
    if (gate.notMono) {
        if (gate.bwd.size() == 1 && hasFixedClausesSize(gate.fwd, 2)) {
            return cAND;
        } else if (gate.fwd.size() == 1 && hasFixedClausesSize(gate.bwd, 2)) {
            return cOR;
        } else if (gate.fwd.size() == 1 && gate.bwd.size() == 1
                    && gate.fwd.clauseAt(0).size() == 1 && gate.bwd.clauseAt(0).size() == 1) {
            return cEQUIV;
        } else if (gate.bwd.size() == 2 && gate.fwd.size() == 2 && gate.inp.size() == 2) {
            return cXOR;
        }
    } else {
        if (hasFixedClausesSize(gate.fwd, 2)) {
            return cHALFAND;
        } else if (gate.fwd.size() == 1) {
            return cHALFOR;
        } else if (gate.fwd.size() == 2 && gate.fwd.clauseAt(0).size() == 3 && gate.fwd.clauseAt(1).size() == 2) {
            return cCONSTRAINTGATE;
        }
    }
    return cUNDEF;
}

inline GateClassifier::characteristicFunction GateClassifier::classify(const TupleGate &gate) {
    return patternCheck(gate);
}

void GateClassifier::classify() {
    classification.resize(circuit->numGates(), GateClassifier::cUNDEF);
    for (const GateVertex& gate : circuit->getGates()) {
        classification.at(gate.getId() - 1) = classify(gate.getGate());
    }
}

vector<GateClassifier::characteristicFunction> GateClassifier::getResult() {
    return GateClassifier::classification;
}

GateClassifier::characteristicFunction GateClassifier::getResult(const GateVertex &gate) {
    return classification.at(gate.getId() - 1);
}
