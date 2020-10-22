#ifndef CANDY_TUPLENOTATION_H
#define CANDY_TUPLENOTATION_H

#include <vector>
#include "candy/core/SolverTypes.h"

using namespace std;
using namespace Candy;

class TupleNotation {
private:
    vector<vector<int>> negativeClauses;
    vector<vector<int>> mixedClauses;
    vector<vector<int>> positiveClauses;
    vector<bool> marks;

public:
    const vector<vector<int>> &getNegativeClauses() const {
        return negativeClauses;
    }

    const vector<vector<int>> &getMixedClauses() const {
        return mixedClauses;
    }

    const vector<vector<int>> &getPositiveClauses() const {
        return positiveClauses;
    }

    static vector<int> asTuple(const Cl *cl) {
        vector<int> tuple;
        for (const Lit lit : *cl) {
            int sign = (lit.sign() ? -1 : 1);
            tuple.push_back(sign * (lit.var() + 1)); // todo: is this correct?
        }
        return tuple;
    }

    bool isMarked(const unsigned int clausePos) const {
        return marks[clausePos];
    }

    void mark(const unsigned int clausePos) {
        marks[clausePos] = true;
    }

    void unmark(const unsigned int clausePos) {
        marks[clausePos] = false;
    }

    static bool isNegativeClause(const Cl* cl) {
        for (const Lit lit : *cl) if (!lit.sign()) return false;
        return true;
    }

    static bool isNegativeClause(const vector<int>& clause) {
        for (const int lit : clause) if (lit > 0) return false;
        return true;
    }

    static bool isPositiveClause(const Cl* cl) {
        for (const Lit lit : *cl) if (lit.sign()) return false;
        return true;
    }

    static bool isPositiveClause(const vector<int>& clause) {
        for (const int lit : clause) if (lit < 0) return false;
        return true;
    }

    static bool isMixedClause(const Cl* cl) {
        bool foundPositive = false;
        bool foundNegative = false;
        for (const Lit lit : *cl) {
            if (lit.sign()) {
                foundNegative = true;
            } else {
                foundPositive = true;
            }
            if (foundPositive && foundNegative) return true;
        }
        return false;
    }

    static bool isMixedClause(const vector<int>& clause) {
        bool foundPositive = false;
        bool foundNegative = false;
        for (const int lit : clause) {
            if (lit < 0) {
                foundNegative = true;
            } else {
                foundPositive = true;
            }
            if (foundPositive && foundNegative) return true;
        }
        return false;
    }

    static vector<vector<int>> extractAllNegativeClauses(const For &formula) {
        vector<vector<int>> negativeClauses;
        for (const Cl* clause : formula) {
            if (isNegativeClause(clause)) negativeClauses.push_back(asTuple(clause));
        }
        return negativeClauses;
    }

    static vector<vector<int>> extractAllNegativeClauses(const vector<vector<int>> &formulaAsTuple) {
        vector<vector<int>> negativeClauses;
        for (const vector<int>& clause : formulaAsTuple) {
            if (isNegativeClause(clause)) negativeClauses.push_back(clause);
        }
        return negativeClauses;
    }

    static vector<vector<int>> extractAllPositiveClauses(const For &formula) {
        vector<vector<int>> positiveClauses;
        for (const Cl* cl : formula) {
            if (isPositiveClause(cl)) positiveClauses.push_back(asTuple(cl));
        }
        return positiveClauses;
    }

    static vector<vector<int>> extractAllPositiveClauses(const vector<vector<int>> &formulaAsTuple) {
        vector<vector<int>> positiveClauses;
        for (const vector<int>& clause : formulaAsTuple) {
            if (isPositiveClause(clause)) positiveClauses.push_back(clause);
        }
        return positiveClauses;
    }

    static vector<vector<int>> extractAllMixedClauses(const For &formula) {
        vector<vector<int>> mixedClauses;
        for (const Cl* cl : formula) {
            if (isMixedClause(cl)) mixedClauses.push_back(asTuple(cl));
        }
        return mixedClauses;
    }

    static vector<vector<int>> extractAllMixedClauses(const vector<vector<int>> &formulaAsTuple) {
        vector<vector<int>> mixedClauses;
        for (const vector<int>& clause : formulaAsTuple) {
            if (isMixedClause(clause)) mixedClauses.push_back(clause);
        }
        return mixedClauses;
    }

    static bool compareClauseLengths(const vector<int>& clause1, const vector<int>& clause2) {
        return clause1.size() < clause2.size();
    }

    static bool compareNegClauseLengths(const vector<int>& cl1, const vector<int>& cl2) {
        return cl1.size() > cl2.size();
    }

    static bool compareClausePolarity(const int lit1, const int lit2) {
        return lit1 <= lit2;
    }

    static unsigned numNegativeLiterals(const vector<int> *clause) {
        unsigned int numNegativeLiterals = 0;
        for (const int i : *clause) if (i < 0) numNegativeLiterals++;
        return numNegativeLiterals;
    }

    static unsigned int numNegativeLiterals(const vector<int>& clause) {
        unsigned int numNegativeLiterals = 0;
        for (const int i : clause) if (i < 0) numNegativeLiterals++;
        return numNegativeLiterals;
    }

    static unsigned numPositiveLiterals(const vector<int> *clause) {
        return clause->size() - numNegativeLiterals(clause);
    }

