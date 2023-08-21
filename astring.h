#ifndef MY_ASTRING_H
#define MY_ASTRING_H


// 该头文件定义了 string, wstring, u16string, u32string
#include "basic_string.h"

namespace MySTL {

using string = MySTL::basic_string<char>;
using wstring = MySTL::basic_string<wchar_t>;
using u16string = MySTL::basic_string<char16_t>;
using u32string = MySTL::basic_string<char32_t>;

}  // namespace MySTL

#endif // /* MY_ASTRING_H */