#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

// 双向链表类的实现
#include <initializer_list>
#include <ios>
#include <type_traits>

#include "iterator.h"
#include "memory"
#include "functional.h"
#include "memory.h"
#include "myallocator.h"
#include "util.h"
#include "exceptdef.h"


namespace MySTL {

template <class T> struct list_node_base;
template <class T> struct list_node;

template <class T>
struct node_traits {
    typedef list_node_base<T>* base_ptr;
    typedef list_node<T>*      node_ptr;
};

// list结点结构
template <class T>
struct list_node_base {
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    base_ptr prev;
    base_ptr next;

    list_node_base() = default;

    node_ptr as_node() {
        return static_cast<node_ptr>(self());
    }

    void unlink() {
        prev = next = self();
    }

    base_ptr self(){
        return static_cast<base_ptr>(&*this);
    }
};

template <class T>
struct list_node : public list_node_base<T> {
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    T value;  // 结点数据

    list_node() = default;

    list_node(const T& v) : value(v) {}

    list_node(T&& v) : value(MySTL::move(v)) {}

    base_ptr as_base() {
        return static_cast<base_ptr>(&*this);
    }

    node_ptr self() {
        return static_cast<node_ptr>(&*this);
    }
};

// list迭代器设计
template <class T>
struct list_iterator : public MySTL::iterator<MySTL::bidirectional_iterator_tag, T> {
    typedef T                                 value_type;
    typedef T*                                pointer;
    typedef T&                                reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_iterator<T>                  self;

    base_ptr node_;  // 指向当前节点

    /*********************************** 构造函数 ***********************************/

    list_iterator() = default;

    list_iterator(base_ptr x) : node_(x) {}

    list_iterator(node_ptr x) : node_(x->as_base()) {}

    list_iterator(const list_iterator& rhs) : node_(rhs.node_) {}

    /*********************************** 迭代器相关操作 ***********************************/

    reference operator*() const { return node_->as_node()->value; }
    pointer   operator->() const { return &(operator*()); }

    self& operator++() {  // 返回引用，方便链式操作，effective c++ item 6
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_->next;
        return *this;
    }

