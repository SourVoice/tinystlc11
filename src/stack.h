#ifndef MY_STACK_H
#define MY_STACK_H

#include <initializer_list>

#include "deque.h"
#include "functional.h"
#include "uninitialize.h"

/// 含 stack 模板类

namespace MySTL {

template <class T, typename Container = MySTL::deque<T>>
class stack {
public:
    typedef Container                           container_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "The value_type of Container should be same with T");

private:
    container_type c_; // 使用 deque 作为底层

public:
    /*********************************** 构造，复制，移动，析构 ***********************************/

    // 构造
    stack() = default;

    stack(size_type n) : c_(n) {}

    explicit stack(size_type n, const value_type& value) : c_(n, value) {}

    template<class Iter>
    stack(Iter first, Iter last) : c_(first, last) {}

    stack(std::initializer_list<value_type> ilist) : c_(ilist.begin(), ilist.end()) {}

    // 拷贝
    stack(const Container& c) : c_(c) {}

    stack(const stack& rhs) : c_(rhs.c_) {}


    // 移动
    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
    : c_(MySTL::move(c)) {}

    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) :
        c_(MySTL::move(rhs.c_)) {}


    // 赋值拷贝
    stack& operator=(const stack& rhs)  {
        c_ = rhs.c_;
        return *this;
    }

    // 赋值移动
    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) {
        c_ = MySTL::move(rhs.c_);
        return *this;
    }

    stack& operator=(std::initializer_list<T> ilist) {
        c_ = ilist;
        return *this;
    }

    // 析构
    ~stack() = default;

public:
    /***********************************  迭代器相关操作 ***********************************/

    // 访问
    reference       top() { return c_.back(); }
    const_reference top() const { return c_.back(); }

    // 容量
    bool      empty() const noexcept { return c_.empty(); }
    size_type size() const noexcept { return c_.size(); }

    // 容器操作
    template <class... Args>
    void emplace(Args&&... args) { c_.emplace_back(MySTL::forward<Args>(args)...); }

    void push(const value_type& value) { c_.push_back(value); }
    void push(value_type&& value) { c_.push_back(MySTL::move(value)); }

    void pop() { c_.pop_back(); }

    void clear() { while (!empty()) pop(); }

    // 交换
    void swap(stack& rhs) noexcept(noexcept(MySTL::swap(c_, rhs.c_))) {
        MySTL::swap(c_, rhs.c_);
    }

public:
    friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator<(const stack& lhs, const stack& rhs) { return lhs.c_ < rhs.c_; }
};

// 重载比较运算符
template <class T, class Container>
bool operator==(stack<T, Container>& lhs, stack<T, Container>& rhs) { return lhs == rhs; }

template <class T, class Container>
bool operator<(stack<T, Container>& lhs, stack<T, Container>& rhs) { return lhs < rhs; }

template <class T, class Container>
bool operator!=(stack<T, Container>& lhs, stack<T, Container>& rhs) { return !(lhs == rhs); }

template <class T, class Container>
bool operator>(stack<T, Container>& lhs, stack<T, Container>& rhs) { return rhs < lhs; }

template <class T, class Container>
bool operator>=(stack<T, Container>& lhs, stack<T, Container>& rhs) { return !(lhs < rhs); }

template <class T, class Container>
bool operator<=(stack<T, Container>& lhs, stack<T, Container>& rhs) { return !(rhs < lhs); }

// 重载全局swap
template <class T, class Container>
void swap(stack<T, Container> lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace MySTL

#endif /* MY_STACK_H */