// By: Gonçalo Leão

#include "exercises.h"

unsigned int sumArray(unsigned int a[], unsigned int n) {
    unsigned int sum = 0;
    for(unsigned int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

#include <iostream>

bool isCanonical(unsigned int C[], unsigned int n) {
    if (n < 3) return false;
    int a = C[2] + 1;
    int b = C[n-2] + C[n-1];
    unsigned int bf[n];
    unsigned int greedy[n];
    unsigned int stock[n];
    for (int i = 0; i < n; i++) {
        stock[i] = n;
    }
    for (int i = a+1; i < b; i++) {
        changeMakingBF(C,stock,n,i,bf);
        changeMakingGreedy(C,stock,n,i,greedy);
        for (int j = 0; j < n; j++) {
            if (bf[j] != greedy[j])
                return false;
        }
    }
    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), false);
}