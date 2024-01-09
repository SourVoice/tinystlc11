#ifndef MY_BASIC_STRING_H_
#define MY_BASIC_STRING_H_


#include "myallocator.h"
#include "iterator.h"
#include "type_traits.h"
#include "exceptdef.h"
#include "algorithm.h"
#include "uninitialize.h"

#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <type_traits>

#define STRING_INIT_SIZE 32

namespace MySTL {

// 字符串类型萃取器
template <class CharType>
struct char_traits {
    typedef CharType char_type;

    static size_t length(const char_type* s) {
        size_t len = 0;
        for (; *s != char_type(0); s++) {  // char_type(0) means a way to create a null character
            len++;
        }
        return len;
    }

    static size_t compare(const char_type* s1, const char_type* s2, size_t n) {
        for (; n != 0; --n, s1++, s2++) {
            if (*s1 < *s2)
                return -1;
            if (*s2 < *s1)
                return 1;
        }
        return 0;
    }

    static char_type* copy(char_type* dst, char_type* src, size_t n) {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);  // 确保src和dst不会重叠
        char_type* r = dst;
        for (; n != 0; --n, ++dst, ++src)
            *dst = *src;
        return r;
    }

    static char_type* move(char_type* dst, const char_type* src, size_t n) {
        char_type* r = dst;
        if (dst < src) {
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
        } else if (src < dst) {
            dst += n;
            src += n;
            for (; n != 0; --n)
                *--dst = *--src;
        }
        return r;
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count) {
        char_type* r = dst;
        for (; count > 0; count--, dst++)
            *dst = ch;
        return r;
    }
};

// 一字节字符特化
template <>
struct char_traits<char> {
    typedef char char_type;

    static size_t length(const char_type* s) noexcept {  // noexcept means this function will not throw any exception
        return std::strlen(s);
    }

    static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
        return memcmp(s1, s2, n);
    }

    static char_type* copy(char_type* dst, char_type* src, size_t n) noexcept {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);
        return static_cast<char_type*>(std::memcpy(dst, src, n));
    }

    static char_type* move(char_type* dst, char_type* src, size_t n) noexcept {
        return static_cast<char_type*>(std::memmove(dst, src, n));
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
        return static_cast<char_type*>(std::memset(dst, ch, count));
    }
};

// 宽字符特化 typically two or four bytes in size
template <>
struct char_traits<wchar_t> {
    typedef wchar_t char_type;

    static size_t length(const char_type* s) noexcept {
        return std::wcslen(s);
    }

    static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
        return std::wmemcmp(s1, s2, n);
    }

    static char_type* copy(char_type* dst, char_type* src, size_t n) noexcept {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);
        return static_cast<char_type*>(std::wmemcpy(dst, src, n));
    }

    static char_type* move(char_type* dst, char_type* src, size_t n) noexcept {
        return static_cast<char_type*>(std::wmemmove(dst, src, n));
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
        return static_cast<char_type*>(std::wmemset(dst, ch, count));
    }
};

// 16位字符特化
template <>
struct char_traits<char16_t> {
    typedef char16_t char_type;

    static size_t length(const char_type* src) noexcept {
        size_t len = 0;
        for (; *src != char_type(0); ++src, ++len) {
        }
        return len;
    }

    static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
        for (; n != 0; s1++, s2++, n--) {
            if (*s1 < *s2)
                return -1;
            if (*s2 < *s1)
                return 1;
        }
        return 0;
    }

    static char_type* copy(const char_type* src, char_type* dst, size_t n) noexcept {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);
        char_type* r = dst;
        for (; n != 0; --n, ++dst, ++src)
            *dst = *src;
        return r;
    }

    static char_type* move(const char_type* src, char_type* dst, size_t n) noexcept {
        char_type* r = dst;
        if (dst < src) {
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
        } else if (src < dst) {
            dst += n;
            src += n;
            for (; n != 0; --n)
                *--dst = *--src;
        }
        return r;
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
        char_type* r = dst;
        for (; count > 0; --count, --dst)
            *dst = ch;
        return r;
    }
};

// 32位字符特化
template <>
struct char_traits<char32_t> {
    typedef char32_t char_type;

    static size_t length(const char_type* src) noexcept {
        size_t len = 0;
        for (; *src != char_type(0); ++src, ++len) {
        }
        return len;
    }

    static size_t compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
        for (; n != 0; --n, s1++, s2++) {
            if (*s1 < *s2)
                return -1;
            if (*s2 < *s1)
                return 1;
        }
        return 0;
    }

    static char_type* copy(const char_type* src, char_type* dst, size_t n) noexcept {
        MYSTL_DEBUG(src + n <= dst || dst + n <= src);
        char_type* r = dst;
        for (; n != 0; --n, ++dst, ++src)
            *dst = *src;

        return r;
    }

    static char_type* move(const char_type* src, char_type* dst, size_t n) noexcept {
        char_type* r = dst;
        if (dst < src)
        {
            for (; n != 0; --n, ++dst, ++src)
                *dst = *src;
        } else if (src < dst)
        {
            dst += n;
            src += n;
            for (; n != 0; --n)
                *--dst = *--src;
        }
        return r;
    }

    static char_type* fill(char_type* dst, char_type ch, size_t count) noexcept {
        char_type* r = dst;
        for (; count > 0; --count, ++dst)
            *dst = ch;
        return r;
    }
};

template <class CharType, class CharTraits = MySTL::char_traits<CharType>>
class basic_string {
public:  // alias declarations
    typedef CharTraits                              traits_type;
    typedef CharTraits                              char_traits;

    typedef MySTL::allocator<CharType>              allocator_type;
    typedef MySTL::allocator<CharType>              data_allocator;

    typedef typename allocator_type::value_type      value_type;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef value_type*                             iterator;
    typedef const value_type*                       const_iterator;
    typedef MySTL::reverse_iterator<iterator>       reverse_iterator;
    typedef MySTL::reverse_iterator<const_iterator> const_reverse_iterator;

    allocator_type get_allocator() const noexcept { return allocator_type(); }

