#include "Unificator.h"
#include <candy/gates/datastructures/TupleNotation.h>
#include <candy/gates/datastructures/Signature.h>

/**
* Try to unify lit1 with literal2 using eta. If eta(lit1) is undefined and the polarities of the literals match
* then update eta by setting eta(var(lit1)) := var(literal2) and return true. If eta(var(lit1)) = var(literal2)
* return true. Otherwise return false;
* @param literal1
* @param literal2
* @param eta
* @return
 * int literal1, int literal2, map<unsigned int, unsigned int>& eta
*/
/*bool Unificator::unifyLiteral(int literal1, int literal2, map<unsigned int, unsigned int>& eta) {
    return true;
    if (isDefined(eta, literal1)) {
        return eta.at(literal1) == var(literal2);
    } else {
        eta.insert({ var(literal1), var(literal2) });
        return true;
    }
}
 */

/**
 *
 * @param clause1
 * @param clause2
 * @param eta
 * @return
 */
/*
bool Unificator::unifyClauseL2(const vector<int>& clause1,
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
            if (mx_1->second == y_1) {
                if (getPolarity(x_2) == getPolarity(y_2)) {
                    eta.insert({ var(x_2), var(y_2) });
                    return true;
                } else return false;
            } else if (mx_1->second == y_2) {
                if (getPolarity(x_2) == getPolarity(y_1)) {
                    eta.insert({ var(x_2), var(y_1) });
                    return true;
                } else return false;
            } else return false;
        } else if (!isDefined(eta, x_1) && isDefined(eta, x_2)) {
            auto mx_2 = eta.find(var(x_2));
            if (mx_2->second == y_1) {
                if (getPolarity(x_1) == getPolarity(y_1)) {
                    eta.insert({ var(x_1), var(y_1) });
                    return true;
                } else return false;
            } else if (mx_2->second == y_2) {
                if (getPolarity(x_1) == getPolarity(y_2)) {
                    eta.insert({ var(x_1), var(y_2) });
                } else return false;
            } else return false;
        } else {
            int mx_1 = eta.find(x_1)->second;
            int mx_2 = eta.find(x_2)->second;
            return ((mx_1 == y_1 && mx_2 == y_2) || (mx_1 == y_2 && mx_2 == y_1));
        }
    }
    return false; // todo: this line should be unreachable?
}

bool Unificator::unifyL3(const vector<int>& clause1,
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
        int mx_1 = eta.find(x_1)->second;
        if (mx_1 == y_1) {
            eta.insert({ var(x_2), var(y_2) });
            eta.insert({ var(x_3), var(y_3) });
            return true;
        } else if (mx_1 == y_2) {
            eta.insert({ var(x_2), var(y_1) });
            eta.insert({ var(x_3), var(y_3) });
            return true;
        } else if (mx_1 == y_3) {
            eta.insert({ var(x_2), var(y_1) });
            eta.insert({ var(x_3), var(y_2) });
            return true;
        } else return false;
    } else if (!isDefined(eta, x_1) && isDefined(eta, x_2) && !isDefined(eta, x_3)) {
        int mx_2 = eta.find(x_2)->second;
        if (mx_2 == y_1) {
            eta.insert({ var(x_1), var(y_2) });
            eta.insert({ var(x_3), var(y_3) });
            return true;
        } else if (mx_2 == y_2) {
            eta.insert({ var(x_1), var(y_1) });
            eta.insert({ var(x_3), var(y_3) });
            return true;
        } else if (mx_2 == y_3) {
            eta.insert({ var(x_1), var(y_1) });
            eta.insert({ var(x_3), var(y_2) });
            return true;
        } else return false;
    } else if (!isDefined(eta, x_1) && !isDefined(eta, x_2) && isDefined(eta, x_3)) {
        int mx_3 = eta.find(x_3)->second;
        if (mx_3 == y_1) {
            eta.insert({ var(x_1), var(y_2) });
            eta.insert({ var(x_2), var(y_3) });
            return true;
        } else if (mx_3 == y_2) {
            eta.insert({ var(x_1), var(y_1) });
            eta.insert({ var(x_2), var(y_3) });
            return true;
        } else if (mx_3 == y_3) {
            eta.insert({ var(x_1), var(y_1) });
            eta.insert({ var(x_2), var(y_2) });
            return true;
        } else return false;
    } else if (isDefined(eta, x_1) && isDefined(eta, x_2) && !isDefined(eta, x_3)) {
        int mx_1 = eta.find(x_1)->second;
        int mx_2 = eta.find(x_2)->second;
        if ((mx_1 == y_1 && mx_2 == y_2 && getPolarity(x_3) == getPolarity(y_3))
            || (mx_1 == y_2 && mx_2 == y_1 && getPolarity(x_3) == getPolarity(y_3))) {
            eta.insert({ var(x_3), var(y_3) });
        } else if ((mx_1 == y_1 && mx_2 == y_3 && getPolarity(x_3) == getPolarity(y_2))
                   || (mx_1 == y_3 && mx_2 == y_1 && getPolarity(x_3) == getPolarity(y_2))) {
            eta.insert({ var(x_3), var(y_2) });
        } else if ((mx_1 == y_2 && mx_2 == y_3 && getPolarity(x_3) == getPolarity(y_1))
                   || (mx_1 == y_3 && mx_2 == y_2 && getPolarity(x_3) == getPolarity(y_1))) {
            eta.insert({ var(x_3), var(y_1) });
        } else return false;
    } else if (isDefined(eta, x_1) && !isDefined(eta, x_2) && isDefined(eta, x_3)) { // todo: check this case
        int mx_1 = eta.find(x_1)->second;
        int mx_3 = eta.find(x_3)->second;
        if ((mx_1 == y_1 && mx_3 == y_2 && getPolarity(x_3) == getPolarity(y_3))
            || (mx_1 == y_2 && mx_3 == y_1 && getPolarity(x_3) == getPolarity(y_3))) {
            eta.insert({ var(x_3), var(y_3) });
        } else if ((mx_1 == y_1 && mx_3 == y_3 && getPolarity(x_3) == getPolarity(y_2))
                   || (mx_1 == y_3 && mx_3 == y_1 && getPolarity(x_3) == getPolarity(y_2))) {
            eta.insert({ var(x_3), var(y_2) });
        } else if ((mx_1 == y_2 && mx_3 == y_3 && getPolarity(x_3) == getPolarity(y_1))
                   || (mx_1 == y_3 && mx_3 == y_2 && getPolarity(x_3) == getPolarity(y_1))) {
            eta.insert({ var(x_3), var(y_1) });
        } else return false;
    } else if (!isDefined(eta, x_1) && isDefined(eta, x_2) && isDefined(eta, x_3)) {// todo: check this case
        int mx_2 = eta.find(x_2)->second;
        int mx_3 = eta.find(x_3)->second;
        if ((mx_2 == y_1 && mx_3 == y_2 && getPolarity(x_3) == getPolarity(y_3))
            || (mx_2 == y_2 && mx_3 == y_1 && getPolarity(x_3) == getPolarity(y_3))) {
            eta.insert({ var(x_3), var(y_3) });
        } else if ((mx_2 == y_1 && mx_3 == y_3 && getPolarity(x_3) == getPolarity(y_2))
                   || (mx_2 == y_3 && mx_3 == y_1 && getPolarity(x_3) == getPolarity(y_2))) {
            eta.insert({ var(x_3), var(y_2) });
        } else if ((mx_2 == y_2 && mx_3 == y_3 && getPolarity(x_3) == getPolarity(y_1))
                   || (mx_2 == y_3 && mx_3 == y_2 && getPolarity(x_3) == getPolarity(y_1))) {
            eta.insert({ var(x_3), var(y_1) });
        } else return false;
    } else {
        int mx_1 = eta.find(x_1)->second;
        int mx_2 = eta.find(x_2)->second;
        int mx_3 = eta.find(x_3)->second;
        return (mx_1 == y_1 && mx_2 == y_2 && mx_3 == y_3)
               || (mx_1 == y_1 && mx_2 == y_3 && mx_3 == y_2)
               || (mx_1 == y_2 && mx_2 == y_1 && mx_3 == y_3)
               || (mx_1 == y_2 && mx_2 == y_3 && mx_3 == y_1)
               || (mx_1 == y_3 && mx_2 == y_1 && mx_3 == y_2)
               || (mx_1 == y_3 && mx_2 == y_2 && mx_3 == y_1);
    }
    return false; // todo: this line should be unreachable?

}
*/
