#include<iostream>
#include<list>

//8.3 std::advance, std::distance
template<typename Iterator, typename IteratorCategory>
void my_advance_helper(Iterator& iter, int n, IteratorCategory) {
  for (int i = 0; i < n; ++i, ++iter);
}

template<typename Iterator>
void my_advance_helper(Iterator& iter, int n, std::random_access_iterator_tag) {
  iter += n;
}

template<typename Iter>
void my_advance(Iter& iter, size_t n) {
  my_advance_helper(iter, n, typename std::iterator_traits<Iter>::iterator_category());
}

//8.4 const-iterator (in vector)
//8.5 reverce-iterator
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

int main() {
  std::list<int> v = {1, 2, 3, 4, 5};
  std::list<int>::iterator it1 = v.begin();
  my_advance(it1, 3);
  std::cout << *it1;
  std::list<int>::iterator it2 = v.end();
  std::cout << std::distance(it1, it2);


  std::list<int>::iterator it = v.begin();
  std::list<int>::const_iterator it_const = v.cbegin();
  *it = 2;
  //*it_const = 2; CE

  for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << *it << " ";
  }
}