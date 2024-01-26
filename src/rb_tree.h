#ifndef MY_RBTREE_H
#define MY_RBTREE_H
// 模板类 rb_tree，红黑树

#include <initializer_list>
#include <cassert>

#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "exceptdef.h"

namespace MySTL {

// 红黑树节点颜色
typedef bool                        rb_tree_color_type;
static constexpr rb_tree_color_type rb_tree_red = false;
static constexpr rb_tree_color_type rb_tree_black = true;

// 前置声明
template <class T> struct rb_tree_node_base;
template <class T> struct rb_tree_node;
template <class T> struct rb_tree_iterator;
template <class T> struct rb_tree_const_iterator;

// 红黑树节点值类型特征
template <class T, bool>
struct rb_tree_value_traits_imp {
    typedef T value_type;
    typedef T mapped_type;
    typedef T key_type;

    template <class Ty>
    static const key_type& get_key(const Ty& value) { return value; }

    template <class Ty>
    static const value_type& get_value(const Ty& value) { return value; }
};

// 特化
template <class T>
struct rb_tree_value_traits_imp<T, true> {
    typedef typename std::remove_cv<typename T::first_type>::type key_type;
    typedef typename T::second_type                               mapped_type;
    typedef T                                                     value_type;

    template <class Ty>
    static const key_type& get_key(const Ty& value) { return value.first; }

    template <class Ty>
    static const value_type& get_value(const Ty& value) { return value; }
};

template <class T>
struct rb_tree_value_traits {
    static constexpr bool is_map = MySTL::is_pair<T>::value; 
    typedef rb_tree_value_traits_imp<T, is_map> value_type_traits;

    typedef typename value_type_traits::key_type    key_type;
    typedef typename value_type_traits::mapped_type mapped_type;
    typedef typename value_type_traits::value_type  value_type;

    template <class Ty>
    static const key_type& get_key(const Ty& value) {
        return value_type_traits::get_key(value);
    }

    template <class Ty>
    static const value_type& get_value(const Ty& value) {
        return value_type_traits::get_value(value);
    }
};

/*********************************** 红黑树节点类型萃取器 ***********************************/
template <class T>
struct rb_tree_node_traits {
    typedef rb_tree_color_type                 color_type;
    typedef rb_tree_value_traits<T>            value_traits;

    typedef typename value_traits::key_type    key_type;
    typedef typename value_traits::value_type  value_type;
    typedef typename value_traits::mapped_type mapped_type;

    typedef rb_tree_node_base<T>*              base_ptr;
    typedef rb_tree_node<T>*                   node_ptr;
};

/*********************************** 红黑树节点设计 ***********************************/
template <class T>
struct rb_tree_node_base {
    typedef rb_tree_node_base<T>* base_ptr;
    typedef rb_tree_node<T>*      node_ptr;
    typedef rb_tree_color_type    color_type;

    base_ptr   parent;
    base_ptr   left;
    base_ptr   right;
    color_type color;  // 节点颜色

    base_ptr  get_base_ptr() { return &*this; }
    node_ptr  get_node_ptr() { return reinterpret_cast<node_ptr>(&*this); }
    node_ptr& get_node_ref() { return reinterpret_cast<node_ptr&>(*this); }
};

template <class T>
struct rb_tree_node : public rb_tree_node_base<T> {

    typedef rb_tree_node_base<T>* base_ptr;
    typedef rb_tree_node<T>*      node_ptr;

    T value;

    base_ptr get_base_ptr() { return static_cast<base_ptr>(&*this); }
    node_ptr get_node_ptr() { return &*this; }
};

/*********************************** 红黑树萃取器 ***********************************/
template <class T>
struct rb_tree_traits {
    typedef rb_tree_value_traits<T>            value_traits;
    typedef typename value_traits::key_type    key_type;
    typedef typename value_traits::mapped_type mapped_type;
    typedef typename value_traits::value_type  value_type;
    typedef value_type*                        pointer;
    typedef value_type&                        reference;
    typedef const value_type*                  const_pointer;
    typedef const value_type&                  cosnt_reference;
    typedef rb_tree_node_base<T>               base_type;
    typedef rb_tree_node<T>                    node_type;
    typedef base_type*                         base_ptr;
    typedef node_type*                         node_ptr;
};

/*********************************** 红黑树迭代器设计 ***********************************/
template <class T>
struct rb_tree_iterator_base : public MySTL::iterator<MySTL::bidirectional_iterator_tag, T> {
    typedef typename rb_tree_traits<T>::base_ptr base_ptr;

    base_ptr node;

    rb_tree_iterator_base() noexcept :        node(nullptr) {}

