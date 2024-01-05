#ifndef MY_ALGO_H
#define MY_ALGO_H

// 该头文件包含MySTL中的全部算法

#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "algobase.h"
#include "heap_algo.h"
#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"

namespace MySTL {

/*********************************************  不修改序列的操作 ********************************************/

/**
 * @brief 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && unary_pred(*first))
        first++;
    return first == last;
}

/**
 * @brief 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && !unary_pred(*first))
        first++;
    return first != last;
}

/**
 * @brief 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
 */
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*first))
            return false;
    }
    return true;
}

/**
 * @brief 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
 * @note f() 可返回一个值，但该值会被忽略
 */
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f) {
    for (; first != last; first++)
        f(*first);
    return f;
}

/**
 * @brief 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==
 * @return 返回元素相等的个数
 */
template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value) {
    size_t n = 0;
    for (; first != last; ++first) {
        if (*first == value)
            ++n;
    }
    return n;
}

/**
 * @brief 对[first, last)区间内的每个元素都进行一元 unary_pred 操作
 * @return 返回结果为 true 的个数
 */
template <class InputIter, class UnaryPredicate>
size_t count_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    size_t n = 0;
    for (; first != last; ++first) {
        if (unary_pred(*first))
            ++n;
    }
    return n;
}

/**
 * @brief 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
 */
template <class InputIter, class T>
InputIter find(InputIter first, InputIter last, const T& value) {
    while (first != last && *first != value)
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
 */
template <class InputIter, class UnaryPredicate>
InputIter find_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && !unary_pred(*first))
        ++first;
    return first;
}

/**
 * @brief 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
 */
template <class InputIter, class UnaryPredicate>
InputIter find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred) {
    while (first != last && unary_pred(*first))
        ++first;
    return first;
}

/*****************************************************************************************/
// search()
// 在[first1, last1)中查找[first2, last2)的首次出现点, 返回首次出现处的迭代器
/*****************************************************************************************/
/**
 * @brief 在[first1, last1)中查找[first2, last2)的首次出现点
 * @return [first, last1) 为空返回first1, 没有找到返回last1
 * @note first2, last2 也代表sub_first, sub_last
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

// 重载版本使用cmp代替比较操作
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

/*****************************************************************************************/
// search_n()
// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
/*****************************************************************************************/
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

// 重载版本使用函数对象 cmp 代替比较操作
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
    if (first2 == last2) {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = MySTL::search(first1, last1, first2, last2);
            if (new_result == last1) {
                return result;
            } else {
                result = new_result; // update record
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
    if (rresult == rlast1) {  // 没有找到
        return last1;
    } else {
        auto result = rresult.base();
        MySTL::advance(result, -MySTL::distance(first2, last2));
        return result;
    }
}

// 重载版本使用函数cmp代替比较操作
// find_end_dispatch 的 forward_iterator_tag 使用Compare重载版本
template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag, Compare cmp) {
    if (first2 == last2) {
        return last1;
    } else {
        auto result = last1;
        while (true) {
            auto new_result = MySTL::search(first1, last1, first2, last2, cmp);
            if (new_result == last1) {
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
    if (rresult == rlast1) {  // 没有找到
        return last1;
    } else {
        auto result = rresult.base();
        MySTL::advance(result, -MySTL::distance(first2, last2));
        return result;
    }
}

/**
 * @brief 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方
 * @return 指向范围 [first, last) 中 [first2, last2) 最后一次出现的起始的迭代器。
 * 如果 [first2, last2) 为空或找不到这种序列，那么就会返回 last1
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2) {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return MySTL::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
}

// find_end() 使用 cmp 查找的重载版本
template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter1 last1,
         ForwardIter2 first2, ForwardIter2 last2, Compare cmp) {
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return MySTL::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), cmp);
}

/*****************************************************************************************/
// find_first_of()
// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
/*****************************************************************************************/
/**
 * @brief 在[first1, last1)中查找[first2, last2)中的某些元素
 * @return 指向范围 [first, last) 中等于范围 [s_first, s_last) 中某个元素的首个元素。
 *         如果 [s_first, s_last) 为空或找不到这种元素，那么就会返回 last。
 * @note 's' stand for sub 
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

// 重载版本使用函数对象 cmp 代替比较操作
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

/*****************************************************************************************/
// adjacent_find
// 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
/*****************************************************************************************/
/**
 * @brief 找出第一对匹配的相邻元素，缺省使用 operator== 比较
 * @return 如果找到返回一个迭代器，指向这对元素的第一个元素, 否则返回last
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

/********************************************* （有序范围上的）二分搜索操作 ********************************************/

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
            len = len - (half + 1);
        } else {
            len = half;
        }
    }
    return first;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class T, class Compare>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag, Compare cmp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (cmp(*mid, value)) {
            first = mid;
            ++first;
            len = len - (half + 1);
        } else {
            len = half;
        }
    }
    return first;
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
        mid = first + half;  // random_access_iterator_tag 支持operator+=
        if (*mid < value) {
            first = mid + 1;
            len = len - (half + 1);
        } else {
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
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (cmp(*mid, value)) {
            first = mid;
            ++first;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

/**
 * @brief 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
 */
template <class ForwardIter, class T>
ForwardIter 
lower_bound(ForwardIter first, ForwardIter last, const T& value) {
    return MySTL::lbound_dispatch(first, last, value, iterator_category(first));
}

/**
 * @brief 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
 */
template <class ForwardIter, class T, class Compare>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return MySTL::lbound_dispatch(first, last, value, iterator_category(first), cmp);
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
ubound_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag) {
    auto len = last - first;
    auto half = len;
    RandIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first + half;
        if (value < *mid) {
            len = half;
        } else {
            first = mid + 1;
            len = len - half - 1;
        }
    }
    return first;
}

// 重载版本使用函数对象 comp 代替比较操作
// ubound_dispatch 使用 comp 重载的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compare>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag, Compare comp) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
        if (comp(value, *mid)) {
            len = half;
        } else {
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
    while (len > 0) {
        half = len >> 1;
        mid = first + half;
        if (cmp(value, *mid)) {
            len = half;
        } else {
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
    return MySTL::ubound_dispatch(first, last, value, iterator_category(first));
}

template <class ForwardIter, class T, class Compare>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return MySTL::ubound_dispatch(first, last, value, iterator_category(first), cmp);
}

/*****************************************************************************************/
// binary_search()
// 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
/*****************************************************************************************/
/**
 * @brief 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
 */
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value) {
    auto i = MySTL::lower_bound(first, last, value);
    return i != last && !(value < *i);
}

