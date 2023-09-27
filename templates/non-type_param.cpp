#include<iostream>
#include<array>
#include<vector>

//6.4 Non-type template parameters
template<typename T, size_t N>
class array {
private:
  T arr[N];
};

template <size_t M, size_t N, typename Field>
class Matrix {

};

template <size_t M, size_t N, size_t K,typename Field>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>&, const Matrix<N, K, Field>&);


template<typename T, template <typename, typename> typename Container>
class Stack {
  Container<T, std::allocator<T>> conatainer;
};

int main() {
  std::array<int, 10> a = {1, 2};
  Stack<int, std::vector> std;
}
