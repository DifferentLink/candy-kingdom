#include <gtest/gtest.h>
#include "candy/gates/utilities/FormulaTools.h"
#include "candy/gates/utilities/IOTools.h"
#include "candy/gates/algorithms/GraphBasedSequentialCounterRecognizer.h"

TEST(GraphBasedSequentialCounterRecognizerTest, kOneRecognition_numGatesTest_01) {
    BooleanCircuit seqCounterCircuit(IOTools::atMostSEQ(2, { 1, 2 }, 1));
    TupleNotation remainingClauses;
    StructuralFormula structuralFormula =
            GraphBasedSequentialCounterRecognizer::recognizeLTSEQI(seqCounterCircuit);
    cout << structuralFormula.getCircuit().size() << endl;
    ASSERT_TRUE(structuralFormula.getCircuit().size() == 1);
    ASSERT_TRUE(structuralFormula.getFormula().size() == 1);
}