    static_assert(std::is_pod<CharType>::value, "CharType type of basic_string must be POD");
    static_assert(std::is_same<CharType, typename traits_type::char_type>::value, "CharType must be same as traits_type::char_type");

public:
    // 末尾位置的值
    static constexpr size_type npos = static_cast<size_type>(-1);

private:
    iterator  buffer_;  // 指向存储字符串数据的底层字符数组
    size_type size_;    // 字符串的大小
    size_type cap_;     // 字符串的容量

public:
    // 构造、复制、移动、析构函数

    basic_string() noexcept {
        try_init();
    }

    basic_string(size_type n, value_type ch) :
        buffer_(nullptr), size_(0), cap_(0) {
        fill_init(n, ch);
    }

    // 拷贝构造函数(复制), 创造新的对象
    basic_string(const basic_string& other, size_type pos) :
        buffer_(nullptr), size_(0), cap_(0) {
        init_from(other.buffer_, pos, other.size_ - pos);
    }

    basic_string(const basic_string& other, size_type pos, size_type count) :
        buffer_(nullptr), size_(0), cap_(0) {
        init_from(other.buffer_, pos, count);
    }

    basic_string(const_pointer str) :
        buffer_(nullptr), size_(0), cap_(0) {
        init_from(str, 0, char_traits::length(str));
    }

    basic_string(const_pointer str, size_type count) :
        buffer_(nullptr), size_(0), cap_(0) {
        init_from(str, 0, count);
    }

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    basic_string(Iter first, Iter last) {
        copy_init(first, last, iterator_category(first));
    }

    basic_string(const basic_string& rhs) :
        buffer_(nullptr), size_(0), cap_(0) {
        init_from(rhs.buffer_, 0, rhs.size_);
    }

    // 移动构造函数, 转移所有权
    basic_string(const basic_string&& rhs) noexcept :
        buffer_(rhs.buffer_), size_(rhs.size_), cap_(rhs.cap_) {
        rhs.buffer_ = nullptr;
        rhs.size_ = 0;
        rhs.cap_ = 0;
    }

    basic_string& operator=(const basic_string& rhs);
    basic_string& operator=(basic_string&& rhs) noexcept;
    basic_string& operator=(const_pointer str);
    basic_string& operator=(value_type ch);

    ~basic_string() noexcept {
        destroy_buffer();
    }

public:
    /*********************************** 迭代器相关操作 ***********************************/

    iterator               begin()      noexcept       { return buffer_; }
    const_iterator         begin()      const noexcept { return buffer_; }
    iterator               end()        noexcept       { return buffer_ + size_; }
    const_iterator         end()        const noexcept { return buffer_ + size_; }

    reverse_iterator       rbegin()     noexcept       { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()     const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator       rend()       noexcept       { return reverse_iterator(begin()); }
    const_reverse_iterator rend()       const noexcept { return const_reverse_iterator(end()); };

    const_iterator         cbegin()     const noexcept { return begin(); }
    const_iterator         cend()       const noexcept { return end(); }
    const_reverse_iterator crbegin()    const noexcept { return rbegin(); }
    const_reverse_iterator crend()      const noexcept { return rend(); }

    /*********************************** 容量相关操作 ***********************************/

    bool empty()            const noexcept { return size_ == 0; }

    size_type size()        const noexcept { return size_; }
    size_type length()      const noexcept { return size_; }
    size_type capacity()    const noexcept { return cap_; }
    size_type max_size()    const noexcept {
        // -1 means max of the unsigned type
        return static_cast<size_type>(-1);
    }

    void reserve(size_type n);
    void shink_to_fit();

    /*********************************** 元素访问相关操作 ***********************************/

    reference operator[](size_type n) {
        MYSTL_DEBUG(n <= size_);
        if (n == size_)
            *(buffer_ + n) = value_type();
        return *(buffer_ + n);
    }
    const_reference operator[](size_type n) const {
        MYSTL_DEBUG(n <= size_);
        if (n == size_)
            *(buffer_ + n) = value_type();
        return *(buffer_ + n);
    }

    reference at(size_type n) {
        THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                          " subscript out of range");
        return (*this)[n];
    }
    const_reference at(size_type n) const {
        THROW_OUT_OF_RANGE_IF(n >= size_, "basic_string<Char, Traits>::at()"
                                          " subscript out of range");
        return (*this)[n];
    }

    reference front() {
        MYSTL_DEBUG(!empty());
        return *begin();
    }
    const_reference front() const {
        MYSTL_DEBUG(!empty());
        return *begin();
    }

    reference back() {
        MYSTL_DEBUG(!empty());
        return *(end() - 1);
    }
    const_reference back() const {
        MYSTL_DEBUG(!empty());
        return *(end() - 1);
    }

    const_pointer data()    const noexcept { return to_raw_pointer(); }
    const_pointer c_str()   const noexcept { return to_raw_pointer(); }

    /*********************************** 添加删除相关操作 ***********************************/

    // insert
    iterator insert(const_iterator pos, value_type ch);
    iterator insert(const_iterator pos, size_type count, value_type ch);

    template <class Iter>
    iterator insert(const_iterator pos, Iter first, Iter last);

    // push_back & pop_back
    void push_back(value_type ch) {
        append(1, ch);
    }
    void pop_back() {
        MYSTL_DEBUG(!empty());
        --size_;
    }

    // append ,
    basic_string& append(size_type count, value_type ch);

    basic_string& append(const basic_string& rhs) { return append(rhs, 0, rhs.size_); }
    basic_string& append(const basic_string& rhs, size_type pos) { return append(rhs, pos, rhs.size_ - pos); }
    basic_string& append(const basic_string& rhs, size_type pos, size_type count);

    basic_string& append(const_pointer s) { return append(s, char_traits::length(s)); }
    basic_string& append(const_pointer s, size_type count);

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    basic_string& append(Iter first, Iter last) {
        return append_range(first, last);
    }

    // erase & clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // resize
    void resize(size_type count) {
        resize(count, value_type());
    }
    void resize(size_type count, value_type ch);

    void clear() noexcept { size_ = 0; }


    // basic_stirng相关操作
    /*********************************** compare ***********************************/

