#ifndef MY_ALGORITHM_TEST_H
#define MY_ALGORITHM_TEST_H

// 标准
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

// 目标
#include "../../src/algorithm.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace algorithm_test {

TEST(copy_test) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[5], act[5];

    std::copy(arr1, arr1 + 5, exp);
    MySTL::copy(arr1, arr1 + 5, act);
    EXPECT_CON_EQ(exp, act);

    std::copy(arr1 + 5, arr1 + 10, exp);
    MySTL::copy(arr1 + 5, arr1 + 10, act);
    EXPECT_CON_EQ(exp, act);
}

} // namespace algorithm_test

}  // namespace MySTL::test

}  // namespace MySTL::test::algorithm_test

#endif