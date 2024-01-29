#ifndef MY_SET_H
#define MY_SET_H

// 包含两个模板类，set和multiset
// set : 集合内元素自动排序，键值不允许重复
// multiset : 集合内元素自动排序，键值允许重复

#include "rb_tree.h"

namespace MySTL {

template <class Key, class Compare = MySTL::less<Key>>
class set {
public:
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

private:
    // 简写
    typedef MySTL::rb_tree<value_type, key_compare> base_type;

    base_type tree_;

public:
    // rb_tree 的型别定义
    typedef typename base_type::node_type              node_type;
    typedef typename base_type::pointer                pointer;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::allocator_type         allocator_type;

public:
    /*********************************** 构造，拷贝，移动，析构 ***********************************/

    // 构造
    set() = default;

    template <class InputIterator>
    set(InputIterator first, InputIterator last) : tree_() { tree_.insert_unique(first, last); }
    set(std::initializer_list<value_type> ilist) : tree_() { tree_.insert_unique(ilist.begin(), ilist.end()); }

    // 拷贝
    set(const set& rhs) : tree_(rhs.tree_) {}

    // 移动
    set(set&& rhs) noexcept : tree_(MySTL::move(rhs.tree_)) {}

    // 拷贝赋值
    set& operator=(const set& rhs) {
        tree_ = rhs.tree_;
        return *this;
    }

    // 移动赋值
    set& operator=(set&& rhs) {
        tree_ = MySTL::move(rhs.tree_);
        return *this;
    }

    // 初始化列表赋值
    set& operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_unique(ilist.begin(), ilist.end());
        return *this;
    }

    // 析构
    ~set() { clear(); }

    // 返回接口信息
    key_compare    key_comp() const { return tree_.key_comp(); }
    value_compare  value_comp() const { return tree_.key_comp(); }
    allocator_type get_allocator() const { return tree_.get_allocator(); }

    /*********************************** 迭代器操作 ***********************************/

    iterator               begin() noexcept { return tree_.begin(); }
    const_iterator         begin() const noexcept { return tree_.begin(); }
    iterator               end() noexcept { return tree_.end(); }
    const_iterator         end() const noexcept { return tree_.end(); }

    reverse_iterator       rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator       rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_iterator         cbegin() const noexcept { return begin(); }
    const_iterator         cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    /*********************************** 容器操作 ***********************************/

    // 容量

    bool      empty() const noexcept { return tree_.empty(); }
    size_type size() const noexcept { return tree_.size(); }
    size_type max_size() const noexcept { return tree_.max_size(); }

    // 插入 / 删除

    // empalce

    template <class... Args>
    MySTL::pair<iterator, bool> emplace(Args&&... args) {
        return tree_.emplace_unique(MySTL::forward<Args>(args)...);
    }
    template <class... Args>
    iterator emplace_hint(iterator hint, Args&&... args) {
        return tree_.emplace_unique_use_hint(hint, MySTL::forward<Args>(args)...);
    }

    // insert / only use insert_unique

    MySTL::pair<iterator, bool> insert(const value_type& value) { return tree_.insert_unique(value); }
    MySTL::pair<iterator, bool> insert(value_type&& value) { return tree_.insert_unique(MySTL::move(value)); }

    iterator insert(iterator hint, const value_type& value) { return tree_.insert_unique(hint, value); }
    iterator insert(iterator hint, value_type&& value) { return tree_.insert_unique(hint, MySTL::move(value)); }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) { tree_.insert_unique(first, last); }

    // erase / clear

    void      erase(iterator pos) { tree_.erase(pos); }
    void      erase(iterator first, iterator last) { tree_.erase(first, last); }
    size_type erase(const key_type& key) { return tree_.erase_unique(key); }

    void clear() { tree_.clear(); }

    /*********************************** 算法 ***********************************/

    iterator       find(const key_type& key) { return tree_.find(key); }
    const_iterator find(const key_type& key) const { return tree_.find(key); }

    size_type count(const key_type& key) const { return tree_.count_unique(key); }

    iterator       lower_bound(const key_type& key) { return tree_.lower_bound(key); }
    const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

    iterator       upper_bound(const key_type& key) { return tree_.upper_bound(key); }
    const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

    MySTL::pair<iterator, iterator>
    equal_range(const key_type& key) { return tree_.equal_range_unique(key); }
    MySTL::pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const { return tree_.equal_range_unique(key); }

    void swap(set& rhs) noexcept { tree_.swap(rhs.tree_); }

public:
    friend bool operator==(const set& lhs, const set& rhs) { return lhs.tree_ == lhs.tree_; }
    friend bool operator<(const set& lhs, const set& rhs) { return lhs.tree_ < rhs.tree_; }
};

// 重载比较操作符
template <class Key, class Compare>
bool operator==(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return lhs == rhs; }

template <class Key, class Compare>
bool operator<(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return lhs < rhs; }

template <class Key, class Compare>
bool operator!=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return !(lhs == rhs); }

template <class Key, class Compare>
bool operator>(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return rhs < lhs; }

template <class Key, class Compare>
bool operator<=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return !(rhs < lhs); }

template <class Key, class Compare>
bool operator>=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs) { return !(lhs < rhs); }