    // compare
    int compare(const basic_string& other) const;
    int compare(size_type pos1, size_type count1, const basic_string& other) const;
    int compare(size_type pos1, size_type count1, const basic_string& other,
                size_type pos2, size_type count2 = npos) const;
    int compare(const_pointer s) const;
    int compare(size_type pos1, size_type count1, const_pointer s) const;
    int compare(size_type pos1, size_type count1,
                const_pointer s, size_type count2) const;

    // substr
    basic_string substr(size_type index, size_type count = npos) {
        count = MySTL::min(count, size_ - index);
        return basic_string(buffer_ + index, buffer_ + index + count);
    }

    /*********************************** replace ***********************************/

    // replace for basic_string
    basic_string& replace(size_type pos, size_type count, const basic_string& other) {
        THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace() pos out of range");
        return replace_cstr(buffer_ + pos, count, other.buffer_, other.size_);
    }
    basic_string& replace(const_iterator first, const_iterator last, const basic_string& other) {
        MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
        return replace_cstr(first, static_cast<size_type>(last - first), other.buffer_, other.size_);
    }

    // replace for c-string
    basic_string& replace(size_type pos, size_type count, const_pointer str) {
        THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace() pos out of range");
        return replace_cstr(buffer_ + pos, count, str, char_traits::length(str));
    }
    basic_string& replace(const_iterator first, const_iterator last, const_pointer str) {
        MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
        return replace_cstr(first, static_cast<size_type>(last - first), str, char_traits::length(str));
    }

    // replace for range
    basic_string& replace(size_type pos, size_type count, const_pointer str, size_type count2) {
        THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace() pos out of range");
        return replace_cstr(buffer_ + pos, count, str, count2);
    }
    basic_string& replace(const_iterator first, const_iterator last, const_pointer str, size_type count2) {
        MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
        return replace_cstr(first, static_cast<size_type>(last - first), str, count2);
    }

    // replace fill by ch
    basic_string& replace(size_type pos, size_type count, size_type count2, value_type ch) {
        THROW_OUT_OF_RANGE_IF(pos > size_, "basic_string<Char, Traits>::replace() pos out of range");
        return replace_fill(buffer_ + pos, count, count2, ch);
    }
    basic_string& replace(const_iterator first, const_iterator last, size_type count, value_type ch) {
        MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
        return replace_fill(first, static_cast<size_type>(last - first), count, ch);
    }

    basic_string& replace(size_type pos1, size_type count1, const basic_string& other,
                          size_type pos2, size_type count2 = npos) {
        THROW_OUT_OF_RANGE_IF(pos1 > size_ || pos2 > other.size_, "basic_string<Char, Traits>::replace() pos out of range");
        return replace(buffer_ + pos1, count1, other.buffer_ + pos2, count2);
    }

    // replace for range
    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    basic_string& replace(const_iterator first, const_iterator last, Iter first2, Iter last2) {
        MYSTL_DEBUG(begin() <= first && last <= end() && first <= last);
        return replace_range(first, static_cast<size_type>(last - first), first2, last2);
    }

    void reverse() noexcept;

    void swap(basic_string& rhs) noexcept;

    /*********************************** find ***********************************/

