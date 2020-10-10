#ifndef CANDY_GATEVERTEX_H
#define CANDY_GATEVERTEX_H

#include "candy/gates/GateProblem.h"
#include "TupleNotation.h"
#include "TupleGate.h"
#include <utility>
#include <vector>

using namespace Candy;
using namespace std;

class GateVertex {
private:
    unsigned long id;
    TupleGate gate;

public:
    GateVertex(unsigned long id, const Gate& gate) : id(id), gate(gate) {}

    bool operator==(const GateVertex& gateVertex) const {
        return this->id == gateVertex.id;
    }

    unsigned long getId() const {
        return id;
    }

    const TupleGate &getGate() const {
        return gate;
    }

    static bool isNull(const GateVertex& vertex) {
        return vertex.id == 0;
    }

    vector<unsigned int> getInVariables() const {
        set<unsigned int> variables;
        for (const auto& literal : gate.inp) variables.insert(abs(literal));
        return vector<unsigned int>(variables.begin(), variables.end());
    }

    int getOutLiteral() const {
        return gate.out;
    }

    TupleNotation getFormula() const {
        vector<vector<int>> formula = gate.fwd.getFormula();
        vector<vector<int>> bwd = gate.bwd.getFormula();
        formula.insert(formula.end(), bwd.begin(), bwd.end());
        return TupleNotation(formula);
    }
};

#endif //CANDY_GATEVERTEX_H