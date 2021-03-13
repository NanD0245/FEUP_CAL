#include "exercises.h"
#include <climits>
#include <iostream>

using namespace std;

int max(int a, int b) { return (a > b) ? a : b; }

int max(int a, int b, int c) { return max(max(a, b), c); }

int maxCrossingSubsequence(int A[], int i, int j, int& iRes, int& jRes){
    int mid = (i+j)/2;

    // Start at mid, and go left to find max
    int leftSum = 0, maxLeftSum = -99999;
    for (int l = mid; l >= 0; --l){
        leftSum += A[l];
        if (leftSum > maxLeftSum){
            maxLeftSum = leftSum;
            iRes = l;
        }
    }

    // Start after mid, and go right to find max
    int rightSum = 0, maxRightSum = -99999;
    for (int l = mid + 1; l <= j; ++l){
        rightSum += A[l];
        if (rightSum > maxRightSum){
            maxRightSum = rightSum;
            jRes = l;
        }
    }

    return maxLeftSum + maxRightSum;
}


int maxSubsequenceDCRec(int A[], int i, int j, int& bestI, int& bestJ, int& curMax){
    if (i == j){
        return A[i];
    }

    int mid = (i+j)/2;
    int leftMax = maxSubsequenceDCRec(A, i, mid, bestI, bestJ, curMax);
    int rightMax = maxSubsequenceDCRec(A, mid + 1, j, bestI, bestJ, curMax);
    int crossingI = mid, crossingJ = mid;
    int crossingMax = maxCrossingSubsequence(A, i, j, crossingI, crossingJ);

    int maxIdx = max(leftMax,rightMax,crossingMax);
        if (maxIdx == leftMax) {
            if (leftMax > curMax) {
                curMax = leftMax;
                bestI = i;
                bestJ = mid;
            }
        }
        else if (maxIdx == rightMax) {
            if (rightMax > curMax) {
                curMax = rightMax;
                bestI = mid + 1;
                bestJ = j;
            }
        }
        else if (maxIdx == crossingMax) {
            if (crossingMax > curMax) {
                curMax = crossingMax;
                bestI = crossingI;
                bestJ = crossingJ;
            }
        }
    return maxIdx;
}

int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    if (n == 0) return 0;
    int curMax = INT_MIN;
    int max = maxSubsequenceDCRec(A, 0, n-1, i, j, curMax);
    return max;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    //EXPECT_EQ(maxSubArraySum(A1,0,3), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}