    self operator++(int) {  // 后++
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {    
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_->prev;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self& rhs) const { return node_ == rhs.node_; }
    bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

template <class T>
struct list_const_iterator : public MySTL::iterator<bidirectional_iterator_tag, T> {
    typedef T                                 value_type;
    typedef const T*                          pointer;
    typedef const T&                          reference;
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;
    typedef list_const_iterator<T>            self;

    base_ptr node_;

    list_const_iterator() = default;

    list_const_iterator(base_ptr x) : node_(x) {}

    list_const_iterator(node_ptr x) : node_(x->as_base()) {}

    list_const_iterator(const list_iterator<T>&rhs) : node_(rhs.node_) {}

    list_const_iterator(const list_const_iterator<T>&rhs) : node_(rhs.node_) {}

    reference operator*() const { return node_->as_node()->value(); }
    pointer   operator->() const { return &(operator*()); }

    self& operator++() {
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_->next;
        return *this;
    }

    self operator++(int) {  // 后++
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_->prev;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self& rhs) const { return node_ == rhs.node_; }
    bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

};

template <class T>
class list {

public:
    typedef MySTL::allocator<T>                         allocator_type;
    typedef MySTL::allocator<T>                         data_allocator;
    typedef MySTL::allocator<list_node_base<T>>         base_allocator;
    typedef MySTL::allocator<list_node<T>>              node_allocator;

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef list_iterator<T>                            iterator;
    typedef list_const_iterator<T>                      const_iterator;
    typedef MySTL::reverse_iterator<iterator>           reverse_iterator;
    typedef MySTL::reverse_iterator<const_iterator>     const_reverse_iterator;

    typedef typename node_traits<T>::base_ptr           base_ptr;
    typedef typename node_traits<T>::node_ptr           node_ptr;

    allocator_type get_allocator() {return node_allocator();}


private:
    base_ptr  node_; // node是一个dummy 结点
    size_type size_;

public:
    /*********************************** 构造，复制，移动，析构 ***********************************/

    // 构造

    list() { fill_init(0, value_type()); }

    explicit list(size_type n) { fill_init(n, value_type()); }

    list(size_type n, const T& value) { fill_init(n, value); }

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    list(Iter first, Iter last) { copy_init(first, last); }

    list(std::initializer_list<T> ilist) { copy_init(ilist.begin(), ilist.end()); }

    // 复制
    list(const list& rhs) { copy_init(rhs.begin(), rhs.end()); }

    // 移动
    list(list&& rhs) noexcept :
        node_(rhs.node_), size_(rhs.size_) {
        rhs.node_ = nullptr;
        rhs.size_ = 0;
    }

    // 重载赋值运算符

    list& operator=(const list& rhs) {
        if (this != &rhs) {
            assign(rhs.begin(), rhs.end());
        }
        return *this;
    }

    list& operator=(list &&rhs) {
        clear();
        splice(end(), rhs);
        return *this;
    }

    list& operator=(std::initializer_list<T> ilist) {
        list tmp(ilist.begin, ilist.end());
        swap(tmp);
        return *this;
    }

    // 析构
    ~list() {
        if (node_) {
            clear();
            base_allocator::deallocte(node_);
            node_ = nullptr;
            size_ = 0;
        }
    }

public:

    /***********************************  迭代器相关操作 ***********************************/

    iterator       begin() noexcept { return node_->next; }
    const_iterator begin() const noexcept { return node_->next; }
    iterator       end() noexcept { return node_; }
    const_iterator end() const noexcept { return node_; }

    reverse_iterator       rbgin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbgin() const noexcept { return reverse_iterator(end()); }
    reverse_iterator       rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    /***********************************  容器相关操作 ***********************************/

    bool empty() const noexcept { return node_->next == node_; }

    size_type size() const noexcept { return size_; }

    size_type max_size() const noexcept { return static_cast<size_type>(-1); }


    /***********************************  元素访问 ***********************************/

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
        return *(--end());
    }

    /***********************************  容器调整 ***********************************/

    // assign

    void assign(size_type n, const value_type& value) { fill_assign(n, value); }

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last) { copy_assign(first, last); }

    void assign(std::initializer_list<T> ilist) { copy_assign(ilist.begin(), ilist.end()); }

    // emplace_front & emplace_back & emplace

    template <class... Args>
    void emplace_front(Args&&... args) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(MySTL::forward<Args>(args)...);
        link_nodes_at_front(link_node->as_base(), link_node->ase_base());
        ++size_;
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(MySTL::forward<Args>(args)...);
        link_nodes_at_back(link_node->as_base(), link_node->ase_base());
        ++size_;
    }

