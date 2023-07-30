# construct.h

## 函数列表

| 函数名称           | 函数功能                                            |
| ------------------ | --------------------------------------------------- |
| construct(Ty *ptr) | 使用placenew调用Ty的构造函数并放置在ptr指向的空间上 |
| construct(Ty1 *ptr, const Ty2 &value) | 使用placenew调用Ty的构造函数，向构造函数传入value，并放置在ptr指向的空间上|
| construct(Ty *ptr, Args &&... args) | 使用forward转发引用给Ty的构造函数 |
| destroy(Ty *pointer) | 如果能平凡析构，就什么都不做；否则，调用该指针的指向对象的析构函数 |
| destory(ForwardIter first, ForwardIter last) | 如果能平凡析构，就什么都不做；否则，按顺序调用范围内每个指针指向对象的析构函数|