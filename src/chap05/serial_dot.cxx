#include <string>
#include <iostream>
#include <vector>

/* function prototypes */
template <typename T> T serial_dot(const std::vector<T> &vec1, const std::vector<T> &vec2 );
void read_vector(const std::string &prompt, std::vector<float> &vec, const int n );

int main(void) {
  std::vector<float> vec1;
  std::vector<float> vec2;

  int n_elements;
  std::cout << "Enter the order of the vectors" << std::endl;
  std::cin >> n_elements;

  read_vector("The first vector: ", vec1, n_elements);
  read_vector("The second vector: ", vec2, n_elements);

  float dot = serial_dot(vec1, vec2);

  std::cout << "The dot product is: " << dot << std::endl;

  return 0;
}


template <typename T>
T serial_dot(const std::vector<T> &vec1, /* first */ 
             const std::vector<T> &vec2 /* second */ )
{
  T sum = 0;

  typename std::vector<T>::const_iterator it1 = vec1.begin();
  typename std::vector<T>::const_iterator it2 = vec2.begin();

  for (; it1 != vec1.end(); ++it1, ++it2) {
    sum += *it1 * *it2;
  }

  return sum;
}

void read_vector(const std::string &prompt, /* prompt message */ 
                 std::vector<float> &vec, /* output */ 
                 const int n /* # of elements */ )
{
  std::cout << prompt << std::endl;

  float value = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> value;
    vec.push_back(value);
  }
}


