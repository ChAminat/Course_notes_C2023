#include<iostream>

//8.6 std::list

template<typename T>
struct allocator {
  T* allocate(size_t count) {
    return reinterpret_cast<T*>(new char[count * sizeof(T)]);
  }

  void deallocate(T* ptr, size_t count) {
    delete[] reinterpret_cast<char*>(ptr);
  }

  template<typename... Args>
  void construct(T* ptr, const Args&... args) {
    new (ptr) T(args...);
  }

  void destroy(T* ptr) {
    ptr->~T();
  }

  template<typename U>
  struct rebind {
    using other = allocator<U>;
  };

  template<typename U>
  allocator(const allocator<U>&) {}

  template<typename U>
  allocator& operator=(const allocator<U>&) { return *this; }
};



template<typename T, typename Alloc = std::allocator<T>>
class List {
private:
  struct BaseNode {
    BaseNode* next;
    BaseNode* prev;
  };
  struct Node: BaseNode {
    T value;
  };
  BaseNode fakeNode; //хранится как поле, так как не нужно выделять динам память

  //using NodeAlloc = typename Alloc::template rebind<BaseNode>::other;
  using NodeAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<BaseNode>;
  NodeAlloc nodeAlloc;
public:
  List() {}

  List(const Alloc& alloc = Alloc()): nodeAlloc(alloc) {}

  struct Iterator {
    BaseNode* node;
    //TODO: const and reverce iters
    Iterator& operator++() {
      node = node->next();
      return *this;
    }
    Iterator& operator--() {
      node = node->prev();
      return *this;
    }
  };
};

int main() {
  
}