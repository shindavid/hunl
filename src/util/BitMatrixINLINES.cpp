#include "util/BitMatrixFloatTable.h"
#include <xmmintrin.h>

template<int M, int N>
bool BitMatrix<M,N>::get(int x, int y) const {
  assert(x>=0);
  assert(x<M);
  assert(y>=0);
  assert(y<N);
  return _bits[x][y/64] & (1LL<<(y%64));
}

template<int M, int N>
void BitMatrix<M,N>::set(int x, int y, bool b) {
  assert(x>=0);
  assert(x<M);
  assert(y>=0);
  assert(y<N);
  _bits[x][y/64] |= (b?1LL:0)<<(y%64);
}

template<int M, int N>
void BitMatrix<M,N>::set_block(int x, int yb, int64_t block) {
  assert(x>=0);
  assert(x<M);
  assert(yb>=0);
  assert(yb<sN64);
  _bits[x][yb] = block;
}

template<int M, int N>
void BitMatrix<M,N>::mult(const float* vec, float* product) const {
  for (int i=0; i<M; ++i) {
    float result = 0.0;
    int p = 0;  // index into product
    for (int j=0; j<sN64; ++j) {
      uint64_t mask = _bits[i][j];
      for (int k=0; k<8; ++k) {
        uint8_t submask = (uint8_t)mask;
        float* float_submask = bit_matrix::floatTable[submask];
        mask = mask >> 8;

        //result = vfmadd231(float_submask, &product[p], result);  // TODO(dshin) - how do i do this?

        // TODO(dshin): remove below slow code after uncommenting working versin of vfma* call above
        for (int h=0; h<8; ++h) result += float_submask[h]*product[p+h];
        // end TODO

        p += 8;
      }
    }
    product[i] = result;
  }
}

