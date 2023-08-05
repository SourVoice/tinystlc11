#ifndef MY_SET_ALGO_H
#define MY_SET_ALGO_H

/*
 * 头文件包含 set 的四种算法: union, intersection, difference, symmetric_difference
 * 所有函数都要求序列有序
 */

#include "iterator.h"
#include "algobase.h"

namespace MySTL {

/**
 * @brief 计算 s1 s2并集的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1, InputIter2 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1,
            ++first1;
            ++first2;
        }
        ++result;
    }
    // 拷贝剩余元素
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

/**
 * @brief set_union 的重载版本，使用函数对象 comp 代替比较操作
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_union(InputIter1 first1, InputIter2 last1,
                     InputIter2 first2, InputIter2 last2,
                     OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1,
            ++first1;
            ++first2;
        }
        ++result;
    }
    // 拷贝剩余元素
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

/**
 * @brief 计算 s1 s2交集的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            ++first1;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

/**
 * @brief set_intersection 的重载版本，使用函数对象 comp 代替比较操作
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2,
                            OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            ++first1;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

/**
 * @brief 计算 s1 s2差集(s1 - s2, 保留s1)的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return MySTL::copy(first1, last1, result);
}

/**
 * @brief set_difference 的重载版本，使用函数对象 comp 代替比较操作
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return MySTL::copy(first1, last1, result);
}

/**
 * @brief 计算 s1 s2对称差集(s1 - s2 + s2 - s1)的结果并保存到 result 中，返回一个迭代器指向输出结果的尾部
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                    InputIter2 first2, InputIter2 last2,
                                    OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

/**
 * @brief set_symmetric_difference 的重载版本，使用函数对象 comp 代替比较操作
 */
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                    InputIter2 first2, InputIter2 last2,
                                    OutputIter result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

}  // namespace MySTL

#endif /* MY_SET_ALGO_H */