    size_type find(value_type ch, size_type pos = 0) const noexcept;
    size_type find(const_pointer str, size_type pos = 0) const noexcept;
    size_type find(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type find(const basic_string& str, size_type pos) const noexcept;

    size_type rfind(value_type ch, size_type pos = npos) const noexcept;
    size_type rfind(const_pointer str, size_type pos = npos) const noexcept;
    size_type rfind(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type rfind(const basic_string& str, size_type pos = npos) const noexcept;

    size_type find_first_of(value_type ch, size_type pos = 0) const noexcept;
    size_type find_first_of(const_pointer str, size_type pos = 0) const noexcept;
    size_type find_first_of(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type find_first_of(const basic_string& str, size_type pos) const noexcept;

    size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const_pointer str, size_type pos = 0) const noexcept;
    size_type find_first_not_of(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type find_first_not_of(const basic_string& str, size_type pos) const noexcept;

    size_type find_last_of(value_type ch, size_type pos = 0) const noexcept;
    size_type find_last_of(const_pointer str, size_type pos = 0) const noexcept;
    size_type find_last_of(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type find_last_of(const basic_string& str, size_type pos) const noexcept;

    size_type find_last_not_of(value_type ch, size_type pos = 0) const noexcept;
    size_type find_last_not_of(const_pointer str, size_type pos = 0) const noexcept;
    size_type find_last_not_of(const_pointer str, size_type pos, size_type count) const noexcept;
    size_type find_last_not_of(const basic_string& str, size_type pos) const noexcept;

    size_type count(value_type ch, size_type pos = 0) const noexcept;

public:

    // 重载operator+=
    basic_string& operator+=(const basic_string& rhs) {
        return append(rhs);
    }

    basic_string& operator+=(value_type ch) {
        return append(1, ch);
    }

    basic_string& operator+=(const_pointer str) {
        return append(str);
    }

    // 重载operator>> & operator<< , friend to access its private, stream is the left operand
    friend std::istream& operator>>(std::istream& is, basic_string& str) {
        // 重载了输入流运算符 >>，用于将输入流中的数据读入到 basic_string 对象中。
        // 该函数的实现中，首先创建了一个大小为 4096 的字符数组 buf，然后使用输入流对象 is 读取数据到 buf 中。接着，创建了一个临时的 basic_string 对象 tmp，并将 buf 中的数据移动到 tmp 中。
        // 最后，将 tmp 对象移动到函数参数 str 中，并释放 buf 数组的内存。
        value_type* buf = new value_type[4096];
        is >> buf;
        basic_string tmp(buf);
        str = std::move(tmp);
        delete[] buf;
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const basic_string& str) {
        for (size_type i = 0; i < str.size_; ++i) {
            os << *(str.buffer_ + i);
        }
        return os;
    }

private:

    /*********************************** helper function ***********************************/

    // init and destroy
    void try_init() noexcept;

    void fill_init(size_type n, value_type ch);

    template <class Iter>
    void copy_init(Iter first, Iter last, MySTL::input_iterator_tag);

    template <class Iter>
    void copy_init(Iter first, Iter last, MySTL::forward_iterator_tag);

    void init_from(const_pointer src, size_type pos, size_type n);

    void destroy_buffer();

    // 返回一个指向存储字符串数据的底层字符数组的指针
    const_pointer to_raw_pointer() const;

    // shrink_to_fit
    void reinsert(size_type size);

    template <class Iter>
    basic_string& append_range(Iter first, Iter last);

    int compare_cstr(const_pointer s1, size_type n1, const_pointer s2, size_type n2) const;

    // replace
    basic_string& replace_cstr(const_pointer first, size_type count1, const_pointer str, size_type count2);
    basic_string& replace_fill(const_pointer first, size_type count1, value_type ch, size_type count2);

    template <class Iter>
    basic_string& replace_copy(const_pointer first1, const_iterator last1, Iter first2, Iter last2);

    // reallocate
    void     reallocate(size_type need);
    iterator reallocate_and_fill(iterator pos, size_type n, value_type ch);
    iterator reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);
};

/*********************************** 赋值运算符重载 ***********************************/

// 复制赋值运算符
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
operator=(const basic_string& rhs) {
    if (this != &rhs) {  // effictive c++ item 11
        basic_string tmp(rhs);
        swap(tmp);
    }
    return *this;
}

// 移动赋值运算符
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
operator=(basic_string&& rhs) noexcept {
    destroy_buffer();
    buffer_ = rhs.buffer_;
    size_ = rhs.size_;
    cap_ = rhs.cap_;
    rhs.buffer_ = nullptr;
    rhs.size_ = 0;
    rhs.cap_ = 0;
    return *this;
}

// 使用字符串进行赋值
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
operator=(const_pointer str) {
    const size_type len = char_traits::length(str);
    if (cap_ < len) {
        auto new_buffer = data_allocator::allocate(len + 1);
        buffer_ = new_buffer;
        cap_ = len + 1;
    }
    char_traits::copy(buffer_, str, len);
    size_ = len;
    return *this;
}

// 使用字符进行赋值
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
operator=(value_type ch) {
    if (cap_ < 1) {
        auto new_buffer = data_allocator::allocate(2);
        data_allocator::deallocate(buffer_);
        buffer_ = new_buffer;
        cap_ = 2;
    }
    *buffer_ = ch;
    size_ = 1;
    return *this;
}

/*********************************** 添加 & 删除 & 容量相关操作 ***********************************/

// 预留储存空间
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
    reserve(size_type n) {
    if (cap_ < n) {
        THROW_LENGTH_ERROR_IF(n > max_size(), "n can not lagger than max_size()"
                                              "int basic_string<CharType, CharTraits>::reserve(n)");
        auto new_buffer = data_allocator::allocate(n);
        char_traits::move(new_buffer, buffer_, size_);
    }
}

// 减少不用空间
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
    shink_to_fit() {
    if (size_ != cap_) {
        reinsert(size_);
    }
}

// 在pos处插入一个元素
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator
basic_string<CharType, CharTraits>::
insert(const_iterator pos, value_type ch) {
    iterator r = const_cast<iterator>(pos);
    if (size_ == cap_) {
        return reallocate_and_fill(r, 1, ch);
    }
    char_traits::move(r + 1, r, end() - r);
    ++size_;
    *r = ch;
    return r;
}

// 在pos处插入n个元素
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator
basic_string<CharType, CharTraits>::
insert(const_iterator pos, size_type count, value_type ch) {
    iterator r = const_cast<iterator>(pos);
    if (count == 0) return r;
    if (cap_ - size_ < count) {
        return reallocate_and_fill(r, count, ch);
    }
    if (pos == end()) {
        char_traits::fill(end(), ch, count);
        size_ += count;
        return r;
    }
    char_traits::move(r + count, r, count);
    char_traits::fill(r, ch, count);
    size_ += count;
    return r;
}

// 在pos处插入[first, last)内的元素
template <class CharType, class CharTraits>
template <class Iter>
typename basic_string<CharType, CharTraits>::iterator
basic_string<CharType, CharTraits>::
insert(const_iterator pos, Iter first, Iter last) {
    size_type len = MySTL::distance(first, last);
    iterator  r = const_cast<iterator>(pos);
    if (len == 0) return r;
    if (cap_ - size_ < len) {
        return reallocate_and_copy(r, first, last);
    }
    if (pos == end()) {
        MySTL::uninitialized_copy(first, last, end());
        size_ += len;
        return r;
    }
    char_traits::move(r + len, r, len);
    MySTL::uninitialized_copy(first, last, r);
    size_ += len;
    return r;
}

// 在末尾添加 count 个 ch
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
append(size_type count, value_type ch) {
    THROW_LENGTH_ERROR_IF(size_ + count > max_size(), "basic_string<CharType, CharTraits>::append() size too big");
    if (cap_ - size_ < count) {
        reallocate(count);
    }
    char_traits::fill(buffer_ + size_, ch, count);
    size_ += count;
    return *this;
}

// 在末尾添加 [str[pos], str[pos + count]) 内的元素
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
append(const basic_string& rhs, size_type pos, size_type count) {
    THROW_LENGTH_ERROR_IF(
        size_ + count > max_size(), "basic_string<CharType, CharTraits>::append() size too big");
    if (count == 0)
        return *this;
    if (cap_ < count + size_) {
        reallocate(count);
    }
    char_traits::copy(buffer_ + size_, rhs.buffer_ + pos, count);
    size_ += count;
    return *this;
}

// 在末尾添加 [s, s + count)
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
append(const_pointer s, size_type count) {
    THROW_LENGTH_ERROR_IF(
        size_ + count > max_size(), "basic_string<CharType, CharTraits>::append() size too big");
    if (cap_ < count + size_) {
        reallocate(count);
    }
    char_traits::copy(buffer_ + size_, s, count);
    size_ += count;
    return *this;
}

// 删除pos处元素
template <class CharType, class CharTratis>
typename basic_string<CharType, CharTratis>::iterator
basic_string<CharType, CharTratis>::
erase(const_iterator pos) {
    MYSTL_DEBUG(pos != end());
    iterator r = const_cast<iterator>(pos);
    char_traits::move(r, pos + 1, end() - pos - 1);
    --size_;
    return r;
}

// 删除[first, last)的元素, 可以看到元素并没有真正的删除, 只是将后面的元素向前移动
template <class CharType, class CharTratis>
typename basic_string<CharType, CharTratis>::iterator
basic_string<CharType, CharTratis>::
erase(const_iterator first, const_iterator last) {
    if (first == begin() && last == end()) {
        clear();
        return end();
    }
    const size_type n = end() - last;
    iterator        r = const_cast<iterator>(first);
    char_traits::move(r, last, n);
    size_ -= (last - first);
    return r;
}

// 重置容器大小
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
resize(size_type count, value_type ch) {
    if (count < size_) {
        erase(buffer_ + count, buffer_ + size_);
    } else {
        append(count - size_, ch);
    }
}

/*********************************** 比较 ***********************************/

// 比较两个basic_string, 小于返回-1，大于返回1
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(const basic_string& other) const {
    return compare_cstr(buffer_, size_, other.buffer_, other.size_);
}

// 比较从pos1开始的count1个字符和另一个basci_string
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(size_type pos1, size_type count1, const basic_string& other) const {
    auto n1 = MySTL::min(count1, size_ - pos1);
    return compare_cstr(buffer_ + pos1, n1, other.buffer_, other.size_);
}

// 从 pos1 下标开始的 count1 个字符跟另一个 basic_string 下标 pos2 开始的 count2 个字符比较
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(size_type pos1, size_type count1, const basic_string& other,
        size_type pos2, size_type count2) const {
    auto n1 = MySTL::min(count1, size_ - pos1);
    auto n2 = MySTL::min(count2, other.size_ - pos2);
    return compare_cstr(buffer_ + pos1, n1, other.buffer_, n2);
}

// compare with a string pointer
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(const_pointer s) const {
    auto n2 = char_traits::length(s);
    return compare_cstr(buffer_, size_, s, n2);
}

// 从 pos1 下标开始的 count1 个字符跟另一个 字面量字符串比较
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(size_type pos1, size_type count1, const_pointer s) const {
    auto n1 = MySTL::min(count1, size_ - pos1);
    auto n2 = char_traits::length(s);
    return compare_cstr(buffer_ + pos1, n1, s, n2);
}

// 从 pos1 下标开始的 count1 个字符跟另一个 字面量字符串的前count2比较
template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare(size_type pos1, size_type count1, const_pointer s, size_type count2) const {
    auto n1 = MySTL::min(count1, size_ - pos1);
    auto n2 = MySTL::min(count2, char_traits::length(s));
    return compare_cstr(buffer_ + pos1, n1, s, n2);
}

// 反转basic_string
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
reverse() noexcept {
    for (auto i = begin(), j = end(); i < j;) {
        MySTL::iter_swap(i++, --j);
    }
}

// 交换两个basic_string
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
swap(basic_string& rhs) noexcept {
    if (this != &rhs) {
        MySTL::swap(buffer_, rhs.buffer_);
        MySTL::swap(size_, rhs.size_);
        MySTL::swap(cap_, rhs.cap_);
    }
}

/*********************************** 查找方法 ***********************************/

//
// find
//

// 从下标pos开始查找ch
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find(value_type ch, size_type pos) const noexcept {
    for (auto i = pos; i < size_; i++) {
        if (*(buffer_ + i) == ch)
            return i;
    }
    return npos;
}

// 从下标pos开始查找字符串str
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find(const_pointer str, size_type pos) const noexcept {
    const auto len = char_traits::length(str);
    if (len == 0)
        return pos;
    if (size_ < len + pos)
        return npos;
    const auto left = size_ - len;
    for (auto i = pos; i <= left; i++) {
        if (*(buffer_ + i) == *str) {
            size_type j = 1;
            for (; j < len; j++) {  // 匹配字串
                if (*(buffer_ + i + j) != *(str + j))
                    break;
            }
            if (j == len)
                return i;
        }
    }
    return npos;
}

// 从下标pos开始查找str的前count个字符
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find(const_pointer str, size_type pos, size_type count) const noexcept {
    if (count == 0)
        return pos;
    if (size_ < count + pos)
        return npos;
    const auto left = size_ - count;
    for (auto i = pos; i <= left; i++) {
        if (*(buffer_ + i) == *str) {
            size_type j = 1;
            for (; j < count; j++) {  // 匹配字串
                if (*(buffer_ + i + j) != *(str + j))
                    break;
            }
            if (j == count)
                return i;
        }
    }
    return npos;
}

// 从下标pos开始查找str, basic_string
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find(const basic_string& str, size_type pos) const noexcept {
    const size_type count = str.size_;
    if (count == 0)
        return pos;
    if (size_ < count + pos)
        return npos;
    const auto left = size_ - count;
    for (auto i = pos; i <= left; i++) {
        if (*(buffer_ + i) == str.front()) {
            size_type j = 1;
            for (; j < count; j++) {  // 匹配字串
                if (*(buffer_ + i + j) != str[j])
                    break;
            }
            if (j == count)
                return i;
        }
    }
    return npos;
}

//
// rfind
//

// 从pos反向查找ch
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
rfind(value_type ch, size_type pos) const noexcept {
    if (pos >= size_)
        pos = size_ - 1;
    for (auto i = pos; i != 0; i--) {
        if (*(buffer_ + i) == ch)
            return i;
    }
    return front() == ch ? 0 : npos;
}

// 从pos开始反向查找str, 匹配方式是倒叙匹配字符串
// "string" 从'g'开始反向查找
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
rfind(const_pointer str, size_type pos) const noexcept {
    if (pos >= size_)
        pos = size_ - 1;
    const size_type len = char_traits::length(str);
    switch (len) {
    case 0:
        return pos;
    case 1: {
        for (auto i = pos; i != 0; i--) {
            if (*(buffer_ + i) == *str)
                return i;
        }
        return front() == *str ? 0 : npos;
    }
    default: {
        for (auto i = pos; i >= len - 1; i--) {
            if (*(buffer_ + i) == *(str + len - 1)) {
                size_type j = 0;
                for (; j < len - 1; j++) {
                    if (*(buffer_ + i - j) != *(str + len - j - 1))
                        break;
                }
                if (j == len)
                    return i - len + 1;
            }
        }
        break;
    }
    }
    return npos;
}

// 从下标pos反向查找str的前count
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
rfind(const_pointer str, size_type pos, size_type count) const noexcept {
    if (count == 0)
        return pos;
    if (pos >= size_)
        pos = size_ - 1;
    if (pos < count - 1)
        return npos;
    for (auto i = pos; i >= count - 1; i--) {
        if (*(buffer_ + i) == *(str + count - 1)) {
            size_type j = 1;
            for (; j < count; j++) {
                if (*(buffer_ + i - j) != *(str + count - j - 1))
                    break;
            }
            if (j == count)
                return i - count + 1;
        }
    }
    return npos;
}

// 从下标pos反向查找str
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
rfind(const basic_string& str, size_type pos) const noexcept {
    const size_type count = str.size_;
    if (pos >= size_)
        pos = size_ - 1;
    if (count == 0)
        return pos;
    for(auto i = pos; i >= count - 1; i--) {
        if (*(buffer_ + i) == str[count - 1]) {
            size_type j = 1;
            for(;j < count; j++) {
                if(*(buffer_ + i - j) != str[count - j - 1])
                    break;
            }
            if (j == count)
                return i - count + 1;
        }
    }
    return npos;
}

//
// find_first_of
//

// 从下标pos查找ch出现的第一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_of(value_type ch, size_type pos) const noexcept {
    for (auto i = pos; i < size_; i++)
    {
        if (*(buffer_ + i) == ch)
            return i;
    }
    return npos;
}

// 从下标pos查找str其中的一个字符第一次出现的位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_of(const_pointer str, size_type pos) const noexcept {
    const size_type len = char_traits::length(str);
    if (len == 0)
        return npos;
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; j++) {
            if (ch == *(str + j))  // 找到其中一个相同即可
                return i;
        }
    }
    return npos;
}

