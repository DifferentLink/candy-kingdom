#include "BooleanCircuitVisualizer.h"


string BooleanCircuitVisualizer::toDot(const BooleanCircuit& circuit) {
    string vertices;
    string edges;
    for (auto& fromVertex : circuit.getGates()) {
        string gateName = "\"" + fromVertex.getFormula().toString() + "\"";
        vertices.append("\t" + gateName + "[shape=box, style=filled, fillcolor=grey90]\n");
        for (auto& toVertex : circuit.getOutEdges(fromVertex)) {
            edges.append("\t" + gateName + " -> " + "\"" + toVertex.getFormula().toString() + "\"\n");
        }
    }
    return "digraph {\n" + vertices + edges + "}";
}
