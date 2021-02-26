#include "exercises.h"

bool Activity::operator==(const Activity &a2) const {
    return start == a2.start && finish == a2.finish;
}

bool Activity::overlaps(const Activity &a2) const {
    return (start < a2.finish) && (a2.start < finish);
}

void maxPath(int initial,std::vector<Activity> activities, std::vector<Activity> &attempt, std::vector<Activity> &res) {
    if (initial == activities.size()) {
        if (attempt.size() > res.size())
            res = attempt;
        return;
    }
    if (attempt.empty())
        attempt.push_back(activities[initial]);

    for (int i = initial; i < activities.size(); i++) {
        if (!activities[initial].overlaps(activities[i])) {
            attempt.push_back(activities[i]);
            maxPath(i+1, activities, attempt, res);
            attempt.pop_back();
        }
    }
}


std::vector<Activity> activitySelectionBacktracking(std::vector<Activity> A) {
    std::vector<Activity> res,attempt;
    for (int i = 0; i < A.size(); i++) {
        maxPath(i,A,attempt,res);
    }
    return res;
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

bool noOverlaps(const std::vector<Activity> &acts) {
    for(unsigned int i = 0; i < acts.size(); i++) {
        Activity A1 = acts.at(i);
        for(unsigned int j = i + 1; j < acts.size(); j++) {
            Activity A2 = acts.at(j);
            if(A1.overlaps(A2)) return false;
        }
    }
    return true;
}

TEST(TP2_Ex4, activityScheduling) {
    std::vector<Activity> A = {{10,20}, {30, 35}, {5, 15}, {10, 40}, {40, 50}};
    std::vector<Activity> V = activitySelectionBacktracking(A);
    EXPECT_EQ(V.size(), 3 );
    EXPECT_EQ(noOverlaps(V), true);
}