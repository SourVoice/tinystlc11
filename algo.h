#ifndef MY_ALGO_H
#define MY_ALGO_H

// 该头文件包含MySTL中的全部算法
// TODO: 待实现 heap_algo.h
// TODO: 待实现 set_algo.h
#include "algobase.h"
#include "heap_algo.h"
#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include <cstdlib>
#include <memory>

namespace MySTL {
/*
 * @brief 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && unary_pred(*first))
        first++;
    return first == last;
}  // XXX: 同原作者实现不同

/**
 * @brief 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && !unary_pred(*first))
        first++;
    return first == last;
}  // XXX: 同原作者实现不同

/**
 * @brief 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first)
    {
        if (unary_pred(*first))
            return false;
    }
    return true;
}

/**
 * @brief 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==，返回元素相等的个数
 */
template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value) {
    size_t n = 0;
    for (; first != last; ++first)
    {
        if (*first == value)
            ++n;
    }
    return n;
}

/**
 * @brief 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
 */
template <class InputIter, class UnaryPredicate>
size_t count_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    size_t n = 0;
    for (; first != last; ++first)
    {
        if (unary_pred(*first))
            ++n;
    }
    return n;
}

/**
 * @brief 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
 */
template <class InputIter, class T>
InputIter
find(InputIter first, InputIter last, const T& value) {
    while (first != last && *first != value)
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
 */
template <class InputIter, class UnaryPredicate>
InputIter
find_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && !unary_pred(*first))
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
 */
template <class InputIter, class UnaryPredicate>
InputIter
find_not_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && unary_pred(*first))
        ++first;
    return first;
}

/**
 * @brief 在[first1, last1)中查找[first2, last2)的首次出现点
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2) {
    auto d1 = MySTL::distance(first1, last1);
    auto d2 = MySTL::distance(first2, last2);
    if (d1 < d2) return last1;
    auto curr1 = first1;
    auto curr2 = first2;
    while (curr2 != last2) {
        if (*curr1 == *curr2) {
            ++curr1;
            ++curr2;
        } else {
            if (d1 == d2) {
                return last1;
            } else {
                curr1 = ++first1;
                curr2 = first2;
                --d1;
            }
        }
    }
    return first1;
}

// 重载版本使用comp代替比较操作
template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2, Compare cmp) {
    auto d1 = MySTL::distance(first1, last1);
    auto d2 = MySTL::distance(first2, last2);
    if (d1 < d2) return last1;
    auto curr1 = first1;
    auto curr2 = first2;
    while (curr2 != last2) {
        if (cmp(*curr1, *curr2)) {
            ++curr1;
            ++curr2;
        } else {
            if (d1 == d2) {
                return last1;
            } else {
                curr1 = ++first1;
                curr2 = first2;
                --d1;
            }
        }
    }
    return first1;
}

/**
 * @brief 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
 */
