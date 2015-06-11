#pragma once

#include "pokerstove/peval/Suit.h"

#include <exception>

namespace ps = pokerstove;

/*
 * A set of 4 counts, corresponding to how many of each suit is on the board.
 */
class __attribute__ ((__packed__)) SuitCounts {
private:
  /*
   * Bits 0-7: # of Clubs, in unary
   * Bits 8-15: # of Diamonds, in unary
   * Bits 16-23: # of Hearts, in unary
   * Bits 24-31: # of Spades, in unary
   */
  uint32_t _mask;
  
public:
  int count(ps::Suit suit) const { throw std::exception("TODO"); }
  int size() const { return __builtin_popcount(_mask); }
};
static_assert(sizeof(SuitCounts)==4);

