#ifndef CANDY_GENERALFORMULATOOLS_H
#define CANDY_GENERALFORMULATOOLS_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

unsigned int countVariables(const string& formula) {
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

unsigned int countClauses(const string& formula) {
    unsigned int count = 0;
    for (unsigned long lastFound = 0; lastFound != string::npos; lastFound = formula.find(" 0", ++lastFound)) {
        ++count;
    }
    return count;
}

string generateDIMACSHeader(const string& formula) {
    return "p cnf "
           + to_string(countVariables(formula)) + " "
           + to_string(countClauses(formula));
}

/**
 * Given any sequence of literals l, return the maximum value of all abs(l)
 * @param literals the literals
 * @return the maximal variable
 */
unsigned int getMaxVar(const vector<int>& literals) {
    unsigned int max = 0;
    for (const auto literal : literals) if (abs(literal) > max) max = abs(literal);
    return max;
}

#endif //CANDY_GENERALFORMULATOOLS_H
