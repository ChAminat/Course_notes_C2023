#include<iostream>

//6.5 Basic compile-time computations.

template<size_t N>
struct Fibonacci {
  static const size_t value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<1> {
  static const size_t value = 1;
};

template<>
struct Fibonacci<0> {
  static const size_t value = 0;
};

//шаблонная переменная
template<size_t N>
const size_t Fib = Fibonacci<N>::value;

int main() {
  std::cout << Fibonacci<20>::value << '\n';
  std::cout << Fib<10>;
}