    void inc() {
        if (node->right != nullptr) {  // 找到右子树中的最小节点
            node = rb_tree_min(node->right);
        } else {  // node没有右子节点，分为node是左子节点和node是右子节点两种情况
                  // 1. node是右子节点，向上寻找，直到node不是右子节点
                  // 2. node是左子节点，直接返回
            base_ptr y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
    }
    void dec() {
        if (rb_tree_is_red(node) && node->parent->parent == node) // node为header时，指向最大节点
            node = node->right;
        else if (node->left != nullptr) {
            node = rb_tree_max(node->left);
        } else { // 非 header 节点，无左子节点
            base_ptr y = node->parent;
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
    bool operator==(const rb_tree_iterator_base& rhs) const { return node == rhs.node; }
    bool operator!=(const rb_tree_iterator_base& rhs) const { return node != rhs.node; }
};

template <class T>
struct rb_tree_iterator : public rb_tree_iterator_base<T> {
    typedef rb_tree_traits<T> tree_traits;

    typedef typename tree_traits::value_type value_type;
    typedef typename tree_traits::pointer    pointer;
    typedef typename tree_traits::reference  reference;
    typedef typename tree_traits::base_ptr   base_ptr;
    typedef typename tree_traits::node_ptr   node_ptr;

    typedef rb_tree_iterator<T>       iterator;
    typedef rb_tree_const_iterator<T> const_iterator;
    typedef iterator                  self;

    using rb_tree_iterator_base<T>::node;

    // 构造函数
    rb_tree_iterator() {}
    rb_tree_iterator(base_ptr x) { node = x; }
    rb_tree_iterator(node_ptr x) { node = x; }
    rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
    rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

    // 重载操作符
    reference operator*() const { return node->get_node_ptr()->value; }
    pointer   operator->() const { return &(operator*()); }

    self& operator++() {
        this->inc();
        return *this;
    }
    self operator++(int) {
        self tmp(*this);
        this->inc();
        return tmp;
    }
    self& operator--() {
        this->dec();
        return *this;
    }
    self operator--(int) {
        self tmp(*this);
        this->dec();
        return tmp;
    }
};

template <class T>
struct rb_tree_const_iterator: public rb_tree_iterator_base<T> {
    typedef rb_tree_traits<T> tree_traits;

    typedef typename tree_traits::value_type value_type;
    typedef typename tree_traits::pointer    pointer;
    typedef typename tree_traits::reference  reference;
    typedef typename tree_traits::base_ptr   base_ptr;
    typedef typename tree_traits::node_ptr   node_ptr;

    typedef rb_tree_iterator<T>       iterator;
    typedef rb_tree_const_iterator<T> const_iterator;
    typedef iterator                  self;

    using rb_tree_iterator_base<T>::node;

    // 构造函数
    rb_tree_const_iterator() {}
    rb_tree_const_iterator(base_ptr x) { node = x; }
    rb_tree_const_iterator(node_ptr x) { node = x; }
    rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; }
    rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; }

    // 重载操作符
    reference operator*() const { return node->get_node_ptr()->value; }
    pointer   operator->() const { return &(operator*()); }

    self& operator++() {
        this->inc();
        return *this;
    }
    self operator++(int) {
        self tmp(*this);
        this->inc();
        return tmp;
    }
    self& operator--() {
        this->dec();
        return *this;
    }
    self operator--(int) {
        self tmp(*this);
        this->dec();
        return tmp;
    }
};

/*********************************** 红黑树相关算法 ***********************************/
/**
 * 旋转，元素调整等
 */
template <class Nodeptr>
Nodeptr rb_tree_min(Nodeptr x) noexcept {
    while(x -> left != nullptr) 
        x = x->left;
    return x;
}

template <class Nodeptr>
Nodeptr rb_tree_max(Nodeptr x) noexcept {
    while (x->right != nullptr)
        x = x->right;
    return x;
}

template <class Nodeptr>
bool rb_tree_is_lchild(Nodeptr node) noexcept { return node == node->parent->left; }

template <class Nodeptr>
bool rb_tree_is_red(Nodeptr node) noexcept { return node->color == rb_tree_red; }

template <class Nodeptr>
bool rb_tree_is_black(Nodeptr node) noexcept { return node->color == rb_tree_black; }

template <class Nodeptr>
void rb_tree_set_red(Nodeptr& node) noexcept { node->color = rb_tree_red; }

template <class Nodeptr>
void rb_tree_set_black(Nodeptr& node) noexcept { node->color = rb_tree_black; }

template <class Nodeptr>
Nodeptr rb_tree_next(Nodeptr node) noexcept {
    /*
    * If we have a right-hand child, go down and then left as far
    * as we can.
     */
    if (node->right != nullptr)
        return rb_tree_min(node->right);
    /*
    * No right-hand children. Everything down and left is smaller than us,
    * so any 'next' node must be in the general direction of our parent.
    * Go up the tree; any time the ancestor is a right-hand child of its
    * parent, keep going up. First time it's a left-hand child of its
    * parent, said parent is our 'next' node.
     */
    while (!rb_tree_is_lchild(node))
        node = node->parent;
    return node->parent;
}

// 左旋，参数一为左旋节点，参数二为根节点
template <class Nodeptr>
void rb_tree_rotate_left(Nodeptr x, Nodeptr& root) noexcept {
    auto y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root) // x是根节点，y取代x
        root = y;
    else if (rb_tree_is_lchild(x))
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// 右旋，参数一为右旋节点，参数二为根节点
template <class Nodeptr>
void rb_tree_rotate_right(Nodeptr x, Nodeptr& root) noexcept {
    auto y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root) {
        root = y;
    } else if (rb_tree_is_lchild(x)) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

/** 
 * @brief
 * @param x 
 * @note 
   如果插入的是根结点
 ，由于原树是空树，此情况只会违反性质2 
 ，因此直接把此结点涂为黑色；如果插入的结点的父结点是黑色
 ，由于此不会违反性质2和性质4，红黑树没有被破坏
 ，所以此时什么也不做。

其他情况都为当前父节点为红，
uncle = 祖父节点的另一个子节点
case 1：当前节点父节点为红，且uncle为红
 */
template <class Nodeptr>
void rb_tree_insert_rebalance(Nodeptr x, Nodeptr& root) noexcept {
    rb_tree_set_red(x);
    while (x != root && rb_tree_is_red(x->parent)) {
        if (rb_tree_is_lchild(x->parent)) { // case 1, case2, case3 所有附条件均为当前节点是其父节点的左子节点
            auto uncle = x->parent->parent->right;
            if (uncle != nullptr && rb_tree_is_red(uncle)) {
                // ● 插入修复情况1：叔叔节点是红色，当前节点是其父节点的左子节点
                rb_tree_set_black(x->parent);
                rb_tree_set_black(uncle);
                x = x->parent->parent;
                rb_tree_set_red(x);
            } else {
                if (!rb_tree_is_lchild(x)) {
                    // ● 插入修复情况2：叔叔节点是黑色，当前节点是其父节点的右子节点
                    x = x->parent;
                    rb_tree_rotate_left(x, root);
                }
                // ● 插入修复情况3：叔叔节点是黑色，当前节点是其父节点的左子节点
                rb_tree_set_black(x->parent);
                rb_tree_set_red(x->parent->parent);
                rb_tree_rotate_right(x->parent->parent, root);
                break;
            }
        } else { // 父节点是右子节点对称处理
            auto uncle = x->parent->parent->left;
            if (uncle != nullptr && rb_tree_is_red(uncle)) {
                // case 1：
                rb_tree_set_black(x->parent);
                rb_tree_set_black(uncle);
                x = x->parent->parent;
                rb_tree_set_red(x);
            } else { // uncle为黑
                if (rb_tree_is_lchild(x)) {
                    // case2： 
                    x = x->parent;
                    rb_tree_rotate_right(x, root);
                }
                // case 3：
                rb_tree_set_black(x->parent);
                rb_tree_set_red(x->parent->parent);
                rb_tree_rotate_left(x->parent->parent, root);
                break;
            }
        }
    }
    rb_tree_set_black(root);
}

// 删除节点后使 rb tree 重新平衡，参数一为要删除的节点，参数二为根节点，参数三为最小节点，参数四为最大节点
/**
 * @param z 被删除的节点
 * @param root 根节点
 * @param leftmost 最小节点
 * @param rightmost 最大节点
 */
// FIGUREOUT: erase与insert reblance的异同
template <class Nodeptr>
Nodeptr rb_tree_erase_reblance(Nodeptr z, Nodeptr& root, Nodeptr& leftmost, Nodeptr& rightmost) {
    auto    y = (z->left || z->right == nullptr) ? z : rb_tree_next(z);
    auto    x = y->left != nullptr ? y->left : y->right;
    Nodeptr xp = nullptr;  // parent of x

    if (y != z) {
        z->left->parent = y;
        y->left = z->left;
        if (y != z->right) {
            xp = y->parent;
            if (x != nullptr) {
                x->parent = y->parent;
            }
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        } else {
            xp = y;
        }

        // 连接 y 与 z的父节点
        if (root == z)
            root = y;
        else if (rb_tree_is_lchild(z))
            z->parent->left = y;
        else
            z->parent->left = y;
        y->parent = z->parent;
        MySTL::swap(y->color, z->color);
        y = z;
    } else {  //  z == y 说明 z 至多只有一个孩子
        xp = y->parent;
        if (x)
            x->parent = y->parent;
        if (root == z)
            root = x;
        else if (rb_tree_is_lchild(z))
            z->parent->left = x;
        else
            z->parent->right = x;

        if (leftmost == z)
            leftmost = x == nullptr ? xp : rb_tree_min(x);
        else if (rightmost == z)
            rightmost = x == nullptr ? xp : rb_tree_max(x);
    }

    if (!rb_tree_is_red(y))
    {  // x 为黑色时，调整，否则直接将 x 变为黑色即可
        while (x != root && (x == nullptr || !rb_tree_is_red(x)))
        {
            if (x == xp->left)
            {  // 如果 x 为左子节点
                auto brother = xp->right;
                if (rb_tree_is_red(brother))
                {  // case 1
                    rb_tree_set_black(brother);
                    rb_tree_set_red(xp);
                    rb_tree_rotate_left(xp, root);
                    brother = xp->right;
                }
                // case 1 转为为了 case 2、3、4 中的一种
                if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                    (brother->right == nullptr || !rb_tree_is_red(brother->right)))
                {  // case 2
                    rb_tree_set_red(brother);
                    x = xp;
                    xp = xp->parent;
                } else
                {
                    if (brother->right == nullptr || !rb_tree_is_red(brother->right))
                    {  // case 3
                        if (brother->left != nullptr)
                            rb_tree_set_black(brother->left);
                        rb_tree_set_red(brother);
                        rb_tree_rotate_right(brother, root);
                        brother = xp->right;
                    }
                    // 转为 case 4
                    brother->color = xp->color;
                    rb_tree_set_black(xp);
                    if (brother->right != nullptr)
                        rb_tree_set_black(brother->right);
                    rb_tree_rotate_left(xp, root);
                    break;
                }
            } else  // x 为右子节点，对称处理
            {
                auto brother = xp->left;
                if (rb_tree_is_red(brother))
                {  // case 1
                    rb_tree_set_black(brother);
                    rb_tree_set_red(xp);
                    rb_tree_rotate_right(xp, root);
                    brother = xp->left;
                }
                if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                    (brother->right == nullptr || !rb_tree_is_red(brother->right)))
                {  // case 2
                    rb_tree_set_red(brother);
                    x = xp;
                    xp = xp->parent;
                } else
                {
                    if (brother->left == nullptr || !rb_tree_is_red(brother->left))
                    {  // case 3
                        if (brother->right != nullptr)
                            rb_tree_set_black(brother->right);
                        rb_tree_set_red(brother);
                        rb_tree_rotate_left(brother, root);
                        brother = xp->left;
                    }
                    // 转为 case 4
                    brother->color = xp->color;
                    rb_tree_set_black(xp);
                    if (brother->left != nullptr)
                        rb_tree_set_black(brother->left);
                    rb_tree_rotate_right(xp, root);
                    break;
                }
            }
        }
        if (x != nullptr)
            rb_tree_set_black(x);
    }
    return y;
}

