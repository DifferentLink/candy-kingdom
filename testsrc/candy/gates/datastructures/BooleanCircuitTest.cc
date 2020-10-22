#include <gtest/gtest.h>
#include <candy/gates/datastructures/BooleanCircuit.h>

TEST(BooleanCircuitTest, fromFormulaTest_01) {
    string formula = SequentialCounterEncoder::atMostSEQ(2, {1, 2 }, 1);
    BooleanCircuit circuit(formula);
    ASSERT_TRUE(circuit.numGates() == 2);
    ASSERT_TRUE(circuit.numEdges() == 1);
}