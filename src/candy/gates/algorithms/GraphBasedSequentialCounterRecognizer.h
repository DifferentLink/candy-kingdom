#ifndef CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H
#define CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H

#include <candy/gates/datastructures/BooleanCircuit.h>
#include <candy/gates/datastructures/StructuralFormula.h>
#include <map>
#include <candy/gates/GateProblem.h>

class GraphBasedSequentialCounterRecognizer {
public:
    static StructuralFormula recognizeLTSEQI(BooleanCircuit circuit);
    static StructuralFormula recognizeLTSEQII(TupleNotation& formula,
                                              BooleanCircuit& circuit,
                                              StructuralFormula& structuralEncoding,
                                              map<string, unsigned int>& eta);
    static vector<GateVertex> recognizeLTSEQIII(TupleNotation& formula,
                                                BooleanCircuit& circuit,
                                                StructuralFormula& structuralEncoding,
                                                vector<GateVertex>& gamma2,
                                                map<string, unsigned int>& eta);
    static StructuralFormula recognizeLTSEQIV(TupleNotation& formula,
                                              BooleanCircuit& circuit,
                                              StructuralFormula& structuralEncoding,
                                              vector<GateVertex>& gamma2,
                                              vector<GateVertex>& gamma3,
                                              map<string, unsigned int>& eta);
    static vector<StructuralFormula> recognizeLTSEQ(TupleNotation& formula);
};

const GateVertex nullVertex = BooleanCircuit::getNullVertex();

#endif //CANDY_GRAPHBASEDSEQUENTIALCOUNTERRECOGNIZER_H
