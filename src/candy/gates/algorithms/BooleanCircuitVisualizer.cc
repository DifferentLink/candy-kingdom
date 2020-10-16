#include <map>
#include "BooleanCircuitVisualizer.h"

BooleanCircuitVisualizer::BooleanCircuitVisualizer(const BooleanCircuit& circuit)
        : classifier(GateClassifier(&circuit)) {
    this->circuit = circuit;
    this->classifier.classify();
}

string clausesAsString(const GateVertex& gateVertex) {
    return gateVertex.getFormula().toString();
}

string variableListAsString(const GateVertex& gateVertex) {
    string out;
    vector<unsigned int> inVariables = gateVertex.getInVariables();
    sort(inVariables.begin(), inVariables.end());
    for (const auto& variable : inVariables) out.append(to_string(variable + 1) + ", ");
    return out.substr(0, out.size() - 2);
}

string BooleanCircuitVisualizer::classificationToString(const GateVertex& gateVertex) {
    string outVariable = to_string(abs(gateVertex.getOutLiteral()) + 1);
    string outLiteral = (gateVertex.getOutLiteral() > 0 ? "-" : "") + outVariable;
    const string &variableList = variableListAsString(gateVertex);
    string classificationString;
    switch (getClassification().at(gateVertex.getId() - 1)) {
        case GateClassifier::cEQUIV:
            classificationString = "EQUIV(" + variableList + ") = " + outLiteral;
        case GateClassifier::cOR:
            classificationString = "OR(" + variableList + ") = " + outLiteral;
        case GateClassifier::cAND:
            classificationString = "AND(" + variableList + ") = " + outLiteral;
        case GateClassifier::cXOR:
            classificationString = "XOR(" + variableList + ") = " + outLiteral;
        case GateClassifier::cUNDEF:
            return clausesAsString(gateVertex);
    }
    return classificationString + "\n" + clausesAsString(gateVertex);
}

string BooleanCircuitVisualizer::toDot(const bool renderInputVariables) {
    string vertices;
    string edges;
    map<unsigned int, bool> isUsed;
    vector<string> gateNames;
    gateNames.resize(circuit.numGates());

    for (const auto& vertex : circuit.getGates()) {
        isUsed.insert({ abs(vertex.getOutLiteral()), true});
        string gateName = "\"" + BooleanCircuitVisualizer::classificationToString(vertex) + "\"";
        gateNames.insert(gateNames.begin() + vertex.getId() - 1, gateName);
        vertices.append("\t" + gateName + "[shape=box, style=filled, fillcolor=white]\n");
    }

    for (auto& fromVertex : circuit.getGates()) {
        for (auto& toVertex : circuit.getOutEdges(fromVertex)) {
            edges.append("\t" + gateNames.at(fromVertex.getId() - 1) + " -> "
                                + gateNames.at(toVertex.getId() - 1) + "\n");
        }
    }
    if (renderInputVariables) {
        set<unsigned int> inputVars;
        for (const auto& vertex : circuit.getGates()) {
            for (const auto& inputVariable : vertex.getInVariables()) {
                if (isUsed.find(inputVariable) == isUsed.end()) {
                    inputVars.insert(inputVariable + 1);
                }
            }
        }

        for (const auto& inputVariable : inputVars) {
            vertices.append("\t\"" + to_string(inputVariable) + "\""
                            + "[shape=circle, style=filled, fillcolor=white]\n");
        }

        for (const auto& vertex : circuit.getGates()) {
            for (const unsigned int inputVariable : inputVars) {
                if (circuit.hasInput(vertex, inputVariable - 1)) {
                    edges.append("\t\"" + to_string(inputVariable) + "\" -> "
                                 + gateNames.at(vertex.getId() - 1) + "\n");
                }
            }
        }
    }

    return "digraph {\n" + vertices + edges + "}";
}