    template <class... Args>
    void emplace(Args&&... args) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(MySTL::forward<Args>(args)...);
        link_nodes(link_node->as_base(), link_node->ase_base());
        ++size_;
    }
    
    // insert

    iterator insert(const_iterator pos, const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(value);
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, const value_type&& value) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(MySTL::move(value));
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, size_type n, const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ + n > max_size(), "list<T>'s size too big");
        return fill_insert(pos, n, value);
    }

    template <class Iter, typename std::enable_if<MySTL::is_input_iterator<Iter>::value, int>::type = 0>
    iterator insert(const_iterator pos, Iter first, Iter last) {
        size_type len = MySTL::distance(first, last);
        THROW_LENGTH_ERROR_IF(size_ + len > max_size(), "list<T>'s size too big");
        return copy_insert(pos, len, first);
    }

    // push_front & push_back

    void push_front(const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(value);
        link_nodes_at_front(link_node->as_base(), link_node->as_base());
        ++size_;
    }

    void push_front(value_type&& value) {
        emplace_front(MySTL::move(value));
    }

    void push_back(const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");
        auto link_node = create_node(value);
        link_nodes_at_back(link_node->as_base(), link_node->as_base());
        ++size_;
    }

    void push_back(value_type&& value) {
        emplace_back(MySTL::move(value));
    }

    // pop_front & pop_back

    void pop_front() {
        MYSTL_DEBUG(!empty());
        auto n = node_ -> next;
        unlink_nodes(n, n);
        destory_node(n->as_node());
        --size_;
    }

    void pop_back() {
        MYSTL_DEBUG(!empty());
        auto n = node_ -> prev;
        unlink_nodes(n, n);
        destory_node(n->as_node());
        --size_;
    }

    // erase & clear

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void clear();

    // resize
    void resize(size_type new_size) { return resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type& value);

    void swap(list& rhs) noexcept {
        MySTL::swap(node_, rhs.node_);
        MySTL::swap(size_, rhs.size_);
    }

    /*********************************** list整体相关操作 ***********************************/
    
    void splice(const_iterator pos, list& other);
    void splice(const_iterator pos, list& other, const_iterator it);
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

    // remove
    void remove(const value_type& value) {
        remove_if([&](const value_type& v) { return v == value; });
    }
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate pred); 

    // unique
    void unique() { unique(MySTL::equal_to<T>()); }
    template <class BinaryPredicate>
    void unique(BinaryPredicate pred);

    // merge
    void merge(list& x) { merge(x, MySTL::less<T>()); }
    template <class Compare>
    void merge(list& x, Compare comp);

    // sort
    void sort() { list_sort(begin(), end(), size(), MySTL::less<T>()); }
    template <class Compare>
    void sort(Compare cmp) { list_sort(begin(), end(), size(), cmp); }

    void reverse();

private:

    /*********************************** helper functions ***********************************/

    // 创建结点
    template <class... Args>
    node_ptr create_node(Args&&... args);

    void destory_node(node_ptr p);

    // 初始化
    void fill_init(size_type n, const value_type& value);
    template <class Iter>
    void copy_init(Iter first, Iter last);

    // link & unlink
    iterator link_iter_node(const_iterator pos, base_ptr node);
    void     link_nodes(base_ptr p, base_ptr first, base_ptr last);
    void     link_nodes_at_front(base_ptr first, base_ptr last);
    void     link_nodes_at_back(base_ptr first, base_ptr last);
    void     unlink_nodes(base_ptr first, base_ptr last);

    // assign
    void fill_assign(size_type n, const value_type& value);
    template <class Iter>
    void copy_assign(Iter first2, Iter last2);

    // 实现insert
    iterator fill_insert(const_iterator pos, size_type n, const value_type& value);
    template <class Iter>
    iterator copy_insert(const_iterator pos, size_type n, Iter first);

    template <class Compare>
    iterator list_sort(iterator first, iterator last, size_type n, Compare comp);
};

/*********************************** 容器操作 ***********************************/

/**
 * erase
*/

// 删除pos
template <class T>
typename list<T>::iterator
list<T>::erase(const_iterator pos) {
    MYSTL_DEBUG(pos != cend());
    auto n = pos.node_;
    auto next = n->next;
    unlink_nodes(n, n);
    destory_node(n->as_node());
    --size_;
    return iterator(next);
}

// 删除 [first, last), 返回last
template <class T>
typename list<T>::iterator
list<T>::erase(const_iterator first, const_iterator last) {
    if (first != last) {
        unlink_nodes(first.node_, last.node_->prev);
        while (first != last) {
            auto cur = first.node_;
            ++first;
            destory_node(cur->as_node());
            --size_;
        }
    }
    return iterator(last.node_);
}

// 清空list
template <class T>
void list<T>::clear() {
    if (size_ != 0) {
        auto cur = node_->next;
        for (base_ptr next = cur->next; cur != node_; cur = next, next = cur->next) {
            destory_node(cur->as_node());
        }
        node_->unlink();
        size_ = 0;
    }
}

// resize
template <class T>
void list<T>::resize(size_type new_size, const value_type& value) {
    auto      i = begin();
    size_type len = 0;
    while (i != end() && len < new_size) {
        ++i;
        ++len;
    }
    if (len == new_size) {
        erase(i, node_);
    } else {
        insert(node_, new_size - len, value);
    }
}

