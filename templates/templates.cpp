#include <iostream>

template<typename T, typename U>
T max(const T& x, const U& y) {
  return x > y ? x : y;
}

template <typename T>
void swap(T& x, T& y) {
  T tmp = x;
  x = y;
  y = tmp;
}

// STL - standard template labrary
template<typename T>
class vector {
private:
  T* arr;
  size_t sz;
  size_t cap;

public:
  //static const int base = 1;

  template<typename U>
  void push_back(const U& x);
};

template<typename T>
template<typename U>
void vector<T>::push_back(const U& x) {
  
}

//since c++11
template<typename T>
using mymap = std::map<T, T>;

//since c++14
//template<typename T>
//const int base = vector<T>::base;

int main() {
  //mymap<int> m;
}
