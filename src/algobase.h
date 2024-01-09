#ifndef MY_ALGOBASE_H
#define MY_ALGOBASE_H

// 
// impl simple algo for STL
// uitlty function: 
// "max", "min", "iter_swap"
// "copy", "copy_backward", "copy_if", "copy_n"
// "move", "move_backward"
// "equal"
// "fill_n", "fill"
// "lexicographical_compare" * , "mismatch" *
// 

#include <cstring>

#include "iterator.h"
#include "util.h"

namespace MySTL {

// return first when equal
template <class T>
const T& max(const T& lhs, const T& rhs) { return lhs < rhs ? rhs : lhs; }

template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare cmp) { return cmp(lhs, rhs) ? rhs : lhs; }

// return first when equal
template <class T>
const T& min(const T& lhs, const T& rhs) { return rhs < lhs ? rhs : lhs; }

template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare cmp) { return cmp(lhs, rhs) ? rhs : lhs; }

// swap the data of two iterator
template <class Iter1, class Iter2>
void iter_swap(Iter1 lhs, Iter2 rhs) {
    MySTL::swap(*lhs, *rhs);
}

/*****************************************************************************************/
// copy()
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
// copy() for input_iterator_tag
template <class InputIter, class OutputIter>
OutputIter unchecked_copy_cat(InputIter first, InputIter last,
                              OutputIter result, MySTL::input_iterator_tag) {
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

// copy() for random_access_iterator_tag
template <class RandIter, class OutputIter>
OutputIter unchecked_copy_cat(RandIter first, RandIter last,
                              OutputIter result, MySTL::random_access_iterator_tag) {
    for (auto n = last - first; n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_copy(InputIter first, InputIter last,
                          OutputIter result) {
    return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// copy() 为 trivially_copy_assignable类型提供的特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
unchecked_copy(Tp* first, Tp* last,
               Up* result) {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
}

/**
 * @brief copy form [first, last) to [result, result + (last - first))
 * @return 返回拷贝结束的尾部(dst 尾部)
 */
template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last,
                OutputIter result) {
    return unchecked_copy(first, last, result);
}

/*****************************************************************************************/
// copy_backward()
// 把 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
// copy_backward() for bidirectional_iterator_tag
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                               BidirectionalIter2 result, MySTL::bidirectional_iterator_tag) {
    for (auto n = last - first; n > 0; n--)
        *--result = *--last;
    return result;
}

// copy_backward() for random_access_iterator_tag
template <class RandIter, class BidirectionalIter>
BidirectionalIter unchecked_copy_backward_cat(RandIter first, RandIter last,
                                              BidirectionalIter result, MySTL::random_access_iterator_tag) {
    for (auto n = last - first; n > 0; --n, --last, --result)
        *result = *last;
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                           BidirectionalIter2 result) {
    return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
}

// copy_backward() 针对 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
unchecked_copy_backward(Tp* first, Tp* last,
                        Up* result) {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0) {
        result -= n;
        std::memmove(result, first, n * sizeof(Up));
    }
    return result;
}

/**
 * @brief copy form [first, last), to [result - (last - first), result)
 * @return result - (last - first) 处的迭代器
 */
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                 BidirectionalIter2 result) {
    return unchecked_copy_backward(first, last, result);
}

