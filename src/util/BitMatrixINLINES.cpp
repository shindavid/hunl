#include <xmmintrin.h>

BitMatrix::BitMatrix(int m, int n) :
  _m(m), _n(n), _n64(ceil_div<64>(n)), _n8(ceil_div<8>(n))
{
  _bits = new uint64_t[m*_n64];
  memset(_bits, 0, m*_n64*8);
}

bool BitMatrix::get(int x, int y) const {
  assert(x>=0);
  assert(x<_m);
  assert(y>=0);
  assert(y<_n);
  return _bits[x*_n64+y/64] & (1LL<<(y%64));
}

void BitMatrix::set(int x, int y, bool b) {
  assert(x>=0);
  assert(x<_m);
  assert(y>=0);
  assert(y<_n);
  _bits[x*_n64+y/64] |= (b?1LL:0)<<(y%64);
}

/*
 * Required: vec is of length _n
 */
void BitMatrix::mult(float* vec, float* product) const {
  int b = 0;  // index into _bits
  int p = 0;  // index into product
  for (int i=0; i<_m; ++i) {
    float result = 0.0;
    for (int j=0; j<_n64; ++j) {
      uint64_t mask = _bits[b++];
      for (int k=0; k<8; ++k) {
        uint8_t submask = (uint8_t)mask;
        float* float_submask = sFloatTable[submask];
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

