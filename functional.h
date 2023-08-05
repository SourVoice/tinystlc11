#ifndef MY_FINCTIONAL_H
#define MY_FINCTIONAL_H
// 仿函数相关定义. C++11已经弃用
#include <functional>
#include <cstddef>
namespace MySTL{

// 一元函数的参数类型和返回值类型, 通常被继承
template <class Arg, class Result>
struct unarg_function {
    typedef Arg    argument_type;
    typedef Result result_type;
};
// 二元函数的参数类型和返回值类型, 通常被继承
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1   first_argument_type;
    typedef Arg2   secone_argument_type;
    typedef Result result_type;
};
// 算数类仿函数
template <class T>
struct plus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x + y; }
};
template <class T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x - y; }
};
template <class T>
struct multipies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x * y; }
};
template <class T>
struct divides : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x / y; }
};
template <class T>
struct modulus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x % y; }
};
template <class T>
struct negate : public unarg_function<T, T> {
    T operator()(const T& x) const { return -x; }
};

// 加法的证同元素
template <class T>
T identity_element(plus<T>) { return T(0); }
// 乘法的证同元素
template <class T>
T identity_element(multipies<T>) { return T(1); }

// 关系运算符仿函数
template <class T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x == y; }
};
template <class T>
struct not_equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x != y; }
};
template <class T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x > y; }
};
template <class T>
struct less : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T>
struct greater_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x >= y; }
};
template <class T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x <= y; }
};
// 逻辑运算符仿函数
template <class T>
struct logical_and : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x && y; }
};
template <class T>
struct logical_or : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x || y; }
};
template <class T>
struct logical_not : public unarg_function<T, bool> {
    bool operator()(const T& x) const { return !x; }
};

// 证同函数（identity function)。任何数值通过此函数后，不会有任何改变
// 此式运用于<stl_set.h>，用来指定RB-tree所需的 KeyofValue op
// 那是因为set元素的键值即实值，所以采用identity
template <class T>
struct identity : public unarg_function<T, bool> {
    const T& operator()(const T& x) const { return x; }
};
// 选择函数（selection function):接受-个pair，传回其第一元素
// 此式运用于<stl_map.h>，用来指定RB-tree所需的 Keyofvalue op
// 由于map系以pair元素的第一元素为其键值，所以采用select1st
template <class Pair>
struct select1st : public unarg_function<Pair, typename Pair::first_type> {
    const typename Pair::first_type& operator()(const Pair& x) const {
        return x.first;
    }
};
// 选择函数:接受一个pair，传回其第二元素
// SGI STL并未运用此式
template <class Pair>
struct select2nd : public unarg_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type& operator()(const Pair& x) const {
        return x.second;
    }
};
// 投射函数:传回第一参数，忽略第二参数
// SGI STL并未运用此式
template <class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};
// 投射函数:传回第二参数，忽略第一参数
// SGI STL并未运用此式
template <class Arg1, class Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
    Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};
// 哈希
// 大部分类型无操作
template <class Key>
struct hash {};

template <class T>
struct hash<T*> {
    size_t operator()(T* p) const noexcept {
        return reinterpret_cast<size_t>(p);
    }
};
// TODO: 整数类型, 使用宏展开
#define MYSTL_TRIVIAL_HASH_FCN(Type)                            \
    template <>                                                 \
    struct hash<Type> {                                         \
        size_t operator()(Type val) const noexcept              \
        { return static_cast<size_t>(val); }                    \
    };
// template <>
// struct hash<bool> {
//     size_t operator()(bool val) const noexcept { return static_cast<size_t>(val); }
// };
MYSTL_TRIVIAL_HASH_FCN(bool)
MYSTL_TRIVIAL_HASH_FCN(char)
MYSTL_TRIVIAL_HASH_FCN(signed char)
MYSTL_TRIVIAL_HASH_FCN(unsigned char)
MYSTL_TRIVIAL_HASH_FCN(wchar_t)
MYSTL_TRIVIAL_HASH_FCN(char16_t)
MYSTL_TRIVIAL_HASH_FCN(char32_t)
MYSTL_TRIVIAL_HASH_FCN(short)
MYSTL_TRIVIAL_HASH_FCN(unsigned short)
MYSTL_TRIVIAL_HASH_FCN(int)
MYSTL_TRIVIAL_HASH_FCN(unsigned int)
MYSTL_TRIVIAL_HASH_FCN(long)
MYSTL_TRIVIAL_HASH_FCN(unsigned long)
MYSTL_TRIVIAL_HASH_FCN(long long)
MYSTL_TRIVIAL_HASH_FCN(unsigned long long)
#undef MYSTL_TRIVIAL_HASH_FCN

// 浮点数逐位哈希
inline size_t bitwise_hash(const unsigned char* first, size_t count) {
#if ((__GNUC__ || __clang__) && __SIZEOF_POINTER__ == 8)
    const size_t fnv_offset = __LONG_MAX__;
    const size_t fnv_prime = 1ull;
#else
    const size_t fnv_offset = 2u;
    const size_t fnv_prme = 1u;
#endif
    size_t result = fnv_offset;
    for (size_t i = 0; i < count; i++) {
        result ^= (size_t)first[i];
        result *= fnv_prime;
    }
    return result;
}
template <>
struct hash<float> {
    size_t operator()(const float& val) {
        if (val == 0.0f)
            return 0;
        else
            return bitwise_hash((const unsigned char*)&val, sizeof(float));
    }
};

template <>
struct hash<double> {
    size_t operator()(const double& val) {
        if (val == 0.0f)
            return 0;
        else
            return bitwise_hash((const unsigned char*)&val, sizeof(double));
    }
};

template <>
struct hash<long double> {
    size_t operator()(const long double& val) {
        if (val == 0.0f)
            return 0;
        else
            return bitwise_hash((const unsigned char*)&val, sizeof(long double));
    }
};

};  // namespace MySTL

#endif /* MY_FINCTIONAL_H */
