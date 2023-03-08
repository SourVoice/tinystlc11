#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H
// 头文件包含STL的数值算法
#include "type_traits.h"
#include "algobase.h"
#include "util.h"

namespace MySTL{

/**
 * @brief accmulate()
 */
template <class InputIter, class T>
constexpr
T accumulat(InputIter first, InputIter last, T init){
    for(;first != last; first++)
        init = MySTL::move(init) + *first;
    return init;
}

template <class InputIter, class T, class BinaryOperation>
T accumulat(InputIter first, InputIter last, T init, BinaryOperation op){
    for(;first!= last;first++)
        init = op(MySTL::move(init), *first);
}

// TODO: 检查
/**
 * @brief adjacent_difference(), 计算区间内相邻元素差值, 保存至result起始位置
 */
template <class InputIter, class OutputIter>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result) {
    if (first == last) return result;
    typedef typename MySTL::iterator_traits<InputIter>::value_type value_type;
    value_type acc = *first;
    *result = acc;
    while (++first != last)
    {
        value_type val = *first;
        *++result = val - MySTL::move(acc);
        acc = MySTL::move(val);
    }
    return ++result;
}
// 针对BinaryOperation版本
// TODO: 需要检查
template <class InputIter, class OutputIter, class BinaryOperation>
OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result, BinaryOperation op) {
    if (first == last) return result;
    typedef typename MySTL::iterator_traits<InputIter>::value value_type;
    value_type acc = *first;
    *result = acc;
    while (++first != last) {
			value_type val = *first;
        *++first = op(val - MySTL::move(acc));
        acc = MySTL::move(val);
    }
    return ++result;
}

/**
 * @brief  计算区间内元素内积
 * @return 返回init和内积的和
 */
template <class InputIter1, class InputIter2, class T>
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
T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init,
                BinaryOperation1 op1, BinaryOperation2 op2) {
    while (first1 != last1) {
        init = op1(MySTL::move(init), op2(*first1, *first2));
        ++first1;
        ++first2;
    }
    return init;
}

// TODO: iota(), 检查
/**
 * @brief 对[first, last) 内元素使用value递增填充
 */
template <class ForwardIter, class T>
constexpr
void iota(ForwardIter first, ForwardIter last, T value) {
    while (first != last) {
        *first++ = value;
        ++value;
    }
}

// TODO: 待检查, MySTL::move()的调用可能未明确
/**
 * @brief parital_sum(), 进行局部累加求和, 结果放到result区间内
 */
template <class InputIter, class OutputIter>
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
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result, BinaryOperation op) {
    if (first == last) return result;
    typename MySTL::iterator_traits<InputIter>::value_type acc = *first;
    *result = acc;
    while (++first != last) {
        acc = op(MySTL::move(acc) + *first);
        *++result = acc;
    }
    return ++result;
}

} // namespace MySTL


#endif /* MY_NUMERIC_H */
