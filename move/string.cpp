#include<iostream>
#include<cstring>


//10.2 Move sem support for custom types.
class String {
private:
  char* arr;
  size_t sz;
  size_t cap;
public:
  String(size_t n, char chr): arr(new char[n]), sz(n), cap(n) {
    memset(arr, chr, n);
  }

  String(const String& other): String(other.sz, '\0') {
    memcpy(arr, other.arr, sz);
  }

  String(String&& s): arr(s.arr), sz(s.sz), cap(s.cap) {
    s.arr = nullptr;
    s.sz = s.cap = 0;
  }

  String& operator=(String&& s) & { //& запрешает присв объекту типа (rvalue) String("asd") = "dlld;"
    String copy = std::move(s);
    swap(copy);
    return *this;
  }

  String& operator=(const String& other) & {
    String copy = other;
    swap(copy);
    return *this;
  }

  void swap(String& other) {
    std::swap(arr, other.arr);
    std::swap(sz, other.sz);
    std::swap(cap, other.cap);
  }
};

int main() {

}