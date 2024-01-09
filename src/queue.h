#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

// 包含两个模板类，queue和priority_queue

#include "deque.h"
#include "vector.h"
#include "functional.h"
#include "heap_algo.h"

namespace MySTL {

// 模板类 queue
template <class T, class Container = MySTL::deque<T>>
class queue {
public:
    typedef Container                           container_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

private:
    container_type c_;  // 底层容器表现queue
public:
    /*********************************** 构造，复制，移动，析构 ***********************************/

    // 构造
    queue() = default;

    explicit queue(size_type n) : c_(n) {}

    queue(size_type n, const value_type& value) : c_(n, value) {}

    template <class Iter>
    queue(Iter first, Iter last) : c_(first, last) {}

    queue(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()) {}

    queue(const Container& c) : c_(c) {}

    queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) :
        c_(MySTL::move(c)) {}

    // 复制
    queue(const queue& rhs) : c_(rhs.c_) {}

    // 拷贝
    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) :
        c_(MySTL::move(rhs.c_)) {}

    queue& operator=(const queue& rhs) {
        c_ = rhs.c_;
        return *this;
    }

    queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) {
        c_ = MySTL::move(rhs.c_);
        return *this;
    }

    queue& operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        return *this;
    }

    ~queue() = default;

    /***********************************  迭代器相关操作 ***********************************/

    // 访问元素
    reference       front() { return c_.front(); }
    const_reference front() const { return c_.front(); }
    reference       back() { return c_.back(); }
    const_reference back() const { return c_.back(); }

    // 容器相关
    bool      empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 容器修改
    template <class... Args>
    void emplace(Args&&... args) { c_.emplace_back(MySTL::forward<Args>(args)...); }

    void push(const value_type& value) { c_.push_back(value); }
    void push(value_type&& value) { c_.emplace_back(MySTL::move(value)); }

    void pop() { c_.pop_front(); }

    void clear() {
        while (!empty())
            pop();
    }

    void swap(queue& rhs) noexcept(noexcept(MySTL::swap(c_, rhs.c_))) /* 外部的noexcept是标识符，内部的是noexcept运算符 */ {
        MySTL::swap(c_, rhs.c_);
    }

public:
    friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator<(const queue& lhs, const queue& rhs) { return lhs.c_ < rhs.c_; }
};

// 重载比较运算符
template <class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(lhs < rhs);
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

// 模板类 priority_queue
// Compare 指定权重比较方式，默认使用less
template <class T, class Container = MySTL::vector<T>,
          class Compare = MySTL::less<typename Container::value_type>>
class priority_queue{
public: 
    typedef Container                           container_type;
    typedef Compare                             vlaue_compare;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

private:
    container_type c_;  // 使用底层容器表示priority_queue
    Compare        comp_;

public:

    /*********************************** 构造，复制，移动，析构 ***********************************/

    // 构造
    priority_queue() = default;

    priority_queue(const Compare& comp) : c_(), comp_(comp) {}

    explicit priority_queue(size_type n) : c_(n) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    template <class Iter>
    priority_queue(Iter first, Iter last) : c_(first, last) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(std::initializer_list<T> ilist) : c_(ilist) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(const Container& s) : c_(s) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(Container&& s) : c_(MySTL::move(s)) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    // 拷贝构造
    priority_queue(const priority_queue& rhs) : c_(rhs.c_), comp_(rhs.comp_) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    // 移动
    priority_queue(const priority_queue&& rhs) : c_(MySTL::move(rhs.c_)), comp_(rhs.comp_) {
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
    }

    // 赋值拷贝
    priority_queue& operator=(const priority_queue& rhs) {
        c_ = rhs.c_;
        comp_ = rhs.comp_;
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    priority_queue& operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        comp_ = vlaue_compare();
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    // 移动赋值
    priority_queue& operator=(const priority_queue&& rhs) {
        c_ = MySTL::move(rhs.c_);
        comp_ = rhs.comp_;
        MySTL::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    ~priority_queue() = default;

public:

    /*********************************** 容器操作 ***********************************/

    // 访问
    const_reference top() const { return c_.front(); }

    // 容量
    bool      empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 修改容器
    template <class... Args>
    void emplace(Args&&... args) {
        c_.emplace_back(MySTL::forward<Args>(args)...);
        MySTL::push_heap(c_.begin(), c_.end(), comp_);
    }

    void push(const value_type& value) {
        c_.push_back(value);
        MySTL::push_heap(c_.begin(), c_.end(), comp_);
    }

    void push(value_type&& value) {
        c_.push_back(MySTL::move(value));
        MySTL::push_heap(c_.begin(), c_.end(), comp_);
    }

    void pop() {
        MySTL::pop_heap(c_.begin(), c_.end(), comp_);
        c_.pop_back();
    }

    void clear() {
        while (!empty()) pop();
    }

    // 自定义的swap
    void swap(priority_queue& rhs) noexcept(noexcept(MySTL::swap(c_, rhs.c_)) &&
                                            noexcept(MySTL::swap(comp_, rhs.comp_))) {
        MySTL::swap(c_, rhs.c_);
        MySTL::swap(comp_, rhs.comp_);
    }

public:
    friend bool operator==(const priority_queue& lhs, const priority_queue& rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs) { return lhs.c_ != rhs.c_; }
};

// 重载比较操作符

template <class T, class Container, class Compare>
bool operator==(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
    return lhs == rhs;
}

template <class T, class Container, class Compare>
bool operator!=(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
          priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace MySTL

#endif /* MYSTL_QUEUE_H */