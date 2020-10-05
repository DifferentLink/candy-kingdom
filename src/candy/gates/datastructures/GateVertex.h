
#ifndef CANDY_GATEVERTEX_H
#define CANDY_GATEVERTEX_H

#include "Gate.h"

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

    static const GateVertex getNullVertex() {
        return GateVertex(0, Gate::getNullGate());
    }

    static bool isNull(const GateVertex& vertex) {
        return vertex.id == 0 && Gate::isNullGate(vertex.gate);
    }
};

#endif //CANDY_GATEVERTEX_H