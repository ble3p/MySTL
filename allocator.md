# allocator.h

## 模板

allocator模板

~~~C++
 template <class T>
  class allocator
  {
    public:
      // 此为allocator的标准接口定义
      typedef T           value_type;
      typedef T*          pointer;
      typedef const T*    const_pointer;
      typedef T&          reference;
      typedef T&&         right_reference;
      typedef const T&    const_reference;
      typedef size_t      size_type;
      typedef ptrdiff_t   difference_type;

    public:
      static T*   allocate();
      static T*   allocate(size_type n);

      static void deallocate(pointer ptr);
      static void deallocate(pointer ptr, size_type n);

      static void construct(pointer ptr);
      static void construct(pointer ptr, const_reference value);
      static void construct(pointer ptr, right_reference value); // 对右值的引用

      template <class ...Args>
      static void construct(pointer ptr, Args&& ...args);
      
      static void destroy(pointer ptr);
      static void destroy(pointer first, pointer last);
  };
~~~

## 函数列表

以下均包含`tamplate< class T>`

| 函数名称                         | 函数功能 |
| -------------------------------- | -------- |
| allocate() | 通过::operator new 分配一个T类型大小的空间 |
| allocate(size_type n) | 通过::operator new 分配n个T类型大小的空间 |
| deallocate(T *ptr)               | 通过::operator delete 删除ptr所指空间 |
| deallocate(T *ptr， size_type n)  | 通过::operator delete 删除ptr所指空间，通过内存chunk或打标记的方式获取删除空间的大小，不需要使用size_type n,只是用来作函数重载 |
| construct(T *ptr)                | 调用construct.h中的construct方法，使用placenew调用Ty的构造函数并放置在ptr指向的空间上 |
| construct(T *ptr, const_reference value) | 调用construct.h中的construct方法，使用placenew调用Ty的构造函数，向构造函数传入value，并放置在ptr指向的空间上 |
| construct(T *ptr, right_reference value) | 调用construct.h中的construct方法，使用placenew调用Ty的构造函数，向构造函数传入move(value)，也就是调用移动构造函数, 并放置在ptr指向的空间上 |
| construct(T *ptr, Arg&& ...args) | 调用construct.h中的construct方法，使用forward转发引用给Ty的构造函数 |
| destory(T *ptr)                  | 调用construct.h中的destroy方法,如果能平凡析构，就什么都不做；否则，调用该指针的指向对象的析构函数 |
| destory(T *first T *last)        | 调用construct.h中的destroy方法,如果能平凡析构，就什么都不做；否则，按顺序调用范围内每个指针指向对象的析构函数 |

