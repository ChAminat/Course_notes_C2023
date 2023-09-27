#include<iostream>

//9.4 New/delete overloading.
//container -> allocator_traits -> allocator -> new (?) -> malloc

//9.5 Alignments.

void* operator new(size_t count) {
  std::cout << count;
  return malloc(count);
}

void* operator new[](size_t count) {
  std::cout << count;
  return malloc(count);
}

void* operator new(size_t count, const std::nothrow_t&) noexcept {
  std::cout << count;
  return malloc(count);
}

struct S {
  S () { std::cout << "create!"; }
  ~S() {std::cout << "destroyed!";}

  static void* operator new(size_t count) {
    std::cout << "AAAAA";
    return malloc(count);
  }

  static void operator delete(void* ptr) {
    std::cout << "BBBB";
    free(ptr);
  }

};

int main() {
  S* ps = new S[5];
  delete[] ps;
}