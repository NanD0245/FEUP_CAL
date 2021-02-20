// By: Gonçalo Leão

#include "exercises.h"

using namespace std;

int maxSubsequence(int A[], unsigned int n, unsigned int &a, unsigned int &b) {
    int maxSum = 0;
    for (size_t i = 0; i < n; i++) {
        int sum = 0;
        for (size_t j = i; j < n; j++) {
            sum += A[j];
            if (sum > maxSum) {
                maxSum = sum;
                a = i;
                b = j;
            }
        }
    }

    return maxSum;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}