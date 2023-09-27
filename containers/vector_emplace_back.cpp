#include<iostream>
#include<vector>

//8.2 Internals of std::vector.
//9.2 allocator_traits
template<typename Alloc>
struct allocator_traits {
  template<typename... Args>
  static void construct(Alloc& alloc, typename Alloc::value_type* ptr, Args&&... args) {
    if constexpr (true/*Alloc has method construct(Args...)*/) {
      alloc.construct(ptr, std::forward<Args>(args)...);
    } else {
      new (ptr) typename Alloc::value_type(std::forward<Args>(args)...);
    }
  }

  template<typename U>
  using rebind_alloc = int;/* ... */
};

template<typename T>
struct allocator {
  T* allocate(size_t count) {
    return reinterpret_cast<T*>(new char[count * sizeof(T)]);
  }

  void deallocate(T* ptr, size_t count) {
    delete[] reinterpret_cast<char*>(ptr);
  }
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
        std::allocator_traits<Alloc>::construct(alloc, arr + i, value);
      }
    } catch (...) {
      std::allocator_traits<Alloc>::deallocate(alloc, arr, count);
      throw;
    }
  }

  void reserve(size_t count) {
    if (cap >= count) return;
    T* newarr = std::allocator_traits<Alloc>::allocate(alloc, count);

    size_t i = 0;
    try {
      for (; i < sz; ++i) {
        std::allocator_traits<Alloc>::construct(alloc, newarr + i, arr[i]);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        std::allocator_traits<Alloc>::destroy(alloc, newarr + j);
      }
      std::allocator_traits<Alloc>::deallocate(alloc, newarr, count);
      throw;
    }

    for (size_t i = 0; i < sz; ++i) {
      std::allocator_traits<Alloc>::destroy(alloc, arr + i);
    }
    std::allocator_traits<Alloc>::deallocate(alloc, arr, cap);
    arr = newarr;
    cap = count;
  }

  void push_back(const T& value) {
    emplace_back(value);
  }

  void push_back(T&& value) {
    emplace_back(std::move(value));
  }

  template<typename... Args>
  void emplace_back(Args&&... args) {
    if (sz >= cap) {
      T* newarr = std::allocator_traits<Alloc>::allocate(alloc, cap * 2);
      size_t i = 0;
      try {
        for (; i < sz; ++i) {
          std::allocator_traits<Alloc>::construct(alloc, newarr + i, std::move_if_noexcept(arr[i]));
        }
        std::allocator_traits<Alloc>::construct(alloc, newarr + sz, std::forward<Args>(args)...);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          std::allocator_traits<Alloc>::destroy(alloc, newarr + j);
        }
        std::allocator_traits<Alloc>::deallocate(alloc, newarr, cap * 2);
        throw;
      }

      for (size_t i = 0; i < sz; ++i) {
        std::allocator_traits<Alloc>::destroy(alloc, arr + i);
      }
      std::allocator_traits<Alloc>::deallocate(alloc, arr, cap);
      arr = newarr;
      cap *= 2;
    } else {
      std::allocator_traits<Alloc>::construct(alloc, arr + sz, std::forward<Args>(args)...);
    }
    ++sz;
  }

  void pop_back() {
    --sz;
    std::allocator_traits<Alloc>::destroy(alloc, arr + sz);
  }
};

template<typename Alloc>
class Vector<bool, Alloc> {
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