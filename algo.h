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
    if (n <= 0)
        return first;
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
    if (n <= 0)
        return first;
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


/*****************************************************************************************/
// find_end
// 在[first1, last1)区间中查找[first2, last2)最后一次出现的地方，若不存在返回 last1
/*****************************************************************************************/
// find_end_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag)
{
    if (first2 == last2)
    {
        return last1;
    }
    else
    {
        auto result = last1;
        while(true)
        {
            auto new_result = MyStl::search(first1, last1, first2, last2);
            if (new_result == last1)
            {
                return result;
            }
            else
            {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }

}

template <class BidrectionalIter1, class BidrectionalIter2>
BidrectionalIter1
find_end_dispatch(BidrectionalIter1 first1, BidrectionalIter1 last1,
                  BidrectionalIter2 first2, BidrectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag)
{
    typedef reverse_iterator<BidrectionalIter1> rev_iter1;
    typedef reverse_iterator<BidrectionalIter2> rev_iter2;
    rev_iter1 rlast1(first1); // 构造反向迭代器对象
    rev_iter2 rlast2(first2);
    rev_iter1 rresult = MyStl::search(rev_iter1(last1), rlast1, rev_iter2(last2), rlast2);
    if (rresult == rlast1)
    {
        return last1;
    }
    else 
    {
        auto result = rresult.base();
        MyStl::advance(result, MyStl::distance(first2, last2));
        return result; // 逆序查找，然后进行指向看透
    }

}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter2 last1,
         ForwardIter2 first2, ForwardIter2 last2)
{
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return MyStl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
}


template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
                  ForwardIter2 first2, ForwardIter2 last2,
                  forward_iterator_tag, forward_iterator_tag, Compare comp)
{
    if (first2 == last2)
    {
        return last1;
    }
    else
    {
        auto result = last1;
        while(true)
        {
            auto new_result = MyStl::search(first1, last1, first2, last2, comp);
            if (new_result == last1)
            {
                return result;
            }
            else
            {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }

}

template <class BidrectionalIter1, class BidrectionalIter2, class Compare>
BidrectionalIter1
find_end_dispatch(BidrectionalIter1 first1, BidrectionalIter1 last1,
                  BidrectionalIter2 first2, BidrectionalIter2 last2,
                  bidirectional_iterator_tag, bidirectional_iterator_tag, Compare comp)
{
    typedef reverse_iterator<BidrectionalIter1> rev_iter1;
    typedef reverse_iterator<BidrectionalIter2> rev_iter2;
    rev_iter1 rlast1(first1); // 构造反向迭代器对象
    rev_iter2 rlast2(first2);
    rev_iter1 rresult = MyStl::search(rev_iter1(last1), rlast1, rev_iter2(last2), rlast2, comp);
    if (rresult == rlast1)
    {
        return last1;
    }
    else 
    {
        auto result = rresult.base();
        MyStl::advance(result, MyStl::distance(first2, last2));
        return result; // 逆序查找，然后进行指向看透
    }

}


template <class ForwardIter1, class ForwardIter2, class Compare>
ForwardIter1
find_end(ForwardIter1 first1, ForwardIter2 last1,
         ForwardIter2 first2, ForwardIter2 last2, Compare comp)
{
    typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
    typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
    return MyStl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), comp);
}

/*****************************************************************************************/
// find_first_of
// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2)
{
    // search只提供了双方都是ForwardITer版本的迭代器版本
    for(; first1 != last1; ++first1)
    {
        for (auto current = first2; current != last2; ++current)
        {
            if (*first1 == *current)
                return first1;
        }
    }
    return last1;
}

template <class InputIter, class ForwardIter, class Compare>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2, Compare comp)
{
    // search只提供了双方都是ForwardITer版本的迭代器版本
    for(; first1 != last1; ++first1)
    {
        for (auto current = first2; current != last2; ++current)
        {
            if (comp(*first1, *current))
                return first1;
        }
    }
    return last1;
}


/*****************************************************************************************/
// for_each
// 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
// f() 可返回一个值，但该值会被忽略
/*****************************************************************************************/
template <class InputIter, class Function>
Function
for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
    {
        f(*first);
    }
    return f;
}

/*****************************************************************************************/
// adjacent_find
// 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter
adjacent_find(ForwardIter first, ForwardIter last)
{
    if (first == last) return last;
    auto next = first;
    while (++next != last)
    {
        if (*first == *next) return first;
        first = next;
    }
    return last;
}

template <class ForwardIter, class Compare>
ForwardIter
adjacent_find(ForwardIter first, ForwardIter last, Compare comp)
{
    if (first == last) return last;
    auto next = first;
    while (++next != last)
    {
        if (comp(first, next))
            return first;
        first = next;
    }
    return last;
}

/*****************************************************************************************/
// lower_bound
// 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
// The elements in the range shall already be sorted according to this same criterion or at least partitioned with respect to val
/*****************************************************************************************/
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last, const T &value, forward_iterator_tag)
{
    // 二分查找法
    auto len = MyStl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (*middle < value)
        {
            first = middle;
            ++first;
            len = len - half - 1;

        }
        else
        {
            len = half;
        }
        return first;
    }
}

