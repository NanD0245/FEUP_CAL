// By: Gonçalo Leão

#include "exercises.h"
#include <vector>
using namespace std;

bool changeMakingGreedy(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    vector<int> allcoins;
    int t = T;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < Stock[i]; j++)
            allcoins.push_back(C[i]);
        usedCoins[i] = 0;
    }
    for (int i = allcoins.size()-1; i >= 0; i--) {
        if (allcoins[i] > t) continue;
        t -= allcoins[i];
        for (int j = 0; j < n; j++) {
            if (C[j] == allcoins[i]) {
                usedCoins[j]++;
            }
        }
    }
    if (t != 0)
        return false;
    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex4, hasGreedyChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingGreedy(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingGreedy(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);//1
    EXPECT_EQ(usedCoins[3], 2);//3
}

TEST(TP1_Ex4, hasNoGreedyChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingGreedy(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingGreedy(C,Stock,n,1,usedCoins), false);
}