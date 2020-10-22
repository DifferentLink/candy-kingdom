#include <gtest/gtest.h>
#include <candy/gates/algorithms/ClassificationBasedRecognizer.h>

using namespace std;

/*
TEST(EvaluationTest, n10k5) {
    unsigned int maxN = 10;
    vector<Var> constraintVariables = { 1 };
    for (unsigned int n = 2; n <= maxN; n++) constraintVariables.emplace_back(n);
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(maxN, constraintVariables, 5);
    BooleanCircuit circuit(atMostSeq);
    StructuralFormula result(TupleNotation(), { });
    ClassificationBasedRecognizer recognizer(circuit);
    do {
        result = recognizer.recognize();
        cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
}
*/

/*
TEST(EvaluationTest, likewynn_01) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_01_mp1-squ_ali_s10x10_c39_abio_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_02) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abix_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_02_mp1-squ_ali_s10x10_c39_abix_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
                << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_03) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_bail_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_03_mp1-squ_ali_s10x10_c39_bail_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_04) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_sinx_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_04_mp1-squ_ali_s10x10_c39_sinx_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_05) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_sinz_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_05_mp1-squ_ali_s10x10_c39_sinz_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_06) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_any_s09x07_c27_abio_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_06_mp1-squ_any_s09x07_c27_abio_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_07) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_any_s09x07_c27_abix_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_07_mp1-squ_any_s09x07_c27_abix_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_08) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_any_s09x07_c27_bail_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_08_mp1-squ_any_s09x07_c27_bail_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_09) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_any_s09x07_c27_sinx_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_09_mp1-squ_any_s09x07_c27_sinx_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_10) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_any_s09x07_c27_sinz_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_10_mp1-squ_any_s09x07_c27_sinz_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_11) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-tri_ali_s11_c35_abio_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_11_mp1-tri_ali_s11_c35_abio_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_12) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-tri_ali_s11_c35_abix_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_12_mp1-tri_ali_s11_c35_abix_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_13) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-tri_ali_s11_c35_bail_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_13_mp1-tri_ali_s11_c35_bail_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_14) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-tri_ali_s11_c35_sinx_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_14_mp1-tri_ali_s11_c35_sinx_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_15) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-tri_ali_s11_c35_sinz_UNS.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_15_mp1-tri_ali_s11_c35_sinz_UNS";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_16) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/squ_ali_s10x10_c39_abix_SAT-sc2017.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_16_squ_ali_s10x10_c39_abix_SAT-sc2017";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}

TEST(EvaluationTest, likewynn_17) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/squ_any_s09x07_c27_abio_UNS-sc2017.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "likewynn_17_squ_any_s09x07_c27_abio_UNS-sc2017";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    do {
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size())
              << " | " << duration * 10e-3 << endl;
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    oFile.close();
}


// ------------------------------------------------------------------------------------------------------------------ //

TEST(EvaluationTest, removeExistingConstraint_01) {
    CNFProblem problem;
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT.cnf";
    problem.readDimacsFromFile(filename);
    StructuralFormula result(TupleNotation(), { });
    BooleanCircuit circuit(problem);
    ClassificationBasedRecognizer recognizer(circuit);
    string resultFilename = "mp1-squ_ali_s10x10_c39_abio_SAT_ConstraintsRemoved";
    do {
        result = recognizer.recognize();
        vector<GateVertex> gates = result.getCircuit();
        recognizer.deleteGates(gates);
    } while (!result.getCircuit().empty());
    string dimacs = recognizer.getCircuit().toDIMACS();
    SequentialCounterEncoder::writeToDIMACS(dimacs, resultFilename);
}


TEST(EvaluationTest, randomAdd_k1_01) {
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT_constraintsRemoved.cnf";
    ifstream t(filename);
    string likewynn((istreambuf_iterator<char>(t)),
                    istreambuf_iterator<char>());
    string resultFilename = "randomk1_mp1-squ_ali_s10x10_c39_abio_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    int maxN = 40;
    for (unsigned int n = 2; n <= maxN; n++) {
        vector<Var> constraintVariables = FormulaTools::getRandomVariables(n, 1, 1672);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(1672, constraintVariables, 1);
        BooleanCircuit circuit(likewynn + atMostSeq, true);
        StructuralFormula result(TupleNotation(), { });
        ClassificationBasedRecognizer recognizer(circuit);
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << "#Gates: " + to_string(circuit.numGates()) << ", #Connections: " << to_string(circuit.numEdges()) << endl
              << "(n = " + to_string(n) + ", k = 1)"
              << (result.getCircuit().size() == n - 1 ? " found " : " disappeared ") << endl
              << "took " << duration * 10e-3 << " ms" << endl
              << "Vars: " << FormulaTools::asString(constraintVariables)
              << endl
              << endl;
    }
    oFile.close();
}
*/

