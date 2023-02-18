#ifndef UTIL_H
#define UTIL_H
/**
 * general uitlty
 */
#include <cstdlib>
#include <type_traits>
#include "type_traits.h"

namespace MySTL {

/**
 * move
 */
template <typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

/**
 * forward an lvalue.
 */
template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

/**
 * forward an rvalue.
 */
template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value,
                  "template argument"
                  " substituing T is an lvalue reference type");
    return static_cast<T&&>(arg);
}

/**
 * swap for single element
 */
template <typename Tp>
void swap(Tp& a, Tp& b) noexcept {
    Tp tmp = move(a);
    a = move(b);
    b = move(tmp);
}

// FIXME: 区间元素交换
template <typename Tp, size_t Nm>
void swap(Tp (&a)[Nm], Tp (&b)[Nm]) noexcept {
    for (size_t n = 0; n < Nm; n++) {
        swap(a[n], b[n]);
    }
}

// FIXME: 区间元素交换
template <class ForwardIter_1, class ForwardIter_2>
ForwardIter_2 swap_range(ForwardIter_1 first_1, ForwardIter_1 last_1, ForwardIter_2 first_2) {
    for (; first_1 != last_1; first_1++, (void)first_2++)
        swap(*first_1, *first_2);
    return first_2;
}

/*
template <class Tp, size_t Nm>
void swap(TP (&a)[Nm], Tp (&b)[Nm]) {
    swap_range(a, a + Nm, b);
}
 */

/**
 * ------------pair-----------
 * 结构体模板: pair
 */
template <class Ty1, class Ty2>
struct pair {
    typedef Ty1 first_type;
    typedef Ty2 second_type;
    first_type first;
    second_type second;
    // 默认构造函数
    template <class Other1 = Ty1, class Other2 = Ty2,
              typename std::enable_if<std::is_default_constructible<Other1>::value &&
									  std::is_default_constructible<Other2>::value,
                                      void>::type = 0>  // 检查是否可以默认构造
    constexpr pair() :
        first(), second() {}

    /// implict constructiable Construct from two const lvalues, allowing implicit conversions.
    template <typename U1 = Ty1, typename U2 = Ty2,
              typename std::enable_if<std::is_copy_constructible<U1>::value &&
                                      std::is_copy_constructible<U2>::value &&
                                      std::is_convertible<const U1&, Ty1>::value &&
                                      std::is_convertible<const U2&, Ty2>::value,
                                      int>::type = 0>
    constexpr pair(const Ty1& a, const Ty2& b) :
        first(a), second(b) {}