template <class ForwardIter, class Size, class T>
ForwardIter
search_n(ForwardIter first, ForwardIter last, Size n, const T& value) {
    if (n <= 0) {
        return first;
    } else {
        first = MySTL::find(first, last, value);
        while (first != last) {
            auto m = n - 1;
            auto i = first;
            ++i;
            while (i != last && m != 0 && *i == value) {
                ++i;
                --m;
            }
            if (m == 0) {
                return first;
            } else {
                first = MySTL::find(i, last, value);
            }
        }
        return last;
    }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Size, class T, class Compare>
ForwardIter
search_n(ForwardIter first, ForwardIter last, Size n, const T& value, Compare cmp) {
    if (n <= 0) {
        return first;
    } else {
        while (first != last) {
            if (cmp(*first, value))
                break;
            ++first;
        }
        while (first != last) {
            auto m = n - 1;
            auto i = first;
            ++i;
            while (i != last && m != 0 && cmp(*i, value)) {
                ++i;
                --m;
            }
            if (m == 0) {
                return first;
            } else {
                while (i != last) {
                    if (cmp(*i, value))
                        break;
                    ++i;
                }
                first = i;
            }
        }
        return last;
    }
}

/*****************************************************************************************/
// find_end()
// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回 last1
/*****************************************************************************************/
// find_end_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2)
    {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = MySTL::search(first1, last1, first2, last2);
            if (new_result == last1)
            {
                return result;
            } else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

// find_end_dispatch 的 bidirectional_iterator版本
template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter1
find_end_dispatch(BidrectionalIter1 first1, BidrectionalIter1 last1,
                  BidrectionalIter2 first2, BidrectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag) {
    typedef reverse_iterator<BidrectionalIter1> reviter1;
    typedef reverse_iterator<BidrectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    // 前向查找, 找到last2出现的第一次
    reviter1 rresult = MySTL::search(reviter1(last1), rlast1, reviter2(last2), rlast2);
    if (rresult == rlast1)  // 没有找到
    {
        return last1;
    } else {
        auto result = rresult.base();
        MySTL::advance(result, -MySTL::distance(first2, last2));
        return result;
    }
}
// 重载版本使用函数comp代替比较操作
// find_end_dispatch 的 forward_iterator_tag 使用Compare重载版本
template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag, Compare cmp) {
    if (first2 == last2)
    {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = MySTL::search(first1, last1, first2, last2, cmp);
            if (new_result == last1)
            {
                return result;
            } else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

// find_end_dispatch 的 bidirectional_iterator使用Compare重载版本
template <class BidrectionalIter1, class BidrectionalIter2, class Compare>
BidrectionalIter1
find_end_dispatch(BidrectionalIter1 first1, BidrectionalIter1 last1,
                  BidrectionalIter2 first2, BidrectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag, Compare cmp) {
    typedef reverse_iterator<BidrectionalIter1> reviter1;
    typedef reverse_iterator<BidrectionalIter2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    // 前向查找, 找到last2出现的第一次
    reviter1 rresult = MySTL::search(reviter1(last1), rlast1, reviter2(last2), rlast2, cmp);
    if (rresult == rlast1)  // 没有找到
    {
        return last1;
    } else {
        auto result = rresult.base();
        MySTL::advance(result, -MySTL::distance(first2, last2));
        return result;
    }
}

/**
 * @brief 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回 last1
 */
template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2, Compare cmp) {
    typedef typename iterator_traits<ForwardIter1>::iterator_catatory Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_catatory Category2;
    return MySTL::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
}

/**
 * @brief 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
 */
template <class InputIter, class ForwardIter>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2) {
    for (; first1 != last1; ++first1) {
        for (auto iter = first2; iter != last2; ++iter) {
            if (*first1 == *iter)
                return first1;
        }
    }
    return last1;
}
// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter, class ForwardIter, class Compare>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2, Compare cmp) {
    for (; first1 != last1; ++first1) {
        for (auto iter = first2; iter != last2; ++iter) {
            if (cmp(*first1, *iter))
                return first1;
        }
    }
    return last1;
}
/**
 * @brief 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
 * f() 可返回一个值，但该值会被忽略
 */
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f) {
    for (; first != last; first++)
        f(*first);
    return f;
}

/**
 * @brief 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
 */
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
    if (first == last) return last;
    auto next = first;
    while (++next != last) {
        if (*first == *next)
            return first;
        first = next;
    }
    return last;
}
// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compare>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compare cmp) {
    if (first == last) return last;
    auto next = first;
    while (++next != last) {
        if (cmp(*first, *next))
            return first;
        first = next;
    }
    return last;
}

/*****************************************************************************************/
// lower_bound
// 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (*mid < value) {
            first = mid;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
    ;
}

// lbound_dispatch 的 random_access_iterator_tag 版本
template <class RandIter, class T>
RandIter
lbound_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    RandIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        mid = first + half;  // random_access_iterator_tag 支持operator+=
        if (*mid < value) {
            first = mid + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
    ;
}
/*
 * @brief 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
 */
