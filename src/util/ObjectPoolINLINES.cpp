#include "util/Likely.h"

#include <cstdlib>
#include <exception>

namespace obj_pool {
  bool full(uint64_t page) { return page == 0; }
}

template<typename T, int Capacity, bool GracefulOverflow>
ObjectPool<T,Capacity,GracefulOverflow>::ObjectPool() {
  memset(_pool, 0, sizeof(_pool));
 
  _cur_page_index = 0;
  for (uint32_t i=0; i<sNumPages; ++i) {
    _pages[i] = i;
    _bits[i] = ~0LL;
  }
}

template<typename T, int Capacity, bool GracefulOverflow>
T* ObjectPool<T,Capacity,GracefulOverflow>::fetch() {
  uint64_t& page = _bits[_cur_page_index];
  if (unlikely(_cur_page_index==sNumPages && obj_pool::full(page))) {
    if (GracefulOverflow) {
      return (T*) malloc(sizeof(T));
    } else {
      throw std::exception();
    }
  }

  int bit_index = __builtin_ctzll(page);
  T* addr = (T*) &_pool[sTSize * (_cur_page_index*64+bit_index)];

  page &= ~(1LL<<bit_index);
  _cur_page_index += obj_pool::full(page) ? 1 : 0;

  return addr;
}

template<typename T, int Capacity, bool GracefulOverflow>
void ObjectPool<T,Capacity,GracefulOverflow>::release(T* t) {
  int64_t offset = ((int64_t)t - (int64_t)_pool) / sTSize;
  int64_t page_index = offset / 64;
  int bit_index = offset % 64;

  if (unlikely(page_index<0 || page_index>=(int64_t)sNumPages)) {
    if (GracefulOverflow) {
      free(t);
      return;
    } else {
      throw std::exception();
    }
  }

  uint64_t& page = _bits[page_index];
  bool full = obj_pool::full(page);

  page |= (1LL<<bit_index);
  _cur_page_index -= full ? 1 : 0;
  _pages[_cur_page_index] = full ? page_index : _pages[_cur_page_index];
}

