#include <gtest/gtest.h>
#include <candy/gates/algorithms/ClassificationBasedRecognizer.h>

using namespace std;

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n2k1) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(2, { 1, 2 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 1 && result.getFormula().size() == 1);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n3k1) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(3, { 1, 2, 3 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 2);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n3k2) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(3, { 1, 2, 3 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 2);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n4k1) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(4, { 1, 2, 3, 4 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 3);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n4k2) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(4, { 1, 2, 3, 4 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 4);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n4k3) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(4, { 1, 2, 3, 4 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 4);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n5k1) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(5, { 1, 2, 3, 4, 5 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 4);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n5k2) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(5, { 1, 2, 3, 4, 5 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 6);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n5k3) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(5, { 1, 2, 3, 4, 5 }, 3);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 8);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n5k4) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(5, { 1, 2, 3, 4, 5 }, 4);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 10);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n6k1) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(6, {1, 2, 3, 4, 5, 6 }, 1);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 5);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognition_n6k2) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(6, { 1, 2, 3, 4, 5, 6 }, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 8);
}
TEST(ClassificationBasedRecognizerTest, combinedRecognition_n6k3) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(6, { 1, 2, 3, 4, 5, 6 }, 3);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 11);
}
TEST(ClassificationBasedRecognizerTest, combinedRecognition_n6k4) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(6, { 1, 2, 3, 4, 5, 6 }, 4);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 14);
}
TEST(ClassificationBasedRecognizerTest, combinedRecognition_n6k5) {
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(6, { 1, 2, 3, 4, 5, 6 }, 5);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == 17);
}

TEST(ClassificationBasedRecognizerTest, combinedRecognitionk1) {
    unsigned int maxN = 100;
    vector<Var> constraintVariables = { 1 };
    for (unsigned int n = 2; n <= maxN; n++) {
        constraintVariables.emplace_back(n);
        const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVariables, 1);
        BooleanCircuit seqCounterCircuit(atMostSeq);
        ClassificationBasedRecognizer recognizer(seqCounterCircuit);
        StructuralFormula result = recognizer.recognize();
        cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
        ASSERT_TRUE(result.getCircuit().size() == n - 1);
    }
}

TEST(ClassificationBasedRecognizerTest, combinedRecognitionk2) {
    unsigned int maxN = 100;
    vector<Var> constraintVariables = { 1 };
    for (unsigned int n = 2; n <= maxN; n++) constraintVariables.emplace_back(n);
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(maxN, constraintVariables, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == seqCounterCircuit.numGates());
}

TEST(ClassificationBasedRecognizerTest, combinedRecognitionk5) {
    unsigned int maxN = 100;
    vector<Var> constraintVariables = { 1 };
    for (unsigned int n = 2; n <= maxN; n++) constraintVariables.emplace_back(n);
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(maxN, constraintVariables, 2);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    ASSERT_TRUE(result.getCircuit().size() == seqCounterCircuit.numGates());
}

TEST(ClassificationBasedRecognizerTest, performanceTest_n100k99) {
    unsigned int maxN = 100;
    vector<Var> constraintVariables = { 1 };
    for (unsigned int n = 2; n <= maxN; n++) constraintVariables.emplace_back(n);
    const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(maxN, constraintVariables, 9);
    BooleanCircuit seqCounterCircuit(atMostSeq);
    auto t1 = chrono::high_resolution_clock::now();
    ClassificationBasedRecognizer recognizer(seqCounterCircuit);
    StructuralFormula result = recognizer.recognize();
    auto t2 = chrono::high_resolution_clock::now();
    cout << to_string(result.getCircuit().size()) << " : " << to_string(result.getFormula().size()) << endl;
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout << "Recognition took " << duration * 10e-3 << " ms";
    ASSERT_TRUE(result.getCircuit().size() == seqCounterCircuit.numGates());
}

TEST(ClassificationBasedRecognizerTest, performanceTest_n100) {
    unsigned int maxN = 100;
    vector<Var> constraintVariables = { 1, 2, 3, 4 };
    ofstream oFile("performanceTest_n100.csv");
    for (unsigned int n = 5; n <= maxN; n++) {
        constraintVariables.emplace_back(n);
        for (unsigned int k = 1; k < n; k++) {
            cout << to_string(n) << " " << to_string(k) << endl;
            const string &atMostSeq = SequentialCounterEncoder::atMostSEQ(n, constraintVariables, k);
            BooleanCircuit seqCounterCircuit(atMostSeq);
            auto t1 = chrono::high_resolution_clock::now();
            ClassificationBasedRecognizer recognizer(seqCounterCircuit);
            StructuralFormula result = recognizer.recognize();
            auto t2 = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
            oFile << to_string(result.getCircuit().size()) << "; " << duration * 10e-3 << endl;
        }
    }
    oFile.close();
}