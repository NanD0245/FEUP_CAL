// By: Gonçalo Leão

#include "exercises.h"
#include <vector>
using namespace std;

int sum(const vector<int>& a) {
    int sum = 0;
    for (int i : a) {sum += i;}
    return sum;
}

void getcombos(vector<vector<int>> &possibilities,vector<int> &combination,vector<int> &coins,int T,int offset, int k) {
    if (k == 0) {
        if (sum(combination) == T) {
            possibilities.push_back(combination);
        }
        return;
    }
    for (int i = offset; i <= coins.size() - k; ++i) {
        combination.push_back(coins[i]);
        getcombos(possibilities,combination, coins,T,i+1, k-1);
        combination.pop_back();
    }
}

bool changeMakingBF(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    std::vector<int> coins;
    for (unsigned i = 0; i < n; ++i){ // push_back all coins in the pocket
        for (unsigned s = 0; s < Stock[i]; ++s) coins.push_back(C[i]);
        usedCoins[i] = 0;
    }
    vector<vector<int>> possibilities; vector<int> combination;
    for (int k = 0; k < coins.size(); k++) {
        getcombos(possibilities, combination, coins, T, 0, k);
        if (!possibilities.empty())
            break;
    }
    if (possibilities.empty()) return false;
    vector<int> res = possibilities[0];
    for (int i : res) {
        for (int c = 0; c < n; c++) {
            if (i == C[c])
                usedCoins[c]++;
        }
    }
    return true;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBF(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
}

TEST(TP1_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBF(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP1_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBF(C,Stock,n,1,usedCoins), false);
}