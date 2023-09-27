#include <iostream>
int main(){
  int x = 3;
  std::cout << (x < 2 ? x = 1: ++x);
}