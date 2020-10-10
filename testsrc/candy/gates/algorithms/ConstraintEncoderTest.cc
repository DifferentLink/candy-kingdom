#include <gtest/gtest.h>
#include "candy/gates/utilities/IOTools.h"
#include <string>

using namespace std;

TEST(ConstraintEncoderTest, atMost_n_k_01) {
    string atMost_n2_k1 = IOTools::atMostSEQ(2, { 1, 2 }, 1);
    ASSERT_TRUE(atMost_n2_k1 == "-1 3 0\n-2 -3 0\n");
}
