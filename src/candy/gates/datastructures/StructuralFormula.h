#ifndef CANDY_STRUCTURALFORMULA_H
#define CANDY_STRUCTURALFORMULA_H

#include "BooleanCircuit.h"

struct StructuralFormula {
private:
    TupleNotation formula;
    vector<GateVertex> circuit;

public:
    StructuralFormula(const TupleNotation& formula,
                      const vector<GateVertex>& circuit) : formula(formula), circuit(circuit) {}

    static StructuralFormula getEmptyStructuralFormula() {
        return StructuralFormula(BooleanCircuit::emptyFormula, { });
    }

    const TupleNotation &getFormula() const {
        return formula;
    }

    void setFormula(const TupleNotation& formula) {
        StructuralFormula::formula = formula;
    }

    const vector<GateVertex>& getCircuit() const {
        return circuit;
    }

    void setCircuit(const vector<GateVertex>& circuit) {
        StructuralFormula::circuit = circuit;
    }
};

#endif //CANDY_STRUCTURALFORMULA_H
