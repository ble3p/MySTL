#ifndef MYSTL_LIST_H_
#define MYSTL_LIST_H_

// 模板类list
// list: 双向链表

// 异常保证:
// MyStl::list<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证
//  * emplace_front
//  * emplace_back
//  * emplace
//  * push_front
//  * push_back
//  * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"

namespace MyStl
{

template <class T> struct list_node_base;
template <class T> struct list_node;

template <class T>
struct node_traits
{
    typedef list_node_base<T>*  base_ptr;
    typedef list_node<T>*       node_ptr;

};

template <class T>
struct list_node_base
{
    typedef typename node_traits<T>::base_prt base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    base_ptr prev;
    base_ptr next;

    list_node_base() = default;

    node_ptr as_node()
    {
        return static_cast<node_ptr>(self());
    }

    void unlink()
    {
        prev = next = self();
    }

    base_ptr self()
    {
        return static_cast<base_ptr>(&*this);
    }
};

template <class T>
struct list_node : public list_node_base<T>
{
    typedef typename node_traits<T>::base_ptr base_ptr;
    typedef typename node_traits<T>::node_ptr node_ptr;

    T value;
    list_node() = default;
    list_node(const T &v):value(v) {}
    list_node(T &&v):value(MyStl::move(v)) {}

    base_ptr as_base()
    {
        return static_cast<base_ptr>(&*this);
    }
    node_ptr self()
    {
        return static_cast<node_ptr>(&*this);
    }
};

// list 迭代器设计
template <class T>
struct list_iterator : public MyStl::iterator<MyStl::bidirectional_iterator_tag, T>
{
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef typename node_traits<T>::base_ptr   base_ptr;
    typedef typename node_traits<T>::node_ptr   node_ptr;
    typedef list_iterator<T>                    self;

    base_ptr node_;

    // 构造函数
    list_iterator() = default;
    list_iterator(base_ptr x) : node_(x) {}
    list_iterator(node_ptr x) : node_(x -> as_base()) {}
    list_iterator(const list_iterator& rhs) : node_(rhs.node_) {}

    // 重载操作符
    reference operator*() const { return node_ -> as_node() -> value; }
    pointer   operator->() const { return &(operator*()); } // 调用指向对象的成员方法

    self& operator++()
    {
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_ -> next;
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
        
    }
    self& operator--()
    {
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_ -> prev;
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    // 重载比较操作符
    bool operator==(const self &rhs) const { return node_ == rhs.node_ ;}
    bool operator!=(const self &rhs) const { return node_ != rhs.node_ ;}

};

template <class T>
struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
{
    typedef T                                   value_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
    typedef typename node_traits<T>::base_ptr   base_ptr;
    typedef typename node_traits<T>::node_ptr   node_ptr;
    typedef list_const_iterator<T>              self;

    base_ptr node_;

    list_const_iterator() = default;
    list_const_iterator(base_ptr x) : node_(x) {}
    list_const_iterator(node_ptr x) : node_(x -> as_base()) {}
    list_const_iterator(const list_iterator<T> &rhs) : node_(rhs.node_) {}
    list_const_iterator(const list_const_iterator<T> &rhs) : node_(rhs.node_) {}

    reference operator*() const { return node_ -> as_node() -> value; }
    pointer operator->() const { return &(operator*()); } 

    self& operator++()
    {
        MYSTL_DEBUG( node_ != nullptr);
        node_ = node_ -> next;
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--()
    {
        MYSTL_DEBUG(node_ != nullptr);
        node_ = node_ -> prev;
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self &rhs) const { return node_ == rhs.node_; }
    bool operator!=(const self &rhs) const {return node_ != rhs.node_; }
};


// list
template <class T>
class list
{
public:
    typedef MyStl::allocator<T>                         allocator_type;
    typedef MyStl::allocator<T>                         data_allocator;
    typedef MyStl::allocator<list_node_base<T>>         base_allocator;
    typedef MyStl::allocator<list_node<T>>              node_allocator;

    typedef typename allocator_type::value_type         value_type; 
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef list_iterator<T>                            iterator;
    typedef list_const_iterator<T>                      const_iterator;
    typedef MyStl::reverse_iterator<iterator>           reverse_iterator;
    typedef MyStl::reverse_iterator<const_iterator>     const_reverse_iterator;

    typedef typename node_traits<T>::base_ptr           base_ptr;
    typedef typename node_traits<T>::node_ptr           node_ptr;

    allocator_type get_allocator() { return node_allocator(); }
private:
    base_ptr node_;
    size_type size_;

public:
    list()
    { fill_init(0, value_type()); }

    explicit list(size_type n)
    { fill_init(n, value_type()); }

    list(size_type n, const T &value)
    { fill_init(n, value); }

    template <class Iter, typename std::enable_if<
        MyStl::is_input_iterator<Iter>::value, int>::type = 0>
    list(Iter first, Iter last)
    { copy_init(first, last); }

    list(std::initializer_list<T> ilist)
    { copy_init(ilist.begin(), ilist.end()); }

    list(const list &rhs)
    { copy_init(rhs.cbegin(), rhs.cend()); }

    list(list &&rhs) noexcept : node_(rhs.node_), size_(rhs.size_)
    {
        rhs.node_ = nullptr;
        rhs.size_ = 0;
    }
};











#endif

