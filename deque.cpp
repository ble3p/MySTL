#ifndef _MYSTL_DEQUE_H_
#define _MYSTL_DEQUE_H_
#include "allocator.h" 



namespace MyStl
{
    

template <class T>
class _deque_iterator
{
public:
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;

    typedef _deque_iterator             self;

};

template <class T, class Alloc = MyStl::allocator<T>, size_t Bufsize = 0>
class deque
{
public:

    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef const T*                            const_pointer;
    typedef size_t                              size_type;
    typedef ptrdiff_t                           difference_type;
    

    typedef _deque_iterator<T>                  iterator;
    typedef const _deque_iterator<T>            const_iterator;
    typedef MyStl::reverse_iterator<T>          reverse_terator;
    typedef const MyStl::reverse_iterator<T>    const_reverse_terator;

protected:
    typedef T*          map;
    typedef T*          map_pointer;
    typedef size_t      map_size;
    


};





































} // end namespace 


#endif

