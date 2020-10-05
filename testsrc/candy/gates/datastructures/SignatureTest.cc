#include <gtest/gtest.h>
#include <candy/gates/datastructures/Signature.h>

using namespace std;

TEST(SignatureTest, signature_01) {
    Signature signature(TupleNotation({{3, -4, -6}, {1, 2, 3}, {1}, {-3}, {2, 4, -5}, {8, -5, -7, -9}}));
    cout << signature.toString() << endl;
}
