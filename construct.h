#ifndef MY_CONSTRUCT_H
#define MY_CONSTRUCT_H
// 内存池使用
#include <new>
#include "type_traits.h"
#include "util.h"
#include "iterator.h"
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

template <class BidrectionalIter>
void destory_cat(BidrectionalIter, BidrectionalIter, std::true_type) {}

template <class BidrectionalIter>
void destory_cat(BidrectionalIter first, BidrectionalIter last, std::false_type) {
    for (; first != last; first++)
        destory_it(&*first);
}

template <class T>
void destory(T*pointer){
	destory_it(pointer, std::is_trivially_destructible<T>{});
}

// TODO: 接受两个迭代器, 须实现iterator.h和萃取
template <class BidrectionalIter>
void destory(BidrectionalIter* first, BidrectionalIter* last) {
    destory_cat(first, last, std::is_trivially_destructible<typename iterator_traits<BidrectionalIter>::value_type>{});
}

}  // namespace MySTL

#endif /* MY_CONSTRUCT_H */
