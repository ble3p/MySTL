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

// random_access_iterator_tag
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

template <class InputIter, class Size, class OutputIter>
MyStl::pair<InputIter, OutputIter>
unchecked_copy_n(InputIter first, Size n, OutputIter result, MyStl::input_iterator_tag)
{
    for (;n > 0; --n)
    {
        *result = *first;
    }
    return MyStl::pair<InputIter,OutputIter>(first, result);
}


template <class RandomIter, class Size, class OutputIter>
MyStl::pair<RandomIter, OutputIter>
unchecked_copy_n(RandomIter first, Size n, OutputIter result, MyStl::random_access_iterator_tag)
{
    auto last = first + n;
    return MyStl::pair<RandomIter, OutputIter>(last, MyStl::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
MyStl::pair<InputIter, OutputIter>
copy_n(InputIter first, Size n, OutputIter result)
{
    return unchecked_copy_n(first, n, result, iterator_category(first));
}



/*****************************************************************************************/
// move
// 把 [first, last) 区间内的元素移动到 [result , result + (last - first)) 内 
/*****************************************************************************************/

template <class InputIter, class OutputIter>
OutputIter
unchecked_move_cat(InputIter first ,InputIter last, OutputIter result, input_iterator_tag)
{
    for (; first != result; ++first, ++result)
    {
        *result = MyStl::move(*first);
    }
    return result;
}


template <class RandomIter, class OutputIter>
OutputIter
unchecked_move_cat(RandomIter first ,RandomIter last, OutputIter result, random_access_iterator_tag)
{
    for (auto n = last - first; n > 0; --n, ++first, ++ result)
    {
        *result = MyStl::move(*first);
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_move_cat(first, last, result, iterator_category(first));
}


// 为trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
unchecked_move(Tp *first, Tp *last, Up* result)
{
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n*sizeof(Up));
    return result + n;
}

template <class InputIter, class OutputIter>
OutputIter
move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_move(first, last, result);
}


/*****************************************************************************************/
// move_backward
// 将 [first, last) 区间内的元素移动到[result - (last - fist), result) 内
/*****************************************************************************************/

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
unchecked_move_backward_cat(BidrectionalIter1 first, BidrectionalIter1 last,
                            BidrectionalIter2 result, MyStl::bidirectional_iterator_tag)
{
    while(first != last)
        *--result = MyStl::move(*--last);
    return result;
}

template <class RandomIter1, class RandomIter2>
RandomIter2
unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
                            RandomIter2 result, MyStl::random_access_iterator_tag)
{
    for (auto n = last - result; n > 0; --n )
        *--result = MyStl::move(*--last);
    return result;
}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
unchecked_move_backward(BidrectionalIter1 first, BidrectionalIter1 last, BidrectionalIter2 result)
{
    return unchecked_move_backward_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template<class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_reference<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::value
unchecked_move_backward(Tp *first, Tp *last, Up *result)
{
    const size_t n = static_cast<size_t>(last- first);
    if (n != 0)
    {
        result -= n;
        std::memmove(result - n, first, sizeof(result) * n);
    }
    return result;
}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter2
move_backward(BidrectionalIter1 first, BidrectionalIter1 last, BidrectionalIter2 result)
{
    return unchecked_move_backward(first, last, result);

}
/*****************************************************************************************/
// equal
// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first, InputIter1 last, InputIter2 first2)
{
    for (; first != last; ++first, ++first2)
    {
        if (*first != *first2)
            return false;
    }
    return true;
}

template <class InputIter1, class InputIter2, class Compare>
bool equal(InputIter1 first, InputIter1 last, InputIter2 first2, Compare comp)
{
    for (; first != last; ++first, ++first2)
    {
        if (!comp(*first, *first2))
            return false;
    }
    return true;
}


/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充n个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size t, const T &value)
{
    for (;t > 0; --t, ++first)
    {
        *first = value;
    }
    return first;
}

// 为one-byte类型提供特化版本
// 问是否是整形 char
template <class Tp, class Size, class Up>
typename std::enable_if<
    std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
    !std::is_same<Tp,bool>::value &&
    std::is_integral<Up>::value && sizeof(Up) == 1,
    Tp*>::type
unchecked_fill_n(Tp *first, Size t, Up value)
{
    if (t > 0)
    {
        std::memset(first, (unsigned char)value, (size_t)(t));
    }
    return first + t;

}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size t, const T &value)
{
    return unchecked_fill_n(first, t, value);
}

/*****************************************************************************************/
// fill
// 为 [first, last) 区间内的所有元素填充新值
/*****************************************************************************************/
template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T &value, MyStl::forward_iterator_tag)
{
    for (; first != last; ++first)
        *first = value;
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T &value, MyStl::random_access_iterator_tag)
{
    fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T &value)
{
    fill_cat(first, last, value, iterator_category(first));
}


/*****************************************************************************************/
// lexico'graphical_compare 
// sorted in a way that uses a algorithm based on the alphabetical order used in dictionaries
// 两者不一定同大小，故需要两者都提供first和last
// 如果第一序列的元素较小， 返回true， 否则返回false
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (*first1 < *first2)
            return true;
        else if (*first1 > *first2)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                             InputIter2 first2, InputIter2 last2, Compare comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2)
    {
        if (comp(*first1,*first2))
            return true;
        else if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// 针对const ungined char* 的特化版本
// bool lexicographical_compare(const unsigned char *first1,
                             //const unsigned char *last1)
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


/*****************************************************************************************/
// min
// 取两者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
































































































































































































}

#endif