// 从下标pos查找字符串str中的某一个字符
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_of(const_pointer str, size_type pos, size_type count) const noexcept {
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; j++) {
            if (ch == *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标pos查找字符串str中的某一个字符
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_of(const basic_string& str, size_type pos) const noexcept {
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; j++) {
            if (ch == str[j])
                return i;
        }
    }
    return npos;
}

//
// find_first_not_of
//

// 从下标pos找与ch不同的第一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_not_of(value_type ch, size_type pos) const noexcept {
    for (auto i = pos; i < size_; i++) {
        if (*(buffer_ + i) != ch)
            return i;
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 s 其中一个字符不相等的第一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_not_of(const_pointer str, size_type pos) const noexcept {
    const size_type len = char_traits::length(str);
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; j++) {
            if (ch != *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 s 前 count 个字符中不相等的第一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_not_of(const_pointer str, size_type pos, size_type count) const noexcept {
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; j++) {
            if (ch != *(str + j))
                return i;
        }
    }
    return npos;
}

// 下标pos开始查找与str中不相等的第一个字符位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_first_not_of(const basic_string& str, size_type pos) const noexcept {
    for (auto i = pos; i < size_; i++) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; j++) {
            if (ch != str[j])
                return i;
        }
    }
    return npos;
}

//
// find_last_of
//

