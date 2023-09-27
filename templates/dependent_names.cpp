#include<iostream>
#include<vector>

//6.6 Dependent names problem.

template<typename T>
struct Fib {};


template<typename T>
struct S {
  //using A = int;
  template<typename N>
  using A = Fib<N>;
};

template<>
struct S<int> {
  static const size_t A = 25;
};

int x = 10;

template<typename T>
void f(std::vector<T>& v) {
  typename S<T>::A * x; //A is dependent name
  typename std::vector<T>::iterator it = v.begin();

  typename S<T>::template A<10> x;

}