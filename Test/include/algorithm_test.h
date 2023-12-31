#ifndef MY_ALGORITHM_TEST_H
#define MY_ALGORITHM_TEST_H

// 标准
#include <algorithm>
#include <iostream>
#include <numeric>
#include <functional>

// 目标
#include "../../src/algorithm.h"
#include "../../src/vector.h"

#include "../test.h"

namespace MySTL {

namespace test {

namespace algorithm_test {

int for_each_sum = 0;

int  gen()                                  { return 5; }
int  r(int i)                               { return (i * 5 + 1) % 9; }
bool is_odd(int i)                          { return i & 1; }
bool is_even(int i)                         { return !(i & 1); }
void arr_sum(int i)                         { for_each_sum += i; }
bool cmp(const int& a, const int& b)        { return b < a; }
int  unary_op(const int& x)                 { return x + 1; }
int  binary_op(const int& x, const int& y)  { return x + y; }

// *************************** algobase.h ***************************
TEST(max) {
    EXPECT_EQ(std::max(1, 2), MySTL::max(1, 2));
    EXPECT_EQ(std::max(2, 1), MySTL::max(2, 1));
    EXPECT_EQ(std::max(1, 1), MySTL::max(1, 1));
    EXPECT_EQ(std::max('a', 'b'), MySTL::max('a', 'b'));
    EXPECT_EQ(std::max('b', 'a'), MySTL::max('b', 'a'));
    EXPECT_EQ(std::max('a', 'a'), MySTL::max('a', 'a'));
    EXPECT_EQ(std::max(1.0, 2.0), MySTL::max(1.0, 2.0));
    EXPECT_EQ(std::max(2.0, 1.0), MySTL::max(2.0, 1.0));
    EXPECT_EQ(std::max(1.0, 1.0), MySTL::max(1.0, 1.0));
    EXPECT_EQ(std::max(1.0f, 2.0f), MySTL::max(1.0f, 2.0f));
    auto cmp = [](const int& a, const int& b) { return b < a; };
    auto cmp2 = std::greater<int>();
    EXPECT_EQ(std::max(1, 2, cmp), MySTL::max(1, 2, cmp));
    EXPECT_EQ(std::max(2, 1, cmp), MySTL::max(2, 1, cmp));
    EXPECT_EQ(std ::max(1, 1, cmp), MySTL::max(1, 1, cmp));
}

TEST(min) {
    EXPECT_EQ(std::max(1, 2), MySTL::max(1, 2));
    EXPECT_EQ(std::max(2, 1), MySTL::max(2, 1));
    EXPECT_EQ(std::max(1, 1), MySTL::max(1, 1));
    EXPECT_EQ(std::max('a', 'b'), MySTL::max('a', 'b'));
    EXPECT_EQ(std::max('b', 'a'), MySTL::max('b', 'a'));
    EXPECT_EQ(std::max('a', 'a'), MySTL::max('a', 'a'));
    EXPECT_EQ(std::max(1.0, 2.0), MySTL::max(1.0, 2.0));
    EXPECT_EQ(std::max(2.0, 1.0), MySTL::max(2.0, 1.0));
    EXPECT_EQ(std::max(1.0, 1.0), MySTL::max(1.0, 1.0));
    EXPECT_EQ(std::max(1.0f, 2.0f), MySTL::max(1.0f, 2.0f));
    auto cmp = [](const int& a, const int& b) { return b < a; };
    EXPECT_EQ(std::max(1, 2, cmp), MySTL::max(1, 2, cmp));
    EXPECT_EQ(std::max(2, 1, cmp), MySTL::max(2, 1, cmp));
    EXPECT_EQ(std ::max(1, 1, cmp), MySTL::max(1, 1, cmp));

}

TEST(iter_swap) {
    // iter for elem
    int a = 1, b = 2;
    int c = 1, d = 2;
    std::iter_swap(&c, &d);
    MySTL::iter_swap(&a, &b);
    EXPECT_EQ(a, c);
    EXPECT_EQ(b, d);
    // iter for iterator
    MySTL::vector<int> exp{1, 2};
    MySTL::vector<int> act{1, 2};
    std::iter_swap(exp.begin(), exp.begin() + 1);
    MySTL::iter_swap(act.begin(), act.begin() + 1);
    EXPECT_CON_EQ(exp, act);
    // swap for pointer
    int *p1 = &a, *p2 = &b;
    int *p3 = &a, *p4 = &b;
    std::iter_swap(p1, p2);
    MySTL::iter_swap(p3, p4);
    EXPECT_EQ(p1, p3);  // 验证地址
    EXPECT_EQ(p2, p4);
    EXPECT_PTR_EQ(p2, p4);  // 验证值
    EXPECT_PTR_EQ(p1, p3);
}

TEST(copy) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[5], act[5];

    std::copy(arr1, arr1 + 5, exp);
    MySTL::copy(arr1, arr1 + 5, act);
    EXPECT_CON_EQ(exp, act);

    std::copy(arr1 + 5, arr1 + 10, exp);
    MySTL::copy(arr1 + 5, arr1 + 10, act);
    EXPECT_CON_EQ(exp, act);
}

TEST(copy_backward) {
    int arr1[] = {1, 2, 3, 4, 5};
    int exp[5], act[5];
    // Array
    std::copy_backward(arr1, arr1 + 5, exp + 5);
    MySTL::copy_backward(arr1, arr1 + 5, act + 5);
    EXPECT_CON_EQ(exp, act);
    // vector
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{1, 2, 3, 4, 5};
    std::copy_backward(v1.begin(), v1.end(), v1.end());
    MySTL::copy_backward(v2.begin(), v2.end(), v2.end());
    EXPECT_CON_EQ(exp, act);
}

TEST(copy_if) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[5]  = {0}, act[5] = {0};

    std::copy_if(arr1, arr1 + 5, exp, is_odd);
    MySTL::copy_if(arr1, arr1 + 5, act, is_odd);
    EXPECT_CON_EQ(exp, act);

    std::copy_if(arr1 + 5, arr1 + 10, exp, is_even);
    MySTL::copy_if(arr1 + 5, arr1 + 10, act, is_even);
    EXPECT_CON_EQ(exp, act);
}

TEST(copy_n) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[5] = {0}, act[5] = {0};

    std::copy_n(arr1, 5, exp);
    MySTL::copy_n(arr1, 5, act);
    EXPECT_CON_EQ(exp, act);
    CON_COUT(arr1);

    std::copy_n(arr1 + 5, 5, exp);
    MySTL::copy_n(arr1 + 5, 5, act);
    EXPECT_CON_EQ(exp, act);
    CON_COUT(arr1);
}