/*********************************** 红黑树模板类 ***********************************/
// 模板类红黑树，
// 模板参数一为数据类型，参数二为键值比较类型
template <class T, class Compare>
class rb_tree {
public:
    // 简化
    typedef rb_tree_traits<T>                        tree_traits;
    typedef rb_tree_value_traits<T>                  value_traits;
    // 红黑树萃取器
    typedef typename tree_traits::base_type          base_type;
    typedef typename tree_traits::base_ptr           base_ptr;
    typedef typename tree_traits::node_type          node_type;
    typedef typename tree_traits::node_ptr           node_ptr;
    typedef typename tree_traits::key_type           key_type;
    typedef typename tree_traits::mapped_type        mapped_type;
    typedef typename tree_traits::value_type         value_type;

    typedef Compare                                  key_compare;

    typedef MySTL::allocator<T>                      allocator_type;
    typedef MySTL::allocator<T>                      data_allocator;
    typedef MySTL::allocator<base_type>              base_allocator;
    typedef MySTL::allocator<node_type>              node_allocator;
    // 分配器萃取
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    // 迭代器类型
    typedef rb_tree_iterator<T>                      iterator;
    typedef rb_tree_const_iterator<T>                const_iterator;
    typedef MySTL::reverse_iterator<iterator>        reverse_iterator;
    typedef MySTL::reverse_iterator<const_iterator>  const_reverse_iterator;

