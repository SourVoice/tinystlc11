#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H

// 头文件包含STL的数值算法
// 包括accumulate(), inner_product(), partial_sum(), adjacent_difference()
// iota()
// 以及对应的BinaryOperation版本

#include "type_traits.h"
#include "algobase.h"
#include "util.h"

namespace MySTL{

/**
 * @brief accumulate() 用于计算[first, last)内元素的和, 并加上init
 * @param first
 * @param last
 * @param init 初始值，用于在最后计算结果时加上
 * @return 返回区间和+init
 * @note 函数声明为 constexpr 用于C++11, 表示该函数在编译期就能计算出结果
 */
template <class InputIter, class T>
constexpr
T accumulate(InputIter first, InputIter last, T init) {
    for (; first != last; first++)
        init = MySTL::move(init) + *first;
    return init;
}

/**
 * @brief accumulate()针对BinaryOperation op重载版本,使用op代替+
 * @tparam BinaryOperation 二元操作函数
 * @note 函数声明为 constexpr 用于C++11, 表示该函数在编译期就能计算出结果
 */
template <class InputIter, class T, class BinaryOperation>
constexpr
T accumulate(InputIter first, InputIter last, T init, BinaryOperation op) {
    for (; first != last; first++)
        init = op(MySTL::move(init), *first);
    return init;
}

/**
 * @brief adjacent_difference(), 计算区间内相邻元素差值, 保存至result起始位置
 * 该实现抄自cppreference网站
 * @return 返回result的尾部迭代器，result + (last - first)
 */
template <class InputIter, class OutputIter>
constexpr
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result) {
    if (first == last) return result;
    typedef typename MySTL::iterator_traits<InputIter>::value_type value_type;
    value_type acc = *first;
    *result = acc;
    while (++first != last) {
        value_type val = *first;
        *++result = val - MySTL::move(acc);
        acc = MySTL::move(val);
    }
    return ++result;
}

/**
 * @brief adjacent_difference(), 计算区间内相邻元素差值, 保存至result起始位置
 * @note 针对BinaryOperation版本, 使用op代替相邻元素计算
 * 该实现抄自cppreference网站
 */
template <class InputIter, class OutputIter, class BinaryOperation>
constexpr
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result, BinaryOperation op) {
    if (first == last) return result;
    typedef typename MySTL::iterator_traits<InputIter>::value_type value_type;
    value_type acc = *first;
    *result = acc;
    while (++first != last) {
        value_type tmp = *first;
        *++result = op(tmp, MySTL::move(acc));
        acc = MySTL::move(tmp);
    }
    return ++result;
}

/**
 * @brief  计算区间内元素内积, 每次计算结果加上init
 * @param first1 第一个区间的起始
 * @param last1 第一个区间的结束
 * @param first2 第二个区间的起始
 * @param init 初始值
 * @return 返回init和内积的和
 */
template <class InputIter1, class InputIter2, class T>
constexpr
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init) {
    while (first1 != last1) {
        init = MySTL::move(init) + (*first1) * (*first2);
        ++first1;
        ++first2;
    }
    return init;
}

// 为 inner_product() 提供BinayOperation特化版本
// 为 inner_product() 自定义ooperator+ 和 operator*
template <class InputIter1, class InputIter2, class T,
          class BinaryOperation1, class BinaryOperation2>
constexpr
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init,
                BinaryOperation1 op1, BinaryOperation2 op2) {
    while (first1 != last1) {
        init = op1(MySTL::move(init), op2(*first1, *first2));
        ++first1;
        ++first2;
    }
    return init;
}

/**
 * iota
 * @brief 对[first, last) 内元素使用value递增填充
 * @param first 起始位置
 * @param last 结束位置
 * @param value 起始值
 */
template <class ForwardIter, class T>
constexpr
void iota(ForwardIter first, ForwardIter last, T value) {
    while (first != last) {
        *first++ = value;
        ++value;
    }
}

/**
 * @brief parital_sum(), 进行局部累加求和, 结果放到result区间内
 * @return 返回result区间的尾后迭代器
 * @note MySTL::move()用于将上一次的sum值移动到新的sum，而不是复制，避免拷贝操作
 * value_type sum = *first; 底层的value_type类型必须支持移动操作
 */
template <class InputIter, class OutputIter>
constexpr
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result) {
    if (first == last) return result;
    typename MySTL::iterator_traits<InputIter>::value_type sum = *first;
    *result = sum;
    while (++first != last) {
        sum = MySTL::move(sum) + *first;
        *++result = sum;
    }
    return ++result;
}

// 为 partial_sum()提供针对BinaryOperation的特化版本
template <class InputIter, class OutputIter, class BinaryOperation>
constexpr
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result, BinaryOperation op) {
    if (first == last) return result;
    typename MySTL::iterator_traits<InputIter>::value_type acc = *first;
    *result = acc;
    while (++first != last) {
        acc = op(MySTL::move(acc), *first);
        *++result = acc;
    }
    return ++result;
}

} // namespace MySTL


#endif /* MY_NUMERIC_H */
