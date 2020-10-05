#ifndef CANDY_BOOLEANCIRCUITVISUALIZER_H
#define CANDY_BOOLEANCIRCUITVISUALIZER_H


#include <candy/gates/datastructures/BooleanCircuit.h>

class BooleanCircuitVisualizer {
private:
    BooleanCircuit circuit;

public:
    BooleanCircuitVisualizer(const BooleanCircuit &circuit);
    string toDot();
};


#endif //CANDY_BOOLEANCIRCUITVISUALIZER_H
