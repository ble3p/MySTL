#ifndef MYSTL_ALGO_H_
#define MYSTL_ALGO_H_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif 

// 这个头文件包含了MyStl 的一系列算法

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"
#include "heap_algo.h"
#include "functional.h"

namespace MyStl
{

/*****************************************************************************************/
// all_of
// 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
    for (; first != last; ++first)
    {
        if (!unary_pred(*first))
            return false;
    }
    return true;
}

/*****************************************************************************************/
// any_of
// 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
    for (; first != last; ++first)
    {
        if (unary_pred(*first))
            return true;
    }
    return false;
}

/*****************************************************************************************/
// none_of
// 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
    for (; first != last; ++first)
    {
        if (unary_pred(*first))
            return false;
    }
    return true;
}

/*****************************************************************************************/
// count
// 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==，返回元素相等的个数
/*****************************************************************************************/
template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value) 
{
    size_t n = 0;
    for (; first != last; ++first)
    {
        if (*first == value)
            ++n;
    }
    return n;
}


/*****************************************************************************************/
// count_if
// 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
/*****************************************************************************************/
/*****************************************************************************************/
// find
// 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
/*****************************************************************************************/
/*****************************************************************************************/
// find_if
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
/*****************************************************************************************/
/*****************************************************************************************/
// find_if_not
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
/*****************************************************************************************/


/*****************************************************************************************/
// search
// 在[first1, last1)中查找[first2, last2)的首次出现点
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2)
{
    auto d1 = MyStl::distance(first1, last1);
    auto d2 = MyStl::distance(first2, last2);
    if (d1 < d2)
        return last1;
    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2)
    {
        if (*current1 == *current2)
        {
            ++current1;
            ++current2;
        }
        else
        {
            --d1;
            if (d1 < d2)
                return last1;
            else
            {
                current1 = ++first1;
                current2 = first2;
            }
        }
    }
    return first1;
}

template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2, Compare comp)
{
    auto d1 = MyStl::distance(first1, last1);
    auto d2 = MyStl::distance(first2, last2);
    if (d1 < d2)
        return last1;
    auto current1 = first1;
    auto current2 = first2;
    while (current2 != last2)
    {
        if (comp(*current1, *current2))
        {
            ++current1;
            ++current2;
        }
        else
        {
            --d1;
            if (d1 < d2)
                return last1;
            else
            {
                current1 = ++first1;
                current2 = first2;
            }
        }
    }
    return first1;

}


/*****************************************************************************************/
// search_n
// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
/*****************************************************************************************/

template <class ForwardIter, class T>
ForwardIter
search_n(ForwardIter first, ForwardIter last, size_t n, const T &value)
{
    auto d = MyStl::distance(first, last);
    if (d < n)
        return last;
    auto current = first;
    size_t size = n;
    while (size)
    {
        if (*current == value)
        {
            ++current;
            --size;
        }
        else
        {
            --d;
            if (d < n)
                return last;
            else
            {
                current = ++first;
                size = n;
            }
        }
    }
    return first;
}

template <class ForwardIter, class T, class Compare>
ForwardIter
search_n(ForwardIter first, ForwardIter last, size_t n, const T &value, Compare comp)
{
    auto d = MyStl::distance(first, last);
    if (d < n)
        return last;
    auto current = first;
    size_t size = n;
    while (size)
    {
        if (comp(*current, value))
        {
            ++current;
            --size;
        }
        else
        {
            --d;
            if (d < n)
                return last;
            else
            {
                current = ++first;
                size = n;
            }
        }
    }
    return first;
}







































}

#endif