    allocator_type get_allocator()  const { return node_allocator(); }
    key_compare    key_comp()       const { return key_comp_; }

private:
    base_ptr    header_;
    size_type   node_count_;
    key_compare key_comp_;

private:
    // 获取根节点，最小节点，最大节点
    base_ptr& root() const { return header_->parent; }
    base_ptr& leftmost() const { return header_->left; }
    base_ptr& rightmost() const { return header_->right; }

public:
    /*********************************** 构造，拷贝，移动，析构 ***********************************/

    rb_tree() { rb_tree_init(); }

    // 拷贝
    rb_tree(const rb_tree& rhs) noexcept {
        rb_tree_init();
        if (rhs.node_count_ != 0) {
            root() = copy_from(rhs.root(), header_);
            leftmost() = rb_tree_min(root());
            rightmost() = rb_tree_max(root());
        }
        node_count_ = rhs.node_count_;
        key_comp_ = rhs.key_comp_;
    }

    // 移动，
    rb_tree(rb_tree&& rhs) :
        header_(MySTL::move(rhs.header_)), node_count_(rhs.node_count_), key_comp_(rhs.key_comp_) {
        rhs.reset();
    }

    rb_tree& operator=(const rb_tree& rhs) {
        if (this == &rhs) return *this;

        clear();
        if (rhs.node_count_ != 0) {
            root() = copy_from(rhs.root(), header_);
            leftmost() = rb_tree_min(root());
            rightmost() = rb_tree_max(root());
        }
        node_count_ = rhs.node_count_;
        key_comp_ = rhs.key_comp_;
        return *this;
    };

    rb_tree& operator=(rb_tree&& rhs) noexcept {
        if (this == &rhs) return *this;

        clear();
        header_ = MySTL::move(rhs.header_);
        node_count_ = rhs.node_count_;
        key_comp_ = rhs.key_comp_;
        rhs.reset();
        return *this;
    };

    ~rb_tree() { clear(); }

public:
    /*********************************** 迭代器操作 ***********************************/

