#pragma once

#include "util/Arithmetic.h"

/*
 * A BitMatrix is a matrix of all 0's and 1's. 
 */
class BitMatrix {
private:
  const int _m;  // # rows
  const int _n;  // # columns
  const int _n64;  // # of size-64 boxes needed to fit _n balls
  const int _n8;  // # of size-8 boxes needed to fit _n balls

  uint64_t* _bits;  // initialized on heap

  static float sFloatTable[256][8];  // supports mult()

public:
  /*
   * Initializes all bits to 0.
   */
  BitMatrix(int m, int n);

  int getM() const;
  int getN() const;

  bool get(int x, int y) const;
  void set(int x, int y, bool b);

  /*
   * Required: vec has length _n, product has length _m
   *
   * Performs the (_m by _n) * (_n by 1) matrix multiplication, and stores the result in product
   *
   * TODO: add support for multiplication with an (_n by k) matrix.
   */
  float mult(float* vec, float* product) const;
};

#include "util/BitMatrixFloatTable.cpp"
#include "util/BitMatrixINLINES.cpp"

