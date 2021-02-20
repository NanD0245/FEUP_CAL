// By: Gonçalo Leão

#include "exercises.h"

#include <algorithm>
#include <vector>

double minimumAverageCompletionTime(std::vector<unsigned int> tasks, std::vector<unsigned int> &orderedTasks) {
    //TODO...
    std::vector<unsigned int> tasks_sum;
    double sum = 0;
    int index, duration = tasks[0];
    orderedTasks.clear();
    while(!tasks.empty()) {
        index = 0; duration = tasks[0];
        for (size_t i = 0; i < tasks.size();i++) {
            if (tasks[i] < duration) {
                duration = tasks[i];
                index = i;
            }
        }
        orderedTasks.push_back(tasks[index]);
        tasks.erase(tasks.begin()+index);
    }
    for (size_t i = 0; i < orderedTasks.size(); i++) {
        if (i == 0) tasks_sum.push_back(orderedTasks[i]);
        else tasks_sum.push_back(tasks_sum[i-1] + orderedTasks[i]);
    }
    for (auto i: tasks_sum) sum += i;
    return sum/orderedTasks.size();
}

/// TESTS ///
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TP1_Ex7, taskOrdering) {
    std::vector<unsigned int> tasks = {15, 8, 3, 10};
    std::vector<unsigned int> orderedTasks;
    double averageTime = minimumAverageCompletionTime(tasks, orderedTasks);
    EXPECT_EQ(orderedTasks.size(), 4 );
    ASSERT_NEAR(averageTime, 17.75, 0.00001);
    ASSERT_THAT(orderedTasks,  ::testing::ElementsAre(3,8,10,15));
}