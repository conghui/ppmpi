#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template <typename T>
class Matrix
{
public:
  Matrix(int row, int col);
  ~Matrix();

  void readDataFrom(std::istream &is);
  void productWith(const std::vector<T> &vec, std::vector<T> &ret);

private:
  T **_data;
  int _n_row;
  int _n_col;
};

template <typename T> Matrix<T>::Matrix(int row, int col) :
  _data(NULL),
  _n_row(row),
  _n_col(col)
{
  _data = new T *[row];
  for (int i = 0; i < row; i++) {
    _data[i] = new T[col];
  }
}

template <typename T> Matrix<T>::~Matrix() {
  for (int i = 0; i < _n_row; i++) {
    delete [] _data[i];
  }

  delete [] _data;
}

template <typename T>
void Matrix<T>::readDataFrom(std::istream &is) {
  for (int row = 0; row < _n_row; row++) {
    for (int col = 0; col < _n_col; col++) {
      is >> _data[row][col];
    }
  }
}

template <typename T>
void Matrix<T>::productWith(const std::vector<T> &vec,
                            std::vector<T> &ret)
{
  ret.clear();
  for (int row = 0; row < _n_row; row++) {
    T sum = 0;
    for (int col = 0; col < _n_col; col++) {
      sum += _data[row][col] * vec[col];
    }
    ret.push_back(sum);
  }
}

template <typename T>
void read_vector_from(std::vector<T> &vec, int n,
                      std::istream &is)
{
  T val;
  for (int i = 0; i < n; i++) {
    is >> val;
    vec.push_back(val);
  }
}


int main(void)
{
  // read dimension of the matrix
  int n_row;
  int n_col;
  std::cout << "Enter the order of matrix (m x n): ";
  std::cin >> n_row >> n_col;

  Matrix<float> matrix(n_row, n_col);
  std::vector<float> vec;

  // read content of matrix
  std::cout << "Enter all the elements, line by line, "
               "seperated by space" << std::endl;
  matrix.readDataFrom(std::cin);

  // read content of vector
  read_vector_from(vec, n_col, std::cin);

  // calculate the production
  std::vector<float> product;
  matrix.productWith(vec, product);

  // print the result
  std::copy(product.begin(), product.end(),
            std::ostream_iterator<float>(std::cout, " "));

  return 0;
}