    iterator               begin() noexcept { return leftmost(); }
    const_iterator         begin() const noexcept { return leftmost(); }
    iterator               end() noexcept { return header_; }
    const_iterator         end() const noexcept { return header_; }

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
    bool      empty() const noexcept { return node_count_ == 0; }
    size_type size() const noexcept { return node_count_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    // 插入 / 删除

    // empalce
    template <class... Args>
    iterator                    emplace_multi(Args&&... args);
    template <class... Args>
    MySTL::pair<iterator, bool> emplace_unique(Args&&... args);
    template <class... Args>
    iterator                    emplace_multi_use_hint(iterator hint, Args&&... args);
    template <class... Args>
    iterator                    emplace_unique_use_hint(iterator hint, Args&&... args);

    // insert_multi
    iterator insert_multi(const value_type& value);
    iterator insert_multi(value_type&& value) { return emplace_multi(MySTL::move(value)); }

    iterator insert_multi(iterator hint, const value_type& value) { return emplace_multi_use_hint(hint, value); }
    iterator insert_multi(iterator hint, value_type&& value) { return emplace_multi_use_hint(hint, MySTL::move(value)); }

    template <class InputIterator>
    void insesrt_multi(InputIterator first, InputIterator last) {
        size_type n = MySTL::distance(first, last);
        THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Compare>' s size too big");
        for (; n > 0 ; --n, ++first)
            insert_multi(end(), *first);
    }

    // insert_unique
    MySTL::pair<iterator, bool> insert_unique(const value_type& value);
    MySTL::pair<iterator, bool> insert_unique(value_type&& value) { return emplace_unique(MySTL::move(value)); }

    iterator insert_unique(iterator hint, const value_type& value) { return emplace_unique_use_hint(hint, value); }
    iterator insert_unique(iterator hint, value_type&& value) { return emplace_unique_use_hint(hint, MySTL::move(value)); }

    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) {
        size_type n = MySTL::distance(first, last);
        THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
        for (; n > 0; --n, ++first)
            insert_unique(end(), *first);
    }

    iterator  erase(iterator hint);
    size_type erase_multi(const key_type& key);
    size_type erase_unique(const key_type& key);
    void      erase(iterator first, iterator last);
    void      clear();

    // 相关算法

    iterator       find(const key_type& key);
    const_iterator find(const key_type& key) const;

    size_type      count_multi(const key_type& key) const {
        auto p = equal_range_multi(key);
        return static_cast<size_type>(MySTL::distance(p.first, p.second));
    }

    size_type      count_unique(const key_type& key) const { return find(key) != end() ? 1 : 0; }

    iterator       lower_bound(const key_type& key);
    const_iterator lower_bound(const key_type& key) const;

    iterator       upper_bound(const key_type& key);
    const_iterator upper_bound(const key_type& key) const;

    MySTL::pair<iterator, iterator>
    equal_range_multi(const key_type& key) {
        return MySTL::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
    }
    MySTL::pair<const_iterator, const_iterator>
    equal_range_multi(const key_type& key) const {
        return MySTL::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
    }

    MySTL::pair<iterator, iterator>
    equal_range_unique(const key_type& key) {
        iterator it = find(key);
        auto next = it;
        return it == end() ? MySTL::make_pair(it, it) : MySTL::make_pair(it, ++next);
    }
    MySTL::pair<const_iterator, const_iterator>
    equal_range_unique(const key_type& key) const {
        const_iterator it = find(key);
        auto next = it;
        return it == end() ? MySTL::make_pair(it, it) : MySTL::make_pair(it, ++next);
    }

    void swap(rb_tree& rhs) noexcept;

private:
    /*********************************** helper function ***********************************/
    //
    template <class... Args>
    node_ptr create_node(Args&&... args);
    node_ptr clone_node(base_ptr x);
    void     destory_node(node_ptr p);

    // init
    void rb_tree_init();
    void reset();

    // get insert pos
    MySTL::pair<base_ptr, bool>
    get_insert_multi_pos(const key_type& key);
    MySTL::pair<MySTL::pair<base_ptr, bool>, bool>
    get_insert_unique_pos(const key_type& key);

    // insert value / node
    iterator insert_value_at(base_ptr x, const value_type& value, bool add_to_left);
    iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left);

    // insert use hint
    iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node);
    iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node);

    // copy / erase tree 
    base_ptr copy_from(base_ptr x, base_ptr p);
    void     erase_since(base_ptr x);
};

/*********************************** 容器操作，插入 / 删除 ***********************************/

// empalce
/**
 * @brief 就地插入元素，允许插入具有相同键的节点
 * @param args 
 * @return iterator 插入的位置
 */
template <class T, class Compare>
template <class... Args>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::emplace_multi(Args&&... args) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Compare>'s size too big");
    node_ptr np = create_node(MySTL::forward<Args>(args)...);
    auto res = get_insert_multi_pos(value_traits::get_key(np->value));
    return insert_node_at(res.first, np, res.second);
}

/**
 * @brief 就地插入元素，不允许插入具有相同键的节点
 * @param args 变参模板参数
 * @return pair<iterator, bool> 插入的位置，是否插入成功
 */
template <class T, class Compare>
template <class... Args>
MySTL::pair<typename rb_tree<T, Compare>::iterator, bool>
rb_tree<T, Compare>::emplace_unique(Args&&... args) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Compare>'s size too big");
    node_ptr np = create_node(MySTL::forward<Args>(args)...);
    auto    res = get_insert_unique_pos(value_traits::get_key(np->value));
    if (res.second) {
        auto ret = insert_node_at(res.first.first, np, res.first.second);
        return MySTL::make_pair(ret, true);
    }
    destory_node(np);
    return MySTL::make_pair(iterator(res.first.first), false);
}

