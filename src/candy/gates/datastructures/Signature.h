#ifndef CANDY_SIGNATURE_H
#define CANDY_SIGNATURE_H

#include <vector>
#include "TupleNotation.h"


using namespace std;

struct Tuple {
    unsigned long x;
    unsigned long y;
};

class Signature {
private:
    vector<unsigned int> negative;
    vector<Tuple> mixed;
    vector<unsigned int> positive;

public:
    explicit Signature(const TupleNotation& formula) {
        for (const auto& clause : formula.getNegativeClauses()) {
            negative.push_back(clause.size());
        }
        for (const auto& clause : formula.getMixedClauses()) {
            mixed.push_back(Tuple({ clause.size(), TupleNotation::numNegativeLiterals(clause) }));
        }
        for (const auto& clause : formula.getPositiveClauses()) {
            positive.push_back(clause.size());
        }
    }

    explicit Signature(const vector<int>& clause) {
        TupleNotation formula(clause);
        for (const auto& fclause : formula.getNegativeClauses()) {
            negative.push_back(clause.size());
        }
        for (const auto& fclause : formula.getMixedClauses()) {
            mixed.push_back(Tuple({ clause.size(), TupleNotation::numNegativeLiterals(clause) }));
        }
        for (const auto& fclause : formula.getPositiveClauses()) {
            positive.push_back(clause.size());
        }
    }

    static Signature getSignature(const vector<int>& clause) {
        return Signature(clause);
    }

    string toString() {
        string out = "(";
        for (const auto& i : negative) {
            out.append(to_string(i) + ", ");
        }
        out = out.substr(0, out.size() - 3);
        out.append(" | ");
        for (const auto& tuple : mixed) {
            out.append("(" + to_string(tuple.x) + ", " + to_string(tuple.y) + ")");
        }
        out.append(" | ");
        for (const auto& i : positive) {
            out.append(to_string(i));
        }
        out = out.substr(0, out.size() - 3);
        return out;
    }

    inline bool operator==(const Signature& signature) {
        return this->negative == signature.negative
               && this->mixed    == signature.mixed
               && this->positive == signature.positive;
    }

    inline bool operator !=(const Signature& signature) {
        return !(*this == signature);
    }
};

#endif //CANDY_SIGNATURE_H
