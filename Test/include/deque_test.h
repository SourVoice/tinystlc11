#ifndef MY_DEQUE_TEST_H
#define MY_DEQUE_TEST_H
// 测试 deque 接口和 push_front/push_back性能

// 标准
#include <deque> 
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

#include "../../src/deque.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace deque_test {

void deque_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : deque ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int               a[] = {1, 2, 3, 4, 5};
    MySTL::deque<int> d1;
    MySTL::deque<int> d2(5);
    MySTL::deque<int> d3(5, 1);
    MySTL::deque<int> d4(a, a + 5);
    MySTL::deque<int> d5(d2);
    MySTL::deque<int> d6(std::move(d2));
    MySTL::deque<int> d7;
    d7 = d3;
    MySTL::deque<int> d8;
    d8 = std::move(d3);
    MySTL::deque<int> d9{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::deque<int> d10;
    d10 = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    CON_FUN_AFTER(d1, d1.assign(5, 1));
    CON_FUN_AFTER(d1, d1.assign(8, 8));
    CON_FUN_AFTER(d1, d1.assign(a, a + 5));
    CON_FUN_AFTER(d1, d1.assign({1, 2, 3, 4, 5}));
    CON_FUN_AFTER(d1, d1.insert(d1.end(), 6));
    CON_FUN_AFTER(d1, d1.insert(d1.end() - 1, 2, 7));
    CON_FUN_AFTER(d1, d1.insert(d1.begin(), a, a + 5));
    CON_FUN_AFTER(d1, d1.erase(d1.begin()));
    CON_FUN_AFTER(d1, d1.erase(d1.begin(), d1.begin() + 4));
    CON_FUN_AFTER(d1, d1.emplace_back(8));
    CON_FUN_AFTER(d1, d1.emplace_front(8));
    CON_FUN_AFTER(d1, d1.emplace(d1.begin() + 1, 9));
    CON_FUN_AFTER(d1, d1.push_front(1));
    CON_FUN_AFTER(d1, d1.push_back(2));
    CON_FUN_AFTER(d1, d1.pop_back());
    CON_FUN_AFTER(d1, d1.pop_front());
    CON_FUN_AFTER(d1, d1.shrink_to_fit());
    CON_FUN_AFTER(d1, d1.resize(5));
    CON_FUN_AFTER(d1, d1.resize(8, 8));
    CON_FUN_AFTER(d1, d1.clear());
    CON_FUN_AFTER(d1, d1.shrink_to_fit());
    CON_FUN_AFTER(d1, d1.swap(d4));

    FUN_VALUE(*(d1.begin()));    // check
    FUN_VALUE(*(d1.end() - 1));  // check
    FUN_VALUE(*(d1.rbegin()));
    FUN_VALUE(*(d1.rend() - 1));
    FUN_VALUE(d1.front());
    FUN_VALUE(d1.back());
    FUN_VALUE(d1.at(1));
    FUN_VALUE(d1[2]);
    std::cout << std::boolalpha;
    FUN_VALUE(d1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(d1.size());
    FUN_VALUE(d1.max_size());
    PASSED;

#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|     push_front      |";
#if LARGER_TEST_DATA_ON
    CON_TEST_P1(deque<int>, push_front, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
    CON_TEST_P1(deque<int>, push_front, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|     push_back       |";
#if LARGER_TEST_DATA_ON
    CON_TEST_P1(deque<int>, push_back, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
    CON_TEST_P1(deque<int>, push_back, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[----------------- End container test : deque ------------------]" << std::endl;
}

} // MySTL::test::deque_test

} // MySTL::test

} // MySTL

#endif /* MY_DEQUE_TEST_H */