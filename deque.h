#ifndef _MYSTL_DEQUE_H_
#define _MYSTL_DEQUE_H_
#include "allocator.h" 
#include "algobase.h"
#include "uninitialized.h"



namespace MyStl
{
    

template <class T, size_t Bufsize>
class _deque_iterator
{
public:

    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;

    typedef T**                         map_pointer;
    typedef _deque_iterator             self;

    pointer cur;
    pointer first;
    pointer last;
    map_pointer node;

    static size_t buffer_size() { return _deque_buf_size(Bufsize, sizeof(T)); }


public:
    // 构造函数
    _deque_iterator() = default;
    _deque_iterator(pointer cur_, pointer first_, pointer last_, map_pointer node_) : 
        cur(cur_), first(first_), last(last_), node(node_) {} 

    // 重载运算符
    T& operator*() const { return *cur; }
    T* operator->() const { return &(operator*());}

    difference_type operator-(const self &rhs) const
    {
        return difference_type(buffer_size()) * (node - rhs.node - 1) + 
                                                (cur - first) + (rhs.last - rhs.cur);
    }

    self& operator++() 
    {
        ++cur;
        if (cur == last)
        {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return *this;
    }

    self& operator--()
    {
        if (cur == first)
        {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    // random_access
    self& operator+=(difference_type n) 
    {
        size_t len = buffer_size();
        // 计算偏移值
        difference_type offset = cur - first + n;
        if (offset > len)
        {
            set_node(node + offset / difference_type(len));
            cur = first + difference_type(offset - len * (offset / len));
        }
        else if (offset < 0)
        {
            set_node(node + -difference_type(-offset / len + 1));
            cur = last + offset + len * (-offset / len + 1);
        }
        else
        {
            cur = first + offset;
        }
        return *this;

    }

    self operator+(difference_type n) const
    {
        self tmp = *this;
        return tmp += n;
    }

    self& operator-=(difference_type n) const
    {
        return *this += -n; 
    }

    self operator-(difference_type n) const
    {
        self tmp = *this;
        return *this -= n;
    }

    T& operator[](size_t n) const
    {
        return *(*this + n);
    }

    bool operator==(const self &rhs) const
    {
        return (cur == rhs.cur) && (first == rhs.first) && (last == rhs.last) && (node == rhs.node);

    }

    bool operator!=(const self &rhs) const
    {
        return !(*this == rhs);
    }

    bool operator<(const self &rhs) const
    {
        return  node < rhs.node || ((node == rhs.node) && cur < rhs.cur);
    }

    void set_node(map_pointer new_node)
    {
        first = *new_node;
        last = *new_node + difference_type(buffer_size());
        node = new_node;
    }

private:
    static size_t _deque_buf_size(size_t bufsize, size_t sz)
    {
        // bufsize = 0， 则分配512个字节给T类型使用，不够按一计算，否则按bufsize计算
        return bufsize != 0 ? bufsize : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }


};

template <class T, class Alloc = MyStl::allocator<T>, size_t Bufsize = 0>
class deque
{
public:

    typedef T                                           value_type;
    typedef T*                                          pointer;
    typedef T&                                          reference;
    typedef const T*                                    const_pointer;
    typedef size_t                                      size_type;
    typedef ptrdiff_t                                   difference_type;
    

    typedef _deque_iterator<T, Bufsize>                 iterator;
    typedef const _deque_iterator<T, Bufsize>           const_iterator;
    typedef MyStl::reverse_iterator<T>                  reverse_iterator;
    typedef const MyStl::reverse_iterator<T>            const_reverse_iterator;

    typedef MyStl::allocator<T>                         data_allocator;
    typedef MyStl::allocator<pointer>                   map_allocator;

protected:
    typedef pointer*                                    map_pointer;
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size;

    
public:

    // 构造函数
    deque(size_t n, const T &value) : start(), finish(), map(), map_size(0)
    {
        fill_initialize(n, value);
    }

    // 数据结构
    // 迭代器相关
    iterator begin() { return start; }
    const_iterator begin() const { return start; }
    iterator end() { return finish; }
    const_iterator end() const { return finish; }

    const_iterator cbegin() const { return start; }
    const_iterator cend() const { return finish; }

    reverse_iterator rbegin() { return finish; }
    const_reverse_iterator rbegin() const { return finish; }
    reverse_iterator rend() {return start; }
    const_reverse_iterator rend() const { return start; }

    const_reverse_iterator crbegin() const { return finish; }
    const_reverse_iterator crend() const { return start; }

    // 访问
    reference operator[](size_t n) { return start[difference_type(n)];}
    reference front() const { return *start; }
    reference back() const
    {
        iterator tmp = finish;
        return *--tmp;
    }

    // 容量
    size_type size() const { return size_type(finish - start); }
    bool empty() const { return start == finish; }
    size_type max_size() const { return size_type(-1); }

    // 插入
    void push_back(const T &value);
    void push_front(const T &value);
private:

    // 大小
    size_type initial_map_size(size_t n) { return MyStl::max(n + 2, size_t(8));}
    size_type buffer_size() { return _deque_buf_size(Bufsize, sizeof(T)); }
    size_type _deque_buf_size(size_t bufsize, size_t sz) { return bufsize != 0 ? bufsize : (sz < 512 ? size_type(512 / sz) : size_type(1)); }


    // helper function
    void fill_initialize(size_t n, const T &value);
    void create_map_and_nodes(size_t n);
};




// helper function
template <class T, class Alloc, size_t Bufsize>
void deque<T, Alloc, Bufsize>::fill_initialize(size_t n, const T &value)
{
    // 计算节点大小
    create_map_and_nodes(n);
    try
    {
        for (auto cur = start; cur.node < finish.node; ++cur.node)
        {
            MyStl::uninitialized_fill(cur.first, cur.last, value);

        }
        MyStl::uninitialized_fill(finish.first, finish.last, value);

    }
    catch(...)
    {
        throw;
    }
}

template <class T, class Alloc, size_t Bufsize>
void deque<T,Alloc, Bufsize>::create_map_and_nodes(size_t n)
{
    size_t nodes = size_type(n / buffer_size() + 1);
    size_t map_size_ = initial_map_size(nodes);
    map_size = map_size_;
    map = map_allocator::allocate(map_size_);

    // 保持两端扩张能量一致
    map_pointer nfirst = map + difference_type((map_size - nodes) / 2);
    map_pointer nlast = nfirst + difference_type(nodes);

    try
    {
        for (auto cur = nfirst; cur != nlast; ++cur)
        {
            *cur = data_allocator::allocate(buffer_size());
        }
        start.set_node(nfirst);
        start.cur = start.first;
        finish.set_node(nlast - 1);
        finish.cur = finish.first + n % buffer_size();
    }
    catch(...)
    {
        for (auto cur = nfirst; cur != nlast; ++cur)
        {
            data_allocator::deallocate(*cur);
        }
        throw;
    }
}



































} // end namespace 


#endif

