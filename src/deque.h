#ifndef MY_DEQUE_H
#define MY_DEQUE_H

// 双端队列实现
#include <initializer_list>
#include <type_traits>

#include "memory.h"
#include "iterator.h"
#include "uninitialize.h"
#include "util.h"
#include "exceptdef.h"
#include "algo.h"

namespace MySTL {

#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

template <class T>
struct deque_buf_size {
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

// deque 迭代器设计
template <class T, class Ref, class Ptr>
struct deque_iterator : public iterator<random_access_iterator_tag, T> {
    typedef deque_iterator<T, T&, T*>             iterator;
    typedef deque_iterator<T, const T&, const T&> const_iterator;
    typedef deque_iterator                        self;

    static const size_t buffer_size = deque_buf_size<T>::value;

    typedef random_access_iterator_tag iterator_catatory;
    typedef T                          value_type;
    typedef Ptr                        pointer;
    typedef Ref                        reference;
    typedef size_t                     size_type;
    typedef ptrdiff_t                  difference_type;
    typedef T*                         value_pointer;
    typedef T**                        map_pointer;

    value_pointer cur;
    value_pointer first;
    value_pointer last;
    map_pointer   node;  // 缓冲区指针

    /*********************************** 构造，复制，移动，析构 ***********************************/

    deque_iterator() noexcept : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
    deque_iterator(value_pointer v, map_pointer n) noexcept : cur(v), first(*n), last(*n + buffer_size), node(n) {}

    // 复制
    deque_iterator(const iterator& rhs) noexcept : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}
    deque_iterator(const const_iterator& rhs) noexcept : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

    // 移动
    deque_iterator(iterator&& rhs) noexcept : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {
        rhs.cur = nullptr;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.node = nullptr;
    }

    // 赋值运算符
    self& operator=(const iterator& rhs) {
        if (this != &rhs) {
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
    }

    /*********************************** auxiliary function ***********************************/

    // 缓冲区更换
    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    /*********************************** 运算符重载 ***********************************/

    reference operator*() const { return *cur; }
    pointer   operator->() const { return cur; }

    difference_type operator-(const self& x) const{
        return static_cast<difference_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
    }

    self& operator++() {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int) { // 后置++
        self temp = *this;
        ++*this;
        return temp;
    }

    self operator--() {
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self& operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    self& operator+=(difference_type n) {
        const auto offset = n + (cur - first);
        if (offset >= 0 && offset < static_cast<difference_type>(buffer_size)) {  // 仍在当前缓冲区
            cur += n;
        } else {
            const auto node_offset = offset > 0 
                ? offset / static_cast<difference_type>(buffer_size) 
                : -static_cast<difference_type>((-offset - 1) / buffer_size);
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }
        return *this;
    }

    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }

    self operator-=(difference_type n) {
        return *this += -n;
    }

    self operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const { return *(*this + n); }

    // 比较运算符
    bool operator==(const self& rhs) const { return cur == rhs.cur; }
    bool operator < (const self& rhs) const {
        return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
    }
    bool operator != (const self&rhs) const {return !(*this == rhs);}
    bool operator>(const self& rhs) const { return rhs < *this; }
    bool operator<=(const self& rhs) const { return !(rhs < *this); }
    bool operator>=(const self& rhs) const { return !(*this < rhs); }
};

// 模板类 deque
template <class T>
class deque {
public:
    typedef MySTL::allocator<T>                         allocator_type;
    typedef MySTL::allocator<T>                         data_allocator;
    typedef MySTL::allocator<T*>                        map_allocator;  // 该分配器负责分配不连续的每一段bufffer, 分段连续的线性空间` 

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;
    typedef pointer*                                    map_pointer;
    typedef const_pointer*                              const_map_pointer;

    typedef deque_iterator<T, T&, T*>                   iterator;
    typedef deque_iterator<T, const T&, const T*>       const_iterator;
    typedef MySTL::reverse_iterator<iterator>           reverse_iterator;
    typedef MySTL::reverse_iterator<const_iterator>     const_reverse_iterator;

    allocator_type get_allocator() {return data_allocator();}