    /// explicit constructible for this type
    template <class U1 = Ty1, class U2 = Ty2,
              typename std::enable_if<std::is_copy_constructible<U1>::value &&
                                      std::is_copy_constructible<U2>::value &&
                                     (!std::is_convertible<const U1&, Ty1>::value ||
                                      !std::is_convertible<const U2&, Ty2>::value),
                                      int>::type = 0>
    explicit constexpr pair(const Ty1& a, const Ty2& b) :
        first(a), second(b) {}
    pair(const pair& a) = default;
    pair(pair&& a) = default;
    /// implicit constructible for other type
    template <class Other1 = Ty1, class Other2 = Ty2,
              typename std::enable_if<std::is_constructible<Ty1, Other1>::value &&
                                      std::is_constructible<Ty2, Other2>::value &&
                                      std::is_convertible<Other1&&, Ty1>::value &&
                                      std::is_convertible<Other2&&, Ty2>::value,
                                      int>::type = 0>
    constexpr pair(Other1& a, Other2& b) :
        first(forward<Other1>(a)), second(forward<Other2>(b)) {}
    /// explict constructible for other type
    template <class Other1 = Ty1, class Other2 = Ty2,
              typename std::enable_if<std::is_constructible<Ty1, Other1>::value &&
                                      std::is_constructible<Ty2, Other2>::value &&
                                     (!std::is_convertible<Other1&&, Ty1>::value ||
                                      !std::is_convertible<Other2&&, Ty2>::value),
                                      int>::type = 0>
    explicit constexpr pair(Other1& a, Other2& b) :
        first(forward<Other1>(a)), second(forward<Other2>(b)) {}
	/// implicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<std::is_constructible<Ty1, const Other1&>::value &&
                                      std::is_constructible<Ty2, const Other2&>::value &&
                                      std::is_convertible<Other1, Ty1>::value &&
                                      std::is_convertible<Other2, Ty2>::value,
                                      int>::value = 0>
    constexpr pair(pair<Other1, Other2>&& other) :
        first(MySTL::forward<Other1>(other.first)),
        second(MySTL::forward<Other2>(other.second)) {}
	/// explicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<std::is_constructible<Ty1, Other1>::value &&
                                      std::is_constructible<Ty2, Other2>::value &&
                                     (!std::is_convertible<Other1, Ty1>::value ||
                                      !std::is_convertible<Other2, Ty2>::value),
                                      int>::type = 0>
    explicit constexpr pair(pair<Other1, Other2>&& other) :
        first(forward<Other1>(other.first)), second(forward<Other2>(other.second)) {}
	/// copy assign for this pair
	pair&operator=(const pair&rval){
        if (this != &rval) {
            first = rval.first;
            second = rval.second;
        }
        return *this;
    }
	/// move assign for this pair
	pair& operator=(pair&&rval){
		if(this != &rval){
			first = move(rval.first);
			second = move(rval.second);
		}
		return *this;
	}
	/// copy assign for other pair
    template <class Other1, class Other2>
    pair& operator=(pair<Other1, Other2>& other) {
        first = other.first;
        second = other.second;
		return *this;
    }
	/// move assign for other pair
    template <class Other1, class Other2>
    pair& operator=(pair<Other1, Other2>&& other) {
        first = forward<Other1>(other.first);
        second = forward<Other2>(other.second);
        return *this;
    }
    ~pair() = default;

    void swap(pair& other) {
        if(this != &other){
			swap(first, other.first);
            swap(second, other.second);
        }
    }
};

// reload compare operator
// lexicographically compares the values in the pair
// reload ==
template <class Ty1, class Ty2>
bool operator==(const pair<Ty1, Ty2>& larg, const pair<Ty1, Ty2>& rarg) {
    return larg.first == rarg.first && larg.second == rarg.second;
}
// reload != 
template <class Ty1, class Ty2>
bool operator!=(const pair<Ty1, Ty2>& larg, const pair<Ty1, Ty2>& rarg) {
    return !(larg == rarg);
}
// reload <
template <class T1, class T2>
bool operator<(const pair<T1, T2>& larg, const pair<T1, T2>& rarg) {
    return larg.first < rarg.first || (larg.first == rarg.first &&
                                       larg.second < rarg.second);
}
// reload <=
template <class T1, class T2>
bool operator<=(const pair<T1, T2>& larg, const pair<T1, T2>& rarg) {
    return !(rarg < larg);
}
// reload >
template <class Ty1, class Ty2>
bool operator>(const pair<Ty1, Ty2>& larg, const pair<Ty1, Ty2>& rarg) {
    return rarg < larg;
}
// reload >=
template <class Ty1, class Ty2>
bool operator>=(const pair<Ty1, Ty2>& larg, const pair<Ty1, Ty2>& rarg) {
    return !(larg < rarg);
}
// reload MySTL::swap
template <class Ty1, class Ty2>
void swap(pair<Ty1, Ty2>& larg, pair<Ty1, Ty2>& rarg) {
    larg.swap(rarg);
}

// 两个变量成为pair
template<class T1, class T2>
pair< T1,  T2> make_pair(T1&&first, T2&&second){
    return pair<T1, T2>(forward<T1>(first), forward<T2>(second));
}

}  // namespace MySTL

#endif /*UTIL_H*/