template <class ForwardIter, class T>
ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& value) {
    return MySTL::lbound_dispatch(first, last, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// lbound_dispatch 使用 comp 重载的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compare>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag, Compare cmp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0)
    {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (cmp(*mid, value))
        {
            first = mid;
            ++first;
            len = len - half - 1;
        } else
        {
            len = half;
        }
    }
    return first;
}
// lbound_dispatch 使用 comp 重载的 random_access_iterator_tag 版本
template <class RandIter, class T, class Compare>
RandIter
lbound_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag, Compare cmp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    RandIter mid;
    while (len > 0)
    {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (cmp(*mid, value))
        {
            first = mid;
            ++first;
            len = len - half - 1;
        } else
        {
            len = half;
        }
    }
    return first;
}
/*
 * @brief lower_bound 使用 comp 重载的版本
 */
template <class ForwardIter, class T, class Compare>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return MySTL::lbound_dispatch(first, last, value, cmp);
}

/*****************************************************************************************/
// upper_bound
// 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// ubound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (value < *mid) {
            len = half;
        } else {
            first = mid;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

// ubound_dispatch 的 random_access_iterator_tag 版本
template <class RandIter, class T>
RandIter
ubound_dispatch(RandIter first, RandIter last,
                const T& value, random_access_iterator_tag) {
    auto len = last - first;
    auto half = len;
    RandIter mid;
    while (len > 0)
    {
        half = len >> 1;
        mid = first + half;
        if (value < *mid)
        {
            len = half;
        } else
        {
            first = mid + 1;
            len = len - half - 1;
        }
    }
    return first;
}
/**
 * @brief 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
 */
template <class ForwardIter, class T>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value) {
    return MySTL::ubound_dispatch(first, last, value);
}

// 重载版本使用函数对象 comp 代替比较操作
// ubound_dispatch 使用 comp 重载的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compare>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag, Compare comp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0)
    {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (comp(value, *mid))
        {
            len = half;
        } else
        {
            first = mid;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

// ubound_dispatch 使用 comp 重载的 random_access_iterator_tag 版本
template <class RandIter, class T, class Compare>
RandIter
ubound_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag, Compare cmp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    RandIter mid;
    while (len > 0)
    {
        half = len >> 1;
        mid = first;
        mid = first + half;
        if (cmp(value, *mid))
        {
            len = half;
        } else
        {
            first = mid + 1;
            len = len - half - 1;
        }
    }
    return first;
}
/**
 * @brief upper_bound 使用 comp 重载的版本
 */
template <class ForwardIter, class T, class Compare>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return MySTL::ubound_dispatch(first, last, value, cmp);
}

/**
 * @brief 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
 */
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value) {
    auto i = MySTL::lower_bound(first, last, value);
    return i != last && !(value < *i);
}
/**
 * @brief binary_search 使用 comp 的重载版本
 */
template <class ForwardIter, class T, class Compare>
bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    auto i = MySTL::lower_bound(first, last, value, cmp);
    return i != last && !cmp(value, *i);
}

/*****************************************************************************************/
// equal_range
// 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
// 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
/*****************************************************************************************/
template <class ForwardIter, class T>
MySTL::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid, left, right;
    while (len > 0) {
        if (*mid < value) {
            first = mid;
            ++first;
            len = len - half - 1;
        } else if (value < *mid) {
            len = half;
        } else {
            left = MySTL::lower_bound(first, last, value);
            MySTL::advance(first, len);
            right = MySTL::upper_bound(++mid, first, value);
            return MySTL::pair<ForwardIter, ForwardIter>(left, right);
        }
    }
    return MySTL::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random_access_iterator_tag 版本
template <class RandIter, class T>
MySTL::pair<RandIter, RandIter>
erange_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag) {
    auto len = last - first;
    auto half = len;
    RandIter mid, left, right;
    while (len > 0) {
        half = len >> 1;
        mid = first + half;
        if (*mid < value) {
            first = mid + 1;
            len = len - half - 1;
        } else if (value < *mid) {
            len = half;
        } else {
            left = MySTL::lower_bound(first, mid, value);
            right = MySTL::upper_bound(++mid, first + len, value);
            return MySTL::pair<RandIter, RandIter>(left, right);
        }
    }
    return MySTL::pair<RandIter, RandIter>(last, last);
}

