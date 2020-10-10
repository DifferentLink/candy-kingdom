#include <gtest/gtest.h>
#include <candy/gates/datastructures/BooleanCircuit.h>
#include <candy/gates/utilities/IOTools.h>
#include <candy/gates/utilities/FormulaTools.h>
#include <candy/gates/algorithms/BooleanCircuitVisualizer.h>

TEST(BooleanCircuitVisualizer, LTSEQ_vis_01) {
    const string &atMostSeq = IOTools::atMostSEQ("", { 1, 2 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    string dot = BooleanCircuitVisualizer::toDot(seqCounterCircuit);
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n2_k1.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_02) {
    const string &atMostSeq = IOTools::atMostSEQ("", { 1, 2, 3, 4 }, 3);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    string dot = BooleanCircuitVisualizer::toDot(seqCounterCircuit);
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n4_k3.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_03) {
    vector<Var> constraintVars;
    for (unsigned int i = 1; i <= 25; i++) constraintVars.emplace_back(i);
    const string &atMostSeq = IOTools::atMostSEQ("", constraintVars, constraintVars.size() - 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    string dot = BooleanCircuitVisualizer::toDot(seqCounterCircuit);
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n25_k24.dot");
}