// binary_search() 查找条件 使用 comp 的重载版本
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
// erange_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
MySTL::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag) {
    auto len = MySTL::distance(first, last);
    auto half = len;
    ForwardIter mid, left, right;
    while (len > 0) {
        half = len >> 1;
        mid = first;
        MySTL::advance(mid, half);
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

// 重载版本使用函数comp代替比较操作
// erange_dispatch 的 forward_iterator_tag 使用Compare重载版本
template <class ForwardIter, class T, class Compare>
MySTL::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last, const T& value, forward_iterator_tag, Compare cmp) {
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
erange_dispatch(RandIter first, RandIter last, const T& value, random_access_iterator_tag, Compare cmp) {
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
 * @return 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
 */
template <class ForwardIter, class T>
MySTL::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value) {
    return erange_dispatch(first, last, value, MySTL::iterator_category(first));
}

/**
 * @brief 使用 compare 代替 operator==
 */
template <class ForwardIter, class T, class Compare>
MySTL::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value, Compare cmp) {
    return erange_dispatch(first, last, value, MySTL::iterator_category(first), cmp);
}

/*****************************************************************************************/
// generate
// 查将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
/*****************************************************************************************/
/**
 * @brief 查将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
 */
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen) {
    for (; first != last; first++) {
        *first = gen();
    }
}

/*****************************************************************************************/
// generate_n
// 如果 n > 0，则将给定函数对象 g 所生成的值对始于 first 的范围的前 n 个元素赋值。否则不做任何事。
/*****************************************************************************************/
/**
 * @brief 如果 n > 0，则将给定函数对象 g 所生成的值对始于 first 的范围的前 n 个元素赋值。否则不做任何事。
 */
template <class ForwardIter, class Size, class Generator>
void generate_n(ForwardIter first, Size n, Generator gen) {
    for (; n > 0; --n, ++first) {
        *first = gen();
    }
}

/*****************************************************************************************/
// includes
// 判断序列一S1 是否包含序列二S2. S1, S2 是有序的
/*****************************************************************************************/
/**
 * @brief 判断序列一S1 是否包含序列二S2. S1, S2 是有序的
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

/*********************************************  堆操作 ********************************************/

/**
 * @brief 检查[first, last)内的元素是否为一个堆(二叉树), 如果是, 返回true
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
template <class RandIter, class Compare>
bool is_heap(RandIter first, RandIter last, Compare cmp) {
    auto n = MySTL::distance(first, last);
    auto parent = 0;
    for (auto child = 1; child < n; child++) {
        if (cmp(first[parent], first[child]))
            return false;
        if ((child & 1) == 0) ++parent;
    }
    return true;
}

/*********************************************  排序操作 ********************************************/

/*****************************************************************************************/
// is_sorted
// @brief 检查[first, last)内的元素是否升序，如果是升序，则返回 true, [first, last)是有序队列
/*****************************************************************************************/
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

/*****************************************************************************************/
// median
// @brief 找出三个值的中间值
/*****************************************************************************************/
/**
 * @brief 找出三个值的中间值
 */
template <class T>
const T& median(const T& left, const T& mid, const T& right) {
    if (left < mid) {
        if (mid < right)  // left < mid < right
            return mid;
        else if (left < right)  // left < right <= mid
            return right;
        else  // right <= left < mid
            return left;
    }
    else if (left < right)  // mid <= left < right
        return left;
    else if (mid < right)  // mid < right <= left
        return right;
    else  // right <= mid <= left
        return mid;
}

// 重载版本使用函数comp代替比较操作
template <class T, class Compare>
const T& median(const T& left, const T& mid, const T& right, Compare cmp) {
    if (cmp(left, mid)) {
        if (cmp(mid, right))
            return mid;
        else if (cmp(left, right))
            return right;
        else
            return left;
    }
    else if (cmp(left, right))
        return left;
    else if (cmp(mid, right))
        return right;
    else
        return mid;
}

/*****************************************************************************************/
// max_element
// max_element，指向序列中最大的元素
/*****************************************************************************************/
/**
 * @brief max_element，指向序列中最大的元素
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

/*****************************************************************************************/
// min_element
// 返回一个迭代器，指向序列中最小的元素
/*****************************************************************************************/
/**
 * @brief 返回一个迭代器，指向序列中最小的元素
 */
template <class ForwardIter>
ForwardIter min_elememt(ForwardIter first, ForwardIter last) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last) {
        if (*first < *result)
            result = first;
    }
    return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter min_element(ForwardIter first, ForwardIter last, Compared comp) {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last) {
        if (comp(*first, *result))
            result = first;
    }
    return result;
}


/*****************************************************************************************/
// swap_ranges
// 将[first1, last1)从 first2 开始，交换相同个数元素
/*****************************************************************************************/
/**
 * @brief 将[first1, last1)从 first2 开始，交换相同个数元素
 * 交换的区间长度必须相同，两个序列不能互相重叠，返回一个迭代器指向序列二最后一个被交换元素的下一位置
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter2
swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
            ForwardIter2 first2) {
    for (; first1 != last1; ++first1, ++first2)
        MySTL::iter_swap(first1, first2);
    return first2;
}

/*****************************************************************************************/
// transform
// 以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
/*****************************************************************************************/
/**
 * @brief 以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
 * @return 指向最后一个变换的元素的输出迭代器。
 */
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter
transform(InputIter first, InputIter last,
          OutputIter result, UnaryOperation unary_op) {
    for (; first != last; first++, result++)
        *result = unary_op(*first);
    return result;
}

/**
 * @brief 函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
 */
template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
OutputIter
transform(InputIter1 first1, InputIter1 last1,
          InputIter2 first2, OutputIter result, BinaryOperation binary_op) {
    for (; first1 != last1; first1++, first2++, result++)
        *result = binary_op(*first1, *first2);
    return result;
}

/*****************************************************************************************/
// remove_copy
// 移除区间内与指定 value 相等的元素(不相等的复制到目标位置)，并将结果复制到以 result 标示起始位置的容器上
/*****************************************************************************************/
/**
 * @brief 移除区间内与指定 value 相等的元素(不相等的复制到目标位置)，并将结果复制到以 result 标示起始位置的容器上
 * @return 指向最后被复制元素之后的迭代器。
 * @note Ignores all elements that are equal to value, 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
 */
template <class InputIter, class OutputIter, class T>
OutputIter
remove_copy(InputIter first, InputIter last, OutputIter result, const T& value) {
    for (; first != last; ++first) {
        if (*first != value) {
            *result++ = *first;
        }
    }
    return result;
}

