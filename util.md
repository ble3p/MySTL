# Util.h

## 模板

pair结构体模板，存入两个类型值,两个类型可以相同也可以不同

~~~C++
template <class Ty1, class Ty2>
  struct pair
  {
    typedef Ty1       first_type;
    typedef Ty2       second_type;

    first_type first;
    second_type second;
  }
~~~

### 模板函数

| 函数名称 | 函数功能 |
| -------- | -------- |
| 构造函数 |          |



## 功能函数

| 函数名称 | 函数功能     |
| -------- | ------------ |
| move     | 返回右值引用 |
| forward | 将左值返回成左值，右值返回成右值,完美转发|
| swap | 使用移动构造函数和移动赋值运算符来进行交换|
| swap_range | 交换两个容器中的一定范围的值,长度应相等|