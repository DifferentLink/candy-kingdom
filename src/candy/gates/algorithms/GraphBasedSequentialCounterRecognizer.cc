#include "GraphBasedSequentialCounterRecognizer.h"
#include "Unificator.h"

static const GateVertex& findFirstMatching(vector<vector<int>> pattern,
                                           BooleanCircuit& circuit,
                                           map<string, unsigned int> eta) {
    TupleNotation formula(pattern);
    for (const GateVertex& gate : circuit.getGates()) {
        //if (Unificator::unify(formula, gate.getGate().formula)) return gate;
    }
    return GateVertex::getNullVertex();
}

StructuralFormula GraphBasedSequentialCounterRecognizer::recognizeLTSEQ_n_1(TupleNotation& formula,
                                                                            BooleanCircuit& circuit,
                                                                            map<string, unsigned int> eta) {
    vector<GateVertex> gamma;
    GateVertex baseVertex = findFirstMatching({{-1, 2}}, circuit, eta);
    if (GateVertex::isNull(baseVertex)) return StructuralFormula::getEmptyStructuralFormula();
    eta.insert({"x_1", Unificator::var(baseVertex.getGate().getFormula()[0])});
    eta.insert({"s_1_1", Unificator::var(baseVertex.getGate().getFormula()[1])});
    gamma.push_back(baseVertex);

    unsigned int n = 2;
    GateVertex& currentVertex = baseVertex;
    GateVertex nullVertex = GateVertex::getNullVertex();
    GateVertex& stepVertex = nullVertex;

    do {
        stepVertex = nullVertex;
        for (auto& w : circuit.getOutEdges(currentVertex)) {
            if (w.getGate().getFormula().size() != 2) break;
            //if (Unificator::unify()) {
            //    stepVertex = w;
            //}
        }
    } while (!GateVertex::isNull(stepVertex));

    TupleNotation encoding(findNonGateClausesOfLTSEQ_n_1());
    return StructuralFormula{ encoding, gamma };
}

const TupleNotation &GraphBasedSequentialCounterRecognizer::findNonGateClausesOfLTSEQ_n_1() {
    TupleNotation formula((vector<vector<int>>) {{}});
    return formula;
}