/*****************************************************************************************/
// remove
// 移除所有与指定 value 相等的元素, 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
/*****************************************************************************************/
/**
 * @brief 移除所有与指定 value 相等的元素
 * @return 指向最后被复制元素之后的迭代器。
 * @note 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
 */
template <class ForwardIter, class T>
ForwardIter
remove(ForwardIter first, ForwardIter last, const T& value) {
    first = MySTL::find(first, last, value);
    auto next = first;
    return first == last ? first : MySTL::remove_copy(++next, last, first, value);
}

/*****************************************************************************************/
// remove_copy_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
/*****************************************************************************************/
/**
 * @brief 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
 * @note remove_copy_if Ignores all elements for which predicate p returns true
 */
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter
remove_copy_if(InputIter first, InputIter last,
               OutputIter result, UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (!unary_pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

/*****************************************************************************************/
// remove_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素
/*****************************************************************************************/
/**
 * @brief 移除区间内所有令一元操作 unary_pred 为 true 的元素
 * @return 新范围的尾后迭代器
 */
template <class ForwardIter, class UnaryPredicate>
ForwardIter
remove_if(ForwardIter first, ForwardIter last,
          UnaryPredicate unary_pred) {
    first = find_if(first, last, unary_pred);
    if (first != last) {
        for (ForwardIter i = first; ++i != last;) {
            if (!unary_pred(*i)) {
                *first++ = MySTL::move(*i);
            }
        }
    }
    return first;
}

/*****************************************************************************************/
// replace
// 将区间内所有的 old_value 都以 new_value 替代
/*****************************************************************************************/
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

/*****************************************************************************************/
// replace_if
// 将区间内所有令一元操作为 unary_pred 为 true 的元素都用 new_value 代替
/*****************************************************************************************/
/**
 * @brief 将区间内所有令一元操作为 unary_pred 为 true 的元素都用 new_value 代替
 */
template <class ForwardIter, class UnaryPredicate, class T>
void replace_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred, const T& new_value) {
    for (; first != last; ++first) {
        if (unary_pred(*first))
            *first = new_value;
    }
}

/*****************************************************************************************/
// replace_copy
// 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
/*****************************************************************************************/
/**
 * @brief 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
 * @return 指向最后复制元素后一元素的迭代器。
 */
template <class InputIter, class OutputIter, class T>
OutputIter
replace_copy(InputIter first, InputIter last,
             OutputIter result, const T& old_value, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = (*first == old_value ? new_value : *first);
    }
    return result;
}

/*****************************************************************************************/
// replace_copy_if
// 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
// 使用一元函数unqry_pred代替相等
/*****************************************************************************************/
/**
 * @brief 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
 * @note 使用一元函数unqry_pred代替相等
 */
template <class InputIter, class OutputIter, class UnaryPredicate, class T>
OutputIter
replace_copy_if(InputIter first, InputIter last,
                OutputIter result, UnaryPredicate unary_pred, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = (unary_pred(*first) ? new_value : *first);
    }
    return result;
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
        MySTL::iter_swap(first++, --last);
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
// reverse_copy
// 行为与 reverse 类似，不同的是将结果复制到 result 所指容器中
/*****************************************************************************************/
template <class BidirectionalIter, class OutputIter>
OutputIter
reverse_copy(BidirectionalIter first, BidirectionalIter last,
             OutputIter result) {
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}

/*****************************************************************************************/
// random_shuffle
// 将[first, last)内的元素次序随机重排
/*****************************************************************************************/
/**
 * @brief 将[first, last)内的元素次序随机重排
*/
template <class RandIter>
void random_shuffle(RandIter first, RandIter last) {
    if (first == last) return;
    srand((unsigned)time(0));
    for (auto i = first + 1; i != last; ++i) {
        MySTL::iter_swap(i, first + (rand() % (i - first + 1)));
    }
}

// 重载版本使用一个产生随机数的函数对象 rand
template <class RandIter, class RandomNumberGenerator>
void random_shuffle(RandIter first, RandIter last,
                    RandomNumberGenerator& rand) {
    if (first == last) return;
    auto len = MySTL::distance(first, last);
    for (auto i = first + 1; i != last; ++i) {
        MySTL::iter_swap(i, first + (rand(i - first + 1) % len));
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
        MySTL::reverse_dispatch(first, mid, bidirectional_iterator_tag());
        return first;
    }
}

