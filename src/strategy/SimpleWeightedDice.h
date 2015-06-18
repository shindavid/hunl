#pragma once

#include <cstdlib>
#include <cmath>

/*
 * A weighted N-sided dice. Roll to get an int in the range [0, N-1]
 *
 * NOTE(dshin): There is a way to get O(N) construction and O(1) roll(). I didn't implement that here.
 */
template<int N>
class SimpleWeightedDice {
private:
  double _p[N];

  bool _validate() const {
    double sum = 0.0;
    for (int i=0; i<N; ++i) {
      sum += _p[i];
    }

    return fabs(sum-1.0) < .0001;
  }

public:
  SimpleWeightedDice(double p[N]) {
    for (int i=0; i<N; ++i) {
      _p[i] = p[i];
    }

    assert(_validate());
  }

  int roll() const {
    double x = ((double)rand()) / RAND_MAX;  // [0,1)
    
    double sum = 0.0;
    for (int i=0; i<N; ++i) {
      sum += _p[i];
      if (sum > x) return i;
    }
    return N-1;
  }
};