    static const size_type buffer_size = deque_buf_size<T>::value;

// 数据段
private:
    iterator    begin_;
    iterator    end_;
    map_pointer map_;       // 指向map，map是一个指针数组，指向每一个缓冲区的起始位置
    size_type   map_size_;  // map内指针是的数目

public:
    /*********************************** 构造，复制，移动，析构 ***********************************/

    // 构造
    deque() { fill_init(0, value_type()); }

    explicit deque(size_type n) { fill_init(n, value_type()); }

    deque(size_type n, const value_type& value) { fill_init(n, value); }

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    deque(Iter first, Iter last) {
        MYSTL_DEBUG(!(last < first));
        copy_init(first, last, iterator_category(first));
    }

    template <class Iter, typename std::enable_if<MySTL::is_forward_iterator<Iter>::value, int>::type = 0>
    deque(Iter first, Iter last) {
        MYSTL_DEBUG(!(last < first));
        copy_init(first, last, iterator_category(first));
    }

    deque(std::initializer_list<value_type> ilist) { copy_init(ilist.begin(), ilist.end(), MySTL::forward_iterator_tag()); }

    // 复制
    deque(const deque& rhs) { copy_init(rhs.begin(), rhs.end(), MySTL::forward_iterator_tag()); }

    // 移动
    deque(deque&& rhs) noexcept :
        begin_(MySTL::move(rhs.begin_)),
        end_(MySTL::move(rhs.end_)),
        map_(rhs.map_),
        map_size_(rhs.map_size_) {
        rhs.map_ = nullptr;
        rhs.map_size_ = 0;
    }

    // 重载赋值运算符
    deque& operator=(const deque& rhs);
    deque& operator=(deque&& rhs);

    deque&operator=(std::initializer_list<value_type> ilist) {
        deque tmp(ilist);
        swap(tmp);
        return *this;
    }

    ~deque() {
        if (map_ != nullptr) {
            clear();
            data_allocator::deallocate(*begin_.node, buffer_size);
            *begin_.node = nullptr;
            map_allocator::deallocate(map_, map_size_);
            map_ = nullptr;
        }
    }

public:
    /*********************************** 迭代器相关操作 ***********************************/

    iterator       begin() noexcept { return begin_; }
    const_iterator begin() const noexcept { return begin_; }
    iterator       end() noexcept { return end_; }
    const_iterator end() const noexcept { return end_; }

    reverse_iterator       rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    reverse_iterator       rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

    const_iterator         cbegin() const noexcept { return begin(); }
    const_iterator         cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    /*********************************** 容器操作 ***********************************/

    // 容量操作

    bool      empty() const noexcept { return begin() == end(); }
    size_type size() const noexcept { return end_ - begin_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }
    void      resize(size_type new_size) { resize(new_size, value_type()); }
    void      resize(size_type new_size, const value_type& value);
    void      shrink_to_fit() noexcept;

    // 元素访问

    reference operator[](size_type n) {
        MYSTL_DEBUG(n < size());
        return begin_[n];
    }
    const_reference operator[] (size_type n) const {
        MYSTL_DEBUG(n < size());
        return begin_[n];
    }

    reference at(size_type n) {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
        return (*this)[n]; // create a object to use operator[]
    }
    const_reference at(size_type n) const {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
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

    // assign
    void assign(size_type n, const value_type& value) { fill_assign(n, value); }

    template <class Iter, typename std::enable_if<
                              MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last) { copy_assign(first, last, iterator_category(first)); }

    void assign(std::initializer_list<value_type> ilist) { copy_assign(ilist.begin(), ilist.end(), MySTL::forward_iterator_tag()); }

    // emplace
    template <class... Args>
    void emplace_front(Args&&... args);

    template <class... Args>
    void emplace_back(Args&&... args);

    template <class... Args>
    iterator emplace(iterator pos, Args&&... args);

    // push_back
    void push_front(const value_type& value);
    void push_back(const value_type& value);

    void push_front(value_type&& value) { emplace_front(MySTL::move(value)); }
    void push_back(value_type&& value) { emplace_back(MySTL::move(value)); }

    // pop_back
    void pop_front();
    void pop_back();

    // inset
    iterator insert(iterator pos, const value_type& value);
    iterator insert(iterator pos, value_type&& value);
    void     insert(iterator pos, size_type n, const value_type& value);

    template <class Iter, typename std::enable_if<
                              MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    void insert(iterator pos, Iter first, Iter last) { insert_dispatch(pos, first, last, iterator_category(first)); }

