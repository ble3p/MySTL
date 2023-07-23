#ifndef MYSTL_ITERATOR_H_
#define MYSTL_ITERATOR_H_

// 这个头文件用于迭代器设计, 包含了一些模板结构体与全局函数,


#include <cstddef>

#include "type_traits.h"

namespace MyStl
{


// 五中迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// 这些classes只作为标记用，帮助函数重载进行区分，所以不需要任何成员

// iterator 模板
template <class Category, class T, class Distance = ptrdiff_t,
         class Pointer = T*, class Reference = T&>
struct iterator
{
  typedef Category              iterator_category;
  typedef T                     value_type;
  typedef Pointer               pointer;
  typedef Reference             reference;
  typedef Distance              difference_type;
};

// iterator traits

template <class T>
struct has_iterator_cat // cat for category
{
  private:
    struct two {char a; char b;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::iterator_category* = 0);
  public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char); // SFINAE(Substitution Failure Is Not An Error) 技术来检查一个类型'T'是否拥有特定的成员函数或成员变量, 如果拥有iterator_category成员，则会通过SFINAE匹配到返回值为char的test，否则匹配到返回值为two的test，这样就区分出来是否有iterator_category
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
  typedef typename Iterator::iterator_category  iterator_category;
  typedef typename Iterator::value_type         value_type;
  typedef typename Iterator::pointer            pointer;
  typedef typename Iterator::reference          reference;
  typedef typename Iterator::difference_type    difference_type;
};

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
  : public iterator_traits_impl<Iterator,
  std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value
||
  std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value
 >
{};


// 萃取迭代器的特性
template <class Iterator>
struct iterator_traits
  : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*>
{

  typedef random_access_iterator_tag      iterator_category;
  typedef T                               value_type;
  typedef T*                              pointer;
  typedef T&                              reference;
  typedef ptrdiff_t                       difference_type;
};
  





























}














#endif
