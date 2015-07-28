#pragma once

#include "util/Arithmetic.h"

/*
 * A BitMatrix is a matrix of all 0's and 1's. 
 */

template<int M, int N>
class BitMatrix {
public:
  static const int sM = M;
  static const int sN = N;
  static const int sN64 = ceil_div<64>(N);
  static const int sN8 = ceil_div<8>(N);
  static float sFloatTable[256][8];  // supports mult()

  uint64_t _bits[M][sN64];

public:
  bool get(int x, int y) const;
  void set(int x, int y, bool b);

  /*
   * Required: vec has length _n, product has length _m
   *
   * Performs the (_m by _n) * (_n by 1) matrix multiplication, and stores the result in product
   *
   * TODO: add support for multiplication with an (_n by k) matrix.
   */
  void mult(float* vec, float* product) const;
};

#include "util/BitMatrixFloatTable.cpp"
#include "util/BitMatrixINLINES.cpp"

