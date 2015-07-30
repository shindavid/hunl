#pragma once

#include "engine/Holding.h"
#include "util/BitMatrix.h"

/*
 * A compact representation of a Holding as a single uint16_t.
 */
class __attribute__ ((__packed__)) CompactHolding {
private:
  uint16_t _code;

public:
  CompactHolding() {}
  CompactHolding(uint16_t code);
  CompactHolding(Holding holding);
  Holding toHolding() const;
  uint16_t code() const;
};
static_assert(sizeof(CompactHolding)==2, "CompactHolding should fit in 2 bytes");

/*
 * A HoldingIndexing is an ordering of a subset of the universe of possible Holding's. This is to be
 * used in conjunction with HoldingMap.
 *
 * To illustrate, a standard std::map would be used like this:
 *
 * Holding holding;
 * std::map<Holding, T> holding_map;
 * T t = holding_map[holding];
 *
 * A HoldingIndexing + HoldingMap would be used instead like this:
 *
 * HoldingIndexing indexing;
 * HoldingMap<T> holding_map;
 * Holding holding = indexing[0];
 * T t = holding_map[0];
 *
 * Note that direct look-ups are not supported. This is because we don't need it for any of the 
 * algorithms.
 *
 * Q: Why not keep HoldingIndexing as part of HoldingMap? 
 * A: The same indexing can be shared by multiple objects.
 */

/*
 * See comments for HoldingIndexing.
 */
template <typename T> class HoldingMap {
private:
  T _t[Holding::sNumHoldings];
  const int _size;  // I don't like how size is kept on both the HoldingIndexing and the HoldingMap...

public:
  HoldingMap(int size) : _size(size) {}

  int size() const { return _size; }
  
  const T& operator[](int i) const { return _t[i]; }
  T& operator[](int i) { return _t[i]; }
  
  template <class Compare> void sort(Compare comp) {
    std::sort(_t, &_t[_size], comp);
  }

  const T* getArray() const { return _t; }
  T* getArray() { return _t; }
};

typedef HoldingMap<CompactHolding> HoldingIndexing;

#include "strategy/RangeINLINES.cpp"

