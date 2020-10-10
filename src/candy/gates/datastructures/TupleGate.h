#ifndef CANDY_TUPLEGATE_H
#define CANDY_TUPLEGATE_H

#include "TupleNotation.h"

class TupleGate {
public:
    int out = 0;
    TupleNotation fwd, bwd;
    bool notMono = false;
    vector<int> inp;

    explicit TupleGate(const Gate& gate) {
        out = (gate.out.sign() ? - 1 : 1) * gate.out.var();
        fwd = TupleNotation(gate.fwd);
        bwd = TupleNotation(gate.bwd);
        notMono = gate.notMono;
        for (const auto& literal : gate.inp) {
            int var = literal.var();
            inp.push_back((literal.sign() ? - 1 : 1) * var);
        }
    }

    inline bool isDefined() const {
        return out == 0;
    }

    inline bool hasNonMonotonicParent() const {
        return notMono;
    }
};

#endif //CANDY_TUPLEGATE_H
