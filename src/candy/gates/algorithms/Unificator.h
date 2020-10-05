
#ifndef CANDY_UNIFICATOR_H
#define CANDY_UNIFICATOR_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class Unificator {
public:
    /**
     * @param literal
     * @return the variable corresponding to the literal
     */
    static inline unsigned int var(const int literal) {
        return abs(literal);
    }

    /**
     * @param literal
     * @return the variable corresponding to the literal
     */
    static inline string var(const string& literal) {
        return (literal.at(0) == '-' ? literal.substr(1, literal.size() - 1) : literal);
    }

    /**
     * @param literal get the polarity of this literal
     * @return -1 if the literal is negative, 1 if the literal is positive
     */
    static inline int getPolarity(const int literal) {
        return (literal < 0 ? -1 : 1);
    }

    /**
     * @param literal get the polarity of this literal
     * @return -1 if the literal is negative, 1 if the literal is positive
     */
    static inline int getPolarity(const string& literal) {
        return (literal.at(0) == '-' ? -1 : 1); // todo: does this work as expected?
    }

    /**
     * Check if eta is defined for the given variable in the given literal.
     * @param eta the function
     * @param lit the literal
     * @return whether eta(var(literal)) is defined
     */
    static inline bool isDefined(const map<unsigned int, unsigned int> eta, const int literal) {
        auto iterator = eta.find(var(literal));
        return iterator != eta.end();
    }

    /**
     * Check if eta is defined for the given variable in the given literal.
     * @param eta the function
     * @param lit the literal
     * @return whether eta(var(literal)) is defined
     */
    static inline bool isDefined(const map<string, unsigned int>& eta, const string& literal) {
        auto iterator = eta.find(var(literal));
        return iterator != eta.end();
    }

    static inline bool unifyLiteral(int literal1, int literal2, map<unsigned int, unsigned int>& eta);

    static bool unifyClauseL2(const vector<int>& clause1,
                              const vector<int>& clause2,
                              map<unsigned int, unsigned int>& eta);

    static bool unifyL3(const vector<int>& clause1,
                        const vector<int>& clause2,
                        map<unsigned int, unsigned int>& eta);

    };


#endif //CANDY_UNIFICATOR_H