// 下标pos开始与ch相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_of(value_type ch, size_type pos) const noexcept {
    for (auto i = size_ - 1; i >= pos; i--) { // 从后面查找
        if (*(buffer_ + i) == ch)
            return i;
    }
}

// 从下标 pos 开始查找与字符串 str 的字符中不相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_of(const_pointer str, size_type pos) const noexcept {
    size_type len = char_traits::length(str);
    for (auto i = size_ - 1; i >= pos; i--) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; j++) {
            if (ch == *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 s 前 count 个字符中相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_of(const_pointer str, size_type pos, size_type count) const noexcept {
    for (auto i = size_ - 1; i >= pos; i--) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; j++) {
            if (ch == *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 str 字符中相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_of(const basic_string& str, size_type pos) const noexcept {
    for (auto i = size_ - 1; i >= pos; i--) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; j++) {
            if (ch == str[j])
                return i;
        }
    }
    return npos;
}

//
// find_last_not_of
//

// 从下标 pos 开始查找与 ch 字符不相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_not_of(value_type ch, size_type pos) const noexcept {
    for (auto i = size_ - 1; i >= pos; --i) {
        if (*(buffer_ + i) != ch)
            return i;
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 s 的字符中不相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_not_of(const_pointer str, size_type pos) const noexcept {
    const size_type len = char_traits::length(str);
    for (auto i = size_ - 1; i >= pos; --i) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < len; ++j) {
            if (ch != *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 s 前 count 个字符中不相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_not_of(const_pointer str, size_type pos, size_type count) const noexcept {
    for (auto i = size_ - 1; i >= pos; --i) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < count; ++j) {
            if (ch != *(str + j))
                return i;
        }
    }
    return npos;
}

// 从下标 pos 开始查找与字符串 str 字符中不相等的最后一个位置
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
find_last_not_of(const basic_string& str, size_type pos) const noexcept {
    for (auto i = size_ - 1; i >= pos; --i) {
        value_type ch = *(buffer_ + i);
        for (size_type j = 0; j < str.size_; ++j) {
            if (ch != str[j])
                return i;
        }
    }
    return npos;
}

// 返回从下标 pos 开始字符为 ch 的元素出现的次数
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::size_type
basic_string<CharType, CharTraits>::
count(value_type ch, size_type pos) const noexcept {
    size_type n = 0;
    for (auto i = pos; i < size_; i++) {
        if (*(buffer_ + i) == ch)
            ++n;
    }
    return n;
}

/*********************************** helper function ***********************************/

template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
try_init() noexcept {
    try {
        buffer_ = data_allocator::allocate(static_cast<size_type>(STRING_INIT_SIZE));
        size_ = 0;
        cap_ = 0;
    } catch (...) {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
    }
}

template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
fill_init(size_type n, value_type ch) {
    const auto init_size = MySTL::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
    buffer_ = data_allocator::allocate(init_size);
    char_traits::fill(buffer_, ch, n);
    size_ = n;
    cap_ = init_size;
}

template <class CharType, class CharTraits>
template <class Iter>
void basic_string<CharType, CharTraits>::
copy_init(Iter first, Iter last, MySTL::input_iterator_tag) {
    size_type  n = MySTL::distance(first, last);
    const auto init_size = MySTL::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
    try {
        buffer_ = data_allocator::allocate(init_size);
        size_ = n;
        cap_ = init_size;
    } catch (...) {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
        throw;
    }
    for (; n > 0; --n, ++first) {
        append(*first);
    }
}

template <class CharType, class CharTraits>
template <class Iter>
void basic_string<CharType, CharTraits>::
copy_init(Iter first, Iter last, MySTL::forward_iterator_tag) {
    const size_type n = MySTL::distance(first, last);
    const auto      init_size = MySTL::max(static_cast<size_type>(STRING_INIT_SIZE), n + 1);
    try {
        buffer_ = data_allocator::allocate(init_size);
        size_ = n;
        cap_ = init_size;
        MySTL::uninitialized_copy(first, last, buffer_);
    } catch (...) {
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
        throw;
    }
}

template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
init_from(const_pointer src, size_type pos, size_type count) {
    const auto init_size = MySTL::max(static_cast<size_type>(STRING_INIT_SIZE), count + 1);
    buffer_ = data_allocator::allocate(init_size);
    char_traits::copy(buffer_, src + pos, count);
    size_ = count;
    cap_ = init_size;
}

template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
destroy_buffer() {
    if (buffer_ != nullptr){
        data_allocator::deallocate(buffer_, cap_);
        buffer_ = nullptr;
        size_ = 0;
        cap_ = 0;
    }
}

template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::const_pointer
basic_string<CharType, CharTraits>::
to_raw_pointer() const {
    *(buffer_ + size_) = value_type();
    return buffer_;
}

// reinsert, 重新分配内存 
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
reinsert(size_type size) {
    auto new_buffer = data_allocator::allocate(size);
    try {
        char_traits::move(new_buffer, buffer_, size);
    } catch (...) {
        data_allocator::deallocate(new_buffer);
    }
    buffer_ = new_buffer;
    size_ = size;
    cap_ = size;
}

// append_range, 末尾追加[first, last), 内的字符
template <class CharType, class CharTraits>
template <class Iter>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
append_range(Iter first, Iter last) {
    const size_type n = MySTL::distance(first, last);
    THROW_LENGTH_ERROR_IF(size_ > max_size() - n,
                          "basic_string<Char, Traits>'s size too big.");
    if (cap_ - size_ < n) { // 如果空间不够，多分配一些
        reallocate(n);
    }
    MySTL::uninitialized_copy_n(first, n, buffer_ + size_);
    size_ += n;
    return *this;
}

template <class CharType, class CharTraits>
int basic_string<CharType, CharTraits>::
compare_cstr(const_pointer s1, size_type n1,
             const_pointer s2, size_type n2) const {
    auto rlen = MySTL::min(n1, n2);
    auto res = char_traits::compare(s1, s2, rlen);
    if (res != 0) return res;
    if (n1 < n2) return -1;
    if (n1 > n2) return 1;
}

// relplace_cstr， 用str替换[first, first + count1)的字符
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
replace_cstr(const_pointer first, size_type count1,
             const_pointer str, size_type count2) {
    if (static_cast<size_type>(cend() - first) < count1) {
        count1 = cend() - first;
    }
    if (count1 < count2) {
        const size_type add = count2 - count1;
        THROW_LENGTH_ERROR_IF(size_ > max_size() - add,
                              "basic_string<Char, Traits>'s size too big.");
        if (size_ > cap_ - add) {
            reallocate(add);
        }
        pointer r = const_cast<pointer>(first);
        char_traits::move(r + count2, first + count1, end() - (first + count1));
        char_traits::copy(r, str, count2);
        size_ += add;
    } else {
        pointer r = const_cast<pointer>(first);
        char_traits::move(r + count2, first + count1, end() - (first + count1));
        char_traits::copy(r, str, count2);
        size_ -= (count1 - count2);
    }
    return *this;
}

// replace_fill, 用count2个ch替换[first, first + count1)的字符
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
replace_fill(const_pointer first, size_type count1,
             value_type ch, size_type count2) {
    if (static_cast<size_type>(cend() - first) < count1) {
        count1 = cend() - first;
    }
    if (count1 < count2) {
        const size_type add = count2 - count1;
        THROW_LENGTH_ERROR_IF(size_ > max_size() - add,
                              "basic_string<Char, Traits>'s size too big.");
        if (size_ > cap_ - add) {
            reallocate(add);
        }
        pointer r = const_cast<pointer>(first);
        char_traits::move(r + count2, first + count1, end() - (first + count1));
        char_traits::fill(r, ch, count2);
        size_ += add;
    } else {
        pointer r = const_cast<pointer>(first);
        char_traits::move(r + count2, first + count1, end() - (first + count1));
        char_traits::fill(r, ch, count2);
        size_ -= (count1 - count2);
    }
    return *this;
}

// replace_copy, 把[first1, last1)替换为[first2, last2)的字符
template <class CharType, class CharTraits>
template <class Iter>
basic_string<CharType, CharTraits>&
basic_string<CharType, CharTraits>::
replace_copy(const_pointer first1, const_iterator last1,
             Iter first2, Iter last2) {
    size_type len1 = last1 - first1;
    size_type len2 = last2 - first2;
    if (len1 < len2) {
        const size_type add = len2 - len1;
        THROW_LENGTH_ERROR_IF(size_ > max_size() - add,
                              "basic_string<Char, Traits>'s size too big.");
        if (size_ > cap_ - add) {
            reallocate(add);
        }
        pointer r = const_cast<pointer>(first1);
        char_traits::move(r + len2, first1 + len1, end() - (first1 + len1));
        char_traits::copy(r, first2, len2);
        size_ += add;
    } else {
        pointer r = const_cast<pointer>(first1);
        char_traits::move(r + len2, first1 + len1, end() - (first1 + len1));
        char_traits::copy(r, first2, len2);
        size_ -= (len1 - len2);
    }
    return *this;
}

// reallocate, 分配更多的内存
template <class CharType, class CharTraits>
void basic_string<CharType, CharTraits>::
reallocate(size_type need) {
    const auto new_cap = MySTL::max(cap_ + need, cap_ + (cap_ >> 1));
    auto       new_buffer = data_allocator::allocate(new_cap);
    char_traits::move(new_buffer, buffer_, size_);
    data_allocator::deallocate(buffer_);
    buffer_ = new_buffer;
    cap_ = new_cap;
}

// reallocate_and_fill, 在pos处插入n个ch, 后续内容不变
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator
basic_string<CharType, CharTraits>::
reallocate_and_fill(iterator pos, size_type n, value_type ch) {
    const auto r = pos - buffer_;
    const auto old_cap = cap_;
    const auto new_cap = MySTL::max(old_cap, old_cap + (old_cap >> 1));
    auto       new_buffer = data_allocator::allocate(new_cap);
    auto       e1 = char_traits::move(new_buffer, buffer_, r) + r; // 转移所有权后的新的插入所在起始位置
    auto       e2 = char_traits::fill(e1, ch, n) + n;
    char_traits::move(e2, buffer_ + r, size_ - r); // 转移buffer_后面内容的所有权
    data_allocator::deallocate(buffer_, old_cap);
    buffer_ = new_buffer;
    size_ += n;
    cap_ = new_cap;
    return buffer_ + r;
}

// reallocate_and_copy, 在pos处插入[first, last)的内容, 后续内容不变
template <class CharType, class CharTraits>
typename basic_string<CharType, CharTraits>::iterator
basic_string<CharType, CharTraits>::
reallocate_and_copy(iterator pos, const_iterator first, const_iterator last) {
    const auto      r = pos - buffer_;
    const auto      old_cap = cap_;
    const size_type n = MySTL::distance(first, last);
    const auto      new_cap = MySTL::max(old_cap + n, old_cap + (old_cap >> 1));
    auto            new_buffer = data_allocator::allocate(new_cap);
    auto            e1 = char_traits::move(new_buffer, buffer_, r) + r;
    auto            e2 = uninitialized_copy_n(first, n, e1) + n;
    char_traits::move(e2, buffer_ + r, size_ - r); // 保留pos后面的内容
    data_allocator::allocate(buffer_, old_cap);
    buffer_ = new_buffer;
    size_ += n;
    cap_ = new_cap;
    return buffer_ + r;
}

/*********************************** 重载全局操作符 ***********************************/

// 重载 operator+
template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs,
          const basic_string<CharType, CharTraits>& rhs) {
    basic_string<CharType, CharTraits> tmp(lhs);
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const CharType*                           lhs,
          const basic_string<CharType, CharTraits>& rhs) {
    basic_string<CharType, CharTraits> tmp(lhs);
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(CharType                                  ch,
          const basic_string<CharType, CharTraits>& rhs) {
    basic_string<CharType, CharTraits> tmp(1, ch);
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs,
          const CharType*                           rhs) {
    basic_string<CharType, CharTraits> tmp(lhs);
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>& lhs,
          CharType*                                 ch) {
    basic_string<CharType, CharTraits> tmp(lhs);
    tmp.append(1, ch);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>&& lhs,
          const basic_string<CharType, CharTraits>&  rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(lhs));
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>&  lhs,
          const basic_string<CharType, CharTraits>&& rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(rhs));
    tmp.insert(tmp.begin(), lhs.begin(), lhs.end());
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const basic_string<CharType, CharTraits>&& lhs,
          const basic_string<CharType, CharTraits>&& rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(lhs));
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(const CharType*                      lhs,
          basic_string<CharType, CharTraits>&& rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(rhs));
    tmp.insert(tmp.begin(), lhs, lhs + char_traits<CharType>::length(lhs));
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(CharType                             ch,
          basic_string<CharType, CharTraits>&& rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(rhs));
    tmp.insert(tmp.begin(), ch);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs,
          const CharType*                      rhs) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(lhs));
    tmp.append(rhs);
    return tmp;
}

