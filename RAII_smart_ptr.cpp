#include<iostream>

//7.4 RAII idiom and idea of smart pointer.
//Resource acquesition is initialization!

struct SmartPtr {
  int* p;
  SmartPtr(int* p): p(p) {}
  ~SmartPtr() {
    delete p;
  }

};

void g(int x) {
  if (x == 0) {
    throw std::runtime_error("Boom!");
  }
}

int f(int x) {
  SmartPtr p(new int(5));
  g(x);
  return 0;
}

////////////////////////////////////////
//function-try block

void g(int x) noexcept try {
  if (x == 0) {
    throw std::runtime_error("Boom!");
  }
} catch (...) {

}

struct B {
  B() { std::cout << "B"; }
  ~B() noexcept(false) {
    if (!std::uncaught_exception()) {
      g(0);
    }
    std::cout << "~B";
  }
};

struct A {
  B b;
  A(int x) try: b() {
    if (x == 0) {
      throw std::string("OOoops!");
    }
  } catch (...) {

  }

  ~A() {std::cout << "~A";}
};


//7.5 Exception specification and exception safety


int main() {
  f(0);
}