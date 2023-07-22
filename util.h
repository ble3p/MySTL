#ifndef MYSTL_UTIL_H_
#define MYSTL_UTIL_H_

// 这个文件包含一些通用工具，包含move, forward, swap等函数, 以及pair等

#include <cstddef>

#include "type_traits.h"

namespace MyStl
{
  // move

  template <class T>
  typename std::remove_reference<T>::type&& move(T &&arg) noexcept
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
  }

  // forward
  template <class T>
  T&& forward(typename std::remove_reference<T>::type &arg) noexcept
  {
    return static_cast<T &&>(arg);
  }


  template <class T>
  T&& forward(typename std::remove_reference<T>::type &&arg) noexcept
  {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T &&>(arg);
  }

  // swap

  template <class Tp>
  void swap(Tp &lhs, Tp &rhs)
  {
    auto tmp(MyStl::move(lhs));
    lhs = MyStl::move(rhs);
    rhs = MyStl::move(tmp);
  }

  template <class ForwardIter1, class ForwardIter2>
  ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
  {
    for (; first1 != last1; ++first1, (void) ++first2)
      MyStl::swap(*first1, *first2);
    return first2;
  }

  template <class Tp, size_t N>
  void swap(Tp(&a)[N], Tp(&b)[N])
  {
    MyStl::swap_range(a, a + N, b);
  }

  //----------------------------------------------------------------------------------




}

#endif