/**
 * @brief 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
 */
template <class ForwardIter, class T>
MySTL::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value) {
    return erange_dispatch(first, last, value, MySTL::iterator_category(first));
}

// 重载版本使用函数comp代替比较操作
// erange_dispatch 的 forward_iterator_tag 使用Compare重载版本
template <class ForwardIter, class T, class Compare>
MySTL::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compare cmp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid, left, right;
    while (len > 0)
    {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (cmp(*mid, value)) {
            first = mid;
            ++first;
            len = len - half + 1;
        } else if (cmp(value, *mid)) {
            len = half;
        } else {
            left = MySTL::lower_bound(first, last, value, cmp);
            MySTL::advance(first, len);
            right = MySTL::upper_bound(++mid, first, value, cmp);
            return MySTL::pair<ForwardIter, ForwardIter>(first, last);
        }
    }
    return MySTL::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random_access_iterator_tag 使用Compare重载版本
template <class RandIter, class T, class Compare>
MySTL::pair<RandIter, RandIter>
erange_dispatch(RandIter first, RandIter last,
                const T& value, random_access_iterator_tag, Compare cmp) {
    auto len = last - first;
    auto half = len;
    RandIter mid, left, right;
    while (len > 0) {
        half = len >> 1;
        mid = first + half;
        if (cmp(*mid, value)) {
            first = mid + 1;
            len = len - half - 1;
        } else if (cmp(value, *mid)) {
            len = half;
        } else {
            left = MySTL::lower_bound(first, mid, value, cmp);
            right = MySTL::upper_bound(++mid, first + len, value, cmp);
            return MySTL::pair<RandIter, RandIter>(left, right);
        }
    }
    return MySTL::pair<RandIter, RandIter>(last, last);
}

/**
 * @brief 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
 */
template <class ForwardIter, class T, class Compare>
MySTL::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return erange_dispatch(first, last, value, MySTL::iterator_category(first), cmp);
}

/**
 * @brief 查将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
 */
template <class ForwardIter, class Size, class Generator>
void generate(ForwardIter first, ForwardIter last, Size n, Generator gen) {

    for (; first != last; first++) {
        *first = gen();
    }
}

/**
 * @brief 将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
 */
template <class ForwardIter, class Size, class Generator>
void generate_n(ForwardIter first, Size n, Generator gen) {

    for (; n > 0; --n, ++first) {
        *first = gen();
    }
}

/**
 * @brief 判断序列一S1 是否包含序列二S2, S1, S2 是有序的
 */
template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1, InputIter2 last1,
              InputIter2 first2, InputIter2 last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            return false;
        } else if (*first1 < *first2) {
            ++first1;
        } else {
            ++first1, ++first2;
        }
    }
    return first2 == last2;
}

// 重载版本使用函数comp代替比较操作
/**
 * @brief 判断序列一S1 是否包含序列二S2, S1, S2 是有序的
 */
template <class InputIter1, class InputIter2, class Compare>
bool includes(InputIter1 first1, InputIter2 last1,
              InputIter2 first2, InputIter2 last2, Compare cmp) {
    while (first1 != last1 && first2 != last2) {
        if (cmp(*first2, *first1)) {
            return false;
        } else if (cmp(*first1, *first2)) {
            ++first1;
        } else {
            ++first1, ++first2;
        }
    }
    return first2 == last2;
}

// TODO: 弄明白 is_heap()使用
/**
 * @brief 检查[first, last)内的元素是否为一个堆, 如果是, 返回true
 */
template <class RandIter>
bool is_heap(RandIter first, RandIter last) {
    auto n = MySTL::distance(first, last);
    auto parent = 0;
    for (auto child = 1; child < n; child++) {
        if (first[parent] < first[child])
            return false;
        if ((child & 1) == 0) ++parent;
    }
    return true;
}

// 重载版本使用函数comp代替比较操作
/**
 * @brief 检查[first, last)内的元素是否为一个堆, 如果是, 返回true
 */
