#include<iostream>
#include<vector>

template<typename T>
class Optional{
private:
  char objeckt[sizeof(T)];
  bool exist = false;
public:
  Optional() {};
  Optional (const T& obj) exist(true) {
    T* aaa = reinterpret_cast<T*>(objeckt);
    new(aaa) T(obj);
  }

  Optional(T&& obj) {
    T* aaa = reinterpret_cast<T*>(objeckt);
    new(aaa) T(std::move(obj));s
  }

  Optional(const Optional& other): exist(other.exist) {
    if (exist) {
      T* aaa = reinterpret_cast<T*>(objeckt);
      new(aaa) T(obj);
    }
  }

  Optional(Optional&& other): exist(other.exist) {
    other.exist = false;
    if (exist) {
      T* aaa = reinterpret_cast<T*>(objeckt);
      new(aaa) T(std::move(obj));
    }
  }

  Optional& operator=(const Optional& other){
    reset();
    exist = other.exist;
    if (exist) {
      T* aaa = reinterpret_cast<T*>(objeckt);
      new(aaa) T(obj);
    }
    return *this;
  }

  Optional& operator=(Optional&& other) {
    reset();
    exist = other.exist;
    if (exist) {
      T* aaa = reinterpret_cast<T*>(objeckt);
      new(aaa) T(std::move(obj));
    }
    return *this;
  }

  Optional& operator=(const T& new_obj){
    objeckt = new_obj;
    exist = true;
    return *this;
  }

  Optional& operator=(T&& new_obj){
    objeckt = std::move(new_obj);
    exist = true;
    return *this;
  }

  T& operator*() {
    return objeckt;
  }

  const T* operator->() const {
    return objeckt;
  }

  void reset() {
    // if (exist)
    objeckt->~T(); //
    exist = false;
  }

  operator bool() {
    return exist;
  }
  
  ~Optional() {
    reset();
  }
};

int main() {
  std::vector<int> vs = {1, 2, 3};
  Optional<std::vector<int>> op = vs;
}