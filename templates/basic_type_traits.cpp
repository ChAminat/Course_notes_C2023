#include <iostream>
#include <type_traits>

//6.7 Basic type traits

//since c++11
template <typename T, typename U>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static const bool value = true;
};

//since c++17
template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;

template <typename T>
void f(T x) {
  if constexpr (std::is_same<T, int>::value) {
    std::cout << x;
  }
}

struct S{};

///////////////////////////////////////////////////
template <typename T>
struct is_pointer {
  static const bool value = false;
};

template <typename T>
struct is_pointer<T*> {
  static const bool value = true;
};

template <typename T>
const bool is_pointer_v = is_pointer<T>::value;

//////////////////////////////////////////////////
template<typename T>
struct remove_reference {
  using type = T;
};

template<typename T>
struct remove_reference<T&> {
  using type = T;
};

template <typename T>
using remove_ref_t = typename remove_reference<T>::type;


//////////////////////////////////////////////////
template<bool B, typename T, typename F>
struct conditional {
  using type = F;
};

template<typename T, typename F>
struct conditional<true, T, F> {
  using type = T;
};

template<bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;


int main() {
  S s;
  f(s);

  int x = 5;
  int* p = &x;
  std::cout << is_pointer_v<int>;
  std::cout << is_same_v<int*, int>;
}