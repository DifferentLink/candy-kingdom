#include <gtest/gtest.h>
#include <candy/gates/datastructures/TupleNotation.h>
#include "candy/core/SolverTypes.h"

using namespace Candy;
using namespace std;

TEST(TupleNotationTest, tupleNotation_01) {
    TupleNotation tupleNotation(TupleNotation({{3, -4, -6}, {1, 2, 3}, {1}, {-3}, {2, 4, -5}, {8, -5, -7, -9}}));
    cout << tupleNotation.toString() << endl;
}

TEST(TupleNotationTest, tupleNotation_02) {
    For andFor({ new Cl({~1_L, 2_L}), new Cl({~1_L, 3_L}), new Cl({1_L, ~2_L, ~3_L}) });
    TupleNotation tupleNotation(andFor);
    cout << tupleNotation.toString() << endl;
}

TEST(TupleNotationTest, tupleNotation_03) {
    TupleNotation tupleNotation(TupleNotation({{3, -4, -6}, {1, 2, 3}, {1}, {-3}, {2, 4, -5}, {8, -5, -7, -9}}));
    cout << tupleNotation.toString() << endl;
    for (unsigned int i = 0; i < tupleNotation.size(); i++) {
        for (const auto& literal : tupleNotation.clauseAt(i)) {
            cout << literal << " ";
        }
    }
}