// 将list x 接合于pos之前
template <class T>
void list<T>::splice(const_iterator pos, list& x) {
    MYSTL_DEBUG(this != &x);
    if (!x.empty()) {
        THROW_LENGTH_ERROR_IF(size_ + x.size_ > max_size(), "list<T>'s size too big");
        auto first = x.node_->next;
        auto last = x.node_->prev;
        x.unlink_nodes(first, last);
        link_nodes(pos.node_, first, last);
        size_ += x.size_;
        x.size_ = 0;
    }
}

// 将list x 中的it 所指节点 接合于pos之前
template <class T>
void list<T>::splice(const_iterator pos, list& x, const_iterator it) {
    if (pos.node_ != it.node_ && pos.node_ != it.node_ -> next) {
        THROW_LENGTH_ERROR_IF(size_ + 1 > max_size(), "list<T>'s size too big");

        auto f = it.node_;
        x.unlink_nodes(f, f);
        link_nodes(pos.node_, f, f);

        ++size_;
        --x.size_;
    }
}

// 将list x 的[first, last) 接合于pos之前
template <class T>
void list<T>::splice(const_iterator pos, list& x, const_iterator first, const_iterator last) {
    if (first != last && this != &x) {
        size_type len = MySTL::distance(first, last);
        THROW_LENGTH_ERROR_IF(size_ + len > max_size(), "list<T>'s size too big");
        auto f = first.node_;
        auto l = last.node_->prev;
        x.unlink_nodes(f, l);
        link_nodes(pos.node_, f, l);
        x.size_ -= len;
        size_ += len;
    }
}

// 一元操作pred作为true的元素进行移除
template <class T>
template <class UnaryPredicate>
void list<T>::remove_if(UnaryPredicate pred) {
    auto f = begin();
    auto l = end();
    for (auto next = f; f != l; f = next) {
        ++next;
        if (pred(*f)) {
            erase(f);
        }
    }
}

// 移除list 中满足pred为true的重复元素
template <class T>
template <class BinaryPredicate>
void list<T>::unique(BinaryPredicate pred) {
    auto i = begin();
    auto e = end();
    auto j = i;
    ++j;
    while (j != e) {
        if (pred(*i, *j)) {
            erase(j);
        } else {
            i = j;
        }
        j = i;
        ++j;
    }
}

// 与另一个list合并
template <class T>
template <class Compare>
void list<T>::merge(list& x, Compare cmp) {
    if (this != &x) {
        THROW_LENGTH_ERROR_IF(size_ + x.size_ > max_size(), "list<T>'s size too big");

        auto first1 = begin();
        auto last1 = end();
        auto first2 = x.begin();
        auto last2 = x.end();
        while (first1 != last1 && first2 != last2) {

            if (cmp(*first2, *first1)) {
                // 找出使 cmp 为true的一段区间
                auto next = first2;
                ++next;
                for (; next != last2 && cmp(*next, *first1); ++next)
                    ;
                auto f = first2.node_;
                auto l = next.node_->prev;
                first2 = next;

                // link node
                x.unlink_nodes(f, l);
                link_nodes(first1.node_, f, l);
                ++first1;
            } else {
                first1++;
            }
        }

        // 连接剩余部分
        if (first2 != last2) {
            auto f = first2.node_;
            auto l = last2.node_->prev;
            x.unlink_nodes(f, l);
            link_nodes(last1.node_, f, l);
        }
        size_ += x.size_;
        x.size_ = 0;
    }
}

// 反转list
template <class T>
void list<T>::reverse() {
    if (size_ <= 1) {
        return;
    }
    auto i = begin();
    auto e = end();
    while (i.node_ != e.node_) {
        MySTL::swap(i.node_->prev, i.node_->next);
        i.node_ = i.node_->prev;
    }
    MySTL::swap(e.node_->prev, e.node_->next);
}

/*********************************** helper functions ***********************************/

