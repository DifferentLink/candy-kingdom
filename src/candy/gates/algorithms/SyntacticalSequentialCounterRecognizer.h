#ifndef CANDY_SYNTACTICALSEQUENTIALCOUNTERRECOGNIZER_H
#define CANDY_SYNTACTICALSEQUENTIALCOUNTERRECOGNIZER_H

#include <vector>
#include <map>
#include <candy/gates/datastructures/TupleNotation.h>
#include <candy/gates/datastructures/StructuralFormula.h>
#include "candy/gates/algorithms/Unificator.h"

using namespace std;

struct LocatedClause {
    vector<int> clause;
    unsigned int i;

    LocatedClause(const vector<int> &clause, const unsigned int i) : clause(clause), i(i) {}
};

struct ClausePair {
    vector<int> clause1;
    vector<int> clause2;

    ClausePair(const vector<int> &clause1, const vector<int> &clause2) : clause1(clause1), clause2(clause2) {}
};

class SyntacticalSequentialCounterRecognizer {


    static ClausePair recognizeLTSEQI(TupleNotation& formula,
                                               vector<bool>& deleted,
                                               map<string, unsigned int>& eta);
    static vector<vector<int>> recognizeLTSEQII_Base(TupleNotation& formula,
                                                     vector<bool>& deleted,
                                                     ClausePair& baseClauses,
                                                     map<string, unsigned int> eta);
    static vector<vector<int>> recognizeLTSEQII_Recursive(TupleNotation& formula,
                                                     vector<bool>& deleted,
                                                     map<string, unsigned int> eta);
    static vector<vector<int>> recognizeLTSEQIII_Base(TupleNotation& formula,
                                                     vector<vector<int>> baseClauses,
                                                     map<string, unsigned int> eta);
    static vector<vector<int>> recognizeLTSEQIII_Recursive(TupleNotation& formula,
                                                          vector<vector<int>> baseClauses,
                                                          map<string, unsigned int> eta);
    static StructuralFormula ltSEQGateRecognition(vector<vector<int>>& structuralEncoding,
                                                  map<string, unsigned int> eta);
    static StructuralFormula recognizeLTSEQ(TupleNotation& formula);
};


#endif //CANDY_SYNTACTICALSEQUENTIALCOUNTERRECOGNIZER_H
