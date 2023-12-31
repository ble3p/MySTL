# MySTL
C++ implement subset of STL

## TODO

- [ ] memory.h 和 文件框架梳理
- [ ] uninitialiazed.h 和文件框架梳理
- [ ] testing file 用来测试algobase.h 中的基础算法
- [x] util.h iterator.h type_traits.h construct.h allocate.h 梳理

​    

## 概念解释


### 引用折叠

在C++11中，新加入右值的概念，所以引用的类型就有两种形式：左值引用`T&` 和 右值引用`T&&`

> 当在声明变量时，声明符&的数量多于2时，会发生引用折叠。对引用折叠可能的情况进行排列组合，可以得出如下四种情况:
>
> * T& & 			对左值引用进行左值引用，得到的结果仍是左值引用
> * T& && 	     对左值引用进行右值引用，得到的结果仍是左值引用
> * T&& &          对右值引用进行左值引用，得到的结果仍是左值引用
> * T&& &&       对右值引用进行右值引用，得到的结果是右值引用

所有的引用折叠最终都代表一个引用，要么是左值引用，要么是右值引用。规则为: 如果任一引用为左值引用，则结果为左值引用。否则，结果为右值引用。

引用折叠现象会发生在模板函数的参数推导上，由此可以引出万能引用

### 万能引用

万能引用既能接受左值类型的参数，也能接受右值类型的参数，所以叫万能引用。

~~~C++
// 万能引用的形式如下
template<typename T>
typename std::remove_reference<T>::type&&
move(T &&arg) noexcept
{
    ......
        
}
~~~

无论T被推导为引用时时哪种引用，都会根据引用折叠变成左值引用或右值引用的一种，这样，当我们既需要接受左值引用类型，又需要接受右值引用类型时，就不需要写两个重载函数了，只需要一个万能引用就可以实现。

### 完美传递


## 文件说明

### 关于iterator.h

[iterator.md](./iterator.md)

### 关于type_traits.h

[type_traits.h](./type_traits.md)

### 关于util.h  

[util.h](./util.md)

### 关于 construct.h

[construct.h](./construct.md)

### 关于 allocator.h

[allocator.h](./allocator.md)