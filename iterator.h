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
struct has_iterator_cat
{
  private:
    struct two {char a; char b;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::iterator_category* = 0);
  public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
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
  
template <class T>
struct iterator_traits<const T*>
{
  typedef random_access_iterator_tag      iterator_category;
  typedef T                               value_type;
  typedef const T*                        pointer;
  typedef const T&                        reference;
  typedef ptrdiff_t                       difference_type;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
  : public m_bool_constant<std::is_convertible<
    typename iterator_traits<T>::iterator_category, U>::value>
{
};

// 萃取某种迭代器
template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};


template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator:
  public m_bool_constant<is_input_iterator<Iterator>::value ||
    is_output_iterator<Iterator>::value>
{};

// 萃取某个迭代器的 category
  template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
  typedef typename iterator_traits<Iterator>::iterator_category Category;
  return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
typename iterator_traits<Iterator>::distance_type*
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}



















}














#endif
