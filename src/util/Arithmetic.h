#pragma once

#include <cmath>
#include <assert.h>

float EPSILON = 0.000001;

bool approximately_equal(float x, float y) {
  return fabs(x-y) < EPSILON;
}

/*
 * returns ceil(x*1.0/n)
 */
template<int n> int ceil_div(int x) {
  return (x+n-1)/n;
}

/*
 * http://forums.overclockers.co.uk/showthread.php?p=8773984
 */
float fast_inv_sqrt(float x) {
  float xhalf = 0.5f*x;
  int i = *(int*)&x; // get bits for floating value
  i = 0x5f375a86- (i>>1); // gives initial guess y0
  x = *(float*)&i; // convert bits back to float
  x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
  return x;
}

float fast_sqrt(float x) {
  return 1.0 / fast_inv_sqrt(x);
}

/*
 * Functions to encode/decode an ordered pair of ints into a single int.
 *
 * 1 0 -> 0
 * 2 0 -> 1
 * 2 1 -> 2
 * 3 0 -> 3
 * 3 1 -> 4
 * 3 2 -> 5
 * 4 0 -> 6
 * ...
 *
 * In general, <k, 0> maps to a triangular number.
 */
namespace ordered_ints {
  /*
   * (x,y)->n, x>y>=0
   */
  int encode(int x, int y) {
    assert(x>y);

    return x*(x-1)/2 + y;
  }

  /*
   * n->(x,y), x>y
   *
   * WARNING(dshin): Could break for large values of n due to approximate nature of fast_sqrt()
   * function. 
   */
  std::pair<int,int> decode(int n) {
    int floor_sqrt = (unsigned) floor(fast_sqrt(8*n+1.5));
    int x = (floor_sqrt+1)/2;
    int y = n - x*(x-1)/2;

    assert(encode(x,y) == n);

    return std::make_pair(x,y);
  }
}

