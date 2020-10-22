#include <candy/gates/utilities/FormulaTools.h>
#include "SyntacticalSequentialCounterRecognizer.h"



LocatedClause findFirstClauseL2(TupleNotation& formula,
                         vector<bool>& deleted,
                         vector<string>& pattern,
                         map<string, unsigned int>& eta) {
    vector<int> clause;
    for (unsigned int i = 0; i < formula.size(); i++) {
        if (!deleted.at(i)) {
            clause = formula.clauseAt(i);
            if (Unificator::unifyClauseL2(pattern, clause, eta)) return { clause, i };
        }
    }
    return { BooleanCircuit::emptyClause, 0 };
}

ClausePair SyntacticalSequentialCounterRecognizer::recognizeLTSEQI(TupleNotation &formula,
                                                                            vector<bool>& deleted,
                                                                            map<string, unsigned int>& eta) {
    vector<LocatedClause> toUndelete;
    vector<string> c1pattern = { "-x_1", "s_1_1" };
    vector<string> c2pattern = { "-x_2", "-s_1_1" };
    LocatedClause c1({ }, 0);
    LocatedClause c2({ }, 0);
    c1 = findFirstClauseL2(formula, deleted, c1pattern, eta);
    if (!c1.clause.empty()) {
        deleted[c1.i] = true;
        c2 = findFirstClauseL2(formula, deleted, c2pattern, eta);
        if (!c2.clause.empty()) {
            return { c1.clause, c2.clause };
        } else {
            deleted[c1.i] = false;
        }
    }
    return { { }, { } };
}

vector<vector<int>>
SyntacticalSequentialCounterRecognizer::recognizeLTSEQII_Base(TupleNotation &formula,
                                                              vector<bool>& deleted,
                                                              ClausePair& baseClauses,
                                                              map<string, unsigned int> eta) {
    vector<vector<int>> e_n_1;
    vector<string> nextPattern = { "-s_1_1", "s_2_1"};
    LocatedClause c_im = findFirstClauseL2(formula, deleted, nextPattern, eta);
    vector<string> pClausePattern = { "-x_2", "s_2_1" };
    while (!c_im.clause.empty()) {
        LocatedClause pClause = findFirstClauseL2(formula, deleted, pClausePattern, eta);
        if (!pClause.clause.empty()) {
            e_n_1 = recognizeLTSEQII_Recursive(formula, deleted, eta);
        } else {

        }
    }
    return {{  }}; // todo: FormulaTools::unionOf({ e_n_1, (vector<vector<int>>){ baseClauses.clause1, baseClauses.clause2 } });
}

vector<vector<int>> SyntacticalSequentialCounterRecognizer::recognizeLTSEQII_Recursive(TupleNotation &formula,
                                                                                       vector<bool>& deleted,
                                                                                       map<string, unsigned int> eta) {

    return vector<vector<int>>();
}

vector<vector<int>>
SyntacticalSequentialCounterRecognizer::recognizeLTSEQIII_Base(TupleNotation &formula, vector<vector<int>> baseClauses,
                                                               map<string, unsigned int> eta) {
    return vector<vector<int>>();
}

vector<vector<int>> SyntacticalSequentialCounterRecognizer::recognizeLTSEQIII_Recursive(TupleNotation &formula,
                                                                                        vector<vector<int>> baseClauses,
                                                                                        map<string, unsigned int> eta) {
    return vector<vector<int>>();
}

StructuralFormula SyntacticalSequentialCounterRecognizer::ltSEQGateRecognition(vector<vector<int>> &structuralEncoding,
                                                                               map<string, unsigned int> eta) {
    return StructuralFormula(TupleNotation(), { });
}

StructuralFormula SyntacticalSequentialCounterRecognizer::recognizeLTSEQ(TupleNotation &formula) {
    return StructuralFormula(TupleNotation(), { });
}
