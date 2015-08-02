#pragma once

#ifndef _DEBUG
#define EIGEN_NO_DEBUG
#endif  // _DEBUG

#include "engine/Holding.h"
#include <Eigen/Core>

using namespace Eigen;

template<int N> class EquityMatrix {
private:
  /*
   * NOTE(dshin) The Eigen documentation states:
   *
   * "Fixed-size means that the numbers of rows and columns are known are compile-time. In this case, Eigen allocates the array of coefficients as a fixed-size array, as a class member. This makes sense for very small matrices, typically up to 4x4, sometimes up to 16x16. Larger matrices should be declared as dynamic-size even if one happens to know their size at compile-time."
   *
   * I don't know why they state that larger matrices should be declared as dynamic-size. I'm assuming
   * for now that that advice is ill-advised. Later I can experiment to see.
   */
  Matrix<float, N, N, RowMajor> _M;  // RowMajor should be faster mult (why is ColMajor the default??)

public:
  float& operator()(int i, int j) { return _M(i,j); }
  const float& operator()(int i, int j) const { return _M(i,j); }

  /*
   * T is a type that has 
   *
   * float weight[2];
   * float equity[2];
   */
  template<typename T> void compute(HoldingMap<N,T>& map);
};

#include "strategy/EquityMatrixINLINES.cpp"