/**
 * @brief hint位置与插入位置重复时，采用更优算法  
 * @param hint 插入位置的提示
 */
template <class T, class Compare>
template <class... Args>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::emplace_multi_use_hint(typename rb_tree<T, Compare>::iterator hint, Args&&... args) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
    node_ptr np = create_node(MySTL::forward<Args>(args)...);

    if (node_count_ == 0)
        return insert_node_at(header_, np, true);

    key_type key = value_traits::get_key(np->value);
    if (hint == begin()) {  // 位于 begin 处
        if (key_comp_(key, value_traits::get_key(*hint))) {
            return insert_node_at(hint.node, np, true);
        } else {
            auto pos = get_insert_multi_pos(key);
            return insert_node_at(pos.first, np, pos.second);
        }
    } else if (hint == end()) {  // 位于 end 处
        if (!key_comp_(key, value_traits::get_key(rightmost()->get_node_ptr()->value))) {
            return insert_node_at(rightmost(), np, false);
        } else {
            auto pos = get_insert_multi_pos(key);
            return insert_node_at(pos.first, np, pos.second);
        }
    }
    return insert_multi_use_hint(hint, key, np);
}

/**
 * @brief hint位置与插入位置重复时，采用更优算法
 * @param hint 插入位置的提示
 * @return iterator 插入的位置
 */
template <class T, class Compare>
template <class... Args>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::emplace_unique_use_hint(iterator hint, Args&&... args) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");

    node_ptr np = create_node(MySTL::forward<Args>(args)...);

    if (node_count_ == 0)
        return insert_node_at(header_, np, true);

    key_type key = value_traits::get_key(np->value);
    if (hint == begin()) {
        if (key_comp_(key, value_traits::get_key(*hint))) {
            return insert_node_at(hint.node, np, true);
        } else {
            auto res = get_insert_unique_pos(value_traits::get_key(np->value));
            if (!res.second) {
                destory_node(np);
                return res.first.first;
            }
            return insert_node_at(res.first.first, np, res.first.second);
        }
    } else if (hint == end()) {
        if (key_comp_(value_traits::get_key(rightmost()->get_node_ptr()->value), key)) {
            return insert_node_at(rightmost(), np, false);
        } else {
            auto pos = get_insert_unique_pos(key);
            if (!pos.second) {
                destory_node(np);
                return pos.first.first;
            }
            return insert_node_at(pos.first.first, np, pos.first.second);
        }
    }
    return insert_unique_use_hint(hint, key, np);
}

template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::insert_multi(const value_type& value) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
    auto pos = get_insert_multi_pos(value_traits::get_key(value));
    return insert_value_at(pos.first, value, pos.second);
}

/**
 * @brief 插入节点，不允许插入具有相同键的节点
 * @return pair<iterator, bool> 插入的位置，是否插入成功 
 */
template <class T, class Compare>
MySTL::pair<typename rb_tree<T, Compare>::iterator, bool>
rb_tree<T, Compare>::insert_unique(const value_type& value) {
    THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
    auto pos = get_insert_unique_pos(value_traits::get_key(value));
    if (pos.second) {
        auto it = insert_value_at(pos.first.first, value, pos.first.second);
        return MySTL::make_pair(it, pos.second);
    }

    return MySTL::make_pair(pos.first.fisrt, pos.second);
}

/**
 * @brief 删除 hint 处节点
 * @return iterator 指向被删除节点的下一个节点
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::erase(iterator hint) {
    auto     pnode = hint.node->get_node_ptr();
    iterator next(pnode);
    ++next;
    rb_tree_erase_reblance(hint.node, root(), leftmost(), rightmost());
    destory_node(pnode);
    --node_count_;
    return next;
}

/**
 * @brief 所有键为 key 的元素，返回删除的个数
 * @return 删除的个数
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::erase_multi(const key_type & key) {
    auto      p = equal_range_multi(key);
    size_type n = MySTL::distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
}

/**
 * @brief 删除键为 key 的元素，返回删除的个数
 * @return 删除的个数
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::size_type
rb_tree<T, Compare>::erase_unique(const key_type& key) {
    auto it = find(key);
    if (it != end()) {
        erase(it);
        return 1;
    }
    return 0;
}

/**
 * @brief 删除 [first, last) 内的元素]
 */
template <class T, class Compare>
void rb_tree<T, Compare>::
erase(iterator first, iterator last) {
    if (first == begin() && last == end()) {
        clear();
    } else {
        while (first != last)
            erase(first++);
    }
}

/**
 * @brief 删除 [first, last) 内的元素]
 */
template <class T, class Compare>
void rb_tree<T, Compare>::
clear() {
    if (node_count_ != 0) {
        erase_since(root());
        leftmost() = header_;
        rightmost() = header_;
        root() = nullptr;
        key_comp_ = key_compare();
        node_count_ = 0;
    }
}

// 相关算法 /

