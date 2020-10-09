#include "BooleanCircuitVisualizer.h"


string BooleanCircuitVisualizer::toDot(const BooleanCircuit& circuit) {
    string vertices;
    string edges;

    for (auto& fromVertex : circuit.getGates()) {
        string gateName = "\"" + fromVertex.getFormula().toString() + "\"\n";
        vertices.append(gateName);
        for (auto& toVertex : circuit.getOutEdges(fromVertex)) {
            edges.append(gateName + "->" + "\"" + toVertex.getFormula().toString() + "\"\n");
        }
    }
    string out = vertices.append(edges);
    return "digraph {\n" + vertices + "\n}";
}
