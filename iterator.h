#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H
#include <cstddef>
#include "type_traits.h"

// 类型萃取技术

namespace MySTL{

/**
 * @defgroup iterator_tags Iterator Tags
 * 这些是空类型, 用于区分不同的迭代器, 区分度不在于内容而在于命名.
 * 一些算法能够应用不同的迭代器类型
 * detail from iterator.cc from stl and bits/stl_iterator_base_types.h
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};         //
struct bidirectional_iterator_tag : public forward_iterator_tag {};  // 双向的
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category            iterator_catatory;
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

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true> : public iterator_traits_impl<Iterator, 
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
    {};

// 萃取迭代器特性
template <class Iterator>
struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// TODO: 查阅资料, 偏特化
// 原生指针偏特化
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef ptrdiff_t                   difference_type;
};

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
template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
iterator_category(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_eype*>(0);
}
// 萃取value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
iterator_category(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}
/**
 * 计算迭代器距离
 * 
 */
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

template <class InputIter>
typename iterator_traits<InputIter>::difference_type
distance(InputIter first, InputIter last) {
    return distance_dispatch(first, last, iterator_category(first));
}

/**
 * @brief
 * 迭代器前进 n 个距离
 * advance
 */
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

template <class InputIter, class Distance>
void advance_dispatch(InputIter& i, Distance n) {
    advance_dispatch(i, n, iterator_category(i));
}

/**
 * @brief reverse_iterator
 * 反向迭代器
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

    // 运算符重载
    reference operator*() const {
        auto tmp = current;
        return *--tmp;
    }

    self& operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
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