template <class CharType, class CharTraits>
basic_string<CharType, CharTraits>
operator+(basic_string<CharType, CharTraits>&& lhs,
          CharType                             ch) {
    basic_string<CharType, CharTraits> tmp(MySTL::move(lhs));
    tmp.append(1, ch);
    return tmp;
}

// 重载 比较操作符

template <class CharType, class CharTraits>
bool operator==(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs) {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

template <class CharType, class CharTraits>
bool operator!=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs) {
    return lhs.size() != rhs.size() || lhs.compare(rhs) != 0;
}

template <class CharType, class CharTraits>
bool operator<(const basic_string<CharType, CharTraits>& lhs,
               const basic_string<CharType, CharTraits>& rhs) {
    return lhs.compare(rhs) < 0;
}

template <class CharType, class CharTraits>
bool operator<=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs) {
    return lhs.compare(rhs) <= 0;
}

template <class CharType, class CharTraits>
bool operator>(const basic_string<CharType, CharTraits>& lhs,
               const basic_string<CharType, CharTraits>& rhs) {
    return lhs.compare(rhs) > 0;
}

template <class CharType, class CharTraits>
bool operator>=(const basic_string<CharType, CharTraits>& lhs,
                const basic_string<CharType, CharTraits>& rhs) {
    return lhs.compare(rhs) >= 0;
}

// 重载全局 swap
template <class CharType, class CharTraits>
void swap(basic_string<CharType, CharTraits>& lhs,
          basic_string<CharType, CharTraits>& rhs) noexcept {
    lhs.swap(rhs); // effictive c++ item 25, use member swap
}

// 特化 MySTL::hash
template <class CharType, class CharTraits>
struct hash<basic_string<CharType, CharTraits>> {
    size_t operator()(const basic_string<CharType, CharTraits>& str) const {
        return bitwise_hash((const unsigned char*)str.c_str(), str.size() * sizeof(CharType));
    }
};

}  // namespace MySTL

#endif /* MY_BASIC_STRING_H */