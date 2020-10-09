#ifndef CANDY_IOTOOLS_H
#define CANDY_IOTOOLS_H

#include "candy/gates/GateProblem.h"
#include "FormulaTools.h"
#include <string>
#include <vector>

using namespace Candy;
using namespace std;

class IOTools {
public:
    static void writeStringToFile(const string &content, const string &filename) {
        ofstream oFile(filename);
        oFile << content << endl;
        oFile.close();
    }

    static void writeToDIMACS(const string& formula, const string& filename) {
        writeStringToFile(IOTools::generateDIMACSHeader(formula) + "\n" + formula, filename);
    }

    static void writeToDIMACS(const TupleNotation &formula, const string &filename) {
        writeToDIMACS(formula.toDIMACS(), filename);
    }

    static string generateDIMACSHeader(const string &formula) {
        return "p cnf "
               + to_string(FormulaTools::countVariables(formula)) + " "
               + to_string(FormulaTools::countClauses(formula));
    }

    /**
     * Calculate s_{i,j} in string representation, as described in the paper on sequential counter encodings by Sinz.
     * @param i as in the paper
     * @param j as in the paper
     * @param k as in the paper
     * @param sign the polarity of the variable
     * @param offset the number of total variables in the formula
     * @return integer representation of s_{i,j}
     */
    static string getsij(const unsigned int i, const unsigned int j, const unsigned int k, const bool sign = false,
                  const unsigned int offset = 0) {
        return (sign ? "-" : "") +  to_string(offset + (i - 1)*k + j);
    }

    static string getxi(const vector<Var> &constraintVars, unsigned int i, const bool sign = false) {
        return (sign ? "-" : "") + to_string(constraintVars.at(i - 1));
    }

    /**
     * Encodes an at most constraint using the sequential counter encoding by Sinz.
     * @param offset is the total number of variables in the formula to which this constraint will be added
     * @param constraintVars is the set of variables to be constrained
     * @param k is the maximum number of variables that are allowed to be true at the same time
     * @return the constraint as a string to be appended to the corresponding formula
     */
    static string atMostSEQ(const unsigned int offset, const vector<Var> &constraintVars, const unsigned int k) {
        unsigned int n = constraintVars.size();
        assert(n > 1 && k > 0);
        string cnfLTSEQ;
        cnfLTSEQ.append(getxi(constraintVars, 1, true)                  + " "
                        + getsij(1, 1, k, false, offset)                + " 0\n");   // -x_1      s_1,1
        for (unsigned int j = 2; j <= k; j++) {
            cnfLTSEQ.append(getsij(1, j, k, true, offset)               + " 0\n");  // -s_1,j
        }

        for (unsigned int i = 2; i < n; i++) {
            cnfLTSEQ.append(getxi(constraintVars, i, true)                 + " "
                            + getsij(i, 1, k, false, offset)               + " 0\n");  // -x_i      s_i,1
            cnfLTSEQ.append(getsij(i - 1, 1, k, true, offset)         + " "
                            + getsij(i, 1, k, false, offset)              + " 0\n");   // -s_i-1,1  s_i,1
            for (unsigned int j = 2; j <= k; j++) {
                cnfLTSEQ.append(getxi(constraintVars, i, true)             + " "
                                + getsij(i - 1, j - 1, k, true, offset) + " "
                                + getsij(i, j, k, false, offset)             + " 0\n");  // -x_i     -s_i-1,j-1  s_i,j
                cnfLTSEQ.append(getsij(i - 1, j, k, true, offset)       + " "
                                + getsij(i, j, k, false, offset)             + " 0\n");  //          -s_i-1,j    s_i,j
            }
            cnfLTSEQ.append(getxi(constraintVars, i, true)                 + " "
                            + getsij(i - 1, k, k, true, offset)           + " 0\n");  // -x_i      -s_i-1,k
        }
        cnfLTSEQ.append(getxi(constraintVars, n, true)                    + " "
                        + getsij(n - 1, k, k, true, offset)               + " 0\n");  // -x_n      -s_n-1,k
        return cnfLTSEQ;
    }

    /**
     * Encodes an at most constraint using the sequential counter encoding by Sinz.
     * @param offset is the total number of variables in the formula to which this constraint will be added
     * @param constraintLits is the set of literals to be constrained
     * (internally the corresponding variables are constraint)
     * @param k is the maximum number of variables that are allowed to be true at the same time
     * @return the constraint as a string to be appended to the corresponding formula
     */
    static string atMostSEQ(const unsigned int offset, const vector<Lit> &constraintLits, const unsigned int k) {
        vector<Var> vars;
        vars.reserve(constraintLits.size());
        for (const auto& lit : constraintLits) vars.push_back(lit.var());
        return atMostSEQ(offset, vars, k);
    }

    /**
     * Encodes an at most constraint using the sequential counter encoding by Sinz.
     * @param allVars is the set of all variables occurring in the formula
     * @param constraintVars is the set of variables to be constrained
     * @param k is the maximum number of variables that are allowed to be true at the same time
     * @return the constraint as a string to be appended to the corresponding formula
     */
    static string atMostSEQ(const vector<Var> &allVars, const vector<Var> &constraintVars, const unsigned int k) {
        unsigned int offset = 0;
        for (auto variable : allVars) {
            if (variable > offset) offset = variable;
        }
        return atMostSEQ(offset, constraintVars, k);
    }
};


#endif //CANDY_IOTOOLS_H