// auxiliary 求最大因子
template <class EuclideanRingElement> // (欧几里得环元素)
EuclideanRingElement 
rgcd(EuclideanRingElement m, EuclideanRingElement n) {
    while (n != 0) {
        auto t = m % n;
        m = n;
        n = t;
    }
    return m;
}

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
    auto cycle_times = rgcd(n, l);
    for (auto i = 0; i < cycle_times; i++) {
        auto tmp = *first;
        auto p = first;
        if (l < r) {
            for (auto j = 0; j < r / cycle_times; j++) {
                if (p > first + r) {
                    *p = *(p - r);
                    p -= r;
                }
                *p = *(p + l);
                p += l;
            }
        } else {
            for (auto j = 0; j < l / cycle_times - 1; ++j) {
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
 * @return 返回交换后新的mid的位置
 */
template <class ForwardIter>
ForwardIter
rotate(ForwardIter first, ForwardIter mid, ForwardIter last) {
    if (first == mid) return last;
    if (mid == last) return first;
    return MySTL::rotate_dispatch(first, mid, last, iterator_category(first));
}

/*****************************************************************************************/
//  rotate_copy
//  判断[first1,last1)是否为[first2, last2)的排列组合
/*****************************************************************************************/
/**
 * @brief 行为与rotate类似, 将结果保存至 result 所指的容器中
 */
template <class ForwardIter, class OutputIter>
ForwardIter
rotate_copy(ForwardIter first, ForwardIter mid,
            ForwardIter last, OutputIter result) {
    return MySTL::copy(first, mid, MySTL::copy(mid, last, result));
}

/*****************************************************************************************/
//  is_permutation
//  判断[first1,last1)是否为[first2, last2)的排列组合
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation_aux(ForwardIter1 first1, ForwardIter2 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        BinaryPred pred) {
    constexpr bool is_ra_it = MySTL::is_random_access_iterator<ForwardIter1>::value &&
                              MySTL::is_random_access_iterator<ForwardIter2>::value;
    if (is_ra_it) {
        auto len1 = last1 - first1;
        auto len2 = last2 - first2;
        if (len1 != len2) return false;
    }

    // 找到相同前缀段
    for (; first1 != last1 && first2 != last2; ++first1, (void)++first2) {
        if (!pred(*first1, *first2))
            break;
    }
    if (is_ra_it) {
        if (first1 == last1) return true;
    } else {
        auto len1 = MySTL::distance(first1, last1);
        auto len2 = MySTL::distance(first2, last2);
        if (len1 == 0 && len2 == 0) return true;
        if (len1 != len2) return false;
    }

    for (auto i = first1; i != last1; i++) {
        bool is_repeated = false;

        // 避免重复查找
        for (auto j = first1; j != i; j++) {
            if (pred(*j, *i)) {
                is_repeated = true;
                break;
            }
        }

        if (!is_repeated) {
            // 计算 *i 在 [first2, last2) 中出现的次数
            auto c2 = 0;
            for (auto j = first2; j != last2; j++) {
                if (pred(*i, *j)) {
                    ++c2;
                }
            }
            if (c2 == 0) return false;

            // 计算 *i 在 [first1, last1) 中出现的次数
            auto c1 = 1;
            auto j = i;
            for (++j; j != last1; j++) {
                if (pred(*i, *j)) {
                    ++c1;
                }
            }
            if (c1 != c2)
                return false;
        }
    }
    return true;
}

/**
 * @brief 判断[first1,last1)是否为[first2, last2)的排列组合
 */
template <class ForwardIter1, class ForwardIter2>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2) {
    typedef typename iterator_traits<ForwardIter1>::value_type v1;
    typedef typename iterator_traits<ForwardIter2>::value_type v2;
    static_assert(std::is_same<v1, v2>::value,
                  "the value type of ForwardIter1 and ForwardIter2 must be same");
    return MySTL::is_permutation_aux(first1, last1, first2, last2,
                                     MySTL::equal_to<v1>());
}

/**
 * @brief 判断[first1,last1)是否为[first2, last2)的排列组合， 并以二元谓词 pred 作为比较标准
 */
template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    BinaryPred pred) {
    return MySTL::is_permutation_aux(first1, last1, first2, last2, pred);
}

/*****************************************************************************************/
// next_permutation
// 取得[first, last)所标示序列的下一个排列组合，如果没有下一个排序组合，返回 false，否则返回 true
/*****************************************************************************************/
/**
 * @brief 将 [first, last) 的下一个排列组合存储于 [first, last) 中, 如果不存在下一个排列组合, 则返回 false
 */
template <class BidrectionalIter>
bool next_permutation(BidrectionalIter first, BidrectionalIter last) {
    auto i = last;
    if (first == last || first == --i)
        return false;

    for (;;) {
        auto ii = i;
        if (*--i < *ii) {
            auto j = last;
            while (!(*i < *--j)) {}// 找到大于 *i 的元素
            MySTL::iter_swap(i, j);
            MySTL::reverse(ii, last);
            return true;
        }
        if (i == first) {
            MySTL::reverse(first, last);
            return false;
        }
    }
}

// next_permutation 的重载版本, 使用 comp 作为比较标准
template <class BidrectionalIter, class Compare>
bool next_permutation(BidrectionalIter first, BidrectionalIter last, Compare cmp) {
    auto i = last;
    if (first == last || first == --i)
        return false;

    for (;;) {
        auto ii = i;
        if (cmp(*--i, *ii)) {
            auto j = last;
            while (!cmp(*i, *--j)) {}
                MySTL::iter_swap(i, j);
                MySTL::reverse(ii, last);
                return true;
        }
        if (i == first) {
            MySTL::reverse(first, last);
            return false;
        }
    }
}

/*****************************************************************************************/
// prev_permutation
// 将 [first, last) 的上一个排列组合存储于 [first, last) 中, 如果不存在上一个排列组合, 则返回 false
/*****************************************************************************************/
/**
 * @brief 将 [first, last) 的上一个排列组合存储于 [first, last) 中, 如果不存在上一个排列组合, 则返回 false
 */
template <class BidrectionalIter>
bool prev_permutation(BidrectionalIter first, BidrectionalIter last) {
    auto i = last;
    if (first == last || first == --i)
        return false;

    for (;;) {
        auto ii = i;
        if (*ii < *--i) {
            auto j = last;
            while (!(*i > *--j)) {} // 找到大于 *i 的元素
            MySTL::iter_swap(i, j);
            MySTL::reverse(ii, last);
            return true;
        }
        if (i == first) {
            MySTL::reverse(first, last);
            return false;
        }
    }
}

// prev_permutation 的重载版本, 使用 comp 作为比较标准
template <class BidrectionalIter, class Compare>
bool prev_permutation(BidrectionalIter first, BidrectionalIter last, Compare cmp) {
    auto i = last;
    if (first == last || first == --i)
        return false;

    for (;;) {
        auto ii = i;
        if (cmp(*ii, *--i)) {
            auto j = last;
            while (!cmp(*--j, *i)) {}
            MySTL::iter_swap(i, j);
            MySTL::reverse(ii, last);
            return true;
        }
        if (i == first) {
            MySTL::reverse(first, last);
            return false;
        }
    }
}

/*****************************************************************************************/
// merge
// 将两个有序序列 [first1, last1) 和 [first2, last2) 合并存储于以 result 为起始的序列中
/*****************************************************************************************/
/**
 * @brief 将两个有序序列 [first1, last1) 和 [first2, last2) 合并存储于以 result 为起始的序列中
 * @return 返回一个迭代器指向存储的最后一个元素的下一个位置
 */
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2,
                 OutputIter result) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