// 创建结点
template <class T>
template <class... Args>
typename list<T>::node_ptr
list<T>::create_node(Args&&... args) {
    node_ptr p = node_allocator::allocate(1);
    try {
        data_allocator::construct(MySTL::address_of(p->value), MySTL::forward<Args>(args)...);
        p->prev = nullptr;
        p->next = nullptr;
    } catch (...) {
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}

// 销毁节点
template <class T>
void list<T>::destory_node(node_ptr p) {
    data_allocator::destory(MySTL::address_of(p->value));
    node_allocator::deallocate(p);
}

// 使用 n 个 value 初始化容器
template <class T>
void list<T>::fill_init(size_type n, const value_type& value) {
    node_ = base_allocator::allocate(1);
    node_->unlink();
    size_ = n;
    try {
        for (; n > 0; --n) {
            auto node = create_node(value);
            link_nodes_at_back(node->as_base(), node->as_base());
        }
    } catch (...) {
        clear();
        base_allocator::deallocate(node_);
        node_ = nullptr;
        throw;
    }
}

// 使用 [first, last) 初始化容器
template <class T>
template <class Iter>
void list<T>::copy_init(Iter first, Iter last) {
    node_ = base_allocator::allocate(1);
    node_->unlink();
    size_type n = MySTL::distance(first, last);
    size_ = n;
    try {
        for (; n > 0; --n, ++first) {
            auto node = create_node(*first);
            link_nodes_at_back(node->as_base(), node->as_base());
        }
    } catch (...) {
        clear();
        base_allocator::deallocate(node_);
        node_ = nullptr;
        throw;
    }
}

/**
 * link part
*/

// 在pos处连接节点
template <class T>
typename list<T>::iterator
list<T>::link_iter_node(const_iterator pos, base_ptr link_node) {
    if (pos == node_->next) {
        link_nodes_at_front(link_node, link_node);
    } else if (pos == node_) {
        link_nodes_at_back(link_node, link_node);
    } else {
        link_nodes(pos.node_, link_node, link_node);
    }
    return iterator(link_node);
}

// 在 pos 处连接 [first, last]
template <class T>
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last) {
    pos->prev->next = first;
    first->prev = pos->prev;
    pos->prev = last;
    last->next = pos;
}

// 在头部连接 [first, last]
template <class T>
void list<T>::link_nodes_at_front(base_ptr first, base_ptr last) {
    first->prev = node_;
    last->next = node_->next;
    last->next->prev = last;
    node_->next = first; // 在哑结点处形成环形
}

// 在尾部连接 [first, last]
template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last) {
    last->next = node_;
    first->prev = node_->prev;
    first->prev->next = first;
    node_->prev = last;
}

// 容器断开与 [first, last] 的连接
template <class T>
void list<T>::unlink_nodes(base_ptr first, base_ptr last) {
    first->prev->next = last->next;
    last->next->prev = first->prev;
}

/**
 * assign
*/

// 用n个元素为容器赋值
template <class T>
void list<T>::fill_assign(size_type n, const value_type& value) {
    auto i = begin();
    auto e = end();
    for (; n > 0 && i != e; --n, ++i) {
        *i = value;
    }
    if (n > 0)
        insert(e, n, value);
    else
        erase(i, 0);
}

// 复制 [first, last) 为容器赋值
template <class T>
template <class Iter>
void list<T>::copy_assign(Iter first2, Iter last2) {
    auto first1 = begin();
    auto last1 = end();
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        *first1 = *first2;
    }
    if (first2 == last2)
        erase(first1, last1);
    else
        insert(last1, first2, last2);
}

/**
 * insert
*/

// 在pos处插入n个元素
template <class T>
typename list<T>::iterator
list<T>::fill_insert(const_iterator pos, size_type n, const value_type& value) {
    iterator r(pos.node_);
    if (n != 0) {
        const auto add_size = n;
        auto       node = create_node(value);
        node->prev = nullptr;
        r = iterator(node);  // r 作为temp保存首位节点
        iterator end = r;
        try {
            for (--n; n > 0; --n, ++end) {  // 还需要n - 1个
                auto next = create_node(value);
                end.node_->next = next->as_base();
                next->prev = end.node_;
            }
            size_ += add_size;
        } catch (...) {
            auto enode = end.node_;
            while (true) {  // 从end向前释放
                auto prev = enode->prev;
                destory_node(enode->as_node());
                if (prev == nullptr)
                    break;
                enode = prev;
            }
            throw;
        }
        link_nodes(pos.node_, r.node_, end.node_);
    }
    return r;
}

