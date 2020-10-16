#include <gtest/gtest.h>
#include <candy/gates/datastructures/BooleanCircuit.h>
#include <candy/gates/utilities/IOTools.h>
#include <candy/gates/utilities/FormulaTools.h>
#include <candy/gates/algorithms/BooleanCircuitVisualizer.h>

TEST(BooleanCircuitVisualizer, LTSEQ_vis_01) {
    const string &atMostSeq = IOTools::atMostSEQ("", { 1, 2 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot();
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n2_k1.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_02) {
    const string &atMostSeq = IOTools::atMostSEQ("", { 1, 2, 3 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot();
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n3_k2.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_03) {
    vector<Var> constraintVars;
    unsigned int n = 3;
    for (unsigned int i = 1; i <= n; i++) constraintVars.emplace_back(i);
    const string &atMostSeq = IOTools::atMostSEQ("", constraintVars, n - 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot(true);
    cout << dot << endl;
    IOTools::writeStringToFile(dot, "ltseq_n" + to_string(n) + "_k1.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_04) {
    vector<Var> constraintVars;
    for (unsigned int i = 1; i <= 8; i++) {
        constraintVars.emplace_back(i);
        for (unsigned int k = 1; k < i; k++) {
            const string &atMostSeq = IOTools::atMostSEQ("", constraintVars, k);
            BooleanCircuit seqCounterCircuit(atMostSeq);
            BooleanCircuitVisualizer visualizer(seqCounterCircuit);
            string dot = visualizer.toDot();
            cout << dot << endl;
            IOTools::writeStringToFile(dot, "ltseq_n" + to_string(i) + "_k" + to_string(k) + ".dot");
        }
    }
}

TEST(BooleanCircuitVisualizer, flat200_3_0_vis) {
        CNFProblem problem;
        problem.readDimacsFromFile("problems/mp1-squ_ali_s10x10_c39_abio_SAT.cnf");
        BooleanCircuit seqCounterCircuit(problem);
        BooleanCircuitVisualizer visualizer(seqCounterCircuit);
        string dot = visualizer.toDot(false);
        cout << dot << endl;
        IOTools::writeStringToFile(dot, "mp1-squ_ali_s10x10_c39_abio_SAT.dot");
}