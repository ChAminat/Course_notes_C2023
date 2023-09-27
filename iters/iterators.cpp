#include<iostream>
#include<algorithm>
#include<vector>
#include<iterator>
#include<list>

///////////////////////////////////////////////////////////////////////////////////////////////
//8.5 std::advance, std::distance
template<typename Iterator>
void my_advance(Iterator& iter, size_t n) {
  if constexpr(std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
              std::random_access_iterator_tag>::value) {
      iter += n;
  } else {
    for (size_t i = 0; i < n; ++i, ++iter);
  }
}

//другой вариант, is_base_of - явл ли А родителем Б
template<typename Iterator>
void my_advance1(Iterator& iter, size_t n) {
  if constexpr(std::is_base_of<std::random_access_iterator_tag,
               typename std::iterator_traits<Iterator>::iterator_category>::value) {
      iter += n;
  } else {
    for (size_t i = 0; i < n; ++i, ++iter);
  }
}

template<typename Iter>
void process(Iter begin, Iter end) {
  --end;
  typename std::iterator_traits<Iter>::value_type tmp = *begin;
  *begin = *end;
  *end = tmp;
}

///////////////////////////////////////////////////////////////////////////////////////////
// 8.6 Back insert iterator.
template<typename Container>
struct back_insert_iterator {
private:
  Container& container;
public:
  back_insert_iterator(Container& container): container(container) {}
  back_insert_iterator<Container>& operator++() {
    return *this;
  }
  back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
    container.push_back(value);
    return *this;
  }
  back_insert_iterator<Container>& operator*() {
    return *this;
  }

};

template<typename Container>
back_insert_iterator<Container> back_inserter(Container& container) {
  return back_insert_iterator<Container>(container);
}

//////////////////////////////////////////////////////////////////////////////////////////
//8.7 Stream iterators

template<typename T>
class IstreamIterator {
private:
  std::istream& in;
  T value;
public:
  IstreamIterator(std::istream& in): in(in) {
    in >> value;
  }
  IstreamIterator<T>& operator++() {
    in >> value;
  }
  T& operator*() {
    return value;
  }
};

//std::ifstream in("input.txt");
//std::istringsrteam iss(s);

template<typename T>
struct OstreamIterator {
  std::ostream& ostr;
  OstreamIterator(std::ostream& ostr): ostr(ostr) {}

  OstreamIterator& operator++(){
    return *this;
  }
  OstreamIterator& operator=(const T& value){
    ostr << value;
    return *this;
  }
  OstreamIterator& operator*(){
    return *this;
  }
};

int main() {
  std::vector<int> v1 = {1, 2};
  std::vector<int> v2;

  std::ostream_iterator<int> ostr(std::cout, ",");
  std::copy(v1.begin(), v1.end(), ostr);
  std::copy(v1.begin(), v1.end(), std::back_inserter(v2));

  std::list<int> lst = {1, 2, 3, 4, 5};
  std::list<int>::iterator it = lst.begin();
  std::advance(it, 4);
  std::cout << *it;

}