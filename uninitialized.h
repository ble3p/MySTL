#ifndef MYSTL_UNINITIALIZED_H_
#define MYSTL_UNINITIALIZED_H_
// 这个头文件用于对未初始化空间构造元素

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace MyStl
{

/*****************************************************************************************/
// uninitialized_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return MyStl::copy(first, last ,result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for(; first != last; ++first, ++cur)
        {
            MyStl::construct(&*cur, *first);
        }
    }
    catch(...)
    {
        // 中途创建失败就全部销毁
        for(; result != cur; ++result)
            MyStl:destroy(&*result);
        throw;
    }
    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    return MyStl::unchecked_uninit_copy(first, last, result,
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<ForwardIter>::
                                        value_type>{});
    // is_trivially_copy_assignable<> {} 初始化模板，返回一个模板std::false_type或std::true_type
}

/*****************************************************************************************/
// uninitialized_copy_n
// 把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
// 平凡拷贝直接拷贝
template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::true_type) {
    return MyStl::copy_n(first, n, result).second; // 返回result结束位置的指针
}

// 非平凡拷贝交给构造器申领空间进行
template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for(; n > 0; --n, ++cur, ++first)
        {
            MyStl::construct(&*cur, *first);
        }
    }
    catch(...)
    {
        for (; result != cur; ++result)
            MyStl::destroy(&*result);
        throw;
    }
    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
uninitialized_copy_n(InputIter first, Size n, ForwardIter result) 
{
    return unchecked_copy_n(first, n, result,
                            std::is_trivially_copy_assignable<
                            typename MyStl::iterator_traits<ForwardIter>::
                            value_type>{});
}


/*****************************************************************************************/
// uninitialized_fill
// 在 [first, last) 区间内填充元素值
/*****************************************************************************************/
template <class ForwardIter, class T>
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T &value, std::true_type)
{
    MyStl::fill(first, last, value);
}

template <class ForwardIter, class T>
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T &value, std::false_type)
{
    auto cur = first;
    try
    {
        for (; cur != last; ++cur)
        {
            MyStl::construct(&*cur, value);
        }
    }
    catch(...)
    {
        for(; cur != first; ++first)
        {
            MyStl::destroy(&*first);
        }
        throw;
    }
    
}

template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T &value)
{
    unchecked_uninit_fill(first, last, value, 
                          std::is_trivially_copy_assignable<
                          typename MyStl::iterator_traits<ForwardIter>::
                          value_type>{});
}


/*****************************************************************************************/
// uninitialized_fill_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter
unchecked_uninit_fill_n(ForwardIter first, Size n, const T &value, std::true_type)
{
    return MyStl::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter
unchecked_uninit_fill_n(ForwardIter first, Size n, const T &value, std::false_type)
{
    auto cur = first;
    try
    {
        for(; n > 0; ++cur, --n)
        {
            MyStl::construct(&*cur, value);
        }
    }
    catch(...)
    {
        for(; cur != first; ++first)
        {
            MyStl::destroy(&*first);
        }
        throw;
    }

    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, const T &value)
{
    return uncehcked_uninit_fill_n(first, n, value,
                                   std::is_trivially_copy_assignable<
                                   typename MyStl::iterator_traits<ForwardIter>::
                                   value_type>{});
}

/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template<class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return MyStl::move(first, last, result);
}

template<class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for (; first != last; ++first, ++cur)
        {
            MyStl::construct(&*cur, MyStl::move(*first)); // 由forward抛出一个bad alloc异常, 前提是采取...右值参数construct 调用右值forward
            
        }

    }
    catch(...)
    {
        MyStl::destroy(result, cur);
        throw;
    }
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
{
    return MyStl::unchecked_uninit_move(first, last, result,
                                 std::is_trivially_move_assignable<
                                 typename iterator_traits<InputIter>::
                                 value_type>{});
}

/*****************************************************************************************/
// uninitialized_move_n
// 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
{
    return MyStl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for (;n > 0; --n, ++cur, ++ first)
        {
            MyStl::construct(&*cur, MyStl::move(*first));
        }
    }
    catch(...)
    {
        for (;cur != result; ++result)
        {
            MyStl::destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
{
  return MyStl::unchecked_uninit_move_n(first, n, result,
                                        std::is_trivially_move_assignable<
                                        typename iterator_traits<InputIter>::
                                        value_type>{});
}


}
#endif
