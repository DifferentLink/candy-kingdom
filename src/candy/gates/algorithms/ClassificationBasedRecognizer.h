#ifndef CANDY_CLASSIFICATIONBASEDRECOGNIZER_H
#define CANDY_CLASSIFICATIONBASEDRECOGNIZER_H

#include <candy/gates/datastructures/StructuralFormula.h>
#include "GateClassifier.h"
#include "Unificator.h"
#include <map>

using namespace std;

class ClassificationBasedRecognizer {
private:
    TupleNotation formula;
    BooleanCircuit circuit;
    GateClassifier classifier;
    map<string, unsigned int> eta;
    GateVertex underHop(const GateVertex& previousGate, const GateVertex& currentGate);
    GateVertex overHop(const GateVertex& previousGate, const GateVertex& currentGate);
    unsigned int n = 0;
    unsigned int k = 0;
    StructuralFormula recognizeBaseCase();
    const GateVertex &findABaseGate();
    vector<int> findBaseClause();
    StructuralFormula recognizek1(const StructuralFormula &baseCase);
    StructuralFormula recognizeRectangle(const StructuralFormula &baseCase, const StructuralFormula &nColumn);
    StructuralFormula recognizeClosing(const StructuralFormula &rectangle);
    StructuralFormula recognizeFloating();
    StructuralFormula recognizeSpecialCasesZeroColumn(StructuralFormula &baseCase);
    StructuralFormula recognizeSpecialCasesOneColumn(StructuralFormula &baseCase);
    StructuralFormula recognizeSpecialCasesTwoColumn(StructuralFormula &baseCase, StructuralFormula &k1Column);
    StructuralFormula removeDuplicates(const StructuralFormula& formula);
public:
    explicit ClassificationBasedRecognizer(BooleanCircuit &circuit);
    StructuralFormula recognize();
    void deleteGates(vector<GateVertex> &gates);
    BooleanCircuit& getCircuit();
};

#endif //CANDY_CLASSIFICATIONBASEDRECOGNIZER_H