template <class RandomIter, class T>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last, const T &value, random_access_iterator_tag)
{

    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (*middle < value)
        {
            first = middle + 1;
            len = len - half - 1;
        }
        else
        {
            len = half;
        }
        
    }
    return first;
}

template <class ForwardIter, class T>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value)
{
    return MyStl::lbound_dispatch(first, last, value, iterator_category(first));
}


template <class ForwardIter, class T, class Compare>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last, const T &value, forward_iterator_tag, Compare comp)
{
    // 二分查找法
    auto len = MyStl::distance(first, last);
    auto half = len;
    ForwardIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (comp(*middle, value))
        {
            first = middle + 1;
            len = len - half - 1;

        }
        else
        {
            len = half;
        }
        return first;
    }
}

template <class RandomIter, class T, class Compare>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last, const T &value, random_access_iterator_tag, Compare comp)
{

    auto len = last - first;
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (comp(*middle, value))
        {
            first = middle + 1;
            len = len - half - 1;
        }
        else
        {
            len = half;
        }
        
    }
    return first;
}

template <class ForwardIter, class T, class Compare>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value, Compare comp)
{
    return MyStl::lbound_dispatch(first, last, value, iterator_category(first), comp);
}


/*****************************************************************************************/
// upper_bound
// 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/

// ubound_dispatch的forward_iterator_tag版本
template <class ForwardIter, class T>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last, const T &value, forward_iterator_tag)
{
    if (first == last) return last;
    auto len = MyStl::distance(first, last);
    ForwardIter middle;
    auto half = len;
    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (*middle <= value)
        {
            first = middle + 1;
            len = len - half - 1;
        }
        else
        {
            len = len - half;

        }
    }
    return first;
}


// ubound_dispatch的random_access_iterator_tag版本
template <class RandomIter, class T>
RandomIter
ubound_dispatch(RandomIter first, RandomIter last, const T &value, random_access_iterator_tag)
{
    if (first == last) return last;
    auto len = MyStl::distance(first, last);
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (*middle <= value)
        {
            first = middle + 1;
            len = len - half - 1;
        }
        else
        {
            len = len - half;
        }
    }
    return first;
}

template <class ForwardIter, class T>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T &value)
{
    return MyStl::ubound_dispatch(first, last, value, iterator_category(first));
}


template <class ForwardIter, class T, class Compare>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last, const T &value, forward_iterator_tag, Compare comp)
{
    if (first == last) return last;
    auto len = MyStl::distance(first, last);
    ForwardIter middle;
    auto half = len;
    while(len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (comp(value, *middle))
        {
            len = len - half;
        }
        else
        {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}


// ubound_dispatch的random_access_iterator_tag版本
template <class RandomIter, class T, class Compare> // 小于为true
RandomIter
ubound_dispatch(RandomIter first, RandomIter last, const T &value, random_access_iterator_tag, Compare comp)
{
    if (first == last) return last;
    auto len = MyStl::distance(first, last);
    auto half = len;
    RandomIter middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first;
        MyStl::advance(middle, half);
        if (comp(value, *middle))
        {
            len = len - half;
        }
        else
        {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}

template <class ForwardIter, class T, class Compare>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T &value, Compare comp)
{
    return MyStl::ubound_dispatch(first, last, value, iterator_category(first), comp);
}


/*****************************************************************************************/
// binary_search
// 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
/*****************************************************************************************/

template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T &value)
{
    auto res = MyStl::lower_bound(first, last, value);
    return res != last && *res == value; 
}

template <class ForwardIter, class T, class Compare>
bool binary_search(ForwardIter first, ForwardIter last, const T &value, Compare comp) // comp 小于为true
{
    auto res = MyStl::lower_bound(first, last, value);
    return res != last && !comp(value, *res); 
}


/*****************************************************************************************/
// equal_range
// 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
// 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
/*****************************************************************************************/
// erange_dispatch 的 forward_iterator_tag 版本
template <class ForwardITer, class T>
MyStl::pair<ForwardITer, ForwardITer>
erange_dispatch(ForwardITer first, ForwardITer last, const T &value, forward_iterator_tag)
{
    auto iter1 = MyStl::lower_bound(first, last, value);
    if (iter1 == last) return pair<ForwardITer, ForwardITer>(last, last);
    auto iter2 = MyStl::upper_bound(first, last, value);
    return pair<ForwardITer, ForwardITer>(iter1, iter2);

}

// erange_dispatch 的 random_access_iterator_tag 版本

template <class RandomIter, class T>
MyStl::pair<RandomIter, RandomIter>
erange_dispatch(RandomIter first, RandomIter last, const T &value, random_access_iterator_tag)
{
    auto iter1 = MyStl::lower_bound(first, last, value);
    if (iter1 == last) return pair<RandomIter, RandomIter>(last, last);
    auto iter2 = MyStl::upper_bound(first, last, value);
    return pair<RandomIter, RandomIter>(iter1, iter2);

}

template <class ForwardITer, class T>
MyStl::pair<ForwardITer, ForwardITer>
equal_range(ForwardITer first, ForwardITer last, const T &value)
{
    return erange_dispatch(first, last, value, iterator_category(first));
}










}

#endif

