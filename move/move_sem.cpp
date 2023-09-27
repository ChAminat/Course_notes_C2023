#include<iostream>
#include<vector>
#include<string>

//X. Move semantics and rvalue references.
//10.1 Idea and basic usages.
//10.3 std::move implementation

// T& & -> T&
// T&& & -> T&
// T& && -> T&
// T&& && -> T&&

//10.6 Perfect forwarding and std::forward.

template<typename T>
T&& naive_move(T& x) noexcept {
  return static_cast<T&&>(x);
}

template<typename T>
std::remove_reference_t<T>&& move(T&& x) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(x);
}

template<typename T>
T&& forward(std::remove_reference_t<T>& x) noexcept {
  return static_cast<T&&>(x);
}

template<typename T>
T&& forward(std::remove_reference_t<T>&& x) noexcept {
  static_assert(!std::is_lvalue_reference_v<T>);
  return static_cast<T&&>(x);
}

template<typename T>
std::conditional_t<std::is_nothrow_move_constructible<T>::value, T&&, const T&>
move_if_noexept(T& x) noexcept {
  return move(x);
}

template<typename T>
void swap(T& x, T& y) {
  T tmp = std::move(x);
  x = std::move(y);
  y = std::move(tmp);
}

int main() {
  std::vector<std::string> v;
  std::string s = "asdfgh";
  v.push_back(std::move(s));
  std::cout << s.size(); //0

  int x = 2, y = 3;
  swap(x, y);
  std::cout << x << y;
}