TEST(move) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    int exp[5], act[5];
    std::move(arr1, arr1 + 5, exp);
    MySTL::move(arr2, arr2 + 5, act);
    EXPECT_CON_EQ(exp, act);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(move_backward) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    int exp[5], act[5];
    std::move_backward(arr1, arr1 + 5, exp + 5);
    MySTL::move_backward(arr2, arr2 + 5, act + 5);
    EXPECT_CON_EQ(exp, act);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(equal) {
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{1, 2, 3, 4, 5};
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    // 重载1
    EXPECT_EQ(std::equal(v1.begin(), v1.end(), v2.begin()), MySTL::equal(v1.begin(), v1.end(), v2.begin()));
    EXPECT_EQ(std::equal(arr1, arr1 + 5, arr2), MySTL::equal(arr1, arr1 + 5, arr2));
    // 重载2
    EXPECT_EQ(std::equal(v1.begin(), v1.end(), v2.begin(), std::equal_to<int>()), MySTL::equal(v1.begin(), v1.end(), v2.begin(), std::equal_to<int>()));
    EXPECT_EQ(std::equal(arr1, arr1 + 5, arr2, std::equal_to<int>()), MySTL::equal(arr1, arr1 + 5, arr2, std::equal_to<int>()));
}

TEST(fill) {
    MySTL::vector<int> v1(10);
    MySTL::vector<int> v2(10);
    int              arr1[] = {1, 2, 3, 4, 5};
    int              arr2[] = {1, 2, 3, 4, 5};
    std::fill(v1.begin(), v1.end(), 0);
    // 标准的std::vector的iterator类型无法识别，fill底层使用的fill_cat
    // 针对random_access_iterator做了特化，需要萃取出iterator_category
    MySTL::fill(v2.begin(), v2.end(), 0); 
    std::fill(arr1, arr1 + 5, 0), MySTL::fill(arr2, arr2 + 5, 0);
    EXPECT_CON_EQ(v1, v2);
    EXPECT_CON_EQ(arr1, arr2);

    // one_byte特化
    MySTL::vector<unsigned char> v3(10);
    MySTL::vector<unsigned char> v4(10);
    unsigned char              arr3[] = {1, 2, 3, 4, 5};
    unsigned char              arr4[] = {1, 2, 3, 4, 5};
    std::fill(v3.begin(), v3.end(), 'a');
    MySTL::fill(v4.begin(), v4.end(), 'a');
    std::fill(arr3, arr3 + 5, 'a');
    MySTL::fill(arr4, arr4 + 5, 'a');
    EXPECT_CON_EQ(v3, v4);
    EXPECT_CON_EQ(arr3, arr4);
}

TEST(fill_n) {
    std::vector<int> v1(10);
    std::vector<int> v2(10);
    int              arr1[] = {1, 2, 3, 4, 5};
    int              arr2[] = {1, 2, 3, 4, 5};
    std::fill_n(v1.begin(), 5, 0), MySTL::fill_n(v2.begin(), 5, 0);
    std::fill_n(arr1, 5, 0), MySTL::fill_n(arr2, 5, 0);
    EXPECT_CON_EQ(v1, v2);
    EXPECT_CON_EQ(arr1, arr2);
    // one_byte特化
    std::vector<unsigned char> v3(10);
    std::vector<unsigned char> v4(10);
    unsigned char              arr3[] = {1, 2, 3, 4, 5};
    unsigned char              arr4[] = {1, 2, 3, 4, 5};
    std::fill_n(v3.begin(), 5, 'a'), MySTL::fill_n(v4.begin(), 5, 'a');
    std::fill_n(arr3, 5, 'a'), MySTL::fill_n(arr4, 5, 'a');
    EXPECT_CON_EQ(v3, v4);
    EXPECT_CON_EQ(arr3, arr4);
}

TEST(lexicographical_compare) {
    {
        // (1)
        int arr1[] = {1, 2};
        int arr2[] = {1, 3};
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 2, arr2, arr2 + 2),
                  MySTL::lexicographical_compare(arr1, arr1 + 2, arr2, arr2 + 2));
    }
    {
        // (2) reach last1 before last2, return true 
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {1, 2, 3, 4, 5, 6};
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5),
                  MySTL::lexicographical_compare(arr1, arr1 + 6, arr2, arr2 + 6));
    }
    {
        // (3) reach last2 before last1, return false
        int arr1[] = {1, 2, 3, 4};
        int arr2[] = {1, 2, 3};
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 4, arr2, arr2 + 3),
                  MySTL::lexicographical_compare(arr1, arr1 + 4, arr2, arr2 + 3));
    }
    {
        // (4)
        int arr3[] = {1, 2, 3, 4, 5};
        int arr4[] = {1, 2, 3, 4, 5};
        EXPECT_EQ(std::lexicographical_compare(arr3, arr3 + 5, arr4, arr4 + 5),
                  MySTL::lexicographical_compare(arr3, arr3 + 5, arr4, arr4 + 5));
    }
    {
        // unsigned char* 特化
        unsigned char arr1[] = {'a', 'b', 'c', 'd', 'e'};
        unsigned char arr2[] = {'a', 'c', 'c', 'd', 'e'};
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5),
                  MySTL::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5));
    }
    {
        // 使用cmp
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {1, 2, 3, 4, 5};
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5, std::greater<int>()),
                  MySTL::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5, std::greater<int>()));
    }
}

TEST(mismatch) {
    // cmp 版本
    MySTL::vector<int> v1{1, 1, 2, 2, 3, 4, 5};
    MySTL::vector<int> v4{1, 1, 2, 2, 3, 4, 5};
    MySTL::vector<int> v2{1, 1, 2, 2, 3, 3, 3};
    MySTL::vector<int> v3{0, 1, 2, 2, 3, 4, 5};

    auto p = std::mismatch(v1.begin(), v1.end(), v4.begin(), std::equal_to<int>());
    auto q = MySTL::mismatch(v1.begin(), v1.end(), v4.begin(), std::equal_to<int>());
    EXPECT_EQ(p.first, q.first);
    EXPECT_EQ(p.second, q.second);
    auto m = std::mismatch(v1.begin(), v1.end(), v2.begin());
    auto n = MySTL::mismatch(v1.begin(), v1.end(), v2.begin());
    EXPECT_EQ(m.first, n.first);
    EXPECT_EQ(m.second, n.second);
    auto x = std::mismatch(v1.begin(), v1.end(), v3.begin(), std::equal_to<int>());
    auto y = MySTL::mismatch(v1.begin(), v1.end(), v3.begin(), std::equal_to<int>());
}

