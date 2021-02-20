// By: Gonçalo Leão

#include "exercises.h"
#include <iostream>
using namespace std;

bool sum3(unsigned int T, unsigned int nums[3]) {
    bool check = false;
    for (int i = 1; i < 10; i++) {

        for (int j = 1; j < 10; j++) {

            for (int k = 1; k < T; k++) {

                if (i+j+k == T) {
                    nums[0] = i;
                    nums[1] = j;
                    nums[2] = k;
                    check = true;
                    cout << T << " = " << i << " + " << j << " + " << k << endl;
                }
            }
        }
    }
    return check;
}


bool sum3_a(unsigned int T, unsigned int nums[3]) {
    for (int i = 1; i < 10; i++) {
        nums[0] = i;
        for (int j = 1; j < 10; j++) {
            nums[1] = j;
            if (T-i-j>0 && T-i-j<T) {
                nums[2] = T-i-j;
                return true;
            }
        }
    }
    return false;
}

/// TESTS ///
#include <gtest/gtest.h>

void compareSums(unsigned int selected[3], unsigned int expectedSum) {
    EXPECT_EQ(selected[0] + selected[1] + selected[2], expectedSum);
}

TEST(TP1_Ex1, 3sumExists) {
    unsigned int selected[3];

    unsigned int T = 10;
    EXPECT_EQ(sum3(T,selected), true);
    compareSums(selected,T);

    T = 18;
    EXPECT_EQ(sum3(T,selected), true);
    compareSums(selected,T);
}

TEST(TP1_Ex1, 3sumNotExists) {
    unsigned int selected[3];

    unsigned int T = 1;
    EXPECT_EQ(sum3(T,selected), false);
}