    // erase && claer, will return the new iter

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void     clear();

    // swap

    void swap(deque& rhs) noexcept;

private:
    /*********************************** 辅助函数 ***********************************/

    // 创建queue中的buffer缓冲区
    map_pointer create_map(size_type size);
    void        create_buffer(map_pointer nstart, map_pointer nfinish);
    void        destory_buffer(map_pointer nstart, map_pointer nfinish);

    // 初始化，构造函数
    void map_init(size_type nelem);
    void fill_init(size_type n, const value_type& value);
    template <class Iter>
    void copy_init(Iter first, Iter last, input_iterator_tag);
    template <class Iter>
    void copy_init(Iter first, Iter last, forward_iterator_tag);

    void fill_assign(size_type n, const value_type& value);
    template <class Iter>
    void copy_assign(Iter first, Iter last, input_iterator_tag);
    template <class Iter>
    void copy_assign(Iter first, Iter last, forward_iterator_tag);

    // insert 
    template <class... Args>
    iterator insert_aux(iterator pos, Args&&... args);
    void     fill_insert(iterator pos, size_type n, const value_type& value);
    template <class Iter>
    void copy_insert(iterator pos, Iter first, Iter last, size_type n);
    template <class Iter>
    void insert_dispatch(iterator pos, Iter first, Iter last, input_iterator_tag);
    template <class Iter>
    void insert_dispatch(iterator pos, Iter first, Iter last, forward_iterator_tag);

