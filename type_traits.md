# type_traits.h

## 别名

提供给iterators.h中的类型萃取器使用

~~~C++
// 定义m_integral_constant<bool, b> 特化模板别名
using m_bool_constant = m_integral_constant<bool ,b>;
// 定义ture和false
typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;
~~~



## 模板

~~~C++
template <class T, T v>
  struct m_integral_constant
  {
    static constexpr T value = v; // 仅记录true/false
  };
~~~

### 继承方式

Derived struct --->[true] m_bool_constant<true> 

​						  --->[true] m_integral_constant<bool, bool true>(value = true)

Derived struct --->[false] m_bool_constant<false> 

​						   --->[false]  m_integral_constant<bool,bool false>(value = false)