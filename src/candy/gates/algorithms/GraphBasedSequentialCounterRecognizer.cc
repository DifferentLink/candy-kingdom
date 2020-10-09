#include <candy/gates/utilities/FormulaTools.h>
#include "GraphBasedSequentialCounterRecognizer.h"
#include "Unificator.h"

static const GateVertex& findFirstMatching(const vector<string>& pattern,
                                           const BooleanCircuit& circuit,
                                           map<string, unsigned int>& eta) {
    for (const GateVertex& gate : circuit.getGates()) {
        const auto& gateFormula = gate.getFormula();
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

vector<vector<int>> findNonGateClausesOfLTSEQ_n_k_I(const TupleNotation& formula,
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

vector<vector<int>> findNonGateClausesOfLTSEQ_n_k_II(const TupleNotation& formula,
                                                    map<string, unsigned int>& eta,
                                                    unsigned int n) {
    return {{ }};
}

StructuralFormula GraphBasedSequentialCounterRecognizer::recognizeLTSEQI(TupleNotation& formula,
                                                                         BooleanCircuit& circuit) {
    map<string, unsigned int> eta;
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
            const TupleNotation &gateFormula = w.getFormula();
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
    TupleNotation encoding(findNonGateClausesOfLTSEQ_n_k_I(formula, eta, i + 1)); // todo: is this the correct i?
    if (encoding.isEmpty()) gamma = { };
    return StructuralFormula{ encoding, gamma };
}

/*
StructuralFormula
GraphBasedSequentialCounterRecognizer::recognizeLTSEQII(TupleNotation &formula, BooleanCircuit &circuit,
                                                        StructuralFormula &structuralEncoding,
                                                        map<string, unsigned int>& eta) {
    const vector<GateVertex>& gamma1 = structuralEncoding.getCircuit();
    if (gamma1.size() <= 2) return structuralEncoding; // todo: less than or equal more sensible?
    vector<GateVertex> gamma2;
    unsigned int i = 1;
    GateVertex stepVertex = BooleanCircuit::nullVertex;
    GateVertex nextVertex = BooleanCircuit::nullVertex;
    string x_i = "x_1";
    string s_i_jm;
    string s_i_j = "s_1_2";
    string s_im_j;
    while (i <= structuralEncoding.getCircuit().size()) {
        stepVertex = gamma2.at(i);
        unsigned int j = 2;
        do {
            nextVertex = circuit.hop(stepVertex.getId(),
                                     {{"-" + x_i, "-" + s_i_jm, s_i_j}, {"-" + s_im_j, s_i_j}},
                                     eta);
            if (GateVertex::isNull(nextVertex)) return structuralEncoding;
            gamma2.push_back(nextVertex);
            j++;
        } while (Unificator::isDefined(eta, s_i_j)
              && Unificator::isDefined(eta, s_im_j)
              && !GateVertex::isNull(nextVertex));
        i++;
    }
    return StructuralFormula(TupleNotation(), BooleanCircuit().getGates());
}

vector<GateVertex>
GraphBasedSequentialCounterRecognizer::recognizeLTSEQIII(TupleNotation &formula, BooleanCircuit &circuit,
                                                         StructuralFormula &structuralEncoding,
                                                         vector<GateVertex>& gamma2,
                                                         map<string, unsigned int> &eta) {
    vector<GateVertex> gamma3;
    GateVertex w_1_2 = gamma2.back();
    GateVertex w_1_1 = circuit.hop(w_1_2.getId(),
                                   {{ "-x_nmm", "-s_nmmm_kmm", "-s_nmm_km" }, { "-s_nmmm_km", "s_nmm_km" }}); // todo: replace n-q etc. by the actual, now known values
    if (!Unificator::isDefined(eta, "x_nmm") || !Unificator::isDefined(eta, "s_nmm_km")) {
        return structuralEncoding;
    }
    gamma3.push_back(w_1_1);
    // --------------------- //
    bool failedFirstUnification = !Unificator::unifyClauseL3(w_1_2.getGate().getFormula().getFormula().at(0),
                                                             { "-x_nmm", "-s_nmm_km", "-s_nm_k" }, // todo: replace n-q etc. by the actual, now known values
                                                             eta);
    bool failedSecondUnification = !Unificator::unifyClauseL2(w_1_2.getGate().getFormula().getFormula().at(1),
                                                             { "s_nmm_k", "-s_nm_k" }, // todo: replace n-q etc. by the actual, now known values
                                                             eta);
    if (failedFirstUnification || failedSecondUnification) {
        return structuralEncoding;
    }
    gamma3.push_back(w_1_2);
    // --------------------- //
    GateVertex w_2_1 = circuit.traverse(w_1_2.getId());
    failedFirstUnification = !Unificator::unifyClauseL3(w_2_1.getGate().getFormula().getFormula().at(0),
                                                        { "-x_nm", "-s_nmm_km", "s_nm_k" }, // todo: replace n-q etc. by the actual, now known values
                                                        eta);
    failedSecondUnification = !Unificator::unifyClauseL2(w_2_1.getGate().getFormula().getFormula().at(1),
                                                              { "s_nmm_k", "-s_nm_k" }, // todo: replace n-q etc. by the actual, now known values. Is the second one really a placeholder?
                                                              eta);
    gamma3.push_back(w_2_1);
    return gamma3;
}

StructuralFormula
GraphBasedSequentialCounterRecognizer::recognizeLTSEQIV(TupleNotation &formula, BooleanCircuit &circuit,
                                                        StructuralFormula &structuralEncoding,
                                                        vector<GateVertex> &gamma2,
                                                        vector<GateVertex> &gamma3,
                                                        map<string, unsigned int> &eta) {
    vector<GateVertex> gamma4;
    GateVertex stepVertex = findAnyMatching32Pattern(circuit,
                                                     {{ "-x_nm", "-s_nm_ip", "s_nmm_i" }, { "s_nm_i", "s_nmm_i" }},
                                                     eta); // todo: replace n-q etc. by the actual, now known values. Is the second one really a placeholder?
    GateVertex prevVertex = BooleanCircuit::nullVertex;
    GateVertex currentVertex = stepVertex;
    GateVertex nextVertex = stepVertex;
    while (!GateVertex::isNull(nextVertex)) { // todo: is this the correct vertex to check?
        prevVertex = currentVertex;
        currentVertex = nextVertex;
        nextVertex = circuit.travel(nextVertex.getId(), prevVertex.getId());
    }
    stepVertex = currentVertex;
    // todo: line 10 and 11 should happen here
    prevVertex = BooleanCircuit::nullVertex;
    nextVertex = circuit.travel(currentVertex.getId(), prevVertex.getId());
    unsigned int kp = 0;
    while (!GateVertex::isNull(nextVertex)) {
        bool failedFirstUnification = !Unificator::unifyClauseL3(nextVertex.getGate().getFormula().getFormula().at(0),
                                                            { "-x_nm", "-s_nmm_km", "s_nm_k" }, // todo: give the method the correct pattern to match
                                                            eta);
        bool failedSecondUnification = !Unificator::unifyClauseL2(nextVertex.getGate().getFormula().getFormula().at(1),
                                                             { "s_nmm_k", "-s_nm_k" }, // todo: give the method the correct pattern to match
                                                             eta);
        if (failedFirstUnification || failedSecondUnification) return structuralEncoding;
        prevVertex = currentVertex;
        nextVertex = circuit.travel(nextVertex.getId(), currentVertex.getId());
        kp++;
    }
    TupleNotation encoding(findNonGateClausesOfLTSEQ_n_k_II(formula, eta, 1)); // todo: call with correct parameters
    if (kp != 42 || encoding.isEmpty()) {// todo: pass k instead of 42
        return structuralEncoding;
    } else {
        return StructuralFormula(structuralEncoding.getFormula(),
                                 FormulaTools::unionOf((vector<vector<GateVertex>>){ gamma2, gamma3, gamma4 }));
    }
}

vector<StructuralFormula> GraphBasedSequentialCounterRecognizer::recognizeLTSEQ(TupleNotation &formula) {
    vector<StructuralFormula> constraints;
    BooleanCircuit circuit = FormulaTools::toBooleanCircuit(formula);
    vector<StructuralFormula> m;

    return constraints;
}*/
