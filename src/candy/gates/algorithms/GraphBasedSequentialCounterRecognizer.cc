#include "GraphBasedSequentialCounterRecognizer.h"
#include "Unificator.h"

static const GateVertex& findFirstMatching(const vector<string>& pattern,
                                           const BooleanCircuit& circuit,
                                           map<string, unsigned int>& eta) {
    for (const GateVertex& gate : circuit.getGates()) {
        auto& gateFormula = gate.getGate().getFormula();
        if (gateFormula.getFormula().size() != 2) continue;
        if (Unificator::unifyClauseL2(pattern, gateFormula.clauseAt(0), eta)) {

        }
    }
    return BooleanCircuit::nullVertex;
}

/**
 * Search the given formula for the existence of a clause defined over fixed literals.
 * @param formula the formula to search
 * @param pattern the clause
 * @param eta
 * @return
 */
const vector<int>& findFullyDefinedClause(const vector<string>& clause,
                                          const TupleNotation& formula,
                                          map<string, unsigned int>& eta) {
    for (const auto& fclause : formula.getFormula()) {
        if (Unificator::unifyClauseL2(clause, fclause, eta)) return fclause;
    }
    return BooleanCircuit::emptyClause;
}

vector<vector<int>> findNonGateClausesOfLTSEQ_n_1(const TupleNotation& formula,
                                                  map<string, unsigned int>& eta,
                                                  unsigned int n) {
    vector<vector<int>> nonGateClauses;
    string nots_i_1;
    string notx_ip;
    for (unsigned int i = 2; i < n - 1; i++) {
        nots_i_1 = "-s_" + to_string(i) + "_1";
        notx_ip = "-x_" + to_string(i + 1);
        if (!Unificator::isDefined(eta, notx_ip) || !Unificator::isDefined(eta, nots_i_1)) {
            return BooleanCircuit::emptyFormula.getFormula();
        }
        nonGateClauses.push_back(findFullyDefinedClause({ notx_ip,  }, formula, eta));
    }
    return nonGateClauses;
}

StructuralFormula GraphBasedSequentialCounterRecognizer::recognizeLTSEQ_n_1(TupleNotation& formula,
                                                                            BooleanCircuit& circuit,
                                                                            map<string, unsigned int> eta) {
    vector<GateVertex> gamma;
    unsigned int i = 1;
    string s_im_1;
    string s_i_1 = "s_1_1";
    string x_i = "x_i";
    GateVertex baseVertex = findFirstMatching({ "-x_1", "s_1_1" }, circuit, eta);
    if (GateVertex::isNull(baseVertex)) return StructuralFormula::getEmptyStructuralFormula();
    gamma.push_back(baseVertex);

    i++;
    GateVertex& currentVertex = baseVertex;
    GateVertex null = BooleanCircuit::nullVertex;
    GateVertex& stepVertex = null;

    do {
        stepVertex = nullVertex;
        s_im_1 = "s_" + to_string(i - 1) + "_1";
        s_i_1 =  "s_" + to_string(i) + "_1";
        x_i =    "x_" + to_string(i);
        for (auto& w : circuit.getOutEdges(currentVertex)) {
            const TupleNotation &gateFormula = w.getGate().getFormula();
            if (gateFormula.size() != 2) continue;
            auto clause1 = gateFormula.clauseAt(0);
            auto clause2 = gateFormula.clauseAt(1);
            if (clause1.size() != 2 || clause2.size() != 2) continue;
            if (!Unificator::isDefined(eta, clause1.at(0))) swap(clause1, clause2); // todo: does this swap work?
            map<string, unsigned int> eta_1 = eta;
            if (Unificator::unifyClauseL2({ "-" + s_im_1, s_i_1 }, clause1, eta)) {
                if (Unificator::unifyClauseL2({"-" + x_i, s_i_1}, clause2, eta)) {
                    stepVertex = w;
                    gamma.push_back(stepVertex);
                    i++;
                    break;
                }
            }
            eta = eta_1;
        }

    } while (!GateVertex::isNull(stepVertex));
    TupleNotation encoding(findNonGateClausesOfLTSEQ_n_1(formula, eta, i + 1)); // todo: is this the correct i?
    if (encoding.isEmpty()) gamma = { };
    return StructuralFormula{ encoding, gamma };
}
