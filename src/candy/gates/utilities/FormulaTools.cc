#include "FormulaTools.h"

unsigned int FormulaTools::countVariables(const string &formula) {
    unsigned int max = 0;
    istringstream stream(formula);
    for (string s; stream >> s; ) {
        unsigned int var = abs(stoi(s));
        if (var > max) {
            max = var;
        }
    }
    return max;
}

unsigned int FormulaTools::countClauses(const string &formula) {
    unsigned int count = 0;
    for (unsigned long lastFound = 0; lastFound != string::npos; lastFound = formula.find(" 0", ++lastFound)) {
        ++count;
    }
    return count;
}

unsigned int FormulaTools::getMaxVar(const vector<int> &literals) {
    unsigned int max = 0;
    for (const auto literal : literals) if (abs(literal) > max) max = abs(literal);
    return max;
}

GateProblem FormulaTools::gateProblemFromDIMACS(const string& filename) {
    CNFProblem problem;
    problem.readDimacsFromFile(filename.c_str());
    GateAnalyzer gateAnalyzer { problem };
    gateAnalyzer.analyze();
    GateProblem gateProblem = gateAnalyzer.getResult();
    return gateProblem;
}

template<typename T>
vector<T> FormulaTools::unionOf(const vector<vector<T>> disjunctSets) {
    vector<T> out;
    for (const auto& set : disjunctSets) {
        out.insert(out.end(), set.begin(), set.end());
    }
    return out;
}
