#ifndef MY_STACK_TEST_H
#define MY_STACK_TEST_H
// 文件实现对 stack的接口测试, 和push的性能

// 标准
#include <stack> 
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

#include "../../src/stack.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace stack_test {

void stack_cout(MySTL::stack<int> s) {
    while (!s.empty()) {
        std::cout << " " << s.top();
        s.pop();
    }
    std::cout << std::endl;
}

// 遍历输出stack
#define STACK_COUT(s)                       \
    do {                                    \
        std::string s_name = #s;            \
        std::cout << " " << s_name << " :"; \
        stack_cout(s);                      \
    } while (0);

// 输出函数作用于stack之后的stack内容
#define STACK_FUN_AFTER(con, fun)                 \
    do {                                          \
        std::string fun_name = #fun;              \
        std::cout << " After " << fun_name << " " \
                  << std::endl;                   \
        fun;                                      \
        STACK_COUT(con);                          \
    } while (0)

void stack_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : stack ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int               a[] = {1, 2, 3, 4, 5};
    MySTL::deque<int> d1(5);
    MySTL::stack<int> s1;
    MySTL::stack<int> s2(5);
    MySTL::stack<int> s3(5, 1);
    MySTL::stack<int> s4(a, a + 5);
    MySTL::stack<int> s5(d1);
    MySTL::stack<int> s6(std::move(d1));
    MySTL::stack<int> s7(s2);
    MySTL::stack<int> s8(std::move(s2));
    MySTL::stack<int> s9;
    s9 = s3;
    MySTL::stack<int> s10;
    s10 = std::move(s3);
    MySTL::stack<int> s11{1, 2, 3, 4, 5};
    MySTL::stack<int> s12;
    s12 = {1, 2, 3, 4, 5};

    STACK_FUN_AFTER(s1, s1.push(1));
    STACK_FUN_AFTER(s1, s1.push(2));
    STACK_FUN_AFTER(s1, s1.push(3));
    STACK_FUN_AFTER(s1, s1.pop());
    STACK_FUN_AFTER(s1, s1.emplace(4));
    STACK_FUN_AFTER(s1, s1.emplace(5));

    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.top());
    while (!s1.empty())
        STACK_FUN_AFTER(s1, s1.pop());
    STACK_FUN_AFTER(s1, s1.swap(s4));
    STACK_FUN_AFTER(s1, s1.clear());
    PASSED;

#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
    CON_TEST_P1(stack<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
    CON_TEST_P1(stack<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
}

} // MySTL::test::stack_test

} // MySTL::test

} // MySTL

#endif /* MY_STACK_TEST_H */