/**
 * @brief 查找键为 key 的元素，返回其迭代器(二叉查找树)
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::find(const key_type& key) {
    auto y = header_; // y 表示最后一个不小于 key 的节点
    auto x = root(); 
    while(x != nullptr) {
        if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key)) {
            // !(x < key) => x >= key
            y = x; // key <= x，更新 y
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    iterator j = iterator(y);
    // 第二种情况为，x为空，y为最后一个不小于 key 的节点，但是 y > key，表示没有找到
    return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
}

/**
 * @brief 查找键为 key 的元素，返回其迭代器(二叉查找树)
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::find(const key_type& key) const {
    auto y = header_;
    auto x = root(); 
    while(x != nullptr) {
        if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key)) {
            y = x;
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
}

/**
 * @brief 键不小于key的第一个位置，算法同find()方法
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::lower_bound(const key_type& key) {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key)) {
            // x >= key
            y = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return iterator(y);
}

/**
 * @brief 键不小于key的第一个位置，算法同find()方法
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::lower_bound(const key_type& key) const {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key)) {
            // x >= key
            y = x;
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return const_iterator(y);
}

/**
 * @brief 键不小于key的最后一个位置
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::upper_bound(const key_type& key) {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value))) {
            // key < x
            y = x;
            x = x->left;
        } else {
            // key >= x，直到找到第一个大于 key 的节点
            x = x->right;
        }
    }
    return iterator(y);
}

/**
 * @brief 键不小于key的最后一个位置
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::const_iterator
rb_tree<T, Compare>::upper_bound(const key_type& key) const {
    auto y = header_;
    auto x = root();
    while (x != nullptr) {
        if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value))) {
            // key < x
            y = x;
            x = x->left;
        } else {
            // key >= x，直到找到第一个大于 key 的节点
            x = x->right;
        }
    }
    return const_iterator(y);
}

template <class T, class Compare>
void rb_tree<T, Compare>::swap(rb_tree& rhs) noexcept {
    if (this != &rhs) {
        MySTL::swap(header_, rhs.header_);
        MySTL::swap(node_count_, rhs.node_count_);
        MySTL::swap(key_comp_, rhs.key_comp_);
    }
}

/*********************************** helper function ***********************************/

// 创造节点
template <class T, class Compare>
template <class... Args>
typename rb_tree<T, Compare>::node_ptr
rb_tree<T, Compare>::create_node(Args&&... args) {
    auto node = node_allocator::allocate(1);
    try {
        data_allocator::construct(MySTL::address_of(node->value), MySTL::forward<Args>(args)...);
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
    } catch (...) {
        node_allocator::deallocate(node);
        throw;
    }
    return node;
}

// 复制节点
template <class T, class Compare>
typename rb_tree<T, Compare>::node_ptr
rb_tree<T, Compare>::clone_node(base_ptr x) {
    node_ptr tmp = create_node(x->get_node_ptr()->value);
    tmp->color = x->color;
    tmp->left = nullptr;
    tmp->right = nullptr;
    return tmp;
}

// 销毁节点
template <class T, class Compare>
void rb_tree<T, Compare>::destory_node(node_ptr p) {
    data_allocator::destroy(&p->value);
    node_allocator::deallocate(p);
}

// 初始化容器
template <class T, class Compare>
void rb_tree<T, Compare>::rb_tree_init() {
    header_ = base_allocator::allocate(1);
    header_->color = rb_tree_red; // header_与root_互为父节点，颜色区分
    root() = nullptr;
    leftmost() = header_;
    rightmost() = header_;
    node_count_ = 0;
}

/**
 * @brief 重置rb_tree状态
 */
template <class T, class Compare>
void rb_tree<T, Compare>::reset() {
    header_ = nullptr;
    node_count_ = 0;
    key_comp_ = key_compare();
}

/**
 * @brief 寻找合适插入节点的位置，允许插入具有相同键的节点
 * @param key 新节点的键
 * @return pair<指向应该插入位置的父节点，插入在找到的位置的左边还是右边>
 */
template <class T, class Compare>
MySTL::pair<typename rb_tree<T, Compare>::base_ptr, bool>
rb_tree<T, Compare>::get_insert_multi_pos(const key_type& key) {
    auto x = root();
    auto y = header_;
    bool add_to_left = true;
    while (x != nullptr) {
        y = x;
        add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
        x = add_to_left ? x->left : x->right;
    }
    return MySTL::make_pair(y, add_to_left);
}

/**
 * @brief 寻找合适插入节点的位置，不允许插入具有相同键的节点
 * @param key 要插入节点的键值
 * @return 返回一个 pair，第一个元素 pair<插入点的父节点，一个bool表示是否在左边插入>，第二个元素为是否插入成功
 */
template <class T, class Compare>
MySTL::pair<MySTL::pair<typename rb_tree<T, Compare>::base_ptr, bool>, bool>
rb_tree<T, Compare>::get_insert_unique_pos(const key_type& key) {
    auto x = root();
    auto y = header_;
    bool add_to_left = true;
    while (x != nullptr) {
        y = x;
        add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
        x = add_to_left ? x->left : x->right;
    }
    iterator j = iterator(y);
    if (add_to_left) {
        if (y == header_ || j == begin()) { // 树为空 / 插入点在最左节点处
            return MySTL::make_pair(MySTL::make_pair(y, add_to_left), true);
        } else {
            --j;
        }
    }
    if (key_comp_(value_traits::get_key(*j), key)) {
        return MySTL::make_pair(MySTL::make_pair(y, add_to_left), true);
    }
    return MySTL::make_pair(MySTL::make_pair(y, add_to_left), false);
}

