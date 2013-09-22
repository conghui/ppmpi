#include <iostream>
#include "trapezoid.hxx"

Trapezoid::Trapezoid(float a, float b,
                     float n, func f) :
  _f(f),
  _a(a),
  _b(b),
  _n(n),
  _h((b - a) / n)
{

}

float Trapezoid::area() const {
  float x = _a;
  float integral = (_f(_a) + _f(_b)) / 2.0f;

  for (int i = 1; i < _n; i++) {
    x += _h;
    integral += _f(x);
  }

  return integral * _h;
}

void Trapezoid::printArea() const {
  std::cout << "With n = " << _n << " trapezoids, our estimate "
    << "of the integral from " << _a << " to " << _b << " = "
    << area() << std::endl;
}
