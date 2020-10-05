#ifndef CANDY_STRUCTURALFORMULA_H
#define CANDY_STRUCTURALFORMULA_H

#include "BooleanCircuit.h"

struct StructuralFormula {
    TupleNotation formula;
    vector<GateVertex> circuit;

    StructuralFormula(TupleNotation formula, vector<GateVertex> circuit) : formula(formula), circuit(circuit) {}

    static StructuralFormula getEmptyStructuralFormula() {
        return StructuralFormula(TupleNotation(), { });
    }
};

#endif //CANDY_STRUCTURALFORMULA_H
