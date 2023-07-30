#ifndef MYSTL_UTIL_H_
#define MYSTL_UTIL_H_

// 这个文件包含一些通用工具，包含move, forward, swap等函数, 以及pair等

#include <cstddef>

#include "type_traits.h"

namespace MyStl
{
  // 借用引用折叠触发万能引用引用所有可能的情况，包括左值右值，然后进行完美转发,但万能引用内部由于具名有了地址形参都会变成左值，为了改变这种情况，可以通过使用std::forward,。在<Effecitve Modern C++> 中建议：对于右值引用使用std::move, 对于万能引用使用std::forward, 它们仅仅做了类型转换。真正的移动操作都是在移动构造函数或者移动赋值操作符中发生的。
  // move， 实现移动语义，避免拷贝，从而提升程序性能，当可移动对象在<需要拷贝且被拷贝者之后不再需要>的场景，建议使用std::move触发移动语义，提升性能

  template <class T>
  typename std::remove_reference<T>::type&& move(T &&arg) noexcept
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg); // 去除T中的引用部分，只获取其中的类型部分, 是万能引用
  } // 返回右值引用，调用右值引用的拷贝构造函数

  // forward
  template <class T> 
  T&& forward(typename std::remove_reference<T>::type &arg) noexcept // 万能引用使用引用折叠折叠成左值引用
  {
    return static_cast<T &&>(arg); // 引用折叠，所有的引用折叠最终都代表一个引用，要么是左值引用，要么是右值引用。规则是：如果任一引用为左值引用，则结果为左值引用。否则(即两个都是右值引用) , 结果为右值引用
  }

  template <class T>
  T&& forward(typename std::remove_reference<T>::type &&arg) noexcept
  {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward"); // 为flase触发
    return static_cast<T &&>(arg);
  }

  // swap

  template <class Tp>
  void swap(Tp &lhs, Tp &rhs)
  {
    // 使用移动构造函数和移动赋值运算符
    auto tmp(MyStl::move(lhs));
    lhs = MyStl::move(rhs);
    rhs = MyStl::move(tmp);
  }

  template <class ForwardIter1, class ForwardIter2>
  ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
  {
    for (; first1 != last1; ++first1, (void) ++first2) //使用(void)++first2是为了抑制编译器产生未使用变量的警告
      MyStl::swap(*first1, *first2);
    return first2;
  }

  template <class Tp, size_t N>
  void swap(Tp(&a)[N], Tp(&b)[N])
  {
    MyStl::swap_range(a, a + N, b);
  }

  //----------------------------------------------------------------------------------

  // 结构体模板 :pari
  // 用first和second来分别取出第一个数据和第二个数据
  template <class Ty1, class Ty2>
  struct pair
  {
    typedef Ty1       first_type;
    typedef Ty2       second_type;

    first_type first;
    second_type second;

    // default_constructiable
    template <class Other1 = Ty1, class Other2 = Ty2,
      typename std::enable_if<
       std::is_default_constructible<Other1>::value&&
       std::is_default_constructible<Other2>::value, void>::type>
       constexpr pair()
       : first(), second()
   {
   }

    // implicit_constructiable for this type
  // 使用const U1& 作为参数的原因是为了确保在进行隐式转换时不会产生不必要的副作用。当U1类型作为构造函数参数时，如果U1是一个非常量类型，而Ty1是一个常量类型，编译器可能会进行隐式转换，但这可能会产生副作用，因为常量类型无法修改。通过const U1&，我们告诉编译器我们希望检查的是U1类型的常量引用是否可以隐式转换为Ty1类型，这样可以避免不必要的副作用
    template <class U1 = Ty1, class U2 = Ty2,
      typename std::enable_if<
       std::is_copy_constructible<U1>::value &&
       std::is_copy_constructible<U2>::value &&
       std::is_convertible<const U1&, Ty1>::value && //是否可以隐式转换
       std::is_convertible<const U2&, Ty2>::value, int>::type = 0> 
       constexpr pair(const Ty1 &a, const Ty2 &b)
       : first(a), second(b)
   {
   }

    template <class U1 = Ty1, class U2 = Ty2,
      typename std::enable_if<
      std::is_copy_constructible<U1>::value &&
      std::is_copy_constructible<U2>::value &&
      (!std::is_convertible<const U1&, Ty1>::value ||
       !std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
      explicit constexpr pair(const Ty1 &a, const Ty2 &b)
      : first(a), second(b)
    {
    }



      
  };// struct pair




}

#endif
