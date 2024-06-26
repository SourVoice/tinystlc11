#ifndef MY_MEMORY_H
#define MY_MEMORY_H
// 智能指针和内存空间管理

#include <cstddef>
#include <cstdlib>
#include <climits>

#include "myallocator.h"
#include "util.h"
#include "uninitialize.h"
#include "construct.h"
#include "algobase.h"

namespace MySTL {

template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept {
    return &value;
}

// 获取临时缓冲区
template <class T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*) {
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
        len = INT_MAX / sizeof(T);
    while (len > 0) {
        T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
        if (tmp) return pair<T*, ptrdiff_t>(tmp, len);
        len /= 2;
    }
    return pair<T*, ptrdiff_t>(nullptr, 0);
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer_helper(ptrdiff_t len) {
    return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer_helper(ptrdiff_t len, T*) {
    return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
void release_temporary_buffer(T* ptr){
        free(ptr);
}

template <class ForwardIter, class T>
class temporary_buffer {
private:
    ptrdiff_t original_len;  // 缓冲区申请大小
    ptrdiff_t len;           // 缓冲区实际大小
    T*        buffer;        // 缓冲区指针

public:
    // 构造析构函数
    temporary_buffer(ForwardIter first, ForwardIter last);
    ~temporary_buffer() {
        MySTL::destroy(buffer, buffer + len);
        free(buffer);
    }

public:
    ptrdiff_t size()                const noexcept { return len; }
    ptrdiff_t requested_size()      const noexcept { return original_len; }
    T*        begin()               noexcept { return buffer; }
    T*        end()                 noexcept { return buffer + len; }

private:
    // 初始化缓冲区内容
    void allocate_buffer();
    void initialize_buffer(const T& value, std::true_type) {}
    void initialize_buffer(const T& value, std::false_type) {
        MySTL::uninitialized_fill_n(buffer, len, value);
    }

private:
    temporary_buffer(const temporary_buffer&);
    void operator=(const temporary_buffer&);
};

// temporaray_buffer类构造函数
template <class ForwardIter, class T>
temporary_buffer<ForwardIter, T>::temporary_buffer(ForwardIter first, ForwardIter last) {
    try {
        len = MySTL::distance(first, last);
        allocate_buffer();
        if (len > 0)
            initialize_buffer(*first, std::is_trivially_default_constructible<T>());
    } catch (...) {
        free(buffer);
        buffer = nullptr;
        len = 0;
    }
}

template <class ForwardIter, class T>
void temporary_buffer<ForwardIter, T>::allocate_buffer() {
    original_len = len;
    if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T))) len = INT_MAX / sizeof(T);
    while (len > 0) {
        buffer = static_cast<T*>(malloc(len * sizeof(T)));
        if (buffer) break;
        len /= 2;
    }
}

template <class T>
class auto_ptr {
public:
    typedef T elem_type;

private:
    T* m_ptr;  // 实际指针
    // 构造, 拷贝, 赋值,
public:
    explicit auto_ptr(T* p = nullptr) : m_ptr(p) {}
    auto_ptr(auto_ptr& rhs) : m_ptr(rhs.release()) {}
    template <class U>
    explicit auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

    template <class U>
    auto_ptr& operator=(auto_ptr<U>& rhs) {  
        if (this->get() != rhs.get()) {
            delete m_ptr;  // 去掉原有所有权
            m_ptr = rhs.release();
        }
        return *this;
    }
    ~auto_ptr() { delete m_ptr;}

public:
    T& operator*() const { return *m_ptr; }
    T& operator->() const { return m_ptr; }

    T* get() const {return m_ptr;};
    T* release() {
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    void reset(T* p = nullptr) {
        if (m_ptr != p) {
            delete m_ptr;
            m_ptr = p;
        }
    }
};

}  // namespace MySTL

#endif /* MY_MEMORY_H */
