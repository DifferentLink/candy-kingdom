#include "BooleanCircuitVisualizer.h"

BooleanCircuitVisualizer::BooleanCircuitVisualizer(const BooleanCircuit &circuit) : circuit(circuit) {}

string BooleanCircuitVisualizer::toDot() {
    string vertices;
    string edges;

    for (const auto& fromVertex : this->circuit.getGates()) {
        string gateName = "\"" + fromVertex.getGate().getFormula().toString() + "\"";
        vertices.append(gateName);
        for (const auto& toVertex : this->circuit.getOutEdges(fromVertex)) {
            edges.append(gateName + "->" + "\"" + toVertex.getGate().getFormula().toString() + "\"");
        }
    }
    string out = vertices.append("\n" + edges);
    return "digraph {\n" + vertices + "\n}";
}
