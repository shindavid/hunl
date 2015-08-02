#pragma once

#ifndef _DEBUG
#define EIGEN_NO_DEBUG
#endif  // _DEBUG

#include "engine/Holding.h"
#include <Eigen/Core>

using namespace Eigen;

template<int N> class EquityMatrix {
private:
  Matrix<float, Dynamic, Dynamic, RowMajor> _M;
  Matrix<float, Dynamic, Dynamic, ColMajor> _W;
  Matrix<float, Dynamic, Dynamic, ColMajor> _P;

public:
  EquityMatrix();

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

typedef EquityMatrix<Holding::sNumTurnHoldings> TurnEquityMatrix;

#include "strategy/EquityMatrixINLINES.cpp"

