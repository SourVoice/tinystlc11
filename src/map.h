#ifndef MY_MAP_H
#define MY_MAP_H

// 包含两个模板类
// map      : 映射，元素具有键值和实值，会根据键值大小自动排序，键值不允许重复
// multimap : 映射，元素具有键值和实值，会根据键值大小自动排序，键值允许重复

#include "rb_tree.h"

namespace MySTL {

/// @brief map 的实现
/// @tparam Key     键值型别
/// @tparam T   实值型别
/// @tparam Compare     比较函数型别
template <class Key, class T, class Compare = MySTL::less<T>>
class map {
public:
    typedef Key                       key_type;
    typedef T                         mapped_type;
    typedef MySTL::pair<const Key, T> value_type;
    typedef Compare                   key_compare;

    class value_compare : public binary_function<value_type, value_type, bool> {
        friend class map<Key, T, Compare>;  // 友元类，使得 map 可以访问 value_compare 的两个成员函数
    private:
        Compare comp;
        value_compare(Compare cmp) :
            comp(cmp) {}

    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const { return comp(lhs.first, rhs.first); }
    };

private:
    typedef MySTL::rb_tree<value_type, key_compare> base_type;
    base_type tree_;  // 设计模式：组合

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
    map() = default;

    template <class InputIterator>
    map(InputIterator first, InputIterator last) : tree_() { tree_.insert_unique(first, last); }

    map(std::initializer_list<value_type> ilist) : tree_() { tree_.insert_unique(ilist.begin(), ilist.end()); }

    // 拷贝
    map(const map& rhs) : tree_(rhs.tree_) {}

    // 移动
    map(map&& rhs) noexcept : tree_(MySTL::move(rhs.tree_)) {}

    // 拷贝赋值
    map& operator=(const map& rhs) {
        tree_ = rhs.tree_; // 不用检查 this 和 rhs 是否相同，tree_ 的拷贝赋值中已经完成了这部分
        return *this;
    }

    // 移动赋值
    map& operator=(map&& rhs) noexcept {
        tree_ = MySTL::move(rhs.tree_);
        return *this;
    }

    // 初始化列表赋值
    map& operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_unique(ilist.begin(), ilist.end());
        return *this;
    }

    // 析构
    ~map() { clear(); }

    // 返回内部信息
    key_compare    key_comp() const noexcept { return tree_.key_comp(); }
    value_compare  value_comp() const noexcept { return value_compare(tree_.key_comp()); }
    allocator_type get_allocator() const noexcept { return tree_.get_allocator(); }

    /*********************************** 迭代器操作 ***********************************/

    iterator               begin() noexcept         { return tree_.begin(); }
    const_iterator         begin() const noexcept   { return tree_.begin(); }
    iterator               end() noexcept           { return tree_.end(); }
    const_iterator         end() const noexcept     { return tree_.end(); }

    reverse_iterator       rbegin() noexcept        { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept  { return const_reverse_iterator(end()); }
    reverse_iterator       rend() noexcept          { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept    { return const_reverse_iterator(begin()); }

    const_iterator         cbegin() const noexcept  { return begin(); }
    const_iterator         cend() const noexcept    { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept   { return rend(); }

    /*********************************** 元素访问 ***********************************/

    mapped_type& at(const key_type& key) {
        iterator it = lower_bound(key);
        THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key), "map<Key, T> has no such element");
        return it->second;
    }
    mapped_type& at(const key_type& key) const {
        const_iterator it = lower_bound(key);
        THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key), "map<Key, T> has no such element");
        return it->second;
    }

    mapped_type& operator[](const key_type& key) {
        iterator it = lower_bound(key); // >= key
        if (it == end() || key_comp()(key, it->first))
            it = emplace_hint(it, key, T{});
        return it->second;
    }
    mapped_type& operator[](key_type&& key) {
        const_iterator it = lower_bound(key);
        if (it == end() || key_comp()(key, it->first))
            it = emplace_hint(it, MySTL::move(key), T{});
        return it->second;
    }

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

    void swap(map& rhs) noexcept { tree_.swap(rhs.tree_); }

public:
    friend bool operator==(const map& lhs, const map& rhs) { return lhs.tree_ == rhs.tree_; }
    friend bool operator<(const map& lhs, const map& rhs) { return lhs.tree_ < rhs.tree_; }
};

// 重载比较操作符
template <class Key, class T, class Compare>
bool operator==(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return lhs == rhs; }

template <class Key, class T, class Compare>
bool operator<(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return lhs < rhs; }

template <class Key, class T, class Compare>
bool operator!=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return !(lhs == rhs); }

template <class Key, class T, class Compare>
bool operator>(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return rhs < lhs; }

template <class Key, class T, class Compare>
bool operator<=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return !(rhs < lhs); }

template <class Key, class T, class Compare>
bool operator>=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs) { return !(lhs < rhs); }


/// @brief multimap 的模板类，键值允许重复
/// @tparam Key 
/// @tparam T 
/// @tparam Compare 
template <class Key, class T, class Compare = MySTL::less<T>>
class multimap {
public:
    typedef Key                       key_type;
    typedef T                         mapped_type;
    typedef MySTL::pair<const Key, T> value_type;
    typedef Compare                   key_compare;

