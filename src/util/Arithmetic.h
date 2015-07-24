#include <cmath>

float EPSILON = 0.000001;

bool approximately_equal(float x, float y) {
  return fabs(x-y) < EPSILON;
}
