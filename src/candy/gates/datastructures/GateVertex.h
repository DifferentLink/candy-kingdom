
#ifndef CANDY_GATEVERTEX_H
#define CANDY_GATEVERTEX_H

#include "Gate.h"

class GateVertex {
private:
    unsigned long id;
    Recognition::Gate gate;

public:
    GateVertex(const unsigned long id, const Recognition::Gate &gate) : id(id), gate(gate) {};

    GateVertex(const GateVertex& gateVertex) : id(gateVertex.id), gate(gateVertex.gate) {};

    unsigned long getId() const {
        return id;
    }

    const Recognition::Gate &getGate() const {
        return gate;
    }

    static bool isNull(const GateVertex& vertex) {
        return vertex.id == 0 && Recognition::Gate::isNullGate(vertex.gate);
    }
};

#endif //CANDY_GATEVERTEX_H