    class value_compare : public binary_function<value_type, value_type, bool> {
        friend class multimap<Key, T, Compare>;  // 友元类，使得 multimap 可以访问 value_compare 的两个成员函数
    private:
        Compare comp;
        value_compare(Compare cmp) :
            comp(cmp) {}

    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const { return comp(lhs.first, rhs.first); }
    };

private:
    typedef MySTL::rb_tree<value_type, key_compare> base_type;
    base_type tree_;  // 设计模式：组合

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
    multimap() = default;

    template <class InputIterator>
    multimap(InputIterator first, InputIterator last) : tree_() { tree_.insert_multi(first, last); }

    multimap(std::initializer_list<value_type> ilist) : tree_() { tree_.insert_multi(ilist.begin(), ilist.end()); }

    // 拷贝
    multimap(const multimap& rhs) : tree_(rhs.tree_) {}

    // 移动
    multimap(multimap&& rhs) noexcept : tree_(MySTL::move(rhs.tree_)) {}

    // 拷贝赋值
    multimap& operator=(const multimap& rhs) {
        tree_ = rhs.tree_; // 不用检查 this 和 rhs 是否相同，tree_ 的拷贝赋值中已经完成了这部分
        return *this;
    }

    // 移动赋值
    multimap& operator=(multimap&& rhs) noexcept {
        tree_ = MySTL::move(rhs.tree_);
        return *this;
    }

    // 初始化列表赋值
    multimap& operator=(std::initializer_list<value_type> ilist) {
        tree_.clear();
        tree_.insert_multi(ilist.begin(), ilist.end());
        return *this;
    }

    // 析构
    ~multimap() { clear(); }

    // 返回内部信息
    key_compare    key_comp() const noexcept { return tree_.key_comp(); }
    value_compare  value_comp() const noexcept { return value_compare(tree_.key_comp()); }
    allocator_type get_allocator() const noexcept { return tree_.get_allocator(); }

    /*********************************** 迭代器操作 ***********************************/

    iterator               begin() noexcept         { return tree_.begin(); }
    const_iterator         begin() const noexcept   { return tree_.begin(); }
    iterator               end() noexcept           { return tree_.end(); }
    const_iterator         end() const noexcept     { return tree_.end(); }

    reverse_iterator       rbegin() noexcept        { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept  { return const_reverse_iterator(end()); }
    reverse_iterator       rend() noexcept          { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept    { return const_reverse_iterator(begin()); }

    const_iterator         cbegin() const noexcept  { return begin(); }
    const_iterator         cend() const noexcept    { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept   { return rend(); }

    /*********************************** 元素访问 ***********************************/

    mapped_type& at(const key_type& key) {
        iterator it = lower_bound(key);
        THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key), "multimap<Key, T> has no such element");
        return it->second;
    }
    mapped_type& at(const key_type& key) const {
        const_iterator it = lower_bound(key);
        THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key), "multimap<Key, T> has no such element");
        return it->second;
    }

    mapped_type& operator[](const key_type& key) {
        iterator it = lower_bound(key); // >= key
        if (it == end() || key_comp()(key, it->first))
            it = emplace_hint(it, key, T{});
        return it->second;
    }
    mapped_type& operator[](key_type&& key) {
        const_iterator it = lower_bound(key);
        if (it == end() || key_comp()(key, it->first))
            it = emplace_hint(it, MySTL::move(key), T{});
        return it->second;
    }

    /*********************************** 容器操作 ***********************************/

    // 容量

    bool      empty() const noexcept { return tree_.empty(); }
    size_type size() const noexcept { return tree_.size(); }
    size_type max_size() const noexcept { return tree_.max_size(); }

    // 插入 / 删除

    // empalce

    template <class... Args>
    MySTL::pair<iterator, bool> emplace(Args&&... args) {
        return tree_.emplace_multi(MySTL::forward<Args>(args)...);
    }
    template <class... Args>
    iterator emplace_hint(iterator hint, Args&&... args) {
        return tree_.emplace_multi_use_hint(hint, MySTL::forward<Args>(args)...);
    }

    // insert / only use insert_multi

    MySTL::pair<iterator, bool> insert(const value_type& value) { return tree_.insert_multi(value); }
    MySTL::pair<iterator, bool> insert(value_type&& value) { return tree_.insert_multi(MySTL::move(value)); }

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

    void swap(multimap& rhs) noexcept { tree_.swap(rhs.tree_); }

public:
    friend bool operator==(const multimap& lhs, const multimap& rhs) { return lhs.tree_ == rhs.tree_; }
    friend bool operator<(const multimap& lhs, const multimap& rhs) { return lhs.tree_ < rhs.tree_; }
};

// 重载比较操作符
template <class Key, class T, class Compare>
bool operator==(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return lhs == rhs; }

template <class Key, class T, class Compare>
bool operator<(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return lhs < rhs; }

template <class Key, class T, class Compare>
bool operator!=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return !(lhs == rhs); }

template <class Key, class T, class Compare>
bool operator>(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return rhs < lhs; }

template <class Key, class T, class Compare>
bool operator<=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return !(rhs < lhs); }

template <class Key, class T, class Compare>
bool operator>=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs) { return !(lhs < rhs); }
} /* namespace MySTL */

#endif
