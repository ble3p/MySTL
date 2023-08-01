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

// 每一个都有一个value代表true或者false, 并且最终都会继承到type_traits中的m_integral_constant,类中只包含一个bool值，在不断向上继承的过程中由std::is_convertible通过比较萃取出来的iterator_category和给定的category_tag是否一致来决定bool值是true还是false
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
  return Category(); // 并不是一个函数调用，而是通过Category类型的默认构造函数来创建一个Category类型的对象并返回，默认构造函数可能没有任何具体的实现。因此，这个操作实际上是在返回一个表示迭代器类别的对象，而不是调用任何函数
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

// 以下函数用于计算迭代器间的距离

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        ++first;
        ++n;
    }
    return n;
}

// distance 的random_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last,
                  random_access_iterator_tag)
{
    return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}

// 以下函数用于让迭代器前进n个距离

// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator &i, Distance n, input_iterator_tag)
{
    while (n--)
        ++i;
}

// advance 的 bidirectional_iterator_tag的版本
template <class BidrectionalIterator, class Distance>
void advance_dispatch(BidrectionalIterator &i, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
        while (n--) ++i;
    else
        while (n++) --i;
}

// advance 的 random_access_iterator_tag 的版本
template <class BidrectionalIterator, class Distance>
void advance_dispatch(BidrectionalIterator &i, Distance n, random_access_iterator_tag)
{
    i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator &i, Distance n)
{
    advance_dispatch(i, n, iterator_category(i));
}


/*****************************************************************************************/

// 模板类 : reverse_iterator
// 代表反向迭代器，使前进为后退，后退为前进
template <class Iterator>
class reverse_iterator
{
private:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::pointer           pointer;
    typedef typename iterator_traits<Iterator>::reference         reference;

    typedef Iterator                                              iterator_type;
    typedef reverse_iterator<Iterator>                            self;

public:
    reverse_iterator() = default;
    explicit reverse_iterator(iterator_type i) : current(i) {}
    reverse_iterator(const self& rhs) : current(rhs.current) {}

public:
    iterator_type base() const
    {
        return current;
    }

    // 对应正向迭代器的前一个位置,为了让rbegin和 end对齐，rend和begin对齐，所以正向迭代器指向为反向迭代器指向的后一个，要向前一位
    reference operator* () const
    {
        auto temp = current;
        return *--temp;
    }
    pointer operator->() const
    {
        return &(operator*());
    }

    self& operator++()
    {
        --current;
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--()
    {
        ++current;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator+=(difference_type n)
    {
        current -= n;
        return *this;
    }
    
    self operator+(difference_type n) const
    {
        return self(current - n); // 构造函数
    }

    // 返回的是第n个位置的指向的值的引用
    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }
};

// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
          const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() - lhs.base();
}

template<class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() == rhs.base();
}

template<class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() < rhs.base();
}
template<class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return !(lhs == rhs);
}
template<class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() > rhs.base();
}
template<class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return !(lhs > rhs);
}
template<class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return !(lhs < rhs);

} 

} // MyStl

#endif

