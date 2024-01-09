#ifndef MY_UNINITIALIZE_H
#define MY_UNINITIALIZE_H

// 这个头文件用于对未初始化空间构造元素

#include "construct.h"
#include "type_traits.h"
#include "algobase.h"
#include "myallocator.h" 

namespace MySTL {

/*****************************************************************************************/
// uninitialized_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::true_type) {
    return MySTL::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninitialized_copy(InputIter first, InputIter last, ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; first != last; ++first, ++cur)
            MySTL::construct(&*cur, *first);
    } catch (...) {
        for (; first != result; ++result)
            MySTL::destroy(&*result);
    }
    return cur;
}

/**
 * @brief 把 [first, last) 上的内容复制到以 result 为起始处的空间
 * @return 返回复制结束的位置
 */
template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last,
                               ForwardIter result) {
    return unchecked_uninitialized_copy(first, last, result,
                                        std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_copy_n
// copy from [first , first + n) to strat of result
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninitialized_copy_n(InputIter first, Size n, ForwardIter result, std::true_type) {
    return MySTL::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninitialized_copy_n(InputIter first, Size n, ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; n > 0; ++first, ++cur)
            MySTL::construct(&*cur, *first);
    } catch (...) {
        for (; result != cur; cur--)
            MySTL::destroy(&*result);
    }
    return cur;
}

/**
 * @brief copy from [first , first + n) to strat of result
 * @return 返回拷贝的最后位置的迭代器
 */
template <class InputIter, class Size, class ForwardIter>
ForwardIter
uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
    return unchecked_uninitialized_copy_n(first, n, result,
                                          std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_fill
// fill [first, last) with elems
/*****************************************************************************************/
template <class ForwardIter, class T>
void unchecked_uninitialized_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type) {
    MySTL::fill(first, last, value);
}

template <class ForwardIter, class T>
void unchecked_uninitialized_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type) {
    auto cur = first;
    try {
        for (; cur != last; ++cur) {
            MySTL::construct(&*cur, value);
        }
    } catch (...) {
        for (; first != cur; ++first)
            MySTL::destroy(&*first);
    }
}

/**
 * @brief fill [first, last) with elems
 */
template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
    unchecked_uninitialized_fill(first, last, value,
                                 std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_fill_n
//  fill [first, first + n) with elems
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter 
unchecked_uninitialized_fill_n(ForwardIter first, Size n, const T& value, std::true_type) {
    return MySTL::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter
unchecked_uninitialized_fill_n(ForwardIter first, Size n, const T& value, std::false_type) {
    auto cur = first;
    try {
        for (; n > 0; n--, cur++) {
            MySTL::construct(&*cur, *first);
        }
    } catch (...) {
        for (; first != cur; ++first)
            MySTL::destroy(&*first);
    }
    return cur;
}

/**
 * @brief fill [first, first + n) with elems
 * @return 元素填充最终位置
 */
template <class ForwardIter, class Size, class T>
ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
    return unchecked_uninitialized_fill_n(first, n, value,
                                          std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_move
// move [first, last) to the begin of result
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninitialized_move(InputIter first, InputIter last,
                             ForwardIter result, std::true_type) {
    return MySTL::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninitialized_move(InputIter first, InputIter last,
                             ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; cur != last; cur++, result++) {
            MySTL::construct(&*cur, MySTL::move(*first));
        }
    } catch (...) {
        for (; first != cur; cur++)
            MySTL::destroy(result, cur);
    }
    return result;
}

/**
 * @brief move [first, last) to the begin of result
 */
template <class InputIter, class ForwardIter>
ForwardIter
uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
    return unchecked_uninitialized_move(first, last, result, std::is_trivially_move_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_move_n
/*****************************************************************************************/
template <class InputIter, class Size, class ForwradIter>
ForwradIter
unchecked_uninitialized_move_n(InputIter first, Size n, ForwradIter result, std::true_type) {
    return MySTL::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_unitialized_move_n(InputIter first, Size n, ForwardIter result, std::false_type) {
    auto cur = result;
    try {
        for (; n > 0; n--, cur++, first++) {
            MySTL::construct(&*cur, MySTL::move(*first));
        }
    } catch (...) {
        MySTL::destroy(&*cur);
    }
    return result;
}

/**
 * @brief unitialized_move_n()
 */
template <class InputIter, class Size, class ForwardIter>
ForwardIter
uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
    return unchecked_uninitialized_move_n(first, n, result, std::is_trivially_move_assignable<typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace MySTL
#endif /* MY_UNINITIALIZE_H */
