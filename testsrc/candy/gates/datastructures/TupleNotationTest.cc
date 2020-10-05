#include <gtest/gtest.h>
#include <candy/gates/datastructures/TupleNotation.h>

using namespace std;

TEST(TupleNotationTest, tupleNotation_01) {
    TupleNotation tupleNotation(TupleNotation({{3, -4, -6}, {1, 2, 3}, {1}, {-3}, {2, 4, -5}, {8, -5, -7, -9}}));
    cout << tupleNotation.toString() << endl;
}
