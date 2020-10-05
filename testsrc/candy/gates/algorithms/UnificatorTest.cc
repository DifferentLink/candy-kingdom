#include <candy/gates/algorithms/Unificator.h>
#include <gtest/gtest.h>
#include <map>

using namespace std;

TEST(UnificatorTest, unifyLiteral_01) {
    map<unsigned int, unsigned int> eta;
    ASSERT_TRUE(Unificator::unifyLiteral(1, 1, eta));
    unsigned int y = eta.at(1);
    ASSERT_TRUE(y == 1);
    ASSERT_FALSE(Unificator::unifyLiteral(42, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-1, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-42, 1, eta));
}

TEST(UnificatorTest, unifyLiteral_02) {
    map<unsigned int, unsigned int> eta;
    eta.insert({1, 1 });
    ASSERT_TRUE(Unificator::unifyLiteral(1, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(42, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-1, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-42, 1, eta));
}

TEST(UnificatorTest, unifyLiteral_03) {
    map<unsigned int, unsigned int> eta;
    eta.insert({42, 1 });
    ASSERT_FALSE(Unificator::unifyLiteral(1, 1, eta));
    ASSERT_TRUE(Unificator::unifyLiteral(42, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-1, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-42, 1, eta));
}

TEST(UnificatorTest, unifyLiteral_04) {
    map<unsigned int, unsigned int> eta;
    eta.insert({1, 2 });
    ASSERT_FALSE(Unificator::unifyLiteral(1, 1, eta));
    ASSERT_TRUE(Unificator::unifyLiteral(42, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-1, 1, eta));
    ASSERT_FALSE(Unificator::unifyLiteral(-42, 1, eta));
}

TEST(UnificatorTest, unifyClauseL2_01) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, 2 };
    vector<int> clause2 { 1, 2 };
    vector<int> clause3 { 3, 42 };
    vector<int> clause4 { -1, 2 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause2, eta));
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause4, eta));
}

TEST(UnificatorTest, unifyClauseL2_02) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, 2 };
    vector<int> clause3 { 3, 42 };
    vector<int> clause4 { -1, 2 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause3, eta));
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause4, eta));
}

TEST(UnificatorTest, unifyClauseL2_03) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, 2 };
    vector<int> clause2 { -1, 2 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause2, eta));
}

TEST(UnificatorTest, unifyClauseL2_04) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, -2 };
    vector<int> clause2 { -1, 2 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause2, eta));
    vector<int> clause3 { 1, -2 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL2_05) {
    map<unsigned int, unsigned int> eta;
    eta.insert({ 1, 2 });
    vector<int> clause1 { 1, -2 };
    vector<int> clause2 { -1, 2 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause2, eta));
    vector<int> clause3 { 1, -2 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL2_06) {
    map<unsigned int, unsigned int> eta;
    eta.insert({ 1, 1 });
    vector<int> clause1 { 1, -2 };
    vector<int> clause2 { -1, 2 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause2, eta));
    vector<int> clause3 { 1, -2 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL2_07) {
    map<unsigned int, unsigned int> eta;
    eta.insert({ 1, 3 });
    eta.insert({ 2, 4 });
    vector<int> clause1 { 1, -2 };
    vector<int> clause2 { 3, -4 };
    ASSERT_TRUE(Unificator::unifyClauseL2(clause1, clause2, eta));
    vector<int> clause3 { 1, -2 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL3_01) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, 2, 3 };
    vector<int> clause2 { 4, 5, 6 };
    ASSERT_TRUE(Unificator::unifyClauseL3(clause1, clause2, eta));
    vector<int> clause3 { 1, -2, 3 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL3_02) {
    map<unsigned int, unsigned int> eta;
    vector<int> clause1 { 1, -2, 3 };
    vector<int> clause2 { 4, -5, 6 };
    ASSERT_TRUE(Unificator::unifyClauseL3(clause1, clause2, eta));
    vector<int> clause3 { 1, -2, 3 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL3_03) {
    map<unsigned int, unsigned int> eta;
    eta.insert({ 1, 4 });
    vector<int> clause1 { 1, -2, 3 };
    vector<int> clause2 { 4, -5, 6 };
    ASSERT_TRUE(Unificator::unifyClauseL3(clause1, clause2, eta));
    vector<int> clause3 { 1, -2, 3 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}

TEST(UnificatorTest, unifyClauseL3_04) {
    map<unsigned int, unsigned int> eta;
    eta.insert({ 1, 4 });
    eta.insert({ 2, 5 });
    vector<int> clause1 { 1, -2, 3 };
    vector<int> clause2 { 4, -5, 6 };
    ASSERT_TRUE(Unificator::unifyClauseL3(clause1, clause2, eta));
    vector<int> clause3 { 1, -2, 3 };
    ASSERT_FALSE(Unificator::unifyClauseL2(clause1, clause3, eta));
}