# iterator迭代器文件结构

## 迭代器类型

迭代器类型共五种:

- input_iterator_tag
- output_iterator_tag
- forward_iterator_tag : input_iterator_tag
- bidirectional_iterator_tag : forward_iterator_tag
- random_access_iterator_tag : bidirectional_iterator_tag
![image-20230730220349627](iterator.assets/image-20230730220349627.png)
> ​	其中，input 和 output迭代器是最受限的迭代器，他们可以执行顺序的单程输入或输出操作.
> ​	forward迭代器具有input迭代器的所有功能，如果不是常量迭代器，还具有输出迭代器的功能.
>
> ​	bidirectional迭代器很像forward迭代器但是也能向后进行操作
>
> ​	random-access迭代器实现了全部的bidirectional迭代器功能，同时支持随机访问；random-access迭代器和标准指针有很相似的功能

![image-20230730221203657](iterator.assets/image-20230730221203657.png)

## iterator模板展示

全部模板展示

~~~C++
// 五种迭代器类型
struct input_iterator_tag {}
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// iterator 模板
template <class Category, class T, class Distance = ptrdiff_t,
         class Pointer = T*, class Reference = T&>
struct iterator
{
    // 迭代器特性
  typedef Category              iterator_category;
  typedef T                     value_type;
  typedef Pointer               pointer;
  typedef Reference             reference;
  typedef Distance              difference_type;
};

// 萃取器
template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
  typedef typename Iterator::iterator_category  iterator_category;
  typedef typename Iterator::value_type         value_type;
  typedef typename Iterator::pointer            pointer;
  typedef typename Iterator::reference          reference;
  typedef typename Iterator::difference_type    difference_type;
};
~~~



## 相关函数

​																	**迭代器相关函数**

| 功能名称 | 功能描述           |
| -------- | ------------------ |
| distance | 计算迭代器间的距离 |
| advance | 用于让迭代器前进n个距离 |

## 萃取器

### 萃取器概念

> 萃取器针对iterator进行操作，萃取出iterator模板中的属性以供其他功能使用

几种萃取方式

1. iterator_traits<Iterator> 萃取迭代器特性
2. `is_input/output/forward/bidrectional/random_access_iterator<Iterator> `萃取迭代器类型
3. 特性萃取函数
    - `iterator_category(const Iterator&)`
    - `distance_type(const Iterator&)`
    - `value_type(const Iterator&)`

### 原理剖析

#### 符号约定

--->[true] 如果条件满足时继承 例： a --->[true] b // 条件满足为true时a继承于b

--->[false] 条件不满足时继承

----别名 								  例:    a ---- b a是b的别名

#### 继承关系

1. 关于iterator_traits<Iterator>

通过结构体继承方式通过多次条件判断最终继承iterator_traits萃取器模板,以-> 代表继承于

 iterator_traits<Iterator> **--->[true]**  iterator_helper<Iterator, has_iterator_cat<Iterator>::value (true)> --->[true]  iterator_impl<Iterator, true>(为最终模板) 

中途如果任一条件不满足则继承为空，不具有特性



2. 关于萃取迭代器类型

    以is_input_iterator为例，其他类型均为如此

    is_input_iterator --->[true] has_iterator_cat_of<Iterator, input_iterator_tag, bool = has_iterator_cat<iterator_traits<T>>::value(true) --->[true] m_bool_constant<true> (来自traits_type.h) ---> [true]  ---- m_integral_constant<bool, bool true>

3. 关于萃取函数 

    category函数体实现

    ~~~c++
    typedef typename iterator_traits<
        				Iterator>::iterator_category Category;
    return Category(); // 返回通过Category类型的牧人构造函数创建的一个对象
    ~~~
    
    distance_type/ value_type函数体实现
    
    ~~~C++
    return static_cast<
       				typename iterator_traits<
        			Iterator>::difference_type*>(0); 
    // 返回一个指向distance_type的指针
    ~~~
    
    



