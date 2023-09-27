#include<iostream>

//6.8 variadic templates (sicnce c++11

template<typename... Types> //typename1 Type1, typnm2 Type2 ...
void f(const Types&... args) { // Type1 arg1, Type2 arg2, ...
  g(args...); //arg1, arg2 ...
} 

template<typename T>
void print(const T& x) {
  std::cout << x;
}

template<typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
  //static_assert(std::is_same_v<Head, int>);
  std::cout << head << " ";
  print(tail...);
}

template<typename Head, typename... Tail>
void print1(const Head& head, const Tail&... tail) {
  std::cout << head;
  if constexpr (sizeof...(tail) > 0) {
    std::cout << " ";
    print(tail...);
  }
}

//6.9 Fold expressions (since c++17)
template<typename... Args>
bool all_empty(const Args&... args) {
  return (args.is_empty() && ...); // arg1.() && arg2.()...
}

template<typename... Args>
void print3(const Args&... args) {
  //static_assert((!std::is_pointer_v<Args> && ...));
  ((std::cout << args << '\n'), ...);
}

int main() {
  print3(1, 2, 3, 4);
}
