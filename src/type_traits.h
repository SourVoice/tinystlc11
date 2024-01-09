#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H

// 
// <type_traits>用于元编程，萃取类型信息，获取受支持的操作(如移动构造函数)等
// 本头文件主要使用辅助类，intergral_constant, bool_constant, true_type, false_type
// 

#include <type_traits>

namespace MySTL {

template <class T, T v>
struct m_intergral_constant {
    static constexpr T value = v;
};
template <bool b>
using m_bool_constant = m_intergral_constant<bool, b>;

typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;

// is_pair
// forward 前置声明
template <class T1, class T2>
struct pair;
// forward 前置声明 -end
template <class T>
struct is_pair : MySTL::m_false_type {};
template <class T1, class T2>
struct is_pair<MySTL::pair<T1, T2>> : MySTL::m_true_type {};

}  // namespace MySTL

#endif /* MY_TYPE_TRAITS_H */