// *************************** heap_algo.h ***************************
TEST(make_heap) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v3{2, 1, 6, 5, 4, 9, 8, 7, 6};
    MySTL::vector<int> v4{2, 1, 6, 5, 4, 9, 8, 7, 6};
    MySTL::vector<int> v5{1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    MySTL::vector<int> v6{1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    std::make_heap(v1.begin(), v1.end());
    MySTL::make_heap(v2.begin(), v2.end());
    std::make_heap(v3.begin(), v3.end());
    MySTL::make_heap(v4.begin(), v4.end());
    std::make_heap(v5.begin(), v5.end(), std::greater<int>());
    MySTL::make_heap(v6.begin(), v6.end(), MySTL::greater<int>());
    EXPECT_CON_EQ(v1, v2);
    EXPECT_CON_EQ(v3, v4);
    EXPECT_CON_EQ(v5, v6);
}

TEST(push_heap) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v1.begin(), v1.end());
    MySTL::make_heap(v2.begin(), v2.end());
    for (int i = 4; i <= 9; ++i) {
        std::push_heap(v1.begin(), v1.begin() + i);
        MySTL::push_heap(v2.begin(), v2.begin() + i);
        EXPECT_CON_EQ(v1, v2);
    }
    MySTL::vector<int> v3{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v3.begin(), v3.end(), std::greater<int>());
    MySTL::make_heap(v4.begin(), v4.end(), std::greater<int>());
    std::push_heap(v3.begin(), v3.end(), std::greater<int>());
    MySTL::push_heap(v4.begin(), v4.end(), std::greater<int>());
    EXPECT_CON_EQ(v3, v4);
}

TEST(pop_heap) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v1.begin(), v1.end());
    MySTL::make_heap(v2.begin(), v2.end());
    for (int i = 9; i > 0; --i) {
        std::pop_heap(v1.begin(), v1.begin() + i);
        MySTL::pop_heap(v2.begin(), v2.begin() + i);
        EXPECT_CON_EQ(v1, v2);
    }
    MySTL::vector<int> v3{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v3.begin(), v3.end(), std::greater<int>());
    MySTL::make_heap(v4.begin(), v4.end(), std::greater<int>());
    std::pop_heap(v3.begin(), v3.begin() + 9, std::greater<int>());
    MySTL::pop_heap(v4.begin(), v4.begin() + 9, std::greater<int>());
    EXPECT_CON_EQ(v3, v4);
}

TEST(sort_heap) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v1.begin(), v1.end());
    MySTL::make_heap(v2.begin(), v2.end());
    std::sort_heap(v1.begin(), v1.end());
    MySTL::sort_heap(v2.begin(), v2.end());
    EXPECT_CON_EQ(v1, v2);
    MySTL::vector<int> v3{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(v3.begin(), v3.end(), std::greater<int>());
    MySTL::make_heap(v4.begin(), v4.end(), std::greater<int>());
    std::sort_heap(v3.begin(), v3.end(), std::greater<int>());
    MySTL::sort_heap(v4.begin(), v4.end(), std::greater<int>());
    EXPECT_CON_EQ(v3, v4);
}

// *************************** numeric.h ***************************
TEST(accmulate) {
    int arr[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0), MySTL::accumulate(arr, arr + 5, 0));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::minus<int>()), MySTL::accumulate(arr, arr + 5, 0, std::minus<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 1, std::multiplies<int>()), MySTL::accumulate(arr, arr + 5, 1, std::multiplies<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::divides<int>()), MySTL::accumulate(arr, arr + 5, 0, std::divides<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::modulus<int>()), MySTL::accumulate(arr, arr + 5, 0, std::modulus<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 1, std::logical_and<int>()), MySTL::accumulate(arr, arr + 5, 1, std::logical_and<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::logical_or<int>()), MySTL::accumulate(arr, arr + 5, 0, std::logical_or<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::bit_and<int>()), MySTL::accumulate(arr, arr + 5, 0, std::bit_and<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::bit_or<int>()), MySTL::accumulate(arr, arr + 5, 0, std::bit_or<int>()));
    EXPECT_EQ(std::accumulate(arr, arr + 5, 0, std::bit_xor<int>()), MySTL::accumulate(arr, arr + 5, 0, std::bit_xor<int>()));
}

TEST(adjacent_difference) {
    int arr[] = {1, 2, 3, 4, 5};
    int exp[5], act[5];
    std::adjacent_difference(arr, arr + 5, exp);
    MySTL::adjacent_difference(arr, arr + 5, act);
    EXPECT_CON_EQ(exp, act);
    std::adjacent_difference(arr, arr + 5, exp, std::plus<int>());
    MySTL::adjacent_difference(arr, arr + 5, act, std::plus<int>());
    EXPECT_CON_EQ(exp, act);
}

TEST(inner_product) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0), MySTL::inner_product(arr1, arr1 + 5, arr2, 0));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::multiplies<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::multiplies<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::divides<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::divides<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::modulus<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::modulus<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::logical_and<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::logical_and<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::logical_or<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::logical_or<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_and<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_and<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_or<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_or<int>()));
    EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_xor<int>()), MySTL::inner_product(arr1, arr1 + 5, arr2, 0, std::plus<int>(), std::bit_xor<int>()));
}

TEST(iota) {
    int arr1[5], arr2[5];
    std::iota(arr1, arr1 + 5, 0);
    MySTL::iota(arr2, arr2 + 5, 0);
    EXPECT_CON_EQ(arr1, arr2);
    std::iota(arr1, arr1 + 5, -1);
    MySTL::iota(arr2, arr2 + 5, -1);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(partial_sum) {
    int arr1[] = {1, 2, 3, 4, 5};
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> exp(10);
    MySTL::vector<int> act(10);
    // okay
    std::partial_sum(v1.begin(), v1.end(), std::back_inserter(exp));    // std::partial_sum(v1.begin(), v1.end(), exp.begin());
    MySTL::partial_sum(v1.begin(), v1.end(), std::back_inserter(act));  // MySTL::partial_sum(v1.begin(), v1.end(), act.begin());
    EXPECT_CON_EQ(exp, act);
    std::partial_sum(arr1, arr1 + 5, exp.begin(), std::multiplies<int>());  //  okay
    // std::partial_sum(arr1, arr1 + 5, exp, std::plus<int>()); // error(can't use arr1 and exp mix, exp is a vector, should use exp.begin())
    MySTL::partial_sum(arr1, arr1 + 5, act.begin(), std::multiplies<int>());
    EXPECT_CON_EQ(exp, act);
}

// *************************** set_algo.h ***************************
TEST(set_union) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{3, 4, 5, 6, 7};
    MySTL::vector<int> v3(10);
    MySTL::vector<int> v4(10);
    std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    MySTL::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
    EXPECT_CON_EQ(v3, v4);
    std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(), std::greater<int>());
    MySTL::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin(), std::greater<int>());
    EXPECT_CON_EQ(v3, v4);
}

