#include <gtest/gtest.h>
#include <candy/gates/datastructures/BooleanCircuit.h>
#include <candy/gates/utilities/SequentialCounterEncoder.h>
#include <candy/gates/utilities/FormulaTools.h>
#include <candy/gates/algorithms/BooleanCircuitVisualizer.h>

TEST(BooleanCircuitVisualizer, LTSEQ_vis_01) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(2, {1, 2 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot();
    cout << dot << endl;
    SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n2_k1.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_02) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(3, {1, 2, 3 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot();
    cout << dot << endl;
    SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n3_k2.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_03) {
    vector<Var> constraintVars;
    unsigned int n = 3;
    for (unsigned int i = 1; i <= n; i++) constraintVars.emplace_back(i);
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVars, n - 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    BooleanCircuitVisualizer visualizer(seqCounterCircuit);
    string dot = visualizer.toDot(true);
    cout << dot << endl;
    SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n" + to_string(n) + "_k1.dot");
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_04) {
    vector<Var> constraintVars{ 1 };
    for (unsigned int n = 2; n <= 15; n++) {
        constraintVars.emplace_back(n);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVars, 1);
        BooleanCircuit seqCounterCircuit(atMostSeq);
        BooleanCircuitVisualizer visualizer(seqCounterCircuit);
        string dot = visualizer.toDot(false);
        cout << dot << endl;
        SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n" + to_string(n) + "_k" + to_string(1) + ".dot");
    }
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_05) {
    vector<Var> constraintVars;
    unsigned int n = 9;
    for (unsigned int i = 1; i <= n; i++) constraintVars.emplace_back(i);
    for (unsigned int k = 1; k < n; k++) {
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVars, k);
        cout << "LTSEQ_k" + to_string(k) + " = " << endl << atMostSeq << endl;
        BooleanCircuit seqCounterCircuit(atMostSeq);
        BooleanCircuitVisualizer visualizer(seqCounterCircuit);
        string dot = visualizer.toDot(false);
        cout << dot << endl;
        SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n" + to_string(n) + "_k" + to_string(k) + ".dot");
    }
}

TEST(BooleanCircuitVisualizer, LTSEQ_vis_06) {
    vector<Var> constraintVars{ 1 };
    for (unsigned int n = 2; n <= 9; n++) {
        constraintVars.emplace_back(n);
        for (unsigned int k = 1; k < n; k++) {
            const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVars, k);
            BooleanCircuit seqCounterCircuit(atMostSeq);
            BooleanCircuitVisualizer visualizer(seqCounterCircuit);
            string dot = visualizer.toDot(false);
            cout << dot << endl;
            SequentialCounterEncoder::writeStringToFile(dot, "ltseq_n" + to_string(n) + "_k" + to_string(k) + ".dot");
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
        SequentialCounterEncoder::writeStringToFile(dot, "mp1-squ_ali_s10x10_c39_abio_SAT.dot");
}