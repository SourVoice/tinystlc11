#ifndef MY_SET_TEST_H
#define MY_SET_TEST_H
// 测试 set, multiset 接口

// 标准
#include <set>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

#include "../../src/set.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace set_test {

// 输出set函数作用后的结果
#define SET_FUN_AFTER(con, fun)                    \
    do {                                           \
        std::string fun_name = #fun;               \
        std::cout << " After " << fun_name << " :" \
                  << std::endl;                    \
        fun;                                       \
        CON_COUT(con);                             \
    } while (0)

void set_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int                                  a[] = {5, 4, 3, 2, 1};
    MySTL::set<int>                      s1;
    MySTL::set<int, MySTL::greater<int>> s2;
    MySTL::set<int>                      s3(a, a + 5);
    MySTL::set<int>                      s4(a, a + 5);
    MySTL::set<int>                      s5(s3);
    MySTL::set<int>                      s6(std::move(s3));
    MySTL::set<int>                      s7;
    s7 = s4;
    MySTL::set<int> s8;
    s8 = std::move(s4);
    MySTL::set<int> s9{1, 2, 3, 4, 5};
    MySTL::set<int> s10;
    s10 = {1, 2, 3, 4, 5};

    for (int i = 5; i > 0; --i) {
        SET_FUN_AFTER(s1, s1.emplace(i));
    }
    SET_FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
    SET_FUN_AFTER(s1, s1.erase(s1.begin()));
    SET_FUN_AFTER(s1, s1.erase(0));
    SET_FUN_AFTER(s1, s1.erase(1));
    SET_FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));

    for (int i = 0; i < 5; ++i) {
        SET_FUN_AFTER(s1, s1.insert(i));
    }
    SET_FUN_AFTER(s1, s1.insert(a, a + 5));
    SET_FUN_AFTER(s1, s1.insert(5));
    SET_FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_VALUE(s1.count(5));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    SET_FUN_AFTER(s1, s1.erase(s1.begin()));
    SET_FUN_AFTER(s1, s1.erase(1));
    SET_FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    SET_FUN_AFTER(s1, s1.clear());
    SET_FUN_AFTER(s1, s1.swap(s5));
    FUN_VALUE(*s1.begin());
    FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(set<int>, emplace, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  CON_TEST_P1(set<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : set -------------------]" << std::endl;
}

void multiset_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[---------------- Run container test : multiset ----------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int                                       a[] = {5, 4, 3, 2, 1};
    MySTL::multiset<int>                      s1;
    MySTL::multiset<int, MySTL::greater<int>> s2;
    MySTL::multiset<int>                      s3(a, a + 5);
    MySTL::multiset<int>                      s4(a, a + 5);
    MySTL::multiset<int>                      s5(s3);
    MySTL::multiset<int>                      s6(std::move(s3));
    MySTL::multiset<int>                      s7;
    s7 = s4;
    MySTL::multiset<int> s8;
    s8 = std::move(s4);
    MySTL::multiset<int> s9{1, 2, 3, 4, 5};
    MySTL::multiset<int> s10;
    s10 = {1, 2, 3, 4, 5};

    for (int i = 5; i > 0; --i) {
        SET_FUN_AFTER(s1, s1.emplace(i));
    }
    SET_FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
    SET_FUN_AFTER(s1, s1.erase(s1.begin()));
    SET_FUN_AFTER(s1, s1.erase(0));
    SET_FUN_AFTER(s1, s1.erase(1));
    SET_FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));

    for (int i = 0; i < 5; ++i) {
        SET_FUN_AFTER(s1, s1.insert(i));
    }
    SET_FUN_AFTER(s1, s1.insert(a, a + 5));
    SET_FUN_AFTER(s1, s1.insert(5));
    SET_FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_VALUE(s1.count(5));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    SET_FUN_AFTER(s1, s1.erase(s1.begin()));
    SET_FUN_AFTER(s1, s1.erase(1));
    SET_FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    SET_FUN_AFTER(s1, s1.clear());
    SET_FUN_AFTER(s1, s1.swap(s5));
    FUN_VALUE(*s1.begin());
    FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    PASSED;
#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
    CON_TEST_P1(multiset<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
    CON_TEST_P1(multiset<int>, emplace, rand(), SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[---------------- End container test : multiset ----------------]" << std::endl;
}

}  // namespace set_test

}  // namespace test

}  // namespace MySTL

#endif /* MY_SET_TEST_H */