// merge 的重载版本, 使用 comp 作为比较标准
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter merge(InputIter1 first1, InputIter1 last1,
                 InputIter2 first2, InputIter2 last2,
                 OutputIter result, Compare cmp) {
    while (first1 != last1 && first2 != last2) {
        if (cmp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return MySTL::copy(first2, last2, MySTL::copy(first1, last1, result));
}

/*****************************************************************************************/
// inplace_merge and its auxiliary functions
// 把连接在一起的两个有序序列结合成单一序列并保持有序
/*****************************************************************************************/
template <class BidrectionalIter, class Distance>
void merge_without_buffer(BidrectionalIter first, BidrectionalIter middle,
                          BidrectionalIter last, Distance len1, Distance len2) {
    if (len1 == 0 || len2 == 0)
        return;

    if (len1 + len2 == 2) {
        if (*middle < *first)
            MySTL::iter_swap(first, middle);
        return;
    }
    auto     first_cnt = first;
    auto     second_cnt = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
        len11 = len1 >> 1;
        MySTL::advance(first_cnt, len11);
        second_cnt = MySTL::lower_bound(middle, last, *first_cnt);
        len22 = MySTL::distance(middle, second_cnt);
    } else {
        len22 = len2 >> 1;
        MySTL::advance(second_cnt, len22);
        first_cnt = MySTL::upper_bound(first, middle, *second_cnt);
        len11 = MySTL::distance(first, first_cnt);
    }
    auto new_middle = MySTL::rotate(first_cnt, middle, second_cnt);
    // 分治
    MySTL::merge_without_buffer(first, first_cnt, new_middle, len11, len22);
    MySTL::merge_without_buffer(new_middle, second_cnt, last, len1 - len11, len2 - len22);
}

// merge_without_buffer 重载版本，使用函数对象 cmp 代替比较操作
template <class BidrectionalIter, class Distance, class Compared>
void merge_without_buffer(BidrectionalIter first, BidrectionalIter middle,
                          BidrectionalIter last, Distance len1, Distance len2,
                          Compared cmp) {
    if (len1 == 0 || len2 == 0)
        return;

    if (len1 + len2 == 2) {
        if (cmp(*middle, *first))
            MySTL::iter_swap(first, middle);
        return;
    }
    auto first_cnt = first;
    auto second_cnt = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
        len11 = len1 >> 1;
        MySTL::advance(first_cnt, len11);
        second_cnt = MySTL::lower_bound(middle, last, *first_cnt, cmp);
        len22 = MySTL::distance(middle, second_cnt);
    } else {
        len22 = len2 >> 1;
        MySTL::advance(second_cnt, len22);
        first_cnt = MySTL::upper_bound(first, middle, *second_cnt, cmp);
        len11 = MySTL::distance(first, first_cnt);
    }
    auto new_middle = MySTL::rotate(first_cnt, middle, second_cnt);
    MySTL::merge_without_buffer(first, first_cnt, new_middle, len11, len22, cmp);
    MySTL::merge_without_buffer(new_middle, second_cnt, last, len1 - len11, len2 - len22, cmp);
}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter1
merge_backward(BidrectionalIter1 first1, BidrectionalIter1 last1,
               BidrectionalIter2 first2, BidrectionalIter2 last2,
               BidrectionalIter1 result) {
    if (first1 == last1)
        return MySTL::copy_backward(first2, last2, result);

    if (first2 == last2)
        return MySTL::copy_backward(first1, last1, result);
    --last1;
    --last2;

    while (true) {
        if (*last2 < *last1) {
            *--result = *last1;
            if (first1 == last1)
                return MySTL::copy_backward(first2, ++last2, result); // 处理完后拷贝没有处理完的一侧
            --last1;
        } else {
            *--result = *last2;
            if (first2 == last2)
                return MySTL::copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

// merge_backward 重载版本，使用函数对象 cmp 代替比较操作
template <class BidrectionalIter1, class BidrectionalIter2, class Compare>
BidrectionalIter1
merge_backward(BidrectionalIter1 first1, BidrectionalIter1 last1,
               BidrectionalIter2 first2, BidrectionalIter2 last2,
               BidrectionalIter1 result, Compare cmp) {
    if (first1 == last1)
        return MySTL::copy_backward(first2, last2, result);

    if (first2 == last2)
        return MySTL::copy_backward(first1, last1, result);
    --last1;
    --last2;

    while (true) {
        if (cmp(*last2, *last1)) {
            *--result = *last1;
            if (first1 == last1)
                return MySTL::copy_backward(first2, ++last2, result);
            --last1;
        } else {
            *--result = *last2;
            if (first2 == last2)
                return MySTL::copy_backward(first1, ++last1, result);
            --last2;
        }
    }
}

template <class BidrectionalIter1, class BidrectionalIter2, class Distance>
BidrectionalIter1
rotate_adaptive(BidrectionalIter1 first, BidrectionalIter1 middle,
                BidrectionalIter1 last, Distance len1, Distance len2,
                BidrectionalIter2 buffer, Distance buffer_size) {
    BidrectionalIter2 buffer_end;
    if (len1 > len2 && len2 <= buffer_size) {  // 缓冲区足够大, 放置序列 2
        buffer_end = MySTL::copy(middle, last, buffer);
        MySTL::copy_backward(first, middle, last);
        return MySTL::copy(buffer, buffer_end, first);
    } else if (len1 <= buffer_size) {  // 缓冲区足够大, 放置序列 1
        buffer_end = MySTL::copy(first, middle, buffer);
        MySTL::copy(middle, last, first);
        return MySTL::copy_backward(buffer, buffer_end, last);
    } else {  // 缓冲区不够大, 调用rotat， 不使用缓冲区
        MySTL::rotate(first, middle, last);
        return first + (last - middle);
    }
}

// 有缓冲区的情况下合并
template <class BidrectionalIter, class Distance, class Pointer>
void merge_adaptive(BidrectionalIter first, BidrectionalIter middle,
                    BidrectionalIter last, Distance len1, Distance len2,
                    Pointer buffer, Distance buffer_size) {
    // 区间长度足够放入缓冲区
    if (len1 <= len2 && len1 <= buffer_size) {
        Pointer buffer_end = MySTL::copy(first, middle, buffer);
        MySTL::merge(buffer, buffer_end, middle, last, first);
    } else if (len2 <= buffer_size) {
        Pointer buffer_end = MySTL::copy(middle, last, buffer);
        MySTL::merge_backward(first, middle, buffer, buffer_end, last);
    } else { // 缓冲区大小不够，使用merge_withou_buffer的算法(分治)
        auto first_cnt = first;
        auto second_cnt = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1 >> 1;
            MySTL::advance(first_cnt, len11);
            second_cnt = MySTL::lower_bound(middle, last, *first_cnt);
            len22 = MySTL::distance(middle, second_cnt);
        } else {
            len22 = len2 >> 1;
            MySTL::advance(second_cnt, len22);
            first_cnt = MySTL::upper_bound(first, middle, *second_cnt);
            len11 = MySTL::distance(first, first_cnt);
        }
        auto new_middle = MySTL::rotate_adaptive(first_cnt, middle, second_cnt, len1 - len11, len2 - len22, buffer, buffer_size);
        MySTL::merge_adaptive(first, first_cnt, new_middle, len11, len22, buffer, buffer_size);
        MySTL::merge_adaptive(new_middle, second_cnt, last, len1 - len11, len2 - len22, buffer, buffer_size);
    }
}

// merge_adaptive 重载版本，使用函数对象 cmp 代替比较操作
template <class BidrectionalIter, class Distance, class Pointer, class Compare>
void merge_adaptive(BidrectionalIter first, BidrectionalIter middle,
                    BidrectionalIter last, Distance len1, Distance len2,
                    Pointer buffer, Distance buffer_size, Compare cmp) {

    // 区间长度足够放入缓冲区
    if (len1 <= len2 && len1 <= buffer_size) {
        Pointer buffer_end = MySTL::copy(first, middle, buffer);
        MySTL::merge(buffer, buffer_end, middle, last, first, cmp);
    } else if (len2 <= buffer_size) {
        Pointer buffer_end = MySTL::copy(middle, last, buffer);
        MySTL::merge_backward(first, middle, buffer, buffer_end, last, cmp);
    } else {
        auto first_cnt = first;
        auto second_cnt = middle;
        Distance len11 = 0;
        Distance len22 = 0;
        if (len1 > len2) {
            len11 = len1 >> 1;
            MySTL::advance(first_cnt, len11);
            second_cnt = MySTL::lower_bound(middle, last, *first_cnt, cmp);
            len22 = MySTL::distance(middle, second_cnt);
        } else {
            len22 = len2 >> 1;
            MySTL::advance(second_cnt, len22);
            first_cnt = MySTL::upper_bound(first, middle, *second_cnt, cmp);
            len11 = MySTL::distance(first, first_cnt);
        }
        auto new_middle = MySTL::rotate_adaptive(first_cnt, middle, second_cnt, len1 - len11, len2 - len22, buffer, buffer_size);
        MySTL::merge_adaptive(first, first_cnt, new_middle, len11, len22, buffer, buffer_size, cmp);
        MySTL::merge_adaptive(new_middle, second_cnt, last, len1 - len11, len2 - len22, buffer, buffer_size, cmp);
    }
}

template <class BidrectionalIter, class T>
void inplace_merge_aux(BidrectionalIter first, BidrectionalIter middle, BidrectionalIter last, T*) {
    auto len1 = MySTL::distance(first, middle);
    auto len2 = MySTL::distance(middle, last);
    temporary_buffer<BidrectionalIter, T> buf(first, last);
    if (!buf.begin()) {
        MySTL::merge_without_buffer(first, middle, last, len1, len2);
    } else {
        MySTL::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
    }
}

// inplace_merge_aux 重载版本，使用函数对象 cmp 代替比较操作
template <class BidrectionalIter, class T, class Compare>
void inplace_merge_aux(BidrectionalIter first, BidrectionalIter middle, BidrectionalIter last, T*, Compare cmp) {
    auto len1 = MySTL::distance(first, middle);
    auto len2 = MySTL::distance(middle, last);
    temporary_buffer<BidrectionalIter, T> buf(first, last);
    if (!buf.begin()) {
        MySTL::merge_without_buffer(first, middle, last, len1, len2, cmp);
    } else {
        MySTL::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), cmp);
    }
}

