#ifndef SORTING_ALGO_HPP_
#define SORTING_ALGO_HPP_
#include <type_traits>
#include <iterator>
#include <iostream>
#include "vector.h"

/* ============================================冒泡排序=============================================== */
template <class RandomIter>
void bubble_sort(RandomIter first, RandomIter last, bool asc = true)
{
   bool flag;
   auto len = distance(first, last);
   for (size_t i = 0; i < len - 1; ++i)
   {
       flag = true; 
       for (size_t j = 0; j < len - 1 - i; ++j)
       {
           if (first[j] > first[j + 1] == asc)
           {
               swap(first[j], first[j + 1]);
               flag = false;
           }
       }
       if (flag) break;
       
   }
}

/* ============================================选择排序=============================================== */
template <class RandomIter>
void selection_sort(RandomIter first, RandomIter last, bool asc = true)
{
    auto len = distance(first, last);
    size_t index;
    for (size_t i = 0; i < len - 1; ++i)
    {
        index = i;
        for (size_t j = i + 1; j < len; ++j)
        {
            if (first[j] < first[index] == asc)
            {
                index = j;
            }
            
        }
        if (index != i)
            swap(first[i], first[index]);
    }
}
template <class RandomIter, class T>
size_t get_index(RandomIter first, RandomIter last, const T &value, bool asc = true)
{
    // 二分查找获得待插入的index
    auto beg_ = first;
    auto end_ = last;
    auto len = distance(beg_, end_);
    
    if (len == 0) return 0;
    
    auto half = len / 2;
    auto mid = first;
    advance(mid, half);
    
    if (value == *mid) return mid - first; // 相等在其前面插入
    else if (value > *mid == asc) 
    {
        if (mid + 1 == end_ || value < *(mid + 1) == asc)
        {
            return mid + 1 - first;
        }
        beg_ = mid;
        return mid - first + get_index(beg_, end_, value, asc);
    }
    else if (value < *mid == asc)
    {
        if (mid == beg_ || value > *(mid - 1) == asc)
        {
            return mid - first;
        }
        end_ = mid;
        return get_index(beg_, end_, value, asc);
    }
    return 0;
}

/* ============================================插入排序=============================================== */
template <class RandomIter>
void insertion_sort(RandomIter first, RandomIter last, bool asc = true)
{
    size_t index;
    
    auto len = distance(first, last);
    for (size_t i = 1; i < len; ++i)
    {
        // 获取当前待插入的值
        auto val = first[i];
        // 获取待插入的值的index；j
        index = get_index(first, first + i, val, asc);
        // 进行插入
        for (size_t j = i; j > index; --j)
        {
            first[j] = first[j - 1];
        }
        first[index] = val;
        
    }
    
}


/* ============================================shell排序=============================================== */
template <class RandomIter>
void shell_sort(RandomIter first, RandomIter last, bool asc = true)
{
    auto len = distance(first, last);
    
    // 间隔
    for (int i = len / 2; i >= 1; i /= 2 )
    {
        for (int j = i; j < len; ++j)
        {
            auto tmp = first[j];
            auto index = j;
            while (index - i >= 0 && tmp < first[index - i ] == asc)
            {
                    first[index] = first[index - i];
                    index = index - i;
            }
            if (j != index)
                first[index] = tmp;
        }
    }
    
}

/* ============================================quick排序=============================================== */
template <class RandomIter>
void quick_sort(RandomIter first, RandomIter last, bool asc = true)
{
    auto len = distance(first, last);
    if (len <= 1) return;
    auto start_ = first;
    auto end_  = last - 1;
    bool reverse = true;
    auto tmp = *start_;
    while (start_ != end_)
    {
        // 选定第一个为pivot
        if (reverse)
        {
            // 从右往左找第一个比轴小的，交换
            while (end_ != start_)
            {
                if (*end_ < tmp == asc)
                {
                    *start_ = *end_;
                    reverse = false;
                    ++start_;
                    break;
                }
                --end_;
            }
        }
        else 
        {
            while (start_ != end_)
            {
                if (*start_ > tmp == asc)
                {
                    *end_ = *start_;
                    reverse = true;
                    --end_;
                    break;
                }
                ++start_;
            }
        }
    }
    *start_ = tmp;
    // 换完一趟, 按轴为分界
    quick_sort(first, start_, asc);
    quick_sort(start_ + 1, last, asc);
}
/* ============================================merge排序=============================================== */
template <class RandomIter>
void merge(RandomIter f1, RandomIter l1, RandomIter f2, RandomIter l2,bool asc = true)
{

    auto len1 = distance(f1, l1), len2 = distance(f2, l2);
    auto tmp = *f1;
    MyStl::vector<decltype(tmp)> vec;
    auto it1 = f1;
    auto it2 = f2;
    while (it1 != l1 && it2 != l2)
    {
        if (*it1 <= *it2 == asc)
        {
            vec.push_back(*it1);
            ++it1;
        }
        else if (*it2 <= *it1 == asc)
        {
            vec.push_back(*it2);
            ++it2;
        }
    }
    // 总有一个先提取完
    while (it1 != l1)
    {
        vec.push_back(*it1);
        ++it1;
    }
    while (it2 != l2)
    {
        vec.push_back(*it2);
        ++it2;
    }
    
    // 按顺序给原来的数组赋值
    for (auto it = f1; it != l1; ++it)
    {
        *it = vec[it - f1];
    }

    for (auto it = f2; it != l2; ++it)
    {
        *it = vec[l1 - f1 + it - f2];
    }
}

template <class RandomIter>
void merge_sort(RandomIter first, RandomIter last, bool asc = true)
{
    auto len = distance(first, last);
    if (len <= 1) return;
    
    auto half = len / 2;
    auto mid = first + half;

    merge_sort(first, mid, asc);
    merge_sort(mid, last, asc);

    merge(first, mid, mid ,last, asc);
    
}
#endif
