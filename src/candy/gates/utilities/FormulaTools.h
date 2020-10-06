#ifndef CANDY_FORMULATOOLS_H
#define CANDY_FORMULATOOLS_H

#include <string>
#include <vector>
#include <sstream>
#include <candy/gates/datastructures/BooleanCircuit.h>

using namespace std;

class FormulaTools {
public:
    static unsigned int countVariables(const string& formula);

    static unsigned int countClauses(const string& formula);

    static string generateDIMACSHeader(const string& formula);

   /**
    * Given any sequence of literals l, return the maximum value of all abs(l)
    * @param literals the literals
    * @return the maximal variable
    */
    static unsigned int getMaxVar(const vector<int>& literals);

    static GateProblem gateProblemFromDIMACS(const string& filename);

    static BooleanCircuit toCircuit(const TupleNotation& formula);

    static void writeToDIMACS(const TupleNotation& formula, const string& filename);

    static BooleanCircuit toCircuit(const GateProblem &gateProblem);
};

#endif //CANDY_FORMULATOOLS_H
