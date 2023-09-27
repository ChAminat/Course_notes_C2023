#include<iostream>
#include<utility>
#include<map>

using std::pair;

//8.6 std::map
template<typename Key, typename Value, typename Cmp = std::less<Key>>
class map {
private:
  struct BaseNode {
    BaseNode* left;
    BaseNode* right;
    BaseNode* parent;
  };
  struct Node: BaseNode {
    pair<const Key, Value> kv;
    bool red;
  };
  BaseNode* begin_;
  BaseNode* end_;

public:
  class iterator {
    BaseNode* node;
  };

  Value& operator[](const Key& key); //если нужного ключа нет, то созд нов верш. неконст опер!!!
  Value& at(const Key& key);
  const Value& at(const Key& key) const;

};

int main() {
  std::map<int, int> m;
  m[1] = 2;
  m[3] = 5;
  m[2] = 3;
  std::map<int, int>::iterator it = m.begin();
  it->second = 3;

  for (const std::pair<const int, int>& x: m) {
    std::cout << x.first << " " << x.second << '\n';
  }
}