TEST(set_intersection) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{2, 3};
    MySTL::vector<int> v3(10);
    MySTL::vector<int> v4(10);
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    MySTL::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
    EXPECT_CON_EQ(v3, v4);
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(), std::minus<int>());
    MySTL::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin(), std::minus<int>());
    EXPECT_CON_EQ(v3, v4);
}

TEST(set_difference) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{2, 3};
    MySTL::vector<int> v3(10);
    MySTL::vector<int> v4(10);
    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    MySTL::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
    EXPECT_CON_EQ(v3, v4);
    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(), std::minus<int>());
    MySTL::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin(), std::minus<int>());
    EXPECT_CON_EQ(v3, v4);
}

TEST(set_symmetric_difference) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{2, 3, 4, 5, 6, 7};
    MySTL::vector<int> v3(10);
    MySTL::vector<int> v4(10);
    std::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    MySTL::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
    EXPECT_CON_EQ(v3, v4);
    std::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin(), std::minus<int>());
    MySTL::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin(), std::minus<int>());
    EXPECT_CON_EQ(v3, v4);
}

// *************************** algo.h ***************************
#include "../../src/functional.h"
TEST(all_of) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    EXPECT_EQ(std::all_of(v1.begin(), v1.end(), MySTL::negate<int>()), MySTL::all_of(v1.begin(), v1.end(), MySTL::negate<int>()));
}

TEST(any_of) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{-11, 0};
    EXPECT_EQ(std::any_of(v1.begin(), v1.end(), MySTL::negate<int>()), MySTL::any_of(v1.begin(), v1.end(), MySTL::negate<int>()));
    EXPECT_EQ(std::any_of(v2.begin(), v2.end(), MySTL::negate<int>()), MySTL::any_of(v2.begin(), v2.end(), MySTL::negate<int>()));
}

TEST(none_of) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2{-11, 0};
    EXPECT_EQ(std::none_of(v1.begin(), v1.end(), MySTL::negate<int>()), MySTL::none_of(v1.begin(), v1.end(), MySTL::negate<int>()));
    EXPECT_EQ(std::none_of(v2.begin(), v2.end(), MySTL::negate<int>()), MySTL::none_of(v2.begin(), v2.end(), MySTL::negate<int>()));
}

TEST(for_each) {
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::for_each(v1.begin(), v1.end(), arr_sum);
    EXPECT_EQ(15, for_each_sum);
    MySTL::for_each(v1.begin(), v1.end(), arr_sum);
    EXPECT_EQ(30, for_each_sum);
}

TEST(count) {
    MySTL::vector<int> v1{1, 1, 1, 1, 1};
    EXPECT_EQ(std::count(v1.begin(), v1.end(), v1.front()), MySTL::count(v1.begin(), v1.end(), v1.front()));
    FUN_VALUE(MySTL::count(v1.begin(), v1.end(), v1.front()));
}

TEST(count_if) {
    MySTL::vector<int> v1{1, 1, 1, 1, 1};
    EXPECT_EQ(std::count_if(v1.begin(), v1.end(), is_odd), MySTL::count_if(v1.begin(), v1.end(), is_odd));
    EXPECT_EQ(std::count_if(v1.begin(), v1.end(), is_even), MySTL::count_if(v1.begin(), v1.end(), is_even));
}

TEST(find) {
    MySTL::vector<int> v1{1, 2, 3,4,5};
    EXPECT_EQ(std::find(v1.begin(), v1.end(), 3), MySTL::find(v1.begin(), v1.end(), 3));
    EXPECT_EQ(std::find(v1.begin(), v1.end(), 6), MySTL::find(v1.begin(), v1.end(), 6));
}

TEST(find_if) {
    int arr1[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_odd), MySTL::find_if(arr1, arr1 + 5, is_odd));
    EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_even), MySTL::find_if(arr1, arr1 + 5, is_even));
}

TEST(find_if_not) {
    int arr1[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(std::find_if_not(arr1, arr1 + 5, is_odd), MySTL::find_if_not(arr1, arr1 + 5, is_odd));
    EXPECT_EQ(std::find_if_not(arr1, arr1 + 5, is_even), MySTL::find_if_not(arr1, arr1 + 5, is_even));
}

TEST(search) {
    int arr1[] = {1, 2, 3, 3, 3, 3, 4, 5, 6, 6};
    int arr2[] = {1};
    int arr3[] = {3, 3};
    int arr4[] = {5, 6, 6, 6};
    EXPECT_EQ(std::search(arr1, arr1 + 9, arr2, arr2 + 1), MySTL::search(arr1, arr1 + 9, arr2, arr2 + 1));
    EXPECT_EQ(std::search(arr1, arr1 + 9, arr3, arr3 + 2), MySTL::search(arr1, arr1 + 9, arr3, arr3 + 2));
    EXPECT_EQ(std::search(arr1, arr1 + 9, arr3, arr3 + 2), MySTL::search(arr1, arr1 + 9, arr3, arr3 + 2));
    EXPECT_EQ(std::search(arr1, arr1 + 9, arr3, arr3, std::less<int>()), MySTL::search(arr1, arr1 + 9, arr3, arr3, std::less<int>()));
}

TEST(search_n) {
    int arr1[] = {1, 2, 2, 3, 3, 3, 6, 6, 9};
    EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 0), MySTL::search_n(arr1, arr1 + 9, 1, 0));
    EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 2), MySTL::search_n(arr1, arr1 + 9, 2, 2));
    EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 3), MySTL::search_n(arr1, arr1 + 9, 1, 3));
    EXPECT_EQ(std::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()), MySTL::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()));
    EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()), MySTL::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()));
}

TEST(find_end) {
    int arr1[] = {1, 2, 3, 2, 2, 3, 4, 5};
    int arr2[] = {2, 3};
    int arr3[] = {4, 5, 6};
    EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr2, arr2 + 1), MySTL::find_end(arr1, arr1 + 8, arr2, arr2 + 1));
    EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2), MySTL::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
    EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2), MySTL::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
    EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()), MySTL::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()));
}

TEST(find_first_of) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {2, 3, 4};
    int arr3[] = {6, 7, 8};
    EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3), MySTL::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3));
    EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()), MySTL::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()));
}

TEST(adjacent_find) {
    int arr1[] = {1, 2, 3, 2, 2, 3, 4, 5};
    int arr2[] = {2, 3};
    int arr3[] = {4, 5, 6};
    EXPECT_EQ(std::adjacent_find(arr1, arr1 + 8), MySTL::adjacent_find(arr1, arr1 + 8));
    EXPECT_EQ(std::adjacent_find(arr1, arr1 + 8), MySTL::adjacent_find(arr1, arr1 + 8));
    EXPECT_EQ(std::adjacent_find(arr1, arr1 + 8), MySTL::adjacent_find(arr1, arr1 + 8));
    EXPECT_EQ(std::adjacent_find(arr1, arr1 + 8, std::less<int>()), MySTL::adjacent_find(arr1, arr1 + 8, std::less<int>()));
}