// 模板类 multiset
// Compare 指定权重比较方式，默认使用less
template <class Key, class Compare = MySTL::less<Key>>
class multiset {
public:
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;

private:
    // 简写
    typedef MySTL::rb_tree<value_type, key_compare> base_type;

    base_type tree_;

public:
    // rb_tree 的型别定义
    typedef typename base_type::node_type              node_type;
    typedef typename base_type::pointer                pointer;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::reference              reference;
    typedef typename base_type::const_reference        const_reference;
    typedef typename base_type::iterator               iterator;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::reverse_iterator       reverse_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::difference_type        difference_type;
    typedef typename base_type::allocator_type         allocator_type;

public:
    /*********************************** 构造，拷贝，移动，析构 ***********************************/

    // 构造
    multiset() = default;

    template <class InputIterator>
    multiset(InputIterator first, InputIterator last) : tree_() { tree_.insert_multi(first, last); }
    multiset(std::initializer_list<value_type> ilist) : tree_() { tree_.insert_multi(ilist.begin(), ilist.end()); }

    // 拷贝
    multiset(const multiset& rhs) : tree_(rhs.tree_) {}

    // 移动
    multiset(multiset&& rhs) noexcept : tree_(MySTL::move(rhs.tree_)) {}

    // 拷贝赋值
    multiset& operator=(const multiset& rhs) {
        tree_ = rhs.tree_;
        return *this;
    }

    // 移动赋值
    multiset& operator=(multiset&& rhs) {
        tree_ = MySTL::move(rhs.tree_);
        return *this;
    }

    // 初始化列表赋值
    multiset& operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_multi(ilist.begin(), ilist.end());
        return *this;
    }

    // 析构
    ~multiset() { clear(); }

    key_compare    key_comp() const { return tree_.key_comp(); }
    value_compare  value_comp() const { return tree_.key_comp(); }
    allocator_type get_allocator() const { return tree_.get_allocator(); }

    /*********************************** 迭代器操作 ***********************************/

    iterator       begin() noexcept { return tree_.begin(); }
    const_iterator begin() const noexcept { return tree_.begin(); }
    iterator       end() noexcept { return tree_.end(); }
    const_iterator end() const noexcept { return tree_.end(); }

    reverse_iterator       rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator       rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_iterator         cbegin() const noexcept { return begin(); }
    const_iterator         cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    /*********************************** 容器操作 ***********************************/

    // 容量

    bool      empty() const noexcept { return tree_.empty(); }
    size_type size() const noexcept { return tree_.size(); }
    size_type max_size() const noexcept { return tree_.max_size(); }

    // 插入 / 删除

    // empalce

    template <class... Args>
    iterator emplace(Args&&... args) {
        return tree_.emplace_multi(MySTL::forward<Args>(args)...);
    }
    template <class... Args>
    iterator emplace_hint(iterator hint, Args&&... args) {
        return tree_.emplace_multi_use_hint(hint, MySTL::forward<Args>(args)...);
    }

    // insert / only use insert_multi

    iterator insert(const value_type& value) { return tree_.insert_multi(value); }
    iterator insert(value_type&& value) { return tree_.insert_multi(MySTL::move(value)); }

    iterator insert(iterator hint, const value_type& value) { return tree_.insert_multi(hint, value); }
    iterator insert(iterator hint, value_type&& value) { return tree_.insert_multi(hint, MySTL::move(value)); }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) { tree_.insert_multi(first, last); }

    // erase / clear

    void      erase(iterator pos) { tree_.erase(pos); }
    void      erase(iterator first, iterator last) { tree_.erase(first, last); }
    size_type erase(const key_type& key) { return tree_.erase_multi(key); }

    void clear() { tree_.clear(); }

    /*********************************** 算法 ***********************************/

    iterator       find(const key_type& key) { return tree_.find(key); }
    const_iterator find(const key_type& key) const { return tree_.find(key); }

    size_type count(const key_type& key) const { return tree_.count_multi(key); }

    iterator       lower_bound(const key_type& key) { return tree_.lower_bound(key); }
    const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

    iterator       upper_bound(const key_type& key) { return tree_.upper_bound(key); }
    const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

    MySTL::pair<iterator, iterator>
    equal_range(const key_type& key) { return tree_.equal_range_multi(key); }
    MySTL::pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const { return tree_.equal_range_multi(key); }

    void swap(multiset& rhs) noexcept { tree_.swap(rhs.tree_); }

public:
    friend bool operator==(const multiset& lhs, const multiset& rhs) { return lhs.tree_ == lhs.tree_; }
    friend bool operator<(const multiset& lhs, const multiset& rhs) { return lhs.tree_ < rhs.tree_; }
};

// 重载比较操作符
template <class Key, class Compare>
bool operator==(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return lhs == rhs; }

template <class Key, class Compare>
bool operator<(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return lhs < rhs; }

template <class Key, class Compare>
bool operator!=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return !(lhs == rhs); }

template <class Key, class Compare>
bool operator>(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return rhs < lhs; }

template <class Key, class Compare>
bool operator<=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return !(rhs < lhs); }

template <class Key, class Compare>
bool operator>=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs) { return !(lhs < rhs); }

}  // namespace MySTL

#endif  // MYSTL_SET_H
        // https://github.com/chen6556/DSV