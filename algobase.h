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












/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/































































































































































































}

#endif
