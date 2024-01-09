#ifndef MY_HEAP_ALGO_H
#define MY_HEAP_ALGO_H

// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap
// 默认构造最大堆

#include "iterator.h"

namespace MySTL {

// 预先声明
template <class RandIter, class Distance, class T>
void adjust_heap(RandIter first, Distance holeIndex, Distance len, T value);
template <class RandIter, class Distance, class T, class Compare>
void adjust_heap(RandIter first, Distance holeIndex, Distance len, T value, Compare cmp);

// push_heap() 的辅助函数
template <class RandIter, class Distance, class T>
void push_heap_aux(RandIter first, Distance holeIndex, Distance topIndex, T value) {
    auto parent = (holeIndex - 1) / 2;
    // 大顶堆
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandIter, class Distance>
void push_heap_d(RandIter first, RandIter last, Distance*) {
    push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
}

/**
 * @brief push_heap() 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
 */
template <class RandIter>
void push_heap(RandIter first, RandIter last) {
    push_heap_d(first, last, MySTL::distance_type(first));
}

// push_heap_aux() 使用Compare的重载版本
template <class RandIter, class Distance, class T, class Compare>
void push_heap_aux(RandIter first, Distance holeIndex, Distance topIndex, T value, Compare cmp) {
    auto parent = (holeIndex - 1) / 2;
    // 大顶堆
    while (holeIndex > topIndex && cmp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

// push_heap_d() 使用Compare的重载版本
template <class RandIter, class Distance, class Compare>
void push_heap_d(RandIter first, RandIter last, Distance*, Compare cmp) {
    push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1), cmp);
}

/**
 * @brief push_heap() 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
 */
template <class RandIter, class Compare>
void push_heap(RandIter first, RandIter last, Compare cmp) {
    push_heap_d(first, last, MySTL::distance_type(first), cmp);
}

/*****************************************************************************************/
// pop_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
/*****************************************************************************************/
// pop_heap() 的辅助函数
template <class RandIter, class Distance, class T>
void pop_heap_aux(RandIter first, RandIter last, RandIter result, Distance*, T value) {
    *result = *first;                                                     // 取出堆顶元素
    adjust_heap(first, static_cast<Distance>(0), (last - first), value);  // 对剩余的[first, last)进行重排, 此处last已经在pop_heap()中减一
}

// pop_heap_aux() 使用Compare的重载版本
template <class RandIter, class Distance, class T, class Compare>
void pop_heap_aux(RandIter first, RandIter last, RandIter result, Distance*, T value, Compare cmp) {
    *result = *first;                                                          // 取出堆顶元素
    adjust_heap(first, static_cast<Distance>(0), last - first, value, cmp);  // 对剩余的[first, last)进行重排, 此处last已经在pop_heap()中减一
}

/**
 * @brief pop_heap(), 该函数接受两个迭代器，表示一个 heap 容器的首尾，将 heap 的根节点取出放在容器尾部，然后重排 heap
 */
template <class RandIter>
void pop_heap(RandIter first, RandIter last) {
    pop_heap_aux(first, last - 1, last - 1, MySTL::distance_type(first), *(last - 1));
}

// pop_heap() 使用Compare的重载版本
template <class RandIter, class Compare>
void pop_heap(RandIter first, RandIter last, Compare cmp) {
    pop_heap_aux(first, last - 1, last - 1, MySTL::distance_type(first), *(last - 1), cmp);
}

/*****************************************************************************************/
// adjust_heap
// 该函数接受三个迭代器，表示一个 heap 容器的首尾，以及一个新值，将新值放在容器尾部，然后重排 heap
/*****************************************************************************************/
/**
 * @brief adjust_heap(), 该函数接受三个迭代器，表示一个 heap 容器的首尾，以及一个新值，将新值放在容器尾部，然后重排 heap
 * @param first heap 容器的首迭代器
 * @param holeIndex 新值的位置
 * @param len heap 容器的大小
 * @param value 新值
 */
template <class RandIter, class Distance, class T>
void adjust_heap(RandIter first, Distance holeIndex, Distance len, T value) {
    auto topIndex = holeIndex;        // 右子节点的父节点
    auto rchild = holeIndex;
    while (rchild < (len - 1) / 2) {
        rchild = 2 * (rchild + 1);
        if (*(first + rchild) < *(first + rchild - 1))  // rchild - 1 为同级左子节点
            rchild--;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
    }
    if (rchild == (len - 2) / 2 && (len & 1) == 0) {  // 无右子节点
        rchild = 2 * (rchild + 1);
        *(first + holeIndex) = *(first + rchild - 1);
        holeIndex = rchild - 1;
    }
    // 进行上溯
    MySTL::push_heap_aux(first, holeIndex, topIndex, value);
}

// adjust_heap() 使用Compare的重载版本
template <class RandIter, class Distance, class T, class Compare>
void adjust_heap(RandIter first, Distance holeIndex, Distance len, T value, Compare cmp) {
    auto topIndex = holeIndex;
    auto rchild = holeIndex * 2 + 2;
    while (rchild < len) {
        if (cmp(*(first + rchild), *(first + rchild - 1)))
            rchild--;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1); // Error: rchild = 2 * rchild + 1; 导致rchild更新后变为左子节点
    }
    if (rchild == len) {  // 无右子节点
        *(first + holeIndex) = *(first + rchild - 1);
        holeIndex = rchild - 1;
    }
    // 进行上溯
    MySTL::push_heap_aux(first, holeIndex, topIndex, value, cmp);
}

/**
 * @brief sort_heap() 接受首尾迭代器, 将[first, last)转换为一个有序序列
 */
template <class RandIter>
void sort_heap(RandIter first, RandIter last) {
    // 不断将最大元素放到尾部, 直到堆只有一个元素
    while (last - first > 1) {
        MySTL::pop_heap(first, last);
        last--;
    }
}

// sort_heap() 使用Compare的重载版本
template <class RandIter, class Compare>
void sort_heap(RandIter first, RandIter last, Compare cmp) {
    while (last - first > 1) {
        MySTL::pop_heap(first, last, cmp);
        last--;
    }
}

// make_heap()的辅助函数，auxilliary,
template <class RandIter, class Distance>
void make_heap_aux(RandIter first, RandIter last, Distance*) {
    if (last - first < 2) return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2; // 找到最后一个节点的父节点的索引，len处为最后一个右子节点的索引
    while (1) {
        MySTL::adjust_heap(first, holeIndex, len, *(first + holeIndex));
        if (holeIndex == 0)
            return;
        holeIndex--;
    }
}

/**
 * @brief make_heap(), 接受首尾迭代器, 将[first, last)变为一个最大堆
 */
template <class RandIter>
void make_heap(RandIter first, RandIter last) {
    make_heap_aux(first, last, MySTL::distance_type(first));
}

// make_heap_aux使用Compare的重载版本
template <class RandIter, class Distance, class Compare>
void make_heap_aux(RandIter first, RandIter last, Distance*, Compare cmp) {
    if (last - first < 2) return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (1) {
        MySTL::adjust_heap(first, holeIndex, len, *(first + holeIndex), cmp);
        if (holeIndex == 0)
            return;
        holeIndex--;
    }
}

/**
 * @brief make_heap(), 使用Compare的重载版本
 */
template <class RandIter, class Compare>
void make_heap(RandIter first, RandIter last, Compare cmp) {
    make_heap_aux(first, last, MySTL::distance_type(first), cmp);
}

}  // namespace MySTL
#endif /* MY_HEAP_ALGO_H */