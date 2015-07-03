#pragma once

#include "util/Round.h"
#include <stdint.h>

template<typename T, int Capacity, bool GracefulOverflow=false>
class ObjectPool {
  static_assert(Capacity%64==0, "ObjectPool capacity must be multiple of 64");
private:
  static const size_t sTSize = power_up(sizeof(T));
  static const uint32_t sNumPages = Capacity / 64;

  /*
   * _pool stores the raw data.
   *
   * Invariants:
   *
   * - The k'th bit of _bits is 1 iff the k'th block of _pool is free.
   * - The i'th uint64_t of _bits has a 1 bit iff _pages[j]==i for 
   *   some j>=_cur_page_index
   *
   * Note that as result of these invariants, we can always find a free
   * block in O(1) time: we can look up _pages[_cur_page_index] to find
   * an i such that _bits[i] has a 1 bit.  
   */
  char _pool[Capacity*sTSize];
  uint64_t _cur_page_index;
  uint64_t _pages[sNumPages];
  uint64_t _bits[sNumPages];

public:
  ObjectPool();

  T* fetch();
  void release(T*);
};

#include "util/ObjectPoolINLINES.cpp"