    static bool compareNumberOfNegativeLiterals(const vector<int>& cl1, const vector<int>& cl2) {
        return numNegativeLiterals(cl1) < numNegativeLiterals(cl2);
    }

    static bool compareNegNumberOfNegativeLiterals(const vector<int>& cl1, const vector<int>& cl2) {
        return numNegativeLiterals(cl1) > numNegativeLiterals(cl2);
    }

    static void sortByClauseLength(vector<vector<int>>& formula) {
        sort(formula.begin(), formula.end(), compareClauseLengths);
    }

    static void sortDecreasingByClauseLength(vector<vector<int>>& formula) {
        sort(formula.begin(), formula.end(), compareNegClauseLengths);
    }

    /**
     * First list all negative, then all positive literals
     * @param clause the clause to be sorted by polarity
     */
    static void sortByPolarity(vector<int>& clause) {
        sort(clause.begin(), clause.end(), compareClausePolarity);
    }

    static void sortByNumberOfNegativeLiterals(vector<vector<int>>& clause) {
        sort(clause.begin(), clause.end(), compareNumberOfNegativeLiterals);
    }
    static void sortDecreasingByNumberOfNegativeLiterals(vector<vector<int>>& clause) {
        sort(clause.begin(), clause.end(), compareNegNumberOfNegativeLiterals);
    }

    void establishOrdering() {
        sortByClauseLength(negativeClauses);
        sortDecreasingByClauseLength(mixedClauses);
        sortDecreasingByNumberOfNegativeLiterals(mixedClauses);
        for (vector<int>& clause : mixedClauses) sortByPolarity(clause);
        sortByClauseLength(positiveClauses);
    }

    void unmarkAll() {
        for (unsigned long i = 0; i < this->size(); i++) marks[i] = false;
    }

    TupleNotation() {
        this->negativeClauses = { };
        this->mixedClauses    = { };
        this->positiveClauses = { };
        this->marks           = { };
    }

    explicit TupleNotation(const For& formula) {
        this->negativeClauses = extractAllNegativeClauses(formula);
        this->mixedClauses = extractAllMixedClauses(formula);
        this->positiveClauses = extractAllPositiveClauses(formula);
        marks = vector<bool>(this->size());
        for (unsigned long i = 0; i < this->size(); i++) marks[i] = false;
        establishOrdering();
    }

    explicit TupleNotation(const vector<vector<int>>& formulaAsTuple) {
        this->negativeClauses = extractAllNegativeClauses(formulaAsTuple);
        this->mixedClauses = extractAllMixedClauses(formulaAsTuple);
        this->positiveClauses = extractAllPositiveClauses(formulaAsTuple);
        marks = vector<bool>(this->size());
        for (unsigned long i = 0; i < this->size(); i++) marks[i] = false;
        establishOrdering();
    }

    explicit TupleNotation(const vector<int>& clause) {
        vector<vector<int>> formula = { clause };
        this->negativeClauses = extractAllNegativeClauses(formula);
        this->mixedClauses = extractAllMixedClauses(formula);
        this->positiveClauses = extractAllPositiveClauses(formula);
        marks = vector<bool>(this->size());
        for (unsigned long i = 0; i < this->size(); i++) marks[i] = false;
        establishOrdering();
    }

    explicit TupleNotation(const string& formula) {

    }

    vector<vector<int>> getFormula() const {
        vector<vector<int>> formula;
        formula.reserve(negativeClauses.size() + mixedClauses.size() + positiveClauses.size());
        formula.insert(formula.begin(),
                       negativeClauses.begin(),
                       negativeClauses.end()); // todo: is this correct?
        formula.insert(formula.begin() + negativeClauses.size(),
                       mixedClauses.begin(),
                       mixedClauses.end()); // todo: is this correct?
        formula.insert(formula.begin() + negativeClauses.size() + mixedClauses.size(),
                       positiveClauses.begin(),
                       positiveClauses.end()); // todo: is this correct?
        return formula;
    }

    static TupleNotation getEmptyFormula() {
        return TupleNotation();
    }

    int operator[](unsigned int i) const {
        const auto& formula = this->getFormula();
        unsigned int numElements = 0;
        unsigned int j = 0;
        while (numElements < i) {
            numElements += formula.at(j).size();
            j++;
        }
        return formula.at(j).at(i - numElements); // todo: is this correct?
    }

    string toString() const {
        string out = "(";
        for (const auto& clause : this->getFormula()) {
            out.append("(");
            for (const auto& lit : clause) out.append(to_string(lit) + ", ");
            out = out.substr(0, out.size() - 2);
            out.append("), ");
        }
        out = out.substr(0, out.size() - 2);
        out.append(")");
        return out;
    }

    string toDIMACS() const {
        string out;
        const auto& formula = this->getFormula();
        for (const auto& clause : formula) {
            for (const auto& literal : clause) {
                out.append(to_string(literal) + " ");
            }
            out.append(" 0\n");
        }
        return out;
    }

    static bool isEmpty(const TupleNotation& formula) {
        return formula.negativeClauses.empty() && formula.mixedClauses.empty() && formula.positiveClauses.empty();
    }

    bool isEmpty() const {
        return negativeClauses.empty() && mixedClauses.empty() && positiveClauses.empty();
    }

    unsigned long size() const {
        return negativeClauses.size() + mixedClauses.size() + positiveClauses.size();
    }

    vector<int> clauseAt(unsigned int i) const {
        return this->getFormula().at(i);
    }
};

#endif //CANDY_TUPLENOTATION_H
