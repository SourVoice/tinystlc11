#ifndef MY_QUEUE_TEST_H
#define MY_QUEUE_TEST_H
// 文件实现对 queue , priority_queue 队列的接口测试

// 标准
#include <queue> 
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

#include "../../src/queue.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace queue_test {

void queue_cout(MySTL::queue<int> q) {
    while (!q.empty()) {
        std::cout << " " << q.front();
        q.pop();
    }
    std::cout << std::endl;
}

void priority_queue_cout(MySTL::priority_queue<int> pq) {
    while (!pq.empty()) {
        std::cout << " " << pq.top();
        pq.pop();
    }
    std::cout << std::endl;
}

// 遍历输出queue
#define QUEUE_COUT(q)                       \
    do {                                    \
        std::string q_name = #q;            \
        std::cout << " " << q_name << " :"; \
        queue_cout(q);                      \
    } while (0);

// 遍历输出priority_queue
#define PRIORITY_QUEUE_COUT(pq)              \
    do {                                     \
        std::string pq_name = #pq;           \
        std::cout << " " << pq_name << " :"; \
        priority_queue_cout(pq);             \
    } while (0);

// 输出函数作用于queue之后的queu内容
#define QUEUE_FUN_AFTER(con, fun)                 \
    do {                                          \
        std::string fun_name = #fun;              \
        std::cout << " After " << fun_name << " " \
                  << std::endl;                   \
        fun;                                      \
        QUEUE_COUT(con);                          \
    } while (0)

#define PRIORITY_QUEUE_FUN_AFTER(con, fun)        \
    do {                                          \
        std::string fun_name = #fun;              \
        std::cout << " After " << fun_name << " " \
                  << std::endl;                   \
        fun;                                      \
        PRIORITY_QUEUE_COUT(con);                 \
    } while (0)

void queue_test() {
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : queue ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int               a[] = {1, 2, 3, 4, 5};
    MySTL::deque<int> d1(5);
    MySTL::queue<int> q1;
    MySTL::queue<int> q2(5);
    MySTL::queue<int> q3(5, 1);
    MySTL::queue<int> q4(a, a + 5);
    MySTL::queue<int> q5(d1);
    MySTL::queue<int> q6(std::move(d1));
    MySTL::queue<int> q7(q2);
    MySTL::queue<int> q8(std::move(q2));
    MySTL::queue<int> q9;
    q9 = q3;
    MySTL::queue<int> q10;
    q10 = std::move(q3);
    MySTL::queue<int> q11{1, 2, 3, 4, 5};
    MySTL::queue<int> q12;
    q12 = {1, 2, 3, 4, 5};

    QUEUE_FUN_AFTER(q1, q1.push(1));
    QUEUE_FUN_AFTER(q1, q1.push(2));
    QUEUE_FUN_AFTER(q1, q1.push(3));
    QUEUE_FUN_AFTER(q1, q1.pop());
    QUEUE_FUN_AFTER(q1, q1.emplace(4));
    QUEUE_FUN_AFTER(q1, q1.emplace(5));

    std::cout << std::boolalpha;
    FUN_VALUE(q1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(q1.size());
    FUN_VALUE(q1.front());
    FUN_VALUE(q1.back());
    while (!q1.empty())
        QUEUE_FUN_AFTER(q1, q1.pop());
    QUEUE_FUN_AFTER(q1, q1.swap(q4));
    QUEUE_FUN_AFTER(q1, q1.clear());
    PASSED;

#if PERFORMANCE_TEST_ON
    std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
    CON_TEST_P1(queue<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
    CON_TEST_P1(queue<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
    std::cout << std::endl;
    std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
    PASSED;
#endif
    std::cout << "[----------------- End container test : queue ------------------]" << std::endl;
}

void priority_queue_test() {
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------- Run container test : priority_queue -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int                        a[] = {1, 2, 3, 4, 5};
  MySTL::vector<int> v1(5);
  MySTL::priority_queue<int> pq1;
  MySTL::priority_queue<int> pq2(5);
  MySTL::priority_queue<int> pq3(5, 1);
  MySTL::priority_queue<int> pq4(a, a + 5);
  MySTL::priority_queue<int> pq5(v1);
  MySTL::priority_queue<int> pq6(std::move(v1));
  MySTL::priority_queue<int> pq7(pq2);
  MySTL::priority_queue<int> pq8(std::move(pq2));
  MySTL::priority_queue<int> pq9;
  pq9 = pq3;
  MySTL::priority_queue<int> pq10;
  pq10 = std::move(pq3);
  MySTL::priority_queue<int> pq11{ 1,2,3,4,5 };
  MySTL::priority_queue<int> pq12;
  pq12 = { 1,2,3,4,5 };

  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.push(1));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.push(5));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.push(3));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.pop());
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.emplace(7));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.emplace(2));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.emplace(8));

  std::cout << std::boolalpha;
  FUN_VALUE(pq1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(pq1.size());
  FUN_VALUE(pq1.top());
  while (!pq1.empty())
    PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.pop());
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.swap(pq4));
  PRIORITY_QUEUE_FUN_AFTER(pq1, pq1.clear());
  PASSED;

#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(priority_queue<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(priority_queue<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------- End container test : priority_queue -------------]" << std::endl;
}

} // MySTL::test::queue_test

} // MySTL::test

} // MySTL



#endif /* MY_QUEUE_TEST_H */