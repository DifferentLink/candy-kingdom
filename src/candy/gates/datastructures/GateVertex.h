#ifndef CANDY_GATEVERTEX_H
#define CANDY_GATEVERTEX_H

#include "candy/gates/GateProblem.h"
#include "TupleNotation.h"
#include <vector>

using namespace Candy;
using namespace std;

class GateVertex {
private:
    unsigned long id;
    Gate gate;

public:
    GateVertex(const unsigned long id, const Gate &gate) : id(id), gate(gate) {};

    GateVertex(const GateVertex& gateVertex) : id(gateVertex.id), gate(gateVertex.gate) {};

    unsigned long getId() const {
        return id;
    }

    const Gate &getGate() const {
        return gate;
    }

    static bool isNull(const GateVertex& vertex) {
        return vertex.id == 0;
    }

    vector<Var> getInVariables() const {
        vector<Var> vars;
        for (const auto& literal : gate.inp) {
            vars.push_back(literal.var());
        }
        return vars;
    }

    Lit getOutLiteral() const {
        return gate.out;
    }

    TupleNotation getFormula() const {
        For gateFormula;
        gateFormula.insert(gateFormula.end(), gate.fwd.begin(), gate.fwd.end());
        gateFormula.insert(gateFormula.end(), gate.bwd.begin(), gate.bwd.end());
        return TupleNotation(gateFormula);
    }
};

#endif //CANDY_GATEVERTEX_H