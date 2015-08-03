#pragma once

#ifndef _DEBUG
#define EIGEN_NO_DEBUG
#endif  // _DEBUG

#include "engine/Holding.h"
#include "strategy/Range.h"
#include <Eigen/Core>

#ifndef EIGEN_VECTORIZE
CRASH
#endif

using namespace Eigen;

template<int N> class EquityMatrix {
private:
  using RowMatrix = Matrix<float, Dynamic, Dynamic, RowMajor>;
  using ColMatrix = Matrix<float, Dynamic, Dynamic, ColMajor>;

  /*
   * TODO: store the intersection matrix _I as a bit-matrix.
   */
  RowMatrix _M;  // main
  RowMatrix _I;  // intersection
  ColMatrix _W;  // weights, used like local var
  ColMatrix _E;  // equities, used like local var

public:
  EquityMatrix();

  void clearM() { _M = RowMatrix::Zero(); }

  float& M(int i, int j) { return _M(i,j); }
  const float& M(int i, int j) const { return _M(i,j); }
  float& I(int i, int j) { return _I(i,j); }
  const float& I(int i, int j) const { return _I(i,j); }

  /*
   * T is a type that has 
   *
   * float weight[2];
   * float equity[2];
   *
   * Required: weight's are normalized to sum to 1.0
   */
  template<typename T> void compute(HoldingMap<N,T>& map);
};

typedef EquityMatrix<Holding::sNumTurnHoldings> TurnEquityMatrix;

#include "strategy/EquityMatrixINLINES.cpp"

