#ifndef MYSTL_ALLOCATOR_H_
#define MYSTL_ALLOCATOR_H_



#include "construct.h"
#include "util.h"
namespace MyStl
{
  // 模板类: allocator
  // 模板函数代表数据类型
  template <class T>
  class allocator
  {
    public:
      // 此为allocator的标准接口定义
      typedef T           value_type;
      typedef T*          pointer;
      typedef const T*    const_pointer;
      typedef T&          reference;
      typedef T&&         right_reference;
      typedef const T&    const_reference;
      typedef size_t      size_type;
      typedef ptrdiff_t   difference_type;

    public:
      static T*   allocate();
      static T*   allocate(size_type n);

      static void deallocate(pointer ptr);
      static void deallocate(pointer ptr, size_type n);

      static void construct(pointer ptr);
      static void construct(pointer ptr, const_reference value);
      static void construct(pointer ptr, right_reference value); // 对右值的引用

      template <class ...Args>
      static void construct(pointer ptr, Args&& ...args);
      
      static void destroy(pointer ptr);
      static void destroy(pointer first, pointer last);
  };

  template <class T>
  typename allocator<T>::pointer allocator<T>::allocate()
  {
    return static_cast<pointer>(::operator new(sizeof(T)));
  }

  template <class T>
  typename allocator<T>::pointer allocator<T>::allocate(size_type n)
  {
    if (n == 0)
      return nullptr;
    return static_cast<pointer>(::operator new(n * sizeof(T)));
  }

  template <class T>
  void allocator<T>::deallocate(pointer ptr)
  {
    if (ptr == nullptr)
      return;
    ::operator delete(ptr);
  }

  template <class T>
  void allocator<T>::deallocate(pointer ptr, size_type n)
  {
    if (ptr == nullptr)
      return;
    ::operator delete(ptr);
  }

  template <class T>
  void allocator<T>::construct(pointer ptr)
  {
    MyStl::construct(ptr);
  }

  template <class T>
  void allocator<T>::construct(pointer ptr, const_reference value)
  {
    MyStl::construct(ptr, value);
  }

  template <class T>
  void allocator<T>::construct(pointer ptr, right_reference value)
  {
    MyStl::construct(ptr, MyStl::move(value));
  }

  template <class T>
  template <class ...Args>
  void allocator<T>::construct(pointer ptr, Args&& ...args)
  {
    MyStl::construct(ptr, MyStl::forward<Args>(args)...);
  }

  template <class T>
  void allocator<T>::destroy(pointer ptr)
  {
    MyStl::destroy(ptr);
  }

  template <class T>
  void allocator<T>::destroy(pointer first, pointer last)
  {
    MyStl::destroy(first, last);
  }
} // namespace MyStl
#endif