/**
 * @brief 将两个已排序的序列[first,middle)和[middle,last)合并成单一有序序列.
 * @note 若原来是增序，现在也是递增排序，若原来是递减排序，现在也是递减排序
 */
template <class BidrectionalIter>
void inplace_merge(BidrectionalIter first, BidrectionalIter middle, BidrectionalIter last) {
    if (first == middle || middle == last)
        return;
    MySTL::inplace_merge_aux(first, middle, last, value_type(first));
}

/**
 * @brief inplace_merge的重载版本，使用函数对象 cmp 代替比较操作
 */
template <class BidrectionalIter, class Compare>
void inplace_merge(BidrectionalIter first, BidrectionalIter middle, BidrectionalIter last, Compare cmp) {
    if (first == middle || middle == last)
        return;
    MySTL::inplace_merge_aux(first, middle, last, value_type(first), cmp);
}

/********************************************* 排序操作 ********************************************/

/*****************************************************************************************/
// partial_sort
// 对整个序列做部分排序， 保证较小的 N 个元素以递增顺序置于[first, first + N)中]
/*****************************************************************************************/
/**
 * @brief 对整个序列做部分排序， 保证较小的 N 个元素以递增顺序置于[first, first + N)中]
 * 重排元素，使得范围 [first, middle) 含有范围 [first, last) 中已排序的 middle - first 个最小元素。
 * 不保证保持相等元素间的顺序。未指定范围 [middle, last) 中剩余元素的顺序。
 */
template <class RandomIter>
void partial_sort(RandomIter first, RandomIter middle, RandomIter last) {
    MySTL::make_heap(first, middle);
    for (auto i = middle; i < last; i++) {
        if (*i < *first) {
            MySTL::pop_heap_aux(first, middle, i, distance_type(first), *i);
        }
    }
    MySTL::sort_heap(first, middle);
}

// partial_sort的重载版本，使用函数对象 cmp 代替比较操作
template <class RandomIter, class Compare>
void partial_sort(RandomIter first, RandomIter middle, RandomIter last, Compare cmp) {
    MySTL::make_heap(first, middle, cmp);
    for (auto i = middle; i < last; i++) {
        if (cmp(*i, *first)) {
            MySTL::pop_heap_aux(first, middle, i, distance_type(first), *i, cmp);
        }
    }
    MySTL::sort_heap(first, middle, cmp);
}

/*****************************************************************************************/
// partial_sort_copy and its auxiliary functions
// 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
/*****************************************************************************************/
template <class InputIter, class RandomIter, class Distance>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*) {
    if (result_first == result_last)
        return result_last;
    auto result_iter = result_first;
    while (first != last && result_iter != result_last) {
        *result_iter = *first;
        ++result_iter;
        ++first;
    }
    MySTL::make_heap(result_first, result_iter);
    while (first != last) {
        if (*first < *result_first) {
            MySTL::adjust_heap(result_first, static_cast<Distance>(0), result_iter - result_first, *first);
        }
        ++first;
    }
    MySTL::sort_heap(result_first, result_iter);
    return result_iter;
}

// psort_copy_aux 重载版本，使用函数对象 cmp 代替比较操作
template <class InputIter, class RandomIter, class Distance, class Compare>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*, Compare cmp) {
    if (result_first == result_last)
        return result_last;
    auto result_iter = result_first;
    while (first != last && result_iter != result_last) {
        *result_iter = *first;
        ++result_iter;
        ++first;
    }
    MySTL::make_heap(result_first, result_iter, cmp);
    while (first != last) {
        if (cmp(*first, *result_first)) {
            MySTL::adjust_heap(result_first, static_cast<Distance>(0), result_iter - result_first, *first, cmp);
        }
        ++first;
    }
    MySTL::sort_heap(result_first, result_iter, cmp);
    return result_iter;
}

