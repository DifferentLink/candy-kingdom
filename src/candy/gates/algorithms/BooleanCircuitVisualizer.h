#ifndef CANDY_BOOLEANCIRCUITVISUALIZER_H
#define CANDY_BOOLEANCIRCUITVISUALIZER_H


#include <candy/gates/datastructures/BooleanCircuit.h>

class BooleanCircuitVisualizer {
public:
    static string toDot(const BooleanCircuit& circuit);
};


#endif //CANDY_BOOLEANCIRCUITVISUALIZER_H
