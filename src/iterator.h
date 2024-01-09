#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

//
// 迭代器标签, 迭代器模板, 迭代器萃取, 迭代器距离, 迭代器前进, 反向迭代器
//

#include <cstddef>

#include "type_traits.h"

namespace MySTL{

/**
 * @defgroup iterator_tags Iterator Tags
 * 这些是空类型, 用于区分不同的迭代器, 区分度不在于内容而在于命名.
 * 一些算法能够应用不同的迭代器类型
 * detail from iterator.cc from stl and bits/stl_iterator_base_types.h
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};          //
struct bidirectional_iterator_tag : public forward_iterator_tag {};  // 双向的
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category, class T, class Distance = std::ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category            iterator_category;
    typedef T                   value_type;
    typedef Pointer             pointer;
    typedef T*                  reference;
    typedef T&                  difference_type;
};

// 迭代器萃取
template <class T>
struct has_iterator_cat {
private:
    struct two { char a; char b; };

    template <class U>
    static two test(...);

    template <class U>
    static char test(typename U::iterator_category* = 0);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

// ********************************************* iterator_traits_impl ********************************************

/**
 * @brief 用于提取迭代器的五种类型定义
 * 第二个bool 值用于判断迭代器是否有iterator_category
 */
template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true> {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::difference_type   difference_type;
};

// ********************************************* iterator_traits_helper ********************************************

/**
 * @brief 用于判断迭代器是否有iterator_category
 */
template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true> : public iterator_traits_impl<Iterator, 
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag >::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
    {};

// ********************************************* iterator_traits ********************************************

// 萃取迭代器特性
template <class Iterator>
struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 原生指针偏特化
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef ptrdiff_t                   difference_type;
};

// 原生指针偏特化(const)
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
    typedef ptrdiff_t                   difference_type;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of : public m_bool_constant<
        std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value> {};

// 萃取某种迭代器
template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator_tag : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iter>
struct is_iterator : public m_bool_constant<is_input_iterator<Iter>::value ||
                                            is_output_iterator<Iter>::value> {};

// 萃取category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

// 萃取distance_type
// FIGUREOUT: why use a pointer
template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

/*****************************************************************************************/
// distance, 计算迭代器距离
// 针对input_iterator_tag, 逐个累加, random_access_iterator_tag, 直接相减
/*****************************************************************************************/
// 输入迭代器版本
template <class InputIter>
typename iterator_traits<InputIter>::difference_type
distance_dispatch(InputIter first, InputIter last, input_iterator_tag) {
    typename iterator_traits<InputIter>::difference_type n = 0;
    for (; first != last; first++)
        n++;
    return n;
}

// 随机迭代器版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag) {
    return last - first;
}

/**
 * @brief 计算迭代器距离
 */
template <class InputIter>
typename iterator_traits<InputIter>::difference_type
distance(InputIter first, InputIter last) {
    return distance_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// advance, 前进迭代器
// input_iterator_tag, 逐个前进, random_access_iterator_tag, 直接相加
// bidirectional_iterator_tag, 双向迭代器, 根据距离的正负选择前进或后退
/*****************************************************************************************/
// 输入迭代器版本
template <class InputIter, class Distance>
void advance_dispatch(InputIter& i, Distance n, input_iterator_tag) {
    while (n--) {
        i++;
    }
}

// 双向迭代器版本
template <class BidirectionalIter, class Distance>
void advance_dispatch(BidirectionalIter& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0)
        while (n--) i++;
    else
        while (n++) i--;
}

// 随机迭代器
template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag) {
    i += n;
}

/**
 * @brief 迭代器前进 n 个距离
 */
template <class InputIter, class Distance>
void advance(InputIter& i, Distance n) {
    advance_dispatch(i, n, iterator_category(i));
}

/**
 * @brief 反向迭代器
 * @tparam Iterator 直接填入正向迭代器即可
 */
template <class Iterator>
class reverse_iterator {
private:
    Iterator current;

public:
	typedef typename  iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename  iterator_traits<Iterator>::value_type        value_type;
    typedef typename  iterator_traits<Iterator>::pointer           pointer;
    typedef typename  iterator_traits<Iterator>::reference         reference;
    typedef typename  iterator_traits<Iterator>::difference_type   difference_type;

	typedef Iterator											   iterator_type; 
	typedef reverse_iterator<Iterator>							   self;

	// 构造函数
    reverse_iterator(){};
    explicit reverse_iterator(iterator_type i) : current(i) {}
    reverse_iterator(const self & rhs) : current(rhs.current) {}

public:
    iterator_type base() const { return current; }  // 取出正向迭代器

    reference operator*() const {
        auto tmp = current;
        return *--tmp;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator--() {
        ++current;
        return *this;
    }

    self& operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }

    self& operator++() {
        --current;
        return *this;
    }

    self& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    self& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    self operator+(difference_type n) const { return self(current - n); }
    self operator-(difference_type n) const { return self(current + n); }
	reference operator[](difference_type n){
		return *(*this + n);
	} 
};

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return rhs.base() - lhs.base();
}

template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return rhs.base() < lhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return rhs < lhs;
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return !(lhs < rhs);
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
    return !(lhs > rhs);
}

} // namespace MySTL

#endif /* MY_ITERATOR_H */
