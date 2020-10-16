#include <gtest/gtest.h>
#include <candy/gates/datastructures/BooleanCircuit.h>

TEST(BooleanCircuitTest, basicOperationsTest_01) {
    BooleanCircuit circuit;
    TupleNotation for1;
    TupleNotation for2({1, 2});
    Gate gate1{};
    Gate gate2{};
    circuit.addUniqueGate(gate1);
    circuit.addUniqueGate(gate2);
    ASSERT_TRUE(circuit.numGates() == 2);
}

TEST(BooleanCircuitTest, fromFormulaTest_01) {
    string formula = IOTools::atMostSEQ("1 0", { 2, 3 }, 1);
    BooleanCircuit circuit(formula);
    ASSERT_TRUE(circuit.numGates() == 2);
    ASSERT_TRUE(circuit.numEdges() == 1);
}