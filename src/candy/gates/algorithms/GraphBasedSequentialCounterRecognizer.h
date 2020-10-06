#ifndef CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H
#define CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H

#include <candy/gates/datastructures/BooleanCircuit.h>
#include <candy/gates/datastructures/StructuralFormula.h>
#include <map>

class GraphBasedSequentialCounterRecognizer {
private:
    static StructuralFormula recognizeLTSEQ_n_1(TupleNotation& formula,
                                                BooleanCircuit& circuit,
                                                map<string, unsigned int> eta);
};

const GateVertex nullVertex = BooleanCircuit::getNullVertex();

#endif //CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H
