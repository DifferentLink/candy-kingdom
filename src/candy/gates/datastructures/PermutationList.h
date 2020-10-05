#ifndef CANDY_PERMUTATIONLIST_H
#define CANDY_PERMUTATIONLIST_H

#include "Signature.h"

#include <vector>

using namespace std;

class PermutationList {
    static const vector<Tuple> permutationListOf(const TupleNotation& formula) { // todo: debug and correct pseudocode if necessary
        vector<Tuple> list;
        unsigned int formulaSize = formula.size();
        unsigned int start = 0;
        unsigned int finish = 0;
        Signature startSignature = Signature(formula.clauseAt(start));
        Signature finishSignature = startSignature;

        while (finish < formulaSize) {
            while (startSignature == finishSignature && finish + 1 < formulaSize) {
                finishSignature = Signature(formula.clauseAt(++finish));
            }
            list.push_back({ start, finish });
            start = finish + 1;
            finish = start;
            startSignature = Signature(formula.clauseAt(start));
            finishSignature = startSignature;
        }
        return list;
    }
};

#endif //CANDY_PERMUTATIONLIST_H