TEST(lower_bound) {
    int arr1[] = {1, 2, 3, 3, 3, 4, 5};
    EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 1), MySTL::lower_bound(arr1, arr1 + 7, 1));
    EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 2), MySTL::lower_bound(arr1, arr1 + 7, 2));
    EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 3), MySTL::lower_bound(arr1, arr1 + 7, 3));
    EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 5, std::less<int>()), MySTL::lower_bound(arr1, arr1 + 7, 5, std::less<int>()));
}

TEST(upper_bound) {
    int arr1[] = {1, 2, 3, 3, 3, 4, 5};
    EXPECT_EQ(std::upper_bound(arr1, arr1 + 7, 1), MySTL::upper_bound(arr1, arr1 + 7, 1));
    EXPECT_EQ(std::upper_bound(arr1, arr1 + 7, 2), MySTL::upper_bound(arr1, arr1 + 7, 2));
    EXPECT_EQ(std::upper_bound(arr1, arr1 + 7, 3), MySTL::upper_bound(arr1, arr1 + 7, 3));
    EXPECT_EQ(std::upper_bound(arr1, arr1 + 7, 5, std::less<int>()), MySTL::upper_bound(arr1, arr1 + 7, 5, std::less<int>()));
}

TEST(binary_search) {
    int arr1[] = {1, 2, 3, 3, 3, 4, 5};
    EXPECT_EQ(std::binary_search(arr1, arr1 + 7, 1), MySTL::binary_search(arr1, arr1 + 7, 1));
    EXPECT_EQ(std::binary_search(arr1, arr1 + 7, 2), MySTL::binary_search(arr1, arr1 + 7, 2));
    EXPECT_EQ(std::binary_search(arr1, arr1 + 7, 3), MySTL::binary_search(arr1, arr1 + 7, 3));
    EXPECT_EQ(std::binary_search(arr1, arr1 + 7, 5, std::less<int>()), MySTL::binary_search(arr1, arr1 + 7, 5, std::less<int>()));
}

TEST(equal_range) {
    int  arr1[] = {1, 2, 3, 3, 3, 4, 5};
    auto p = std::equal_range(arr1, arr1 + 7, 1);
    auto q = MySTL::equal_range(arr1, arr1 + 7, 1);
    EXPECT_EQ(p.first, q.first);
    EXPECT_EQ(p.second, q.second);
    auto m = std::equal_range(arr1, arr1 + 7, 2);
    auto n = MySTL::equal_range(arr1, arr1 + 7, 2);
    EXPECT_EQ(m.first, n.first);
    EXPECT_EQ(m.second, n.second);
    auto x = std::equal_range(arr1, arr1 + 7, 3);
    auto y = MySTL::equal_range(arr1, arr1 + 7, 3);
    EXPECT_EQ(x.first, y.first);
    EXPECT_EQ(x.second, y.second);
    auto j = std::equal_range(arr1, arr1 + 7, 5, std::less<int>());
    auto k = MySTL::equal_range(arr1, arr1 + 7, 5, std::less<int>());
    EXPECT_EQ(j.first, k.first);
    EXPECT_EQ(j.second, k.second);
}

TEST(generate) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    std::generate(arr1, arr1 + 5, gen);
    MySTL::generate(arr2, arr2 + 5, gen);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(generate_n) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    std::generate_n(arr1, 5, gen);
    MySTL::generate_n(arr2, 5, gen);
    EXPECT_CON_EQ(arr1, arr2);
    CON_COUT(arr1);
    CON_COUT(arr2);
}

TEST(includes) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    MySTL::vector<int> v2{2, 3, 5, 6, 9};
    MySTL::vector<int> v3{0, 1, 2, 3, 4};
    MySTL::vector<int> v4{1, 2, 5, 7, 10};
    MySTL::vector<int> v5;
    EXPECT_EQ(std::includes(v1.begin(), v1.end(), v2.begin(), v2.end()), MySTL::includes(v1.begin(), v1.end(), v2.begin(), v2.end()));
    EXPECT_EQ(std::includes(v1.begin(), v1.end(), v3.begin(), v3.end()), MySTL::includes(v1.begin(), v1.end(), v3.begin(), v3.end()));
    EXPECT_EQ(std::includes(v1.begin(), v1.end(), v4.begin(), v4.end()), MySTL::includes(v1.begin(), v1.end(), v4.begin(), v4.end()));
    EXPECT_EQ(std::includes(v1.begin(), v1.end(), v5.begin(), v5.end()), MySTL::includes(v1.begin(), v1.end(), v5.begin(), v5.end()));
}

TEST(is_heap) {
    int arr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int arr3[] = {1, 3, 5, 7, 9, 0, 2, 4, 6, 8};
    int arr4[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::make_heap(arr4, arr4 + 10);
    EXPECT_EQ(std::is_heap(arr1, arr1 + 10), MySTL::is_heap(arr1, arr1 + 10));
    EXPECT_EQ(std::is_heap(arr2, arr2 + 10, std::less<int>()), MySTL::is_heap(arr2, arr2 + 10, std::less<int>()));
    EXPECT_EQ(std::is_heap(arr3, arr3 + 10), MySTL::is_heap(arr3, arr3 + 10));
    EXPECT_EQ(std::is_heap(arr4, arr4 + 10), MySTL::is_heap(arr4, arr4 + 10));
}

TEST(is_sorted) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 5, 4};
    int arr3[] = {5, 4, 3, 2, 1};
    int arr4[] = {1, 2, 5, 4, 3};
    EXPECT_EQ(std::is_sorted(arr1, arr1 + 5), MySTL::is_sorted(arr1, arr1 + 5));
    EXPECT_EQ(std::is_sorted(arr2, arr2 + 5), MySTL::is_sorted(arr2, arr2 + 5));
    EXPECT_EQ(std::is_sorted(arr3, arr3 + 5, std::less<int>()), MySTL::is_sorted(arr3, arr3 + 5, std::less<int>()));
    EXPECT_EQ(std::is_sorted(arr4, arr4 + 5, std::less<int>()), MySTL::is_sorted(arr4, arr4 + 5, std::less<int>()));
}

TEST(median) {
    int    ia = 2, ib = 1, ic = 3;
    double da = 3.1, db = 3.0, dc = 3.2;
    EXPECT_EQ(2, MySTL::median(ia, ib, ic));
    EXPECT_EQ(3.1, MySTL::median(da, db, dc, std::greater<double>()));
}

