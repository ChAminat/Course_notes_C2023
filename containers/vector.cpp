#include<iostream>
#include<vector>

//8.2 Internals of std::vector.
//9.2 allocator_traits
template<typename Alloc>
struct allocator_traits {
  template<typename... Args>
  static void construct(Alloc& alloc, typename Alloc::value_type* ptr, const Args&... args) {
    if constexpr(/*Alloc has method construct(Args...)*/) {
      alloc.construct(ptr, args...);
    } else {
      new (ptr) typename Alloc::value_type(args...);
    }
  }

  template<typename U>
  using rebind_alloc = /* ... */
};

template<typename T>
struct PoolAllocator {
private:
  T* pool;
  size_t shift = 0;
public:
  PoolAllocator(): pool(reinterpret_cast<T*>(new char[100'000 * sizeof(T)])) {}

  T* allocate(size_t count) {
    size_t old_shif = shift;
    shift += count;
    return pool + old_shif;
  }

  void deallocate(T* ptr) {
    //std::ignore = ptr;
  }

  PoolAllocator select_on_container_copy_construction() {
    return PoolAllocator();
  }

  // template<typename... Args>
  // void construct(T* ptr, const Args&... args) {
  //   new (ptr) T(args...);
  // }

  // void destroy(T* ptr) {
  //   ptr->~T();
  // }

  // template<typename U>
  // struct rebind {
  //   using other = allocator<U>;
  // }  

  ~PoolAllocator() {
    delete[] reinterpret_cast<char*>(pool);
  }

  //TODO: support copy-constr and copy-assign
};

template<typename T>
struct allocator {
  T* allocate(size_t count) {
    return reinterpret_cast<T*>(new char[count * sizeof(T)]);
  }

  void deallocate(T* ptr, size_t count) {
    delete[] reinterpret_cast<char*>(ptr);
  }

  //ушло в traits
  // template<typename... Args>
  // void construct(T* ptr, const Args&... args) {
  //   new (ptr) T(args...);
  // }

  // void destroy(T* ptr) {
  //   ptr->~T();
  // }

  // template<typename U>
  // struct rebind {
  //   using other = allocator<U>;
  // }
};

template<typename Iterator>
struct reverse_iterator {
private:
  Iterator iter;
public:
  reverse_iterator(const Iterator& iter): iter(iter) {}
  reverse_iterator<Iterator>& operator++() {
    --iter;
    return *this;
  }

  Iterator base() const {
    return iter;
  }
};

template<typename T, typename Alloc = std::allocator<T>>
class Vector{
private:
  Alloc alloc;
  T* arr;
  size_t sz;
  size_t cap;

public:

  template<bool IsConst>
  struct common_iterator {
  private:
    std::conditional_t<IsConst, const T*, T*> ptr;
  public:
    common_iterator(T* ptr): ptr(ptr) {}

    std::conditional_t<IsConst, const T&, T&> operator*() const {
      return *ptr;
    }
    std::conditional_t<IsConst, const T*, T*> operator->() {
      return ptr;
    }
    common_iterator& operator++() {
      ++ptr;
      return *this;
    }
  };

  using iterator = common_iterator<true>;
  using const_iterator = common_iterator<false>;  
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() const {
    return iterator(arr);
  }

  iterator end() const {
    return iterator(arr + sz);
  }

  reverse_iterator rbegin() const {
    return reverse_iterator(arr + sz - 1);
  }

  reverse_iterator rend() const {
    return reverse_iterator(arr - 1);
  }

  Vector(size_t count, const T& value) 
              : alloc(),
                arr(std::allocator_traits<Alloc>::allocate(alloc, count)), //arr(reinterpret_cast<T*>(new char[count * sizeof(T)])), //alloc.allocate(count)
                sz(count), 
                cap(count) {
    try {
      for (size_t i = 0; i < count; ++i) {
        //arr[i] = value; WRONG!!!
        //new (arr + i) T(value);
        std::allocator_traits<Alloc>::construct(alloc, arr + i, value);
      }
    } catch (...) {
      //delete[] arr;
      std::allocator_traits<Alloc>::deallocate(alloc, arr, count);
      throw;
    }
  }

  void reserve(size_t count) {
    if (cap >= count) return;

    //T* newarr = new T[count]; //WRONG!
    //T* newarr = reinterpret_cast<T*>(new char[count * sizeof(T)]);
    //T* newarr = alloc.allocate(count);
    T* newarr = std::allocator_traits<Alloc>::allocate(alloc, count);

    size_t i = 0;
    try {
      for (; i < sz; ++i) {
        //newarr[i] = arr[i]; //WRONG!!! uninisialised memory
        //new (newarr + i) T(arr[i]); //placement new
        //alloc.construct(newarr + i, arr[i]);
        std::allocator_traits<Alloc>::construct(alloc, newarr + i, arr[i]);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        //(newarr + j)->~T();
        //alloc.destroy(newarr + j);
        std::allocator_traits<Alloc>::destroy(alloc, newarr + j);
      }
      //delete[] reinterpret_cast<char*>(newarr);
      //alloc.deallocate(newarr, count);
      std::allocator_traits<Alloc>::deallocate(alloc, newarr, count);
      throw;
    }

    for (size_t i = 0; i < sz; ++i) {
      //(arr + i)->~T();
      //alloc.destroy(arr + i);
      std::allocator_traits<Alloc>::destroy(alloc, arr + i);
    }

    //delete[] reinterpret_cast<char*>(arr);
    //alloc.deallocate(arr, cap);
    std::allocator_traits<Alloc>::deallocate(alloc, arr, cap);
    arr = newarr;
    cap = count;
  }

  void push_back(const T& value) {
    if (sz >= cap) {
      //T* newarr = reinterpret_cast<T*>(new char[sizeof(T) * cap * 2]);
      T* newarr = alloc.allocate(cap * 2);

      size_t i = 0;
      try {
        for (; i < sz; ++i) {
          //new (newarr + i) T(arr[i]); //TODO: get rid of copying?
          alloc.construct(newarr + i, arr[i]);
        }
        //new (newarr + sz) T(value);
        alloc.construct(newarr + sz, value);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          //(newarr + j)->~T();
          alloc.destroy(newarr + j);
        }
        //delete[] reinterpret_cast<char*>(newarr);
        alloc.deallocate(newarr, cap * 2);
        throw;
      }

      for (size_t i = 0; i < sz; ++i) {
        //(arr + i)->~T();
        alloc.destroy(arr + i);
      }
      //delete[] reinterpret_cast<char*>(arr);
      alloc.deallocate(arr, cap);
      arr = newarr;
      cap *= 2;
    } else {
      //new (arr + sz) T(value);
      alloc.construct(arr + sz, value);
    }
    ++sz;
  }

  void push_back(T&& value) {
    if (sz >= cap) {
      //T* newarr = reinterpret_cast<T*>(new char[sizeof(T) * cap * 2]);
      T* newarr = alloc.allocate(cap * 2);

      size_t i = 0;
      try {
        for (; i < sz; ++i) {
          //new (newarr + i) T(arr[i]); //TODO: get rid of copying?
          alloc.construct(newarr + i, arr[i]);
        }
        //new (newarr + sz) T(value);
        alloc.construct(newarr + sz, std::move(value));
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          //(newarr + j)->~T();
          alloc.destroy(newarr + j);
        }
        //delete[] reinterpret_cast<char*>(newarr);
        alloc.deallocate(newarr, cap * 2);
        throw;
      }

      for (size_t i = 0; i < sz; ++i) {
        //(arr + i)->~T();
        alloc.destroy(arr + i);
      }
      //delete[] reinterpret_cast<char*>(arr);
      alloc.deallocate(arr, cap);
      arr = newarr;
      cap *= 2;
    } else {
      //new (arr + sz) T(value);
      alloc.construct(arr + sz, std::move(value));
    }
    ++sz;
  }

  void pop_back() {
    --sz;
    //(arr + sz)->~T();
    //alloc.destroy(arr + sz);
    std::allocator_traits<Alloc>::destroy(alloc, arr + sz);
  }
};

template<>
class Vector<bool> {
private:
  char* arr;
  size_t sz;
  size_t cap;

  struct BoolReference {
    char* cell;
    uint8_t index;

    BoolReference operator=(bool b) {
      if (b) {
        *cell |= (1 << index);
      } else {
        *cell &= ~(1 << index);
      }
      return *this;
    }

    operator bool() const {
      return *cell & (1 << index);
    }
  };

  using AllocTraits = std::allocator_traits<Alloc>;

public:
  BoolReference operator[](size_t index) {
    return BoolReference{arr + index / 8, static_cast<uint8_t>(index % 8)};
  }

};

int main() {
  std::vector<bool> vb(5, false);
  vb[3] = true; //assignment to rvalue

  std::vector<std::string> vs;
  vs.emplace_back(5, 'a');
}