/**
 * @brief 插入值
 * @param x 插入点的父节点
 * @param value 要插入的值
 * @param add_to_left 是否在左边插入
 * @return iterator 插入的位置
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::insert_value_at(base_ptr x, const value_type& value, bool add_to_left) {
    node_ptr node = create_node(value);
    node->parent = x;
    auto base_node = node->get_base_ptr();
    if (x == header_) { // 空树
        root() = base_node;
        leftmost() = base_node;
        rightmost() = base_node;
    } else if (add_to_left) {
        x->left = base_node;
        if (leftmost() == x)
        leftmost() = base_node;
    } else {
        x ->right = base_node;
        if (rightmost() == x) {
            rightmost() = base_node;
        }
    }
    rb_tree_insert_rebalance(base_node, root());
    ++node_count_;
    return iterator(node);
}

/**
 * @brief 在 x 节点处插入新节点
 * @param x 插入点的父节点
 * @param node 要插入的节点
 * @param add_to_left 是否在左边插入
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::insert_node_at(base_ptr x, node_ptr node, bool add_to_left) {
    node->parent = x;
    auto base_node = node->get_base_ptr();
    if (x == header_) { // 空树
        root() = base_node;
        leftmost() = base_node;
        rightmost() = base_node;
    } else if (add_to_left) {
        x->left = base_node;
        if (leftmost() == x)
        leftmost() = base_node;
    } else {
        x ->right = base_node;
        if (rightmost() == x)
            rightmost() = base_node;
    }
    rb_tree_insert_rebalance(base_node, root());
    ++node_count_;
    return iterator(node);
}

/**
 * @brief 插入元素，键值允许重复，允许使用hint
 * @param hint 插入位置的迭代器
 * @param key 键值
 * @param node 要插入的节点
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::insert_multi_use_hint(iterator hint, key_type key, node_ptr node) {
    // hint 附近寻找可插入位置
    auto np = hint.node;
    auto before = hint;
    --before;
    auto bnp = before.node;
    if (!key_comp_(key, value_traits::get_key(*before)) &&
        !key_comp_(value_traits::get_key(*hint), key))
    {  //  before <= node <= hint
        if (bnp->right == nullptr)
            return insert_node_at(bnp, node, false);
        else if (np->left == nullptr)
            return insert_node_at(np, node, true);
    }
    auto pos = get_insert_multi_pos(key);
    return insert_node_at(pos.first, node, pos.second);
}

/**
 * @brief 插入元素，键值不允许重复，允许使用hint
 * @param hint 插入位置的迭代器
 * @param key 键值
 * @param node 要插入的节点
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::iterator
rb_tree<T, Compare>::insert_unique_use_hint(iterator hint, key_type key, node_ptr node) {
    // hint 附近寻找可插入位置
    auto np = hint.node;
    auto before = hint;
    --before;
    auto bnp = before.node;
    if (key_comp_(key, value_traits::get_key(*before)) &&
        key_comp_(value_traits::get_key(*hint), key))
    {  //  before <= node <= hint
        if (bnp->right == nullptr)
            return insert_node_at(bnp, node, false);
        else if (np->left == nullptr)
            return insert_node_at(np, node, true);
    }
    auto pos = get_insert_unique_pos(key);
    if (!pos.second) {
        destory_node(node);
        return pos.first.first;
    }
    return insert_node_at(pos.first.first, node, pos.first.second);
}

/**
 * @brief 递归复制整棵树，从节点 x 开始
 * @param x 要复制的节点
 * @param p 复制节点的父节点
 */
template <class T, class Compare>
typename rb_tree<T, Compare>::base_ptr
rb_tree<T, Compare>::copy_from(base_ptr x, base_ptr p) {
    // init
    auto top = clone_node(x);
    top->parent = p;
    try {
        if (x->right)
            top->right = copy_from(x->right, top);
        p = top;
        x = x->left;
        while (x != nullptr) {
            auto y = clone_node(x);
            p->left = y;
            y->parent = p;
            if (x->right)
                y->right = copy_from(x->right, y);
            p = y;
            x = x->left;
        }
    } catch (...) {
        erase_since(top);
        throw;
    }
    return top;
}

/**
 * @brief 递归删除整棵树，从节点 x 开始
 * @param x 要删除的节点(开始)
 */
template <class T, class Compare>
void rb_tree<T, Compare>::erase_since(base_ptr x) {
    while(x != nullptr) {
        erase_since(x->right);
        auto y = x->left;
        destory_node(x->get_node_ptr());
        x = y;
    }
}

// 比较操作符重载
template <class T, class Compare>
bool operator==(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return lhs.size() == rhs.size() && MySTL::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Compare>
bool operator!=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Compare>
bool operator<(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return MySTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Compare>
bool operator>(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return rhs < lhs;
}

template <class T, class Compare>
bool operator<=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Compare>
bool operator>=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs) {
    return !(lhs < rhs);
}

// swap
template <class T, class Compare>
void swap(rb_tree<T, Compare>& lhs, rb_tree<T, Compare>& rhs) noexcept {
    lhs.swap(rhs);
}

} /* namespace MySTL  */

#endif // MY_RBTREE_H