/**
 * @brief 对序列 [first, last) 进行部分排序，结果保存在 [result_first, result_last) 中
 */
template <class InputIter, class RandomIter>
RandomIter
partial_sort_copy(InputIter first, InputIter last, RandomIter result_first, RandomIter result_last) {
    return MySTL::psort_copy_aux(first, last, result_first, result_last, distance_type(result_first));
}

// cmp 用来比较元素的函数对象
template <class InputIter, class RandomIter, class Compare>
RandomIter
partial_sort_copy(InputIter first, InputIter last,
           RandomIter result_first, RandomIter result_last, Compare cmp) {
    return MySTL::psort_copy_aux(first, last, result_first, result_last, distance_type(result_first), cmp);
}

/*****************************************************************************************/
// partition 
// 对区间内元素重排， 使得谓词 pred 为 true 的元素在前，为 false 的元素在后
/*****************************************************************************************/
/**
 * @brief 对区间内元素重排， 使得谓词 pred 为 true 的元素在前，为 false 的元素在后
 */
template <class BdirectionalIter, class UnaryPredicate>
BdirectionalIter
partition(BdirectionalIter first, BdirectionalIter last, UnaryPredicate unary_pred) {
    while (true) {
        while (first != last && unary_pred(*first)) {
            ++first;
        }
        if (first == last)
            break;
        --last;

        while (first != last && !unary_pred(*last)) {
            --last;
        }
        if (first == last)
            break;

        MySTL::iter_swap(first, last);
        ++first;
    }
    return first;
}

/*****************************************************************************************/
// partition_copy, 行为与partition类似
// 对区间内元素重排， 使得谓词 pred 为 true 的元素在前，为 false 的元素在后
/*****************************************************************************************/
/**
 * @brief 行为与partition类似，谓词 pred 为 true 的元素复制到 result_true 中，为 false 的元素复制到 result_false 中
 * @return 
 */
template <class InputIter, class OutputIter1, class OutputIter2, class UnaryPredicate>
MySTL::pair<OutputIter1, OutputIter2>
partition_copy(InputIter first, InputIter last,
               OutputIter1 result_true, OutputIter2 result_false,
               UnaryPredicate unary_pred) {
    for (; first != last; ++first) {
        if (unary_pred(*first)) {
            *result_true++ = *first;
        } else {
            *result_false++ = *first;
        }
    }
    return MySTL::pair<OutputIter1, OutputIter2>(result_true, result_false);
}

/*****************************************************************************************/
// sort and its auxiliary function
// 将 [first, last) 内的元素重新排序
/*****************************************************************************************/

constexpr static size_t kSmallSectionSize = 128;

template <class Size>
Size slg2(Size n) {
    Size k = 0;
    for (; n > 1; n >>= 1)  // 找出 lgk <= n的k的最大值
        ++k;
    return k;
}

// 分割函数，用来分割序列，返回分割点, 用于quick_sort
template <class RandomIter, class T>
RandomIter
unchecked_partition(RandomIter first, RandomIter last, const T& pivot) {
    while (true) {
        while (*first < pivot)
            ++first;
        --last;
        while (pivot < *last)
            --last;
        if (!(first < last))
            return first;
        MySTL::iter_swap(first, last);
        ++first;
    }
}

