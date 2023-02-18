#ifndef CONSTRUCT_H
#define CONSTRUCT_H
// 内存池使用
#include "type_traits.h"
#include <new>
#include "util.h"
namespace MySTL {
// construct
template <class T>
void construct(T* ptr) {
    ::new ((void*)ptr) T();
}

template <class T1, class T2>
void construct(T1* ptr, const T2& value) {
    ::new ((void*)ptr) T1(value);
}
template <class T, class... Args>
void construct(T* ptr, Args... args) {
    ::new ((void*)ptr) T(forward<Args>(args)...);
}
// destory
template <class T>
void destory_it(T*, std::true_type) {}

template <class T>
void destory_it(T* ptr) {
    if (ptr != nullptr) ptr->~T();
}

template <class ForwardIter>
void destory_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
void destory_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for (; first != last; first++) destory_it(&*first);
}

template <class T>
void destory(T*pointer){
	destory_it(pointer, std::is_trivially_destructible<T>{});
}

// TODO: 接受两个迭代器

}  // namespace MySTL

#endif /* CONSTRUCT_H */
