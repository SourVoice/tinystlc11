#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <memory>

#include "construct.h"
#include "type_traits.h"
#include "util.h"

namespace MySTL {

template <class T>
class allocator {
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_p;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
public:
    static T*   allocate();
    static T*   allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    static void construt(T* ptr);
    static void construt(T* ptr, const T& value);
    static void construt(T* ptr, T&& value);

    template <class... Args>
    static void construt(T* ptr, Args&& ...args);

    static void destory(T* ptr);
    static void destory(T* first, T* last);
};

template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}
template <class T>
T* allocator<T>::allocate(size_type n) {
    if (n == 0) return nullptr;
    return static_cast<T*>(::operator new((sizeof(T)) * n));
}
template <class T>
void allocator<T>::deallocate(T* ptr) {
    if (ptr != nullptr) {
        ::operator delete(ptr);
    }
    return;
}
template <class T>
void allocator<T>::deallocate(T* ptr, size_type) {
    if (ptr != nullptr) {
        ::operator delete(ptr);
    }
    return;
}

template <class T>
void allocator<T>::construt(T* ptr) {
    MySTL::construct(ptr);
}
template <class T>
void allocator<T>::construt(T* ptr, const T& value) {
    MySTL::construct(ptr, value);
}
template <class T>
void allocator<T>::construt(T* ptr, T&& value) {
    MySTL::construct(ptr, MySTL::move(value));
}
template <class T>
template <class... Args>
void allocator<T>::construt(T* ptr, Args&&... args) {
    MySTL::construct(ptr, MySTL::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destory(T* ptr) {
    MySTL::destory(ptr);
}

template <class T>
void allocator<T>::destory(T* first, T* last) {
    MySTL::destory(first, last);
}

}  // namespace MySTL

#endif /* MY_ALLOCATOR_H */
