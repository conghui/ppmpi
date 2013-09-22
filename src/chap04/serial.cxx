#include <iostream>
#include "trapezoid.hxx"

/* function prototypes */
float f(float x);

int main()
{
  float a;
  float b;
  int n;

  std::cout << "Enter a, b and n: ";
  std::cin >> a >> b >> n;

  Trapezoid trap(a, b, n, f);
  trap.printArea();

  return 0;
}

float f(float x) {
  return x * x;
}
