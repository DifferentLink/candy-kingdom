#ifndef CANDY_BOOLEANCIRCUITVISUALIZER_H
#define CANDY_BOOLEANCIRCUITVISUALIZER_H


#include <candy/gates/datastructures/BooleanCircuit.h>
#include "GateClassifier.h"

class BooleanCircuitVisualizer {
private:
    BooleanCircuit circuit;
    GateClassifier classifier;

public:
    explicit BooleanCircuitVisualizer(const BooleanCircuit &circuit);
    string toDot(bool renderInputVariables = true);
    string classificationToString(const GateVertex& gateVertex);
    vector<GateClassifier::characteristicFunction> getClassification() {
        return classifier.getResult();
    }
};


#endif //CANDY_BOOLEANCIRCUITVISUALIZER_H
