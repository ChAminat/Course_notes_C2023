#include<iostream>

//static potymorphism
//точная подстановка лучше приведения типа
//частная точная версия лучше шаблона

template<typename T, typename U = int>
U f(T x) {
  std::cout << "Hello" << x;
  return x + 1;
}

void f(int x) {
  std::cout << "Buy" << x;
}

int main() {
  f(0);
  f<int>(0); //явно просим выбрать шаблон
}