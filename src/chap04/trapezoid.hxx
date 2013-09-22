#ifndef TRAPEZOID_HXX_RTBWAFPH
#define TRAPEZOID_HXX_RTBWAFPH

typedef float (*func)(float);

class Trapezoid
{
public:
  Trapezoid(float a, float b, float n, func f);
  float area() const;
  void printArea() const;
private:
  func _f;

private:
  float _a;
  float _b;
  float _n;
  float _h;
};

#endif /* end of include guard: TRAPEZOID_HXX_RTBWAFPH */