TEST(EvaluationTest, randomAdd_k2_01) {
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT_constraintsRemoved.cnf";
    ifstream t(filename);
    string likewynn((istreambuf_iterator<char>(t)),
                    istreambuf_iterator<char>());
    string resultFilename = "randomk2_mp1-squ_ali_s10x10_c39_abio_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    vector<Var> constraintVariables;
    int maxN = 40;
    for (unsigned int n = 4; n <= maxN; n++) {
        constraintVariables = FormulaTools::getRandomVariables(n, 1, 1672);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(1672, constraintVariables, 2);
        BooleanCircuit circuit(likewynn + atMostSeq, true);
        StructuralFormula result(TupleNotation(), { });
        ClassificationBasedRecognizer recognizer(circuit);
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << "#Gates: " + to_string(circuit.numGates()) << ", #Connections: " << to_string(circuit.numEdges()) << endl
              << "(n = " + to_string(n) + ", k = 2)"
              << (result.getCircuit().size() == n - 1 + n - 3 ? " found " : " disappeared ") << endl
              << "took " << duration * 10e-3 << " ms" << endl
              << "Vars: " << FormulaTools::asString(constraintVariables)
              << endl
              << endl;
    }
    oFile.close();
}

TEST(EvaluationTest, randomAdd_k4_01) {
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT_constraintsRemoved.cnf";
    ifstream t(filename);
    string likewynn((istreambuf_iterator<char>(t)),
                    istreambuf_iterator<char>());
    string resultFilename = "randomk4_mp1-squ_ali_s10x10_c39_abio_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    vector<Var> constraintVariables;
    int maxN = 40;
    for (unsigned int n = 9; n <= maxN; n++) {
        constraintVariables = FormulaTools::getRandomVariables(n, 1, 1672);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(1672, constraintVariables, 4);
        BooleanCircuit circuit(likewynn + atMostSeq, true);
        StructuralFormula result(TupleNotation(), { });
        ClassificationBasedRecognizer recognizer(circuit);
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << "#Gates: " + to_string(circuit.numGates()) << ", #Connections: " << to_string(circuit.numEdges()) << endl
              << "(n = " + to_string(n) + ", k = 4)"
              << (result.getCircuit().size() == 26 + (n-9) * 4 ? " found " : " disappeared ") << endl
              << "took " << duration * 10e-3 << " ms" << endl
              << "Vars: " << FormulaTools::asString(constraintVariables)
              << endl
              << endl;
    }
    oFile.close();
}

TEST(EvaluationTest, randomAdd_k10_01) {
    const char *filename = "problems/likewynn/mp1-squ_ali_s10x10_c39_abio_SAT_constraintsRemoved.cnf";
    ifstream t(filename);
    string likewynn((istreambuf_iterator<char>(t)),
                    istreambuf_iterator<char>());
    string resultFilename = "randomk10_mp1-squ_ali_s10x10_c39_abio_SAT";
    ofstream oFile(resultFilename);
    oFile << filename << endl;
    vector<Var> constraintVariables;
    int maxN = 40;
    for (unsigned int n = 11; n <= maxN; n++) {
        constraintVariables = FormulaTools::getRandomVariables(n, 1, 1672);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(1672, constraintVariables, 10);
        BooleanCircuit circuit(likewynn + atMostSeq, true);
        StructuralFormula result(TupleNotation(), { });
        ClassificationBasedRecognizer recognizer(circuit);
        auto t1 = chrono::high_resolution_clock::now();
        result = recognizer.recognize();
        auto t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        oFile << "#Gates: " + to_string(circuit.numGates()) << ", #Connections: " << to_string(circuit.numEdges()) << endl
              << "(n = " + to_string(n) + ", k = 10)"
              << (result.getCircuit().size() > 26 + (n-9) * 4 ? " found " : " disappeared ") << endl
              << "took " << duration * 10e-3 << " ms" << endl
              << "Vars: " << FormulaTools::asString(constraintVariables)
              << endl
              << endl;
    }
    oFile.close();
}