TEST(max_element) {
    int    arr1[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    double arr2[] = {1.0, 2.2, 6.6, 8.8, 8.81, 2.4};
    EXPECT_PTR_EQ(std::max_element(arr1, arr1 + 9), MySTL::max_element(arr1, arr1 + 9));
    EXPECT_PTR_EQ(std::max_element(arr2, arr2 + 6, std::less<double>()), MySTL::max_element(arr2, arr2 + 6, std::less<double>()));
}

TEST(min_element) {
    int    arr1[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    double arr2[] = {1.0, 2.2, 6.6, 8.8, 8.81, 2.4};
    EXPECT_PTR_EQ(std::min_element(arr1, arr1 + 9), MySTL::min_elememt(arr1, arr1 + 9));
    EXPECT_PTR_EQ(std::min_element(arr2, arr2 + 6, std::less<double>()), MySTL::min_element(arr2, arr2 + 6, std::less<double>()));
}

TEST(swap_ranges) {
    int arr1[] = {4, 5, 6, 1, 2, 3};
    int arr2[] = {4, 5, 6, 1, 2, 3};
    int arr3[] = {1, 2, 3, 4, 5, 6};
    int arr4[] = {1, 2, 3, 4, 5, 6};
    int arr5[] = {1, 1, 1};
    int arr6[] = {1, 1, 1};
    std::swap_ranges(arr1, arr1 + 6, arr3);
    MySTL::swap_ranges(arr2, arr2 + 6, arr4);
    std::swap_ranges(arr1, arr1 + 3, arr5);
    MySTL::swap_ranges(arr2, arr2 + 3, arr6);
    EXPECT_CON_EQ(arr1, arr2);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(transform) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int exp[9], act[9];
    std::transform(arr1, arr1 + 9, exp, unary_op);
    MySTL::transform(arr1, arr1 + 9, act, unary_op);
    EXPECT_CON_EQ(exp, act);
    std::transform(arr1, arr1 + 9, arr2, exp, binary_op);
    MySTL::transform(arr1, arr1 + 9, arr2, act, binary_op);
    EXPECT_CON_EQ(exp, act);
}

TEST(remove_copy) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 6, 6};
    MySTL::vector<int> v2(v1);
    std::remove(v1.begin(), v1.end(), 3);
    MySTL::remove(v2.begin(), v2.end(), 3);
    EXPECT_CON_EQ(v1, v2);
    std::remove(v1.begin(), v1.end(), 6);
    MySTL::remove(v2.begin(), v2.end(), 6);
    EXPECT_CON_EQ(v1, v2);
}

TEST(remove) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 6, 6};
    int arr2[] = {1, 2, 3, 4, 5, 6, 6, 6};
    int exp[5], act[5];
    std::remove_copy(arr1, arr1 + 8, exp, 6);
    MySTL::remove_copy(arr2, arr2 + 8, act, 6);
    EXPECT_CON_EQ(exp, act);
    std::remove_copy(arr1, arr1 + 5, exp, 3);
    MySTL::remove_copy(arr2, arr2 + 5, act, 3);
    EXPECT_CON_EQ(exp, act);
}

TEST(remove_copy_if) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int exp[5], act[5];
    std::remove_copy_if(arr1, arr1 + 10, exp, is_odd);
    MySTL::remove_copy_if(arr1, arr1 + 10, act, is_odd);
    EXPECT_CON_EQ(exp, act);
    std::remove_copy_if(arr1, arr1 + 10, exp, is_even);
    MySTL::remove_copy_if(arr1, arr1 + 10, act, is_even);
    EXPECT_CON_EQ(exp, act);
}

TEST(remove_if) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    MySTL::vector<int> v2(v1);
    MySTL::remove_if(v1.begin(), v1.end(), is_odd);
    MySTL::remove_if(v2.begin(), v2.end(), is_odd);
    EXPECT_CON_EQ(v1, v2);
}

TEST(replace) {
    int arr1[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    int arr2[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    std::replace(arr1, arr1 + 9, 1, 4);
    MySTL::replace(arr2, arr2 + 9, 1, 4);
    EXPECT_CON_EQ(arr1, arr2);
    std::replace(arr1, arr1 + 9, 2, 5);
    MySTL::replace(arr2, arr2 + 9, 2, 5);
    EXPECT_CON_EQ(arr1, arr2);
    std::replace(arr1, arr1 + 9, 3, 6);
    MySTL::replace(arr2, arr2 + 9, 3, 6);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(replace_if) {
    MySTL::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    MySTL::vector<int> v2(v1);
    std::replace_if(v1.begin(), v1.end(), is_odd, 1);
    MySTL::replace_if(v2.begin(), v2.end(), is_odd, 1);
    EXPECT_CON_EQ(v1, v2);
    std::replace_if(v1.begin(), v1.end(), is_even, 2);
    MySTL::replace_if(v2.begin(), v2.end(), is_even, 2);
    EXPECT_CON_EQ(v1, v2);
}

TEST(replace_copy) {
    int arr1[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    int exp[9], act[9];
    std::replace_copy(arr1, arr1 + 9, exp, 1, 4);
    MySTL::replace_copy(arr1, arr1 + 9, act, 1, 4);
    EXPECT_CON_EQ(exp, act);
    std::replace_copy(arr1, arr1 + 9, exp, 2, 5);
    MySTL::replace_copy(arr1, arr1 + 9, act, 2, 5);
    EXPECT_CON_EQ(exp, act);
    std::replace_copy(arr1, arr1 + 9, exp, 3, 6);
    MySTL::replace_copy(arr1, arr1 + 9, act, 3, 6);
    EXPECT_CON_EQ(exp, act);
}

TEST(replace_copy_if) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int exp[10] = {0}, act[10] = {0};
    std::replace_copy_if(arr1, arr1 + 10, exp, is_odd, 1);
    MySTL::replace_copy_if(arr1, arr1 + 10, act, is_odd, 1);
    EXPECT_CON_EQ(exp, act);
    std::replace_copy_if(arr1, arr1 + 10, exp, is_even, 2);
    MySTL::replace_copy_if(arr1, arr1 + 10, act, is_even, 2);
    EXPECT_CON_EQ(exp, act);
}

TEST(reverse) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::reverse(arr1, arr1 + 9);
    MySTL::reverse(arr2, arr2 + 9);
    EXPECT_CON_EQ(arr1, arr2);
    std::reverse(arr1, arr1 + 5);
    MySTL::reverse(arr2, arr2 + 5);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(reverse_copy) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[5], act[5];
    std::reverse_copy(arr1, arr1 + 5, exp);
    MySTL::reverse_copy(arr1, arr1 + 5, act);
    EXPECT_CON_EQ(exp, act);
    std::reverse_copy(arr1 + 4, arr1 + 9, exp);
    MySTL::reverse_copy(arr1 + 4, arr1 + 9, act);
    EXPECT_CON_EQ(exp, act);
}

TEST(random_shuffle) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[9];
    for (int i = 0; i < 9; ++i) {
        std::copy(arr1, arr1 + 9, arr2);
        MySTL::random_shuffle(arr1, arr1 + 9);
        EXPECT_CON_NE(arr1, arr2);
    }
    std::copy(arr1, arr1 + 9, arr2);
    MySTL::random_shuffle(arr1, arr1 + 9, r);
    EXPECT_CON_NE(arr1, arr2);
}

TEST(rotate) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::rotate(arr1, arr1 + 3, arr1 + 9);
    MySTL::rotate(arr2, arr2 + 3, arr2 + 9);
    EXPECT_CON_EQ(arr1, arr2);
    std::rotate(arr1 + 3, arr1 + 5, arr1 + 9);
    MySTL::rotate(arr2 + 3, arr2 + 5, arr2 + 9);
    EXPECT_CON_EQ(arr1, arr2);
    std::rotate(arr1, arr1 + 9, arr1 + 9);
    MySTL::rotate(arr2, arr2 + 9, arr2 + 9);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(rotate_copy) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int exp[9], act[9];
    std::rotate_copy(arr1, arr1 + 5, arr1 + 9, exp);
    MySTL::rotate_copy(arr1, arr1 + 5, arr1 + 9, act);
    EXPECT_CON_EQ(exp, act);
    std::rotate_copy(arr1, arr1, arr1 + 9, exp);
    MySTL::rotate_copy(arr1, arr1, arr1 + 9, act);
    EXPECT_CON_EQ(exp, act);
    std::rotate_copy(arr1, arr1 + 9, arr1 + 9, exp);
    MySTL::rotate_copy(arr1, arr1 + 9, arr1 + 9, act);
    EXPECT_CON_EQ(exp, act);
}

