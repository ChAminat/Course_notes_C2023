#include<iostream>


//6.3 Templates specialization.

template<typename T>
class vector {
private:
  T* arr;
  size_t sz;
  size_t cap;
};

//Full spec.
template<>
class vector<bool> {
private:
  char* arr;
  size_t sz;
  size_t cap;
};

////////////////////////////////////////

template<typename T>
struct S {
  void f () {
    std::cout << 1;
  }
};

// Partial spec
template<typename T>
struct S<T*> {
  void f() {
    std::cout << 2;
  }
};

///////////////////////////////////////

template<typename T, typename U>
void f(T x, U y) {
  std::cout << 1;
}

template<>  //spec
void f(int x, int y) {
  std::cout << 2;
}

void f(int x, int y) { //overload
  std::cout << 3;
}

template<typename T> //overload
void f(T x) {
  std::cout << 4;
}


int main() {
  S<int> s;
  s.f(); // 1

  S<void*> ss;
  ss.f(); // 2

  f(5);
  f(1, 2);
}