template <class RandIter, class Compare>
bool is_heap(RandIter first, RandIter last, Compare cmp) {
    auto n = MySTL::distance(first, last);
    auto parent = 0;
    for (auto child = 1; child < n; child++) {
        if (cmp(first[parent] < first[child]))
            return false;
        if ((child & 1) == 0) ++parent;
    }
    return true;
}

// is_sorted
/**
 * @brief 检查[first, last)内的元素是否升序，如果是升序，则返回 true, [first, last)是有序队列
 */
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last) {
    if (first == last)
        return true;
    auto next = first;
    ++next;
    for (; next != last; first = next, ++next) {
        if (*next < *first)
            return false;
    }
    return true;
}

// 重载版本使用函数comp代替比较操作
/**
 * @brief 检查[first, last)内的元素是否升序，如果是升序，则返回 true, [first, last)是有序队列
 */
template <class ForwardIter, class Compare>
bool is_sorted(ForwardIter first, ForwardIter last, Compare cmp) {
    if (first == last)
        return true;
    auto next = first;
    ++next;
    for (; next != last; first = next, ++next) {
        if (cmp(*next, *first))
            return false;
    }
    return true;
}

// median
/**
 * @brief 找出三个值的中间值
 */
template <class T>
const T& median(const T& left, const T& mid, const T& right) {
    if (left < mid)
        if (mid < right)  // left < mid < right
            return mid;
        else if (left < right)  // left < right <= mid
            return right;
        else  // right <= left < mid
            return left;
    else if (left < right)  // mid <= left < right
        return left;
    else if (mid < right)  // mid < right <= left
        return right;
    else  // right <= mid <= left
        return mid;
}

// 重载版本使用函数comp代替比较操作
/**
 * @brief 找出三个值的中间值
 */
template <class T, class Compare>
const T& median(const T& left, const T& mid, const T& right, Compare cmp) {
    if (comp(left, mid))
        if (comp(mid, right))
            return mid;
        else if (comp(left, right))
            return right;
        else
            return left;
    else if (comp(left, right))
        return left;
    else if (comp(mid, right))
        return right;
    else
        return mid;
}

// max_element
/**
 * @brief 返回一个迭代器，指向序列中最大的元素
 */
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last) {
        if (*result < *first)
            result = first;
    }
    return result;
}

// 重载版本使用函数对象 comp 代替比较操作
/**
 * @brief 返回一个迭代器，指向序列中最大的元素
 */
template <class ForwardIter, class Compared>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last) {
        if (comp(*result, *first))
            result = first;
    }
    return result;
}

// min_element
/**
 * @brief 返回一个迭代器，指向序列中最小的元素
 */
template <class ForwardIter>
ForwardIter min_elememt(ForwardIter first, ForwardIter last) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last)
    {
        if (*first < *result)
            result = first;
    }
    return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter min_elememt(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last)
    {
        if (comp(*first, *result))
            result = first;
    }
    return result;
}


// swap_ranges
/**
 * @brief 将[first1, last1)从 first2 开始，交换相同个数元素
 * 交换的区间长度必须相同，两个序列不能互相重叠，返回一个迭代器指向序列二最后一个被交换元素的下一位置
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter2
swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
            ForwardIter2 first2) {
    for (; first1 != last1; ++first1, ++first2)
    {
        MySTL::iter_swap(first1, first2);
    }
    return first2;
}

// transform
/**
 * @brief 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
 */
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter
transform(InputIter first, InputIter last,
          OutputIter result, UnaryOperation unary_op) {
    for (; first != last; first++, result++) {
        *result = unary_op(*first);
    }
}

/**
 * @brief 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
 */
template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
OutputIter
transform(InputIter1 first1, InputIter1 last1,
          InputIter2 first2, OutputIter result, BinaryOperation binary_op) {
    for (; first1 != last1; first1++, first2++, result++) {
        *result = binary_op(*first1, *first2);
    }
}