// 分割函数unchecked_partition 的重载版本
template <class RandomIter, class T, class Compare>
RandomIter
unchecked_partition(RandomIter first, RandomIter last,
                    const T& pivot, Compare cmp) {
    while (true) {
        while (cmp(*first, pivot))
            ++first;
        --last;
        while (cmp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        MySTL::iter_swap(first, last);
        ++first;
    }
}

// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
template <class RandomIter, class Size>
void intro_sort(RandomIter first, RandomIter last, Size depth_limit) {
    while (static_cast<size_t>(last - first) > kSmallSectionSize) {
        if (depth_limit == 0) {  // 达到最大分割深度限制, 改用 heap_sort
            MySTL::partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        auto mid = MySTL::median(*(first), *(first + (last - first) / 2), *(last - 1));
        auto cut = MySTL::unchecked_partition(first, last, mid);
        MySTL::intro_sort(cut, last, depth_limit);
        last = cut;
    }
}

// 内省式排序 intro_sort 的重载版本
template <class RandomIter, class Size, class Compare>
void intro_sort(RandomIter first, RandomIter last,
                Size depth_limit, Compare cmp) {
    while (static_cast<size_t>(last - first) > kSmallSectionSize) {
        if (depth_limit == 0) {  // 达到最大分割深度限制, 改用 heap_sort
            MySTL::partial_sort(first, last, last, cmp);
            return;
        }
        --depth_limit;
        auto mid = MySTL::median(*(first), *(first + (last - first) / 2), *(last - 1));
        auto cut = MySTL::unchecked_partition(first, last, mid, cmp);
        MySTL::intro_sort(cut, last, depth_limit, cmp);
        last = cut;
    }
}

// 插入排序的辅助函数
template <class RandomIter, class T>
void unchecked_linear_insert(RandomIter last, const T& value) {
    auto next = last;
    --next;
    while (value < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

// 插入排序的辅助函数 unchecked_linear_insert 的重载版本
template <class RandomIter, class T, class Compare>
void unchecked_linear_insert(RandomIter last, const T& value, Compare cmp) {
    auto next = last;
    --next;
    while (cmp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

// 插入排序
template <class RandomIter>
void insertion_sort(RandomIter first, RandomIter last) {
    if (first == last)
        return;
    for (auto i = first + 1; i != last; ++i) {
        auto value = *i;
        if (value < *first) {
            MySTL::copy_backward(first, i, i + 1);
            *first = value;
        } else {
            MySTL::unchecked_linear_insert(i, value);
        }
    }
}

// 插入排序 insertion_sort 的重载版本
template <class RandomIter, class Compare>
void insertion_sort(RandomIter first, RandomIter last, Compare cmp) {
    if (first == last)
        return;
    for (auto i = first + 1; i != last; ++i) {
        auto value = *i;
        if (cmp(value, *first)) {
            MySTL::copy_backward(first, i, i + 1);
            *first = value;
        } else {
            MySTL::unchecked_linear_insert(i, value, cmp);
        }
    }
}

// 插入排序辅助函数
template <class RandomIter>
void unchecked_insertion_sort(RandomIter first, RandomIter last) {
    for (auto i = first; i != last; i++) {
        MySTL::unchecked_linear_insert(i, *i);
    }
}

// 插入排序辅助函数 unchecked_insertion_sort 的重载版本
template <class RandomIter, class Compare>
void unchecked_insertion_sort(RandomIter first, RandomIter last, Compare cmp) {
    for (auto i = first; i != last; i++) {
        MySTL::unchecked_linear_insert(i, *i, cmp);
    }
}

template <class RandomIter>
void final_insertion_sort(RandomIter first, RandomIter last) {
    if (static_cast<size_t>(last - first) > kSmallSectionSize) {
        // 判断元素个数是否大于16
        // 则把区间分割成两段，一端长度为16，另一端为剩余的长度
        MySTL::insertion_sort(first, first + kSmallSectionSize);
        MySTL::unchecked_insertion_sort(first + kSmallSectionSize, last);
    } else {
        MySTL::insertion_sort(first, last);
    }
}

// final_insertion_sort 的重载版本
template <class RandomIter, class Compare>
void final_insertion_sort(RandomIter first, RandomIter last, Compare cmp) {
    if (static_cast<size_t>(last - first) > kSmallSectionSize) {
        MySTL::insertion_sort(first, first + kSmallSectionSize, cmp);
        MySTL::unchecked_insertion_sort(first + kSmallSectionSize, last, cmp);
    } else {
        MySTL::insertion_sort(first, last, cmp);
    }
}

/**
 * @brief 对区间内元素进行排序
 */
template <class RandomIter>
void sort(RandomIter first, RandomIter last) {
    if (first != last) {
        // 采用内省式排序， 将区间分为一个个区间， 然后对整体进行插入排序
        MySTL::intro_sort(first, last, MySTL::slg2(last - first) * 2);
        MySTL::final_insertion_sort(first, last);
    }
}

// 对区间内元素进行排序, 使用 cmp 比较元素
template <class RandomIter, class Compare>
void sort(RandomIter first, RandomIter last, Compare cmp) {
    if (first != last) {
        MySTL::intro_sort(first, last, MySTL::slg2(last - first) * 2, cmp);
        MySTL::final_insertion_sort(first, last, cmp);
    }
}

/*****************************************************************************************/
// nth_element
// 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
/*****************************************************************************************/
/**
 * @brief 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
 */
template <class RandomIter>
void nth_element(RandomIter first, RandomIter nth, RandomIter last) {
    if (nth == last)
        return;
    while (last - first > 3) {
        auto cut = MySTL::unchecked_partition(first, last, MySTL::median(*first, *(first + (last - first) / 2), *(last - 1)));
        if (cut <= nth)   // nth 位于后半段
            first = cut;  // 对后半段进行递归
        else
            last = cut;  // nth 位于前半段, 对左半段分割
    }
    MySTL::insertion_sort(first, last);
}

// nth_element 的重载版本, 使用 cmp 比较元素
template <class RandomIter, class Compare>
void nth_element(RandomIter first, RandomIter nth, RandomIter last, Compare cmp) {
    if (nth == last)
        return;
    while (last - first > 3) {
        auto cut = MySTL::unchecked_partition(first, last,
                                              MySTL::median(*first, *(first + (last - first) / 2),
                                                            *(last - 1)),
                                              cmp);
        if (cut <= nth)   // nth 位于后半段
            first = cut;  // 对后半段进行递归
        else
            last = cut;  // nth 位于前半段, 对左半段分割
    }
    MySTL::insertion_sort(first, last, cmp);
}

/********************************************* 修改序列的操作 ********************************************/

/*****************************************************************************************/
// unique_copy
// 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
/*****************************************************************************************/
// unique_copy 的 forward_iterator_tag 版本, 前向迭代器使用while循环, 将不同元素复制到 result 上
template <class InputIter, class ForwardIter>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last, ForwardIter result, forward_iterator_tag) {
    *result = *first;
    while (++first != last) {
        if (*result != *first)
            *++result = *first;
    }
    return ++result;
}

// unique_copy_dispatch 针对 forward_iterator_tag 使用Compare的重载版本
template <class InputIter, class ForwardIter, class Compare>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last, ForwardIter result, Compare cmp, forward_iterator_tag) {
    *result = *first;
    while (++first != last) {
        if (!cmp(*result, *first))
            *++result = *first;
    }
    return ++result;
}

// unique_copy 的 output_iterator_tag 版本
// 由于 output_iterator 只能进行只写操作，所以不能有 *result != *first 这样的判断
// see output_iterator_tag(https://cplusplus.com/reference/iterator/OutputIterator/)
template <class InputIter, class OutputIter>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last, OutputIter result, output_iterator_tag) {
    auto value = *first;
    *result = value;
    while (++first != last) {
        if (value != *first) {
            value = *first;
            *++result = value;
        }
    }
    return ++result;
}

// unique_copy_dispatch 针对 output_iterator_tag 使用Compare的重载版本
template <class InputIter, class OutputIter, class Compare>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last, OutputIter result, Compare cmp, output_iterator_tag) {
    auto value = *first;
    *result = value;
    while (++first != last) {
        if (!cmp(value, *first)) {
            value = *first;
            *++result = *first;
        }
    }
    return ++result;
}

/**
 * @brief 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
 * @note 输入必须是有序
 */
template <class InputIter, class OutputIter>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result) {
    if (first == last)
        return result;
    return MySTL::unique_copy_dispatch(first, last, result, iterator_category(result));
}

// unique_copy 的重载版本, 使用 comp 比较元素
template <class InputIter, class OutputIter, class Compare>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result, Compare cmp) {
    if (first == last)
        return result;
    return MySTL::unique_copy_dispatch(first, last, result, cmp, iterator_category(result));
}

/*****************************************************************************************/
// unique()
// 从[first, last)中移除重复的元素，序列必须有序, 返回不重复序列的尾部
/*****************************************************************************************/
/**
 * @brief 从[first, last)中移除重复的元素，序列必须有序, 返回不重复序列的尾部
 * @note 序列必须有序
 */
template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last) {
    first = MySTL::adjacent_find(first, last);
    return MySTL::unique_copy(first, last, first); // 没有使用cppreference例子，因为需要处理迭代器类型
}

// unique 的重载版本, 使用 cmp 比较元素
template <class ForwardIter, class Compare>
ForwardIter unique(ForwardIter first, ForwardIter last, Compare cmp) {
    first = MySTL::adjacent_find(first, last, cmp);
    return MySTL::unique_copy(first, last, first, cmp);
}

}  // namespace MySTL

#endif /* MY_ALGO_H */
