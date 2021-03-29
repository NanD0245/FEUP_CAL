#include "exercises.h"

#include <vector>

using namespace std;

bool changeMakingUnlimitedDP(unsigned int C[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    // TODO
    /*int min = 9999999;
    int count = 0;
    unsigned int coins[n];
    bool check;
    for (int i = 0; i < n; i++) {
        usedCoins[i] = 0;
        coins[i] = 0;
    }
    for (int j = n - 1; j >= 0; j--)
        while (T != 0) {
            check = false;
            for (int i = n - 1; i >= 0; i--) {
                if (C[i] <= T) {
                    T -= C[i];
                    coins[i]++;
                    count++;
                    check = true;
                    break;
                }
            }
            if (T == 0) break;
            if (!check) break;
        }
    if (min == 9999999)
        for (int i = 0; i < n; i++) {
            usedCoins[i] = coins[i];
        }
    if (!check) return false;
	return true;*/

    int INF = T + 1;
    int minCoins[T + 1];
    int lastCoin[T + 1];

    for (int i = 0; i <=T; i++)
        lastCoin[i] = 0;

    for (int i = 0; i < n; i++)
        usedCoins[i] = 0;

    minCoins[0] = 0;

    for (int k = 1; k <= T; k++)
        minCoins[k] = INF;

    for (int i = 1; i <= n; i++)
        for (int k = C[i - 1]; k <= T; k++)
            if (minCoins[k - C[i - 1]] + 1 < minCoins[k]) {
                minCoins[k] = 1 + minCoins[k - C[i - 1]];
                lastCoin[k] = C[i - 1];
            }

    if (minCoins[T] == INF) return false;

    for (int k = T; k > 0; k-= lastCoin[k]) {
        for (int i = 0; i < n; i++)
            if (C[i] == lastCoin[k])
                usedCoins[i]++;
    }

    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex2, hasChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 1);
    EXPECT_EQ(usedCoins[3], 3);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex2, hasChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);
}

TEST(TP4_Ex2, hasNoChange) {
    unsigned int C[] = {5,10};
    unsigned int n = 2;
    unsigned int usedCoins[2];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,1,usedCoins), false);
}