    // reallocate 实现内存重新组织
    void require_capacity(size_type n, bool front);
    void reallocate_map_at_front(size_type need);
    void reallocate_map_at_back(size_type need);
};

// 复制赋值运算符
template <class T>
deque<T>& deque<T>::operator=(const deque& rhs) {
    if (this != &rhs) {
        const auto len = size();
        if (len >= rhs.size()) {
            erase(MySTL::copy(rhs.begin_, rhs.end_, begin_), end_);
        } else {
            iterator mid = rhs.begin() + static_cast<difference_type>(len);
            MySTL::copy(rhs.begin_, mid, begin_);
            insert(end_, mid, rhs.end_);
        }
    }
    return *this;
}

// 移动赋值运算符
template <class T>
deque<T>& deque<T>::operator=(deque&& rhs) {
    clear();
    begin_ = MySTL::move(rhs.begin_);
    end_ = MySTL::move(rhs.end_);
    map_ = rhs.map_;
    map_size_ = rhs.map_size_;
    rhs.map_ = nullptr;
    rhs.map_size_ = 0;
    return *this;
}

template <class T>
void deque<T>::resize(size_type new_size, const value_type& value) {
    const auto len = size();
    if (new_size < len) {
        erase(begin_ + new_size, end_);
    } else {
        insert(end_, new_size - len, value);
    }
}

template <class T>
void deque<T>::shrink_to_fit() noexcept {
    for (auto cur = map_; cur < begin_.node; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
    for (auto cur = end_.node + 1; cur < map_ + map_size_; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
}

// 头部就地构造元素
template <class T>
template <class... Args>
void deque<T>::emplace_front(Args&&... args) {
    if (begin_.cur != begin_.first) {
        data_allocator::construt(begin_.cur - 1, MySTL::forward<Args>(args)...);
        --begin_.cur;
    } else {
        require_capacity(1, true);
        try {
            --begin_;
            data_allocator::construt(begin_.cur, MySTL::forward<Args>(args)...);
        } catch (...) {
            ++begin_;
            throw;
        }
    }
}

// 在尾部就地构造元素
template <class T>
template <class... Args>
void deque<T>::emplace_back(Args&&... args) {
    if (end_.cur != end_.last - 1) {
        data_allocator::construt(end_.cur, MySTL::forward<Args>(args)...);
        ++end_.cur;
    } else {
        require_capacity(1, false);
        data_allocator::construt(end_.cur, MySTL::forward<Args>(args)...);
        ++end_;
    }
}

template <class T>
template <class... Args>
typename deque<T>::iterator deque<T>::emplace(iterator pos, Args&&... args) {
    if (pos.cur == begin_.cur) {
        emplace_front(MySTL::forward<Args>(args)...);
        return begin_;
    } else if (pos.cur == end_.cur) {
        emplace_back(MySTL::forward<Args>(args)...);
        return end_ - 1;
    }
    return insert_aux(pos, MySTL::forward<Args>(args)...);
}

template <class T>
void deque<T>::push_front(const value_type& value) {
    if (begin_.cur != begin_.first) {
        data_allocator::construt(begin_.cur - 1, value);
        --begin_.cur;
    } else {
        require_capacity(1, true);
        try {
            --begin_;
            data_allocator::construt(begin_.cur, value);
        } catch (...) {
            ++begin_;
            throw;
        }
    }
}

template <class T>
void deque<T>::push_back(const value_type& value) {
    if (end_.cur != end_.last - 1) {
        data_allocator::construt(end_.cur, value);
        ++end_.cur;
    } else {
        require_capacity(1, false);
        data_allocator::construt(end_.cur, value);
        ++end_;
    }
}

template <class T>
void deque<T>::pop_front() {
    MYSTL_DEBUG(!empty());
    if (begin_.cur != begin_.last - 1) {
        data_allocator::destory(begin_.cur);
        ++begin_.cur;
    } else {
        data_allocator::destory(begin_.cur);
        ++begin_;
        destory_buffer(begin_.node - 1, begin_.node - 1);
    }
}

template <class T>
void deque<T>::pop_back() {
    MYSTL_DEBUG(!empty());
    if (end_.cur != end_.first) {
        --end_.cur;
        data_allocator::destory(end_.cur);
    } else {
        --end_;
        data_allocator::destory(end_.cur);
        destory_buffer(end_.node + 1, end_.node + 1);
    }
}

template <class T>
typename deque<T>::iterator
deque<T>::insert(iterator pos, const value_type& value) {
    if (pos.cur == begin_.cur) {
        push_front(value);
        return begin_;
    } else if (pos.cur == end_.cur) {
        push_back(value);
        auto tmp = end_;
        --tmp;
        return tmp;
    } else {
        return insert_aux(pos, value);
    }
}

template <class T>
typename deque<T>::iterator
deque<T>::insert(iterator pos, value_type&& value) {
    if (pos.cur == begin_.cur) {
        emplace_front(MySTL::move(value));
        return begin_;
    } else if (pos.cur == end_.cur) {
        emplace_back(MySTL::move(value));
        auto tmp = end_;
        --tmp;
        return tmp;
    } else {
        return insert_aux(pos, MySTL::move(value));
    }
}

template <class T>
void deque<T>::insert(iterator pos, size_type n, const value_type& value) {
    if (pos.cur == begin_.cur) {
        require_capacity(n, true);
        auto new_begin = begin_ - n;
        MySTL::uninitialized_fill_n(new_begin, n, value);
        begin_ = new_begin;
    } else if (pos.cur == end_.cur) {
        require_capacity(n, false);
        auto new_end = end_ + n;
        MySTL::uninitialized_fill_n(end_, n, value);
        end_ = new_end;
    } else {
        fill_insert(pos, n, value);
    }
}

template <class T>
typename deque<T>::iterator
deque<T>::erase(iterator pos) {
    auto next = pos;
    ++next;
    const size_type elems_before = pos - begin_;
    if (elems_before < (size() / 2)) {
        MySTL::copy_backward(begin_, pos, next);
        pop_front();
    } else {
        MySTL::copy(next, end_, pos);
        pop_back();
    }
    return begin_ + elems_before;
}

template <class T>
typename deque<T>::iterator
deque<T>::erase(iterator first, iterator last) {
    if (first == begin_ && last == end_) {
        clear();
        return end_;
    } else {
        const size_type len = last - first;
        const size_type elems_before = first - begin_;
        if (elems_before < ((size() - len) / 2)) {
            MySTL::copy_backward(begin_, first, last);
            auto new_begin = begin_ + len;
            data_allocator::destory(begin_.cur, new_begin.cur);
            begin_ = new_begin;
        } else {
            MySTL::copy(last, end_, first);
            auto new_end = end_ - len;
            data_allocator::destory(new_end.cur, end_.cur);
            end_ = new_end;
        }
        return begin_ + elems_before;
    }
}

template <class T>
void deque<T>::clear() {
    // clear 保留头部缓冲区
    for (map_pointer cur = begin_.node + 1; cur < end_.node; ++cur) {
        data_allocator::destory(*cur, *cur + buffer_size);
    }
    if (begin_.node != end_.node) {
        MySTL::destory(begin_.cur, begin_.last);
        MySTL::destory(end_.first, end_.cur);
    } else {
        MySTL::destory(begin_.cur, end_.cur);
    }
    shrink_to_fit();
    end_ = begin_;
}

// swap
template <class T>
void deque<T>::swap(deque& rhs) noexcept {
    if (this != &rhs) { // TODO: ? what is &rhs
        MySTL::swap(begin_, rhs.begin_);
        MySTL::swap(end_, rhs.end_);
        MySTL::swap(map_, rhs.map_);
        MySTL::swap(map_size_, rhs.map_size_);
    }
}

/*********************************** 辅助函数 ***********************************/

template <class T>
typename deque<T>::map_pointer
deque<T>::create_map(size_type size) {
    map_pointer mp = nullptr;
    mp = map_allocator::allocate(size);
    for (size_type i = 0; i < size; i++) {
        *(mp + i) = nullptr;
    }
    return mp;
}

// 为map所指区域分配内存
template <class T>
void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinish) {
    map_pointer cur;
    try {
        for (cur = nstart; cur <= nfinish; ++cur) {
            *cur = data_allocator::allocate(buffer_size);
        }
    } catch (...) {
        while (cur != nstart) {
            --cur;
            data_allocator::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }
        throw;
    }
}

template <class T>
void deque<T>::destory_buffer(map_pointer nstart, map_pointer nfinish) {
    for (map_pointer cur = nstart; cur <= nfinish; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
}

/// @brief 
/// @tparam T 
/// @param nelem deque预计容纳元素数量
template <class T>
void deque<T>::map_init(size_type nelem) {
    const size_type nNode = nelem / buffer_size + 1;// 分配缓冲区的大小
    map_size_ = MySTL::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nNode + 2);
    try {
        map_ = create_map(map_size_);
    } catch (...) {
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }

    map_pointer nstart = map_ + (map_size_ - nNode) / 2;
    map_pointer nfinish = nstart + nNode - 1;
    try {
        create_buffer(nstart, nfinish);
    } catch (...) {
        map_allocator::deallocte(map_, map_size_);
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }
    begin_.set_node(nstart);
    end_.set_node(nfinish);
    begin_.cur = begin_.first;
    end_.cur = end_.first + (nelem % buffer_size);
}

template <class T>
void deque<T>::fill_init(size_type n, const value_type& value) {
    map_init(n);
    if (n != 0) {
        for (auto cur = begin_.node; cur < end_.node; ++cur) {
            MySTL::uninitialized_fill(*cur, *cur + buffer_size, value);
        }
        MySTL::uninitialized_fill(end_.first, end_.cur, value);
    }
}

template <class T>
template <class Iter>
void deque<T>::copy_init(Iter first, Iter last, input_iterator_tag) {
    const size_type n = MySTL::distance(first, last);
    map_init(n);
    for (; first != last; ++first)
        emplace_back(*first);
}

/// @brief forward_iterator_tag，使用uninitialized_copy
/// @tparam T 
/// @param first 
/// @param last 
/// @param  
template <class T>
template <class Iter>
void deque<T>::copy_init(Iter first, Iter last, forward_iterator_tag) {
    const size_type n = MySTL::distance(first, last);
    map_init(n);
    for (auto cur = begin_.node; cur < end_.node; ++cur) {
        auto next = first;
        MySTL::advance(next, buffer_size);
        MySTL::uninitialized_copy(first, next, *cur);
        first = next;
    }
    MySTL::uninitialized_copy(first, last, end_.first);
}

template <class T>
void deque<T>::fill_assign(size_type n, const value_type& value) {
    if (n > size()) {
        MySTL::fill(begin(), end(), value);
        insert(end(), n - size(), value);
    } else {
        erase(begin() + n, end());
        MySTL::fill(begin(), end(), value);
    }
}

//
template <class T>
template <class Iter>
void deque<T>::copy_assign(Iter first, Iter last, input_iterator_tag) {
    auto first1 = begin();
    auto last1 = end();
    for (; first != last && first1 != last1; ++first, ++first1) {
        *first1 = *first;
    }
    if (first1 != last1) {
        erase(first1, last1);
    } else {
        insert_dispatch(end_, first, last, input_iterator_tag{});
    }
}

template <class T>
template <class Iter>
void deque<T>::copy_assign(Iter first, Iter last, forward_iterator_tag) {
    const size_type len1 = size();
    const size_type len2 = MySTL::distance(first, last);
    if (len1 < len2) {
        auto next = first;
        MySTL::advance(next, len1);
        MySTL::copy(next, first, begin_);
        insert_dispatch(end_, first, last, input_iterator_tag{});
    } else {
        erase(MySTL::copy(first, last, begin_), end_);
    }
}

template <class T>
template <class... Args>
typename deque<T>::iterator
deque<T>::insert_aux(iterator pos, Args&&... args) {
    const size_type elems_before = pos - begin_;
    value_type      value_copy = value_type(MySTL::forward<Args>(args)...);  // 防止改变原数值
    if (elems_before < (size() / 2)) {                                       // 插入到前半段
        emplace_front(front());
        auto front1 = begin_;
        ++front1;
        auto front2 = front1;
        ++front2;
        pos = begin_ + elems_before;
        auto pos_back = pos;
        ++pos_back;
        MySTL::copy(front2, pos_back, front1);
    } else {
        emplace_back(back());
        auto back1 = end_;
        --back1;
        auto back2 = back1;
        --back2;
        pos = begin_ + elems_before;
        MySTL::copy_backward(pos, back2, back1); // 在pos处空出一位
    }
    *pos = MySTL::move(value_copy);
    return pos;
}

//  fill_insert
template <class T>
void deque<T>::fill_insert(iterator pos, size_type n, const value_type& value) {
    const size_type elems_before = pos - begin_;
    const size_type len = size();
    auto            value_copy = value;
    if (elems_before < (len / 2)) {
        require_capacity(n, true);
        auto old_begin = begin_; // 防止迭代器失效
        auto new_begin = begin_ - n;
        pos = begin_ + elems_before;
        try {
            if (elems_before >= n) {
                auto begin_n = begin_ + n;
                MySTL::uninitialized_copy(begin_, begin_n, new_begin);
                begin_ = new_begin;
                MySTL::copy(begin_n, pos, old_begin);
                MySTL::fill(pos - n, pos, value_copy);
            } else {
                MySTL::uninitialized_fill(
                    MySTL::uninitialized_copy(begin_, pos, new_begin), begin_, value_copy);
                begin_ = new_begin;
                MySTL::fill(old_begin, pos, value_copy);
            }
        } catch (...) {
            if (new_begin.node != begin_.node) {
                destory_buffer(new_begin.node, begin_.node - 1);
            }
            throw;
        }
    } else {
        auto            old_end = end_;
        auto            new_end = end_ + n;
        const size_type elems_after = len - elems_before;
        pos = end_ - elems_after;
        try {
            if (elems_after > n) {
                auto end_n = end_ - n;
                MySTL::uninitialized_copy(end_n, end_, end_);
                end_ = new_end;
                MySTL::copy_backward(pos, end_n, old_end);
                MySTL::fill(pos, pos + n, value_copy);
            } else {
                MySTL::uninitialized_fill(end_, pos + n, value_copy);
                MySTL::uninitialized_copy(pos, end_, pos + n);
                end_ = new_end;
                MySTL::fill(pos, old_end, value_copy);
            }
        } catch (...) {
            if (new_end.node != end_.node) {
                destory_buffer(end_.node + 1, new_end.node);
            }
            throw;
        }
    }
}

template <class T>
template <class Iter>
void deque<T>::insert_dispatch(iterator pos, Iter first, Iter last, input_iterator_tag) {
    if (last <= first) return;
    const size_type n = MySTL::distance(first, last);
    const size_type elems_before = pos - begin_;
    if (elems_before < (size() / 2)) {
        require_capacity(n, true);
    } else {
        require_capacity(n, false);
    }
    pos = begin_ + elems_before;
    auto cur = --last;
    for (size_type i = 0; i < n; i++, --cur) {
        insert(pos, *cur);
    }
}

template <class T>
template <class Iter>
void deque<T>::insert_dispatch(iterator pos, Iter first, Iter last, forward_iterator_tag) {
    if (last <= first) return;
    const size_type n = MySTL::distance(first, last);
    if (pos.cur == begin_.cur) {
        require_capacity(n, true);
        auto new_begin = begin_ - n;
        try {
            MySTL::uninitialized_copy(first, last, new_begin);
            begin_ = new_begin;
        } catch (...) {
            if (new_begin.node != begin_.node)
                destory_buffer(new_begin.node, begin_.node - 1);
            throw;
        }
    } else if (pos.cur == end_.cur) {
        require_capacity(n, false);
        auto new_end = end_ + n;
        try {
            MySTL::uninitialized_copy(first, last, end_);
            end_ = new_end;
        } catch (...) {
            if (new_end.node != end_.node)
                destory_buffer(end_.node + 1, new_end.node);
            throw;
        }
    } else {
        copy_insert(pos, first, last, n);
    }
}

// bool front表示是否在前面插入
template <class T>
void deque<T>::require_capacity(size_type n, bool front) {
    if (front && (static_cast<size_type>(begin_.cur - begin_.first) < n)) {
        const size_type need_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
        if (need_buffer > static_cast<size_type>(begin_.node - map_)) {
            reallocate_map_at_front(need_buffer);
            return;
        }
        create_buffer(begin_.node - need_buffer, begin_.node - 1);
    } else if (!front && (static_cast<size_type>(end_.last - end_.cur - 1))) {
        const size_type need_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
        if (need_buffer > static_cast<size_type>(map_ + map_size_) - end_.node - 1) {
            reallocate_map_at_back(need_buffer);
            return;
        }
        create_buffer(end_.node + 1, end_.node + need_buffer);
    }
}

template <class T>
void deque<T>::reallocate_map_at_front(size_type need_buffer) {
    const size_type new_map_size = MySTL::max(map_size_ << 1, map_size_ + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer     new_map = create_map(new_map_size);
    const size_type old_buffer = end_.node - begin_.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    // map中的指针指向原来的buffer，并开辟新的buffer
    auto begin = new_map + (new_map_size - new_buffer) / 2;
    auto mid = begin + need_buffer;
    auto end = mid + old_buffer;
    create_buffer(begin, mid - 1);
    for (auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, ++begin2) {
        *begin1 = *begin2;
    }
    // 更新数据
    map_allocator::deallocte(map_, map_size_);
    map_ = new_buffer;
    map_size_ = new_map_size;
    begin_ = iterator(*mid + (begin_.cur - begin_.first), mid);
    end_ = iterator(*(end - 1) + (end_.cur - end_.first), end - 1);
}

// FIGUREOUT: 重新分配内存的指针操作
template <class T>
void deque<T>::reallocate_map_at_back(size_type need_buffer) {
    const size_type new_map_size = MySTL::max(map_size_ << 1, map_size_ + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer     new_map = create_map(new_map_size);
    const size_type old_buffer = end_.node - begin_.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    // 新的map中的指针指向原来的buffer，并开辟新的buffer
    auto begin = new_map + (new_map_size - new_buffer) / 2;
    auto mid = begin + old_buffer;
    auto end = mid + need_buffer;
    create_buffer(mid, mid - 1);
    for (auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, ++begin2) {
        *begin1 = *begin2;
    }
    // 更新数据
    map_allocator::deallocte(map_, map_size_);
    map_ = new_map;
    map_size_ = new_map_size;
    begin_ = iterator(*begin + (begin_.cur - begin_.first), begin);
    end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
}

// 重载比较操作符
template <class T>
bool operator==(const deque<T>& lhs, const deque<T>& rhs) {
    return lhs.size() == rhs.size() &&
           MySTL::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const deque<T>& lhs, const deque<T>& rhs) {
    return MySTL::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const deque<T>& lhs, const deque<T>& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator>(const deque<T>& lhs, const deque<T>& rhs) {
    return rhs < lhs;
}

template <class T>
bool operator<=(const deque<T>& lhs, const deque<T>& rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const deque<T>& lhs, const deque<T>& rhs) {
    return !(lhs < rhs);
}

// 重载全局 swap
template <class T>
void swap(MySTL::deque<T>& lhs, MySTL::deque<T>& rhs) {
    lhs.swap(rhs);
}

};  // namespace MySTL


#endif /* MY_DEQUE_H */