// 在 pos 处插入 [first, last) 的元素
template <class T>
template <class Iter>
typename list<T>::iterator
list<T>::copy_insert(const_iterator pos, size_type n, Iter first) {
    iterator r(pos.node_);
    if (n != 0) {
        const auto add_size = n;
        auto       node = create_node(*first);
        node->prev = nullptr;
        r = iterator(node);
        iterator end = r;
        try {
            for (--n, ++first; n > 0; --n, ++end) {  // 还需要n - 1个
                auto next = create_node(*first);
                end.node_->next = next->as_base();
                next->prev = end.node_;
            }
            size_ += add_size;
        } catch (...) {
            auto enode = end.node_;
            while (true) {  // 从end向前释放
                auto prev = enode->prev;
                destory_node(enode->as_node());
                if (prev == nullptr)
                    break;
                enode = prev;
            }
            throw;
        }
        link_nodes(pos.node_, r.node_, end.node_);
    }
    return r;
}

// FIGUREOUT: 归并排序的实现
// list归并排序
template <class T>
template <class Compare>
typename list<T>::iterator
list<T>::list_sort(iterator first1, iterator last2, size_type n, Compare cmp) { // last2和first1属于同一个链表不同前后段的表示方法
    if (n < 2)
        return first1;
    if (n == 2) {
        if (cmp(*--last2, *first1)) {
            auto last_node = last2.node_;
            unlink_nodes(last_node, last_node);
            link_nodes(first1.node_, last_node, last_node);
            return last2;
        }
        return first1;
    }
    auto n2 = n / 2;
    auto last1 = first1;
    MySTL::advance(last1, n2);
    auto result = first1 = list_sort(first1, last1, n2, cmp);  // 前半段
    auto first2 = last1 = list_sort(last1, last2, n - n2, cmp);

    if (cmp(*first2, *first1)) {
        auto m = first2;
        ++m;
        for (; m != last2 && cmp(*m, *first1); ++m)
            ;
        auto f = first2.node_;
        auto l = m.node_->prev;
        result = first2;
        last1 = first2 = m;
        unlink_nodes(f, l);
        m = first1;
        ++m;
        link_nodes(first1.node_, f, l);
        first1 = m;
    } else {
        ++first1;
    }

    // 合并两段有序区间
    while (first1 != last1 && first2 != last2) {
        if (cmp(*first2, *first1)) {
            auto m = first2;
            ++m;
            for (; m != last2 && cmp(*m, *first1); ++m) {
            }
            auto f = first2.node_;
            auto l = m.node_->prev;
            if (last1 == first2)
                last1 = m;
            first2 = m;
            unlink_nodes(f, l);
            m = first1;
            ++m;
            link_nodes(first1.node_, f, l);
            first1 = m;
        } else {
            ++first1;
        }
    }
    return result;
}

/*********************************** 重载比较操作符 ***********************************/

template <class T>
bool operator==(const list<T>& lhs, const list<T>& rhs) {
    auto first1 = lhs.cbegin();
    auto last1 = lhs.cend();
    auto first2 = rhs.cbegin();
    auto last2 = rhs.cend();

    for (; first1 != last1 && first2 != last2 && *first1 == *first2; ++first1, ++first2) {
    }
    return first1 == last1 && first2 == last2;
}

template <class T>
bool operator!=(const list<T>& lhs, const list<T>& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator<(const list<T>& lhs, const list<T>& rhs) {
    return MySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator>(const list<T>& lhs, const list<T>& rhs) {
    return rhs < lhs;
}

template <class T>
bool operator<=(const list<T>& lhs, const list<T>& rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T>& lhs, const list<T>& rhs) {
    !(lhs < rhs);
}

// 重载swap
template <class T>
void swap(list<T>&lhs, list<T>&rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace MySTL

#endif /* MYSTL_LIST_H */