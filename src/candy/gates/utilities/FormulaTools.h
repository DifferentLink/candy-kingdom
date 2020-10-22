#ifndef CANDY_FORMULATOOLS_H
#define CANDY_FORMULATOOLS_H

#include <candy/core/CNFProblem.h>
#include <candy/gates/GateAnalyzer.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <candy/gates/GateProblem.h>
#include <candy/gates/datastructures/TupleNotation.h>

using namespace std;

class FormulaTools {
public:
    static unsigned int countVariables(const string& formula);

    static unsigned int countClauses(const string& formula);


   /**
    * Given any sequence of literals l, return the maximum value of all abs(l)
    * @param literals the literals
    * @return the maximal variable
    */
    static unsigned int getMaxVar(const vector<int>& literals);

    static GateProblem gateProblemFromDIMACS(const string& filename);

    template<typename T>
    static vector<T> unionOf(vector<vector<T>> disjunctSets);

    static vector<Var> getRandomVariables(unsigned int amount, unsigned int start, unsigned int end);

    static string asString(const vector<Var>& vector);
};

#endif //CANDY_FORMULATOOLS_H
