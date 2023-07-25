#ifndef MYSTL_ALGOBASE_H_
#define MYSTL_ALGOBASE_H_

// 这个头文件包含了mystl的基本算法
#include <cstring>

#include "iterator.h"
#include "util.h"

namespace MyStl
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif 


#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif 

/*****************************************************************************************/
// max
// 取两者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/

template <class T>
const T& max(const T& lhs, const T& rhs)
{
  return lhs < rhs ? rhs : lhs;
}


template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp)
{
  return comp(lhs, rhs) ? rhs : lhs;
}


/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/

template <class T>
const T& min(const T& lhs, const T& rhs)
{
  return rhs < lhs ? rhs : lhs;
}


template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp)
{
  return comp(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// iter_swap
// 将两个迭代器所指对象对调
/*****************************************************************************************/
template <class Fiter1, class Fiter2>
void iter_swap(Fiter1 lhs, Fiter2 rhs)
{
  MyStl::swap(*lhs, *rhs);
}

/*****************************************************************************************/
// copy
// 把 [first, last) 区间的元素拷贝到 [result, result + (last - first)) 内
// STL算法的命名规则：以算法所能接受之最低阶迭代器类型，来为其迭代器型别参数明明
/*****************************************************************************************/
// input_iterator_tag
template <class InputIter, class OutputIter>
OutputIter 
unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, 
    MyStl::input_iterator_tag)
{
  for(; first != last; ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// random_access_interator_tag
template <class RandomIter, class OutputIter>
OutputIter
unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
    MyStl::random_access_iterator_tag)
{
  // 对于random_access_iterator_tag，也没有一个快捷的方式直接完成拷贝
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// 为trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if< // SFINAE 技术
  std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
  std::is_trivially_copy_assignable<Up>::value,
  Up*>::type // 条件编译，只有满足条件才能使用这个函数
unchecked_copy(Tp * first, Tp* last, Up* result)
{
  const auto n = static_cast<size_t>(last - first);
  if (n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}


template <class InputIter, class OutputIter>
OutputIter
copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy(first, last, result);
}

/*****************************************************************************************/
// copy_backward
// 将 [first , last) 区间内的元素拷贝到 [result - (last - first) , result)内
/*****************************************************************************************/
// unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
unchecked_copy_backward_cat(BidrectionalIter1 first, BidrectionalIter1 last,
                            BidrectionalIter2 result, MyStl::bidirectional_iterator_tag)
{
  while (first != last)
    *--result = *--last;
  return result;
}


// unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
template <class RandomIter1, class BidrectionalIter2>
BidrectionalIter2
unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                            BidrectionalIter2 result, MyStl::random_access_iterator_tag)
{
  for(auto n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
unchecked_copy_backward(BidrectionalIter1 first, BidrectionalIter1 last,
                        BidrectionalIter2 result)
{
  return unchecked_copy_backward_cat(first, last ,result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
  std::is_same<typename std::remove_reference<Tp>::type, Up>::value &&
  std::is_trivially_copy_assignable<Up>::value,
  Up*>::type
unchecked_copy_backward(Tp * first, Tp* last, Up* result)
{
  const auto n = static_cast<size_t>(last - first);
  if (n != 0)
  {
    result -= n;
    std::memmove(result, first, sizeof(Up) * n);
  }
  return result;
}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
copy_backward(BidrectionalIter1 first , BidrectionalIter1 last , BidrectionalIter2 result)
{
  return unchecked_copy_backward(first, last ,result);
}


/*****************************************************************************************/
// copy_if
// 把[first, last) 内满足医院操作 unary_pre 的元素拷贝到以result为起始的位置上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter
copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
      *result++ = *first;
  }
  return result;
}

/*****************************************************************************************/
// copy_n
// 把[first, first + n)区间上的元素拷贝到 [result, result + n) 上\
// 返回一个pair分别指向拷贝结束的尾部
/*****************************************************************************************/
// template <class InputIter, class OutputIter, class Size>
// MyStl::pair<InputIter, OutputIter>
// unchecked_copy_n(InputIter first, Size n, OutputIter result, MyStl::input_iterator_tag)
// {

// }
/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/































































































































































































}

#endif
