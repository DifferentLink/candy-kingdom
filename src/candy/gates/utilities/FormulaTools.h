#ifndef CANDY_FORMULATOOLS_H
#define CANDY_FORMULATOOLS_H

#include <string>
#include <vector>
#include <sstream>
#include <candy/gates/datastructures/BooleanCircuit.h>

using namespace std;

unsigned int countVariables(const string& formula);

unsigned int countClauses(const string& formula);

string generateDIMACSHeader(const string& formula);

/**
 * Given any sequence of literals l, return the maximum value of all abs(l)
 * @param literals the literals
 * @return the maximal variable
 */
unsigned int getMaxVar(const vector<int>& literals);

BooleanCircuit toCircuit(const TupleNotation& formula);

void writeToDIMACS(const TupleNotation& formula, const string& filename);

#endif //CANDY_FORMULATOOLS_H