// remove_copy
/**
 * @brief 移除区间内与指定 value 相等的元素(不相等的复制到目标位置)，并将结果复制到以 result 标示起始位置的容器上
 * Ignores all elements that are equal to value
 */
template <class InputIter, class OutputIter, class T>
OutputIter
remove_copy(InputIter first, InputIter last, OutputIter result, const T& value) {
    for (; first != last; ++first)
    {
        if (*first != value) {
            *result++ = *first;
        }
    }
    return result;
}

// remove
/**
 * @brief 移除所有与指定 value 相等的元素
 * 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
 */
template <class ForwardIter, class T>
ForwardIter remove(ForwardIter first, ForwardIter last, const T&value){
    first = MySTL::find(first, last, value);
    auto next = first;
    return first == last ? first : MySTL::remove_copy(++next, last, first, value);
}

/**
 * @brief remove_copy_if Ignores all elements for which predicate p returns true
 * 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
 */
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter remove_copy_if(InputIter first, InputIter last,
                          OutputIter result, UnaryPredicate unary_pred) {
    for (; first != last; ++first)
    {
        if (!unary_pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

// remove_if
/**
 * @brief 移除区间内所有令一元操作 unary_pred 为 true 的元素
 */
template <class ForwardIter, class UnaryPredicate>
ForwardIter remove_if(ForwardIter first, ForwardIter last,
                      UnaryPredicate unary_pred) {
    first =  find_if(first, last, unary_pred);
    if(first != last){
        for (ForwardIter i = first; ++i != last;) {
            if(!unary_pred(*i)){
                *first++ = MySTL::move(*i);
            }
        }
    }
    return first;
}

// replace
/**
 * @brief 将区间内所有的 old_value 都以 new_value 替代
 */
template <class ForwardIter, class T>
void replace(ForwardIter first, ForwardIter last,
             const T& old_value, const T& new_value) {
    for (; first != last; ++first) {
        if (*first == old_value)
            *first = new_value;
    }
}

// relpace_if
/**
 * @brief 将区间内所有令一元操作为 unary_pred 为 true 的元素都用 new_value 代替
 */
template<class ForwardIter, class UnaryPredicate, class T>
void replace_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred, const T&new_value ){
    for(;first != last; ++first){
        if(unary_pred(*first))
        *first = new_value;
    }
}

// replace_copy
/**
 * @brief 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
 */
template <class InputIter, class OutputIter, class T>
OutputIter
replace_copy(InputIter first, InputIter last,
             OutputIter result, const T& old_value, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = *first == old_value ? new_value : *first;
    }
    return result;
}

// replace_copy_if
/**
 * @brief 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
 */
template <class InputIter, class OutputIter, class UnaryPredicate, class T>
OutputIter
replace_copy_if(InputIter first, InputIter last,
                OutputIter result, UnaryPredicate unary_pred, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = unary_pred(*first) ? new_value : *first;
    }
}

/*****************************************************************************************/
// reverse
// 将[first, last)区间内的元素反转
/*****************************************************************************************/
// reverse_dispatch 的 bidirectional_iterator_tag 版本
template <class BidrectionalIter>
void reverse_dispatch(BidrectionalIter first, BidrectionalIter last, bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last)
            return;
        MySTL::iter_swap(first++, last);
    }
}

// reverse_dispatch 的 random_access_iterator_tag 版本
template <class RandIter>
void reverse_dispatch(RandIter first, RandIter last, random_access_iterator_tag) {
    while (first < last) {
        MySTL::iter_swap(first++, last);
    }
}

/**
 * @brief 将[first, last) 区间内的元素反转
 */
