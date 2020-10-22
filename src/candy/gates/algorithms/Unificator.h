#ifndef CANDY_UNIFICATOR_H
#define CANDY_UNIFICATOR_H

#include <string>
#include <map>
#include <vector>
#include <candy/gates/datastructures/TupleNotation.h>
#include <candy/gates/datastructures/Signature.h>

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
    static inline bool isDefined(const map<unsigned int, unsigned int>& eta, const int literal) {
        auto iterator = eta.find(var(literal));
        return iterator != eta.end();
    }

    /**
     * Check if eta is defined for the given variable in the given literal.
     * @param eta the function
     * @param lit the literal
     * @return whether eta(var(literal)) is defined
     */
    static inline bool isDefined(const map<string, unsigned int>& eta, const int literal) {
        auto iterator = eta.find(var(to_string(literal)));
        return iterator != eta.end();
    }

    static inline bool isDefined(const map<string, unsigned int>& eta, const string& literal) {
        auto iterator = eta.find(var(literal));
        return iterator != eta.end();
    }

    static inline pair<unsigned int, unsigned int> valueIsDefined(const map<unsigned int, unsigned int> eta, const int literal) {
        for (const auto& x : eta) {
            if (x.second == var(literal)) return x;
        }
        return { 0, 0 };
    }

    static unsigned int numNegativeLiterals(const vector<string>& clause) {
        unsigned int num = 0;
        for (const auto& literal : clause) if (getPolarity(literal) == -1) num++;
        return num;
    }

    /**
    * Try to unify lit1 with literal2 using eta. If eta(lit1) is undefined and the polarities of the literals match
    * then update eta by setting eta(var(lit1)) := var(literal2) and return true. If eta(var(lit1)) = var(literal2)
    * return true. Otherwise return false;
    * @param literal1
    * @param literal2
    * @param eta
    * @return
    */
    static bool unifyLiteral(int literal1, int literal2, map<unsigned int, unsigned int>& eta) {
        if (getPolarity(literal1) != getPolarity(literal2)) return false;
        pair<unsigned int, unsigned int> x = valueIsDefined(eta, literal2);
        if (isDefined(eta, literal1) && x.first != 0) {
            return var(literal1) == x.first;
        } else if (!isDefined(eta, literal1) && x.first == 0){
            eta.insert({ var(literal1), var(literal2) });
            return true;
        }
        return false;
    }

    static bool unifyClauseL2(const vector<int>& clause1,
                              const vector<int>& clause2,
                              map<unsigned int, unsigned int>& eta) {
        if (clause1.size() != 2 || clause2.size() != 2) return false;

        const int x_1 = clause1.at(0);
        const int x_2 = clause1.at(1);
        const int y_1 = clause2.at(0);
        const int y_2 = clause2.at(1);

        if (TupleNotation::numNegativeLiterals(clause1) == TupleNotation::numNegativeLiterals(clause2)) {
            if (!isDefined(eta, x_1) && !isDefined(eta, x_2)) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_2), var(y_2) });
                return true;
            } else if (isDefined(eta, x_1) && !isDefined(eta, x_2)) {
                auto mx_1 = eta.find(var(x_1));
                if (mx_1->second == var(y_1)) {
                    if (getPolarity(x_2) == getPolarity(y_2)) {
                        eta.insert({ var(x_2), var(y_2) });
                        return true;
                    } else return false;
                } else if (mx_1->second == var(y_2)) {
                    if (getPolarity(x_2) == getPolarity(y_1)) {
                        eta.insert({ var(x_2), var(y_1) });
                        return true;
                    } else return false;
                } else return false;
            } else if (!isDefined(eta, x_1) && isDefined(eta, x_2)) {
                auto mx_2 = eta.find(var(x_2));
                if (mx_2->second == var(y_1)) {
                    if (getPolarity(x_1) == getPolarity(y_1)) {
                        eta.insert({ var(x_1), var(y_1) });
                        return true;
                    } else return false;
                } else if (mx_2->second == var(y_2)) {
                    if (getPolarity(x_1) == getPolarity(y_2)) {
                        eta.insert({ var(x_1), var(y_2) });
                    } else return false;
                } else return false;
            } else {
                unsigned int mx_1 = eta.find(var(x_1))->second;
                unsigned int mx_2 = eta.find(var(x_2))->second;
                return ((mx_1 == var(y_1) && mx_2 == var(y_2)) || (mx_1 == var(y_2) && mx_2 == var(y_1)));
            }
        }
        return false; // todo: this line should be unreachable?
    }

    static bool unifyClauseL2(const TupleNotation& clause1,
                              const TupleNotation& clause2,
                              map<unsigned int, unsigned int> eta) {
        assert(clause1.size() == 1 && clause2.size() == 1);
        return unifyClauseL2(clause1.getFormula().at(0), clause2.getFormula().at(0), eta);
    }

    static bool unifyClauseL2(const vector<int>& clause1,
                              const TupleNotation& clause2,
                              map<unsigned int, unsigned int> eta) {
        assert(clause2.size() == 1);
        return unifyClauseL2(clause1, clause2.getFormula().at(0), eta);
    }

    static bool unifyClauseL2(const TupleNotation& clause1,
                              const vector<int>& clause2,
                              map<unsigned int, unsigned int> eta) {
        assert(clause1.size() == 1);
        return unifyClauseL2(clause1.getFormula().at(0), clause2, eta);
    }

    static bool unifyClauseL2(const vector<string>& clause1,
                              const vector<int>& clause2,
                              map<string, unsigned int>& eta) {
        if (clause1.size() != 2 || clause2.size() != 2) return false;

        const string x_1 = clause1.at(0);
        const string x_2 = clause1.at(1);
        const int y_1 = clause2.at(0);
        const int y_2 = clause2.at(1);

        if (numNegativeLiterals(clause1) == TupleNotation::numNegativeLiterals(clause2)) {
            if (!isDefined(eta, var(x_1)) && !isDefined(eta, var(x_2))) {
                return true;
            } else if (isDefined(eta, x_1) && !isDefined(eta, x_2)) {
                auto mx_1 = eta.find(var(x_1));
                if (mx_1->second == var(y_1)) {
                    return getPolarity(x_2) == getPolarity(y_2);
                } else if (mx_1->second == var(y_2)) {
                    return getPolarity(x_2) == getPolarity(y_1);
                } else {
                    return false;
                }
            } else if (!isDefined(eta, x_1) && isDefined(eta, x_2)) {
                auto mx_2 = eta.find(var(x_2));
                if (mx_2->second == var(y_1)) {
                    return getPolarity(x_1) == getPolarity(y_2);
                } else if (mx_2->second == var(y_2)) {
                    return getPolarity(x_1) == getPolarity(y_1);
                } else return false;
            } else {
                unsigned int mx_1 = eta.find(var(x_1))->second;
                unsigned int mx_2 = eta.find(var(x_2))->second;
                return ((mx_1 == var(y_1) && mx_2 == var(y_2)) || (mx_1 == var(y_2) && mx_2 == var(y_1)));
            }
        }
        return false; // todo: this line should be unreachable?
    }

    static bool unifyClauseL2U(const vector<string>& clause1, // todo: this version updates eta with the first match
                              const vector<int>& clause2,
                              map<string, unsigned int>& eta) {
        if (clause1.size() != 2 || clause2.size() != 2) return false;

        const string x_1 = clause1.at(0);
        const string x_2 = clause1.at(1);
        const int y_1 = clause2.at(0);
        const int y_2 = clause2.at(1);

        if (numNegativeLiterals(clause1) == TupleNotation::numNegativeLiterals(clause2)) {
            if (!isDefined(eta, var(x_1)) && !isDefined(eta, var(x_2))) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_2), var(y_2) });
                return true;
            } else if (isDefined(eta, x_1) && !isDefined(eta, x_2)) {
                auto mx_1 = eta.find(var(x_1));
                if (mx_1->second == var(y_1)) {
                    if (getPolarity(x_2) == getPolarity(y_2)) {
                        eta.insert({ var(x_2), var(y_2) });
                        return true;
                    } else return false;
                } else if (mx_1->second == var(y_2)) {
                    if (getPolarity(x_2) == getPolarity(y_1)) {
                        eta.insert({ var(x_2), var(y_1) });
                        return true;
                    } else return false;
                } else return false;
            } else if (!isDefined(eta, x_1) && isDefined(eta, x_2)) {
                auto mx_2 = eta.find(var(x_2));
                if (mx_2->second == var(y_1)) {
                    if (getPolarity(x_1) == getPolarity(y_1)) {
                        eta.insert({ var(x_1), var(y_1) });
                        return true;
                    } else return false;
                } else if (mx_2->second == var(y_2)) {
                    if (getPolarity(x_1) == getPolarity(y_2)) {
                        eta.insert({ var(x_1), var(y_2) });
                    } else return false;
                } else return false;
            } else {
                unsigned int mx_1 = eta.find(var(x_1))->second;
                unsigned int mx_2 = eta.find(var(x_2))->second;
                return ((mx_1 == var(y_1) && mx_2 == var(y_2)) || (mx_1 == var(y_2) && mx_2 == var(y_1)));
            }
        }
        return false; // todo: this line should be unreachable?
    }

    static bool unifyClauseL3(const vector<int>& clause1,
                        const vector<int>& clause2,
                        map<unsigned int, unsigned int>& eta) {
        if (clause1.size() != 3 || clause2.size() != 3) return false;
        if (Signature::getSignature(clause1) != Signature::getSignature(clause2)) return false;

        const int x_1 = clause1.at(0);
        const int x_2 = clause1.at(1);
        const int x_3 = clause1.at(2);
        const int y_1 = clause2.at(0);
        const int y_2 = clause2.at(1);
        const int y_3 = clause2.at(2);

        if (!isDefined(eta, x_1) && !isDefined(eta, x_2) && !isDefined(eta, x_3)) {
            eta.insert({ var(x_1), var(y_1) });
            eta.insert({ var(x_2), var(y_2) });
            eta.insert({ var(x_3), var(y_3) });
            return true;
        } else if (isDefined(eta, x_1) && !isDefined(eta, x_2) && !isDefined(eta, x_3)) {
            unsigned int mx_1 = eta.find(x_1)->second;
            if (mx_1 == var(y_1)) {
                eta.insert({ var(x_2), var(y_2) });
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if (mx_1 == var(y_2)) {
                eta.insert({ var(x_2), var(y_1) });
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if (mx_1 == var(y_3)) {
                eta.insert({ var(x_2), var(y_1) });
                eta.insert({ var(x_3), var(y_2) });
                return true;
            } else return false;
        } else if (!isDefined(eta, x_1) && isDefined(eta, x_2) && !isDefined(eta, x_3)) {
            unsigned int mx_2 = eta.find(x_2)->second;
            if (mx_2 == var(y_1)) {
                eta.insert({ var(x_1), var(y_2) });
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if (mx_2 == var(y_2)) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if (mx_2 == var(y_3)) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_3), var(y_2) });
                return true;
            } else return false;
        } else if (!isDefined(eta, x_1) && !isDefined(eta, x_2) && isDefined(eta, x_3)) {
            unsigned int mx_3 = eta.find(x_3)->second;
            if (mx_3 == var(y_1)) {
                eta.insert({ var(x_1), var(y_2) });
                eta.insert({ var(x_2), var(y_3) });
                return true;
            } else if (mx_3 == var(y_2)) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_2), var(y_3) });
                return true;
            } else if (mx_3 == var(y_3)) {
                eta.insert({ var(x_1), var(y_1) });
                eta.insert({ var(x_2), var(y_2) });
                return true;
            } else return false;
        } else if (isDefined(eta, x_1) && isDefined(eta, x_2) && !isDefined(eta, x_3)) {
            unsigned int mx_1 = eta.find(var(x_1))->second;
            unsigned int mx_2 = eta.find(var(x_2))->second;
            if ((mx_1== var(y_1) && mx_2== var(y_2) && getPolarity(x_3) == getPolarity(y_3))
                || (mx_1== var(y_2) && mx_2== var(y_1)&& getPolarity(x_3) == getPolarity(y_3))) {
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if ((mx_1== var(y_1)&& mx_2== var(y_3) && getPolarity(x_3) == getPolarity(y_2))
                       || (mx_1== var(y_3) && mx_2== var(y_1)&& getPolarity(x_3) == getPolarity(y_2))) {
                eta.insert({ var(x_3), var(y_2) });
                return true;
            } else if ((mx_1== var(y_2) && mx_2== var(y_3) && getPolarity(x_3) == getPolarity(y_1))
                       || (mx_1== var(y_3) && mx_2== var(y_2) && getPolarity(x_3) == getPolarity(y_1))) {
                eta.insert({ var(x_3), var(y_1) });
                return true;
            } else return false;
        } else if (isDefined(eta, x_1) && !isDefined(eta, x_2) && isDefined(eta, x_3)) { // todo: check this case
            unsigned int mx_1 = eta.find(var(x_1))->second;
            unsigned int mx_3 = eta.find(var(x_3))->second;
            if ((mx_1== var(y_1)&& mx_3== var(y_2) && getPolarity(x_3) == getPolarity(y_3))
                || (mx_1== var(y_2) && mx_3== var(y_1)&& getPolarity(x_3) == getPolarity(y_3))) {
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if ((mx_1== var(y_1)&& mx_3== var(y_3) && getPolarity(x_3) == getPolarity(y_2))
                       || (mx_1== var(y_3) && mx_3== var(y_1)&& getPolarity(x_3) == getPolarity(y_2))) {
                eta.insert({ var(x_3), var(y_2) });
                return true;
            } else if ((mx_1== var(y_2) && mx_3== var(y_3) && getPolarity(x_3) == getPolarity(y_1))
                       || (mx_1== var(y_3) && mx_3== var(y_2) && getPolarity(x_3) == getPolarity(y_1))) {
                eta.insert({ var(x_3), var(y_1) });
                return true;
            } else return false;
        } else if (!isDefined(eta, x_1) && isDefined(eta, x_2) && isDefined(eta, x_3)) {// todo: check this case
            unsigned int mx_2 = eta.find(var(x_2))->second;
            unsigned int mx_3 = eta.find(var(x_3))->second;
            if ((mx_2== var(y_1)&& mx_3== var(y_2) && getPolarity(x_3) == getPolarity(y_3))
                || (mx_2== var(y_2) && mx_3== var(y_1)&& getPolarity(x_3) == getPolarity(y_3))) {
                eta.insert({ var(x_3), var(y_3) });
                return true;
            } else if ((mx_2== var(y_1)&& mx_3== var(y_3) && getPolarity(x_3) == getPolarity(y_2))
                       || (mx_2== var(y_3) && mx_3== var(y_1)&& getPolarity(x_3) == getPolarity(y_2))) {
                eta.insert({ var(x_3), var(y_2) });
                return true;
            } else if ((mx_2== var(y_2) && mx_3== var(y_3) && getPolarity(x_3) == getPolarity(y_1))
                       || (mx_2== var(y_3) && mx_3== var(y_2) && getPolarity(x_3) == getPolarity(y_1))) {
                eta.insert({ var(x_3), var(y_1) });
                return true;
            } else return false;
        } else {
            unsigned int mx_1 = eta.find(var(x_1))->second;
            unsigned int mx_2 = eta.find(var(x_2))->second;
            unsigned int mx_3 = eta.find(var(x_3))->second;
            return (mx_1 == var(y_1) && mx_2 == var(y_2) && mx_3 == var(y_3))
                   || (mx_1 == var(y_1) && mx_2 == var(y_3) && mx_3 == var(y_2))
                   || (mx_1 == var(y_2) && mx_2 == var(y_1) && mx_3 == var(y_3))
                   || (mx_1 == var(y_2) && mx_2 == var(y_3) && mx_3 == var(y_1))
                   || (mx_1 == var(y_3) && mx_2 == var(y_1) && mx_3 == var(y_2))
                   || (mx_1 == var(y_3) && mx_2 == var(y_2) && mx_3 == var(y_1));
        }
    }
};


#endif //CANDY_UNIFICATOR_H
