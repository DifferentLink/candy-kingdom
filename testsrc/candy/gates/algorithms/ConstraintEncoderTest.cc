#include <gtest/gtest.h>
#include "candy/gates/utilities/SequentialCounterEncoder.h"
#include <string>

using namespace std;

TEST(ConstraintEncoderTest, atMost_n_k_01) {
    string atMost_n2_k1 = SequentialCounterEncoder::atMostSEQ(2, {1, 2 }, 1);
    ASSERT_TRUE(atMost_n2_k1 == "-1 3 0\n-2 -3 0\n");
}

TEST(ConstraintEncoderTest, atMost_n_k_02) {
    string atMost = SequentialCounterEncoder::atMostSEQ(3, {1, 2, 3 }, 2);
    cout << atMost << endl;
}