TEST(is_permutation) {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {3, 4, 5, 2, 1};
    int arr3[] = {1, 2, 3, 4, 6};
    // 因为提供的是 C++11 的支持，std::is_permutation 可能没有 C++14 的接口
    EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr2), MySTL::is_permutation(arr1, arr1 + 5, arr2, arr2 + 5));
    EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr3), MySTL::is_permutation(arr1, arr1 + 5, arr3, arr3 + 5));
    EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr2, std::equal_to<int>()), MySTL::is_permutation(arr1, arr1 + 5, arr2, arr2 + 5, std::equal_to<int>()));
    EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr3, std::equal_to<int>()), MySTL::is_permutation(arr1, arr1 + 5, arr3, arr3 + 5, std::equal_to<int>()));
}

TEST(next_permutation) {
    int arr1[] = {1, 2, 3, 3};
    int arr2[] = {1, 2, 3, 3};
    int n1 = 0, n2 = 0;
    while (std::next_permutation(arr1, arr1 + 4)) n1++;
    while (MySTL::next_permutation(arr1, arr1 + 4)) n2++;
    EXPECT_EQ(n1, n2);
    for (; n1 > 0; n1--) {
        std::next_permutation(arr1, arr1 + 4);
        MySTL::next_permutation(arr2, arr2 + 4);
        EXPECT_CON_EQ(arr1, arr2);
    }
    std::next_permutation(arr1, arr1 + 4, std::greater<int>());
    MySTL::next_permutation(arr2, arr2 + 4, std::greater<int>());
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(prev_permutation) {
    int arr1[] = {3, 2, 1, 1};
    int arr2[] = {3, 2, 1, 1};
    int n1 = 0, n2 = 0;
    while (std::prev_permutation(arr1, arr1 + 4)) n1++;
    while (MySTL::prev_permutation(arr1, arr1 + 4)) n2++;
    EXPECT_EQ(n1, n2);
    for (; n1 > 0; n1--) {
        std::prev_permutation(arr1, arr1 + 4);
        MySTL::prev_permutation(arr2, arr2 + 4);
        EXPECT_CON_EQ(arr1, arr2);
    }
    std::prev_permutation(arr1, arr1 + 4, std::greater<int>());
    MySTL::prev_permutation(arr2, arr2 + 4, std::greater<int>());
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(merge) {
    int arr1[] = {1, 2, 5, 9, 10};
    int arr2[] = {3, 7, 8, 8, 9};
    int arr3[] = {1, 2, 5, 9, 10};
    int arr4[] = {3, 7, 8, 8, 9};
    int exp[10], act[10];
    std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp);
    MySTL::merge(arr3, arr3 + 5, arr4, arr4 + 5, act);
    EXPECT_CON_EQ(exp, act);
    std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp, std::less<int>());
    MySTL::merge(arr3, arr3 + 5, arr4, arr4 + 5, act, std::less<int>());
    EXPECT_CON_EQ(exp, act);
}