template <class BidrectionalIter>
void reverse(BidrectionalIter first, BidrectionalIter last) {
    MySTL::reverse_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// random_shuffle
// 将[first, last)内的元素次序随机重排
// 重载版本使用一个产生随机数的函数对象 rand
/*****************************************************************************************/
template <class RandIter>
void random_shuffle(RandIter first, RandIter last) {
    if (first == last) return;
    srand((unsigned)time(0));
    for (auto i = first + 1; i != last; ++i) {
        MySTL::iter_swap(i, first + (rand() % (i - first + 1)));
    }
}

/*****************************************************************************************/
// rotate
// 将[first, middle)内的元素和 [middle, last)内的元素互换，可以交换两个长度不同的区间
// 返回交换后 middle 的位置
/*****************************************************************************************/
// rotate_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter>
ForwardIter
rotate_dispatch(ForwardIter first, ForwardIter mid,
                ForwardIter last, forward_iterator_tag) {
    auto first2 = mid;
    do {
        MySTL::swap(*first++, *first2++);
        if (first == mid) mid = first2;
    } while (first2 != last); // 后半段移动到前面
    auto new_mid = first; // iterator return value
    first2 = mid;
    while (first2 != last) { // 不同长度区间调整
        MySTL::swap(*first++, *first2++);
        if (first == mid) {
            mid = first2;
        } else if (first2 == last) {
            first2 = mid;
        }
    }
    return new_mid;
}

// rotate_dispatch 的 bidirectional_iterator_tag 版本
template <class BidrectionalIter>
BidrectionalIter
rotate_dispatch(BidrectionalIter first, BidrectionalIter mid,
                BidrectionalIter last, bidirectional_iterator_tag) {
    MySTL::reverse_dispatch(first, mid, bidirectional_iterator_tag());
    MySTL::reverse_dispatch(mid, last, bidirectional_iterator_tag());
    while (first != mid && mid != last) {
        MySTL::swap(*first++, *--last);
    }
    if (first == mid) {
        MySTL::reverse_dispatch(mid, last, bidirectional_iterator_tag());
        return last;
    } else {
        MySTL::reverse_dispatch(find, mid, bidirectional_iterator_tag());
        return first;
    }
}

// 求最大因子
template <class EuclideanRingElement>
EuclideanRingElement rgcd(EuclideanRingElement m, EuclideanRingElement n) {
    while (n != 0) {
        auto t = m % n;
        m = n;
        n = t;
    }
}

// XXX: figure out the rotate() function for  
// rotate_dispatch 的 random_access_iterator_tag 版本
template <class RandIter>
RandIter
rotate_dispatch(RandIter first, RandIter mid, RandIter last, random_access_iterator_tag) {
    auto n = last - first;
    auto l = mid - first;
    auto r = n - l;
    auto result = first + (last - mid);
    if (l == r) {
        MySTL::swap_ranges(first, mid, mid);
        return result;
    }
    auto cycle_times = rgcs(n, l);
    for (auto i = 0; i < cycle_times; i++) {
        auto tmp = *first;
        auto p = first;
        if (l < r) {
            for (auto j = 0; j < r / cycle_times; j++) {
                if (p > first + r) {
                    *p = *(p - r);
                    p -= r;
                }
                *p = *(p - r);
                p += l;
            }
        } else {
            for (auto j = 0; j < i / cycle_times - 1; ++j) {
                if (p < last - l) {
                    *p = *(p + l);
                    p += l;
                }
                *p = *(p - r);
                p -= r;
            }
        }
        *p = tmp;
        ++first;
    }
    return result;
}

/**
 * @brief 将[first, middle)内的元素和 [middle, last)内的元素互换，可以交换两个长度不同的区间, 返回交换后 middle 的位置
 */
template <class ForwardIter>
ForwardIter
rotate(ForwardIter first, ForwardIter mid, ForwardIter last) {
    if (first == mid) return last;
    if (mid == last) return first;
    return MySTL::rotate_dispatch(first, mid, last, iterator_category(first));
}

/**
 * @brief 行为与rotate类似, 将结果保存至 result 所指的容器中
 */
template <class ForwardIter, class OutputIter>
ForwardIter
rotate_copy(ForwardIter first, ForwardIter mid,
            ForwardIter last, OutputIter result) {
    return MySTL::copy(first, last, MySTL::copy(mid, last, result));
}
// TODO: is_permutation() 
/**
 * @brief 判断[first1,last1)是否为[first2, last2)的排列组合 
 */




}  // namespace MySTL

#endif /* MY_ALGO_H */