/*****************************************************************************************/
// copy_if()
// copy [first, last), 内满足操作 unary_pred 的元素拷贝到result起始的位置
/*****************************************************************************************/
/**
 * @brief copy [first, last), 内满足操作 unary_pred 的元素拷贝到result起始的位置
 * @return 返回 result的尾部
 */
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter copy_if(InputIter first, InputIter last,
                   OutputIter result, UnaryPredicate pred) {
    for (; first != last; ++first) {
        if (pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

/*****************************************************************************************/
// copy_n()
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部(src的尾部和dst的尾部)
/*****************************************************************************************/
// copy_n() for input_iterator_tag
template <class InputIter, class Size, class OutputIter>
MySTL::pair<InputIter, OutputIter>
unchecked_copy_n(InputIter first, Size n, OutputIter result,
                 MySTL::input_iterator_tag) {
    for (; n > 0; n--, first++, result++)
        *result = *first;
    return MySTL::pair<InputIter, OutputIter>(first, result);
}

// copy_n() for random_access_iterator_tag
template <class RandIter, class Size, class OutputIter>
MySTL::pair<RandIter, OutputIter>
unchecked_copy_n(RandIter first, Size n, OutputIter result,
                 MySTL::random_access_iterator_tag) {
    auto last = first + n;
    return MySTL::pair<RandIter, OutputIter>(last, MySTL::copy(first, last, result));
}

/**
 * @brief 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
 * @return 返回一个 pair 分别指向拷贝结束的尾部
 */
template <class InputIter, class Size, class OutputIter>
MySTL::pair<InputIter, OutputIter>
copy_n(InputIter first, Size n, OutputIter result) {
    return unchecked_copy_n(first, n, result, iterator_category(first));
}

/*****************************************************************************************/
// move()
// move [first, last) to [result, result + (last - first))
/*****************************************************************************************/
// move() for input_iterator_tag
template <class InputIter, class OutputIter>
OutputIter unchecked_move_cat(InputIter first, InputIter last,
                              OutputIter result, MySTL::input_iterator_tag) {
    for (; first != last; ++first, ++result)
        *result = MySTL::move(*first);
    return result;
}

// move() for random_access_iterator_tag
template <class RandIter, class OutputIter>
OutputIter uncheckeded_move_cat(RandIter first, RandIter last,
                                OutputIter result, MySTL::random_access_iterator_tag) {
    for (auto n = last - first; n > 0; --n, ++first, ++result)
        *result = MySTL::move(*first);
    return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_move(InputIter first, InputIter last,
                          OutputIter result) {
    return unchecked_move_cat(first, last, result, iterator_category(first));
}

// move() 为 trivially_copy_assignable类型提供的特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_move_assignable<Up>::value,
    Up*>::type
unchecked_move(Tp* first, Tp* last,
               Up* result) {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
}

/**
 * @brief move [first, last) to [result, result + (last - first))
 * @return 返回 dst 的尾部
 */
template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last,
                OutputIter reslut) {
    return unchecked_move(first, last, reslut);
}

/*****************************************************************************************/
// move_backward()
// 将[first, last)内的元素移动到[result - (last - first), result)
/*****************************************************************************************/
// move_backward() for bidirectional_iterator_tag
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                               BidirectionalIter2 result, MySTL::bidirectional_iterator_tag) {
    for (; first != last; --result, --last)
        *result = MySTL::move(last);
    return result;
}

// move_backward() for random_access_iterator_tag
template <class RandIter1, class RandIter2>
RandIter2 unchecked_move_backward_cat(RandIter1 first, RandIter2 last,
                                      RandIter2 result, MySTL::random_access_iterator_tag) {
    for (auto n = last - first; n > 0; n--, --last, --first)
        *result = MySTL::move(*last);
    return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_move_backward(InputIter first, InputIter last,
                                   OutputIter result) {
    return unchecked_move_backward_cat(first, last, result, iterator_category(first));
}

// move_backward() 为 trivially_copy_assignable类型提供的特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
    Up*>::type
unchecked_move_backward(Tp* first, Tp* last, Up* result) {
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0) {
        result -= n;
        std::memmove(result, first, n * sizeof(Up));
    }
    return result;
}

/**
 * @brief 将[first, last)内的元素移动到[result - (last - first), result)
 * @return 返回 result - (last - first) 处的迭代器
 */
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                 BidirectionalIter2 result) {
    return unchecked_move_backward(first, last, result);
}

/*****************************************************************************************/
// equal()
// 比较[first, last) 和第二个序列对应的区间是否相等
/*****************************************************************************************/
/**
 * @brief 比较[first, last) 和第二个序列对应的区间是否相等
 */
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2)
            return false;
    }
    return true;
}

// equal() override with compare
template <class InputIter1, class InputIter2, class Compared>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared cmp) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!cmp(*first1, *first2))
            return false;
    }
    return true;
}

/*****************************************************************************************/
// fill_n()
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value) {
    for (; n > 0; --n, first++)
        *first = value;
    return first;
}

// 为 one-byte提供特化
template <class Tp, class Size, class Up>
typename std::enable_if<
    std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
   !std::is_same<Tp, bool>::value && 
    std::is_integral<Up>::value && sizeof(Up) == 1,
    Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value) {
    if (n > 0)
        std::memset(first, (unsigned char)value, (size_t)n);
    return first + n;
}

/**
 * @brief 从 first 位置开始填充 n 个值
 */
template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
    return unchecked_fill_n(first, n, value);
}

/*****************************************************************************************/
// fill()
// set new value for [first, last)
/*****************************************************************************************/
// fill() for forward_iterator_tag
template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value, MySTL::forward_iterator_tag) {
    for (; first != last; ++first)
        *first = value;
}

// fill() for random_access_iterator_tag
template <class RandIter, class T>
void fill_cat(RandIter first, RandIter last, const T& value, MySTL::random_access_iterator_tag) {
    fill_n(first, last - first, value);
}

/**
 * @brief set new value for [first, last)
 */
template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
    fill_cat(first, last, value, iterator_category(first));
}

/*****************************************************************************************/
// lexicographical_compare()
// details below
/*****************************************************************************************/
/**
 * @brief
 * 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
 * (1)如果第一序列的元素较小，返回 true ，否则返回 false
 * (2)如果到达 last1 而尚未到达 last2 返回 true
 * (3)如果到达 last2 而尚未到达 last1 返回 false
 * (4)如果同时到达 last1 和 last2 返回 false
 */
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// override lexicographical_compare() use comp
template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2, Compare cmp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (cmp(*first1, *first2))
            return true;
        if (cmp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// 针对const unsigned char* 特化
bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
                             const unsigned char* first2, const unsigned char* last2) {
    const auto len1 = last1 - first1;
    const auto len2 = last2 - first2;
    const auto result = std::memcmp(first1, first2, MySTL::min(len1, len2));  // 比较长度相同的部分
    return (result != 0) ? (result < 0) : (len1 < len2); // 若相等, 长度长的大
}

/*****************************************************************************************/
// mismatch
// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
/*****************************************************************************************/
/**
 * @brief 平行比较两个序列, 找到第一处失配的元素
 * @return 返回一对迭代器, 指向两个序列中失配的元素
 */
template <class InputIter1, class InputIter2>
MySTL::pair<InputIter1, InputIter2>
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return MySTL::pair<InputIter1, InputIter2>(first1, first2);
}

// 重载使用comp代替比较
template <class InputIter1, class InputIter2, class Compare>
MySTL::pair<InputIter1, InputIter2>
mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compare cmp) {
    while (first1 != last1 && cmp(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return MySTL::pair<InputIter1, InputIter2>(first1, first2);
}

}  // namespace MySTL

#endif /* MY_ALGOBASE_H */