TEST(inplace_merge) {
    int arr1[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    int arr2[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    int arr3[] = {1, 2, 3, 1, 2, 3, 4, 5};
    int arr4[] = {1, 2, 3, 1, 2, 3, 4, 5};
    std::inplace_merge(arr1, arr1 + 5, arr1 + 10);
    MySTL::inplace_merge(arr2, arr2 + 5, arr2 + 10);
    std::inplace_merge(arr3, arr3 + 3, arr3 + 8, std::less<int>());
    MySTL::inplace_merge(arr4, arr4 + 3, arr4 + 8, std::less<int>());
    EXPECT_CON_EQ(arr1, arr2);
    EXPECT_CON_EQ(arr3, arr4);
}

TEST(partial_sort) {
    int arr1[] = {3, 2, 1, 9, 8, 7, 6, 5, 4};
    int arr2[] = {3, 2, 1, 9, 8, 7, 6, 5, 4};
    int arr3[] = {5, 1, 5, 8, 6, 4, 8, 4, 1, 3, 5, 8, 4};
    int arr4[] = {5, 1, 5, 8, 6, 4, 8, 4, 1, 3, 5, 8, 4};
    std::partial_sort(arr1, arr1 + 2, arr1 + 9);
    MySTL::partial_sort(arr2, arr2 + 2, arr2 + 9);
    std::partial_sort(arr3, arr3 + 5, arr3 + 13, std::greater<int>());
    MySTL::partial_sort(arr4, arr4 + 5, arr4 + 13, std::greater<int>());
    EXPECT_CON_EQ(arr1, arr2);
    EXPECT_CON_EQ(arr3, arr4);
}

TEST(partial_sort_copy) {
    int arr1[] = {3, 2, 1, 9, 8, 7, 6, 5, 4};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr3[] = {1, 6, 8, 4, 2, 1, 6, 8, 4, 7, 6, 2, 1, 3, 6};
    int exp[5], act[5];
    std::partial_sort_copy(arr1, arr1 + 9, exp, exp + 5);
    MySTL::partial_sort_copy(arr1, arr1 + 9, act, act + 5);
    EXPECT_CON_EQ(exp, act);
    std::partial_sort_copy(arr2, arr2 + 9, exp, exp + 5);
    MySTL::partial_sort_copy(arr2, arr2 + 9, act, act + 5);
    EXPECT_CON_EQ(exp, act);
    std::partial_sort_copy(arr3, arr3 + 15, exp, exp + 5, std::greater<int>());
    MySTL::partial_sort_copy(arr3, arr3 + 15, act, act + 5, std::greater<int>());
    EXPECT_CON_EQ(exp, act);
}

TEST(partition) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::partition(arr1, arr1 + 9, is_odd);
    MySTL::partition(arr2, arr2 + 9, is_odd);
    EXPECT_CON_EQ(arr1, arr2);
    std::partition(arr1, arr1 + 9, is_even);
    MySTL::partition(arr2, arr2 + 9, is_even);
    EXPECT_CON_EQ(arr1, arr2);
}

TEST(partition_copy) {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int exp_true[5], exp_false[5];
    int act_true[5], act_false[5];
    std::partition_copy(arr1, arr1 + 10, exp_true, exp_false, is_odd);
    MySTL::partition_copy(arr1, arr1 + 10, act_true, act_false, is_odd);
    EXPECT_CON_EQ(exp_true, act_true);
    EXPECT_CON_EQ(exp_false, act_false);
}

TEST(sort) {
    int arr1[] = {6, 1, 2, 5, 4, 8, 3, 2, 4, 6, 10, 2, 1, 9};
    int arr2[] = {6, 1, 2, 5, 4, 8, 3, 2, 4, 6, 10, 2, 1, 9};
    int arr3[] = {80, 30, 51, 65, 12, 10, 24, 87, 62, 51, 32, 45, 1, 33, 66, 20, 35, 84, 62, 14};
    int arr4[] = {80, 30, 51, 65, 12, 10, 24, 87, 62, 51, 32, 45, 1, 33, 66, 20, 35, 84, 62, 14};
    int arr5[] = {9, 9, 9, 8, 8, 8, 7, 7, 7};
    int arr6[] = {9, 9, 9, 8, 8, 8, 7, 7, 7};
    std::sort(arr1, arr1 + 14);
    MySTL::sort(arr2, arr2 + 14);
    std::sort(arr3, arr3 + 20);
    MySTL::sort(arr4, arr4 + 20);
    std::sort(arr5, arr5 + 9, std::greater<int>());
    MySTL::sort(arr6, arr6 + 9, std::greater<int>());
    EXPECT_CON_EQ(arr1, arr2);
    EXPECT_CON_EQ(arr3, arr4);
    EXPECT_CON_EQ(arr5, arr6);
}

TEST(nth_element) {
    int arr1[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int arr2[] = {1, 2, 3, 4, 5, 6, 3, 2, 1};
    int arr3[] = {1, 2, 8, 9, 6, 5, 3, 4, 7};
    int arr4[] = {1, 5, 1, 5, 8, 4, 9, 6, 8, 4, 10, 13, 20, 4, 2, 1};
    MySTL::nth_element(arr1, arr1 + 4, arr1 + 9);
    MySTL::nth_element(arr2, arr2 + 2, arr2 + 9);
    MySTL::nth_element(arr3, arr3 + 8, arr3 + 9, std::less<int>());
    MySTL::nth_element(arr4, arr4 + 3, arr4 + 16, std::less<int>());
    bool arr1_left_less = true, arr1_right_greater = true;
    bool arr2_left_less = true, arr2_right_greater = true;
    bool arr3_left_less = true, arr3_right_greater = true;
    bool arr4_left_less = true, arr4_right_greater = true;
    for (int i = 0; i < 9; ++i) {
        if (i < 4 && arr1[i] > arr1[4])
            arr1_left_less = false;
        else if (i > 4 && arr1[i] < arr1[4])
            arr1_right_greater = false;
    }
    for (int i = 0; i < 9; ++i) {
        if (i < 2 && arr2[i] > arr2[2])
            arr2_left_less = false;
        else if (i > 2 && arr2[i] < arr2[2])
            arr2_right_greater = false;
    }
    for (int i = 0; i < 9; ++i) {
        if (i < 8 && arr3[i] > arr3[8]) arr3_left_less = false;
    }
    for (int i = 0; i < 16; ++i) {
        if (i < 3 && arr4[i] > arr4[3])
            arr4_left_less = false;
        else if (i > 3 && arr4[i] < arr4[3])
            arr4_right_greater = false;
    }
    EXPECT_TRUE(arr1_left_less);
    EXPECT_TRUE(arr1_right_greater);
    EXPECT_TRUE(arr2_left_less);
    EXPECT_TRUE(arr2_right_greater);
    EXPECT_TRUE(arr3_left_less);
    EXPECT_TRUE(arr3_right_greater);
    EXPECT_TRUE(arr4_left_less);
    EXPECT_TRUE(arr4_right_greater);
}

TEST(unique_copy) {
    int arr1[] = {1, 1, 1, 2, 2, 3, 4, 4, 5, 6};
    int arr2[] = {1, 2, 3, 6, 6, 6, 8, 8, 9};
    int exp[6], act[6];
    std::unique_copy(arr1, arr1 + 10, exp);
    MySTL::unique_copy(arr1, arr1 + 10, act);
    EXPECT_CON_EQ(exp, act);
    std::unique_copy(arr2, arr2 + 9, exp, std::equal_to<int>());
    MySTL::unique_copy(arr2, arr2 + 9, act, std::equal_to<int>());
    EXPECT_CON_EQ(exp, act);
}

TEST(unique) {
    int arr1[] = {1, 1, 1, 2, 2, 3, 4, 4, 5, 6};
    int arr2[] = {1, 1, 1, 2, 2, 3, 4, 4, 5, 6};
    int arr3[] = {1, 2, 3, 6, 6, 6, 8, 8, 9};
    int arr4[] = {1, 2, 3, 6, 6, 6, 8, 8, 9};
    std::unique(arr1, arr1 + 10);
    MySTL::unique(arr2, arr2 + 10);
    std::unique(arr3, arr3 + 9, std::equal_to<int>());
    MySTL::unique(arr4, arr4 + 9, std::equal_to<int>());
    EXPECT_CON_EQ(arr1, arr2);
    EXPECT_CON_EQ(arr3, arr4);
}

} // namespace algorithm_test

}  // namespace MySTL::test

}  // namespace MySTL::test::algorithm_test

#endif