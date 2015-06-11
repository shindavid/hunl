#pragma once

#include "pokerstove/peval/Suit.h"

namespace ps = pokerstove;

/*
 * A SuitMask represents a subset of the 16 possible (ps::Suit, ps::Suit)
 * pairs.
 *
 * It is to be used in conjunction with RankPair.
 */ 
class __attribute__ ((__packed__)) SuitMask {
private:
  /*
   * The i'th lowest bit of this mask represents the pair (suit1, suit2),
   * where suit1.code() == (i%4) and suit2.code() == (i/4).
   *
   * For pocket pairs, only bits where (i%4) > (i/4) will be valid.
   */
  const uint16_t _mask;
  
public:
  SuitMask() : _mask(0) {}
  SuitMask(uint16_t mask) : _mask(mask) {}
  
  static const uint16_t SUITED_MASK = 0x8421;  // 2^0 + 2^5 + 2^10 + 2^15
  static const uint16_t UNSUITED_MASK = 0x7bde;  // 0xffff - SUITED_MASK
  static const uint16_t POCKET_PAIR_MASK = 0x8ce;  // 2^1 + 2^2 + 2^3 + 2^6 + 2^7 + 2^11

  static const uint16_t HIGH_MASKS[4] = {
    0x1111,  // CLUBS,    2^0 + 2^4 + 2^8 + 2^12
    0x2222,  // DIAMONDS, 2^1 + 2^5 + 2^9 + 2^13
    0x3333,  // HEARTS,   2^2 + 2^6 + 2^10 + 2^14
    0x4444   // SPADES,   2^3 + 2^7 + 2^11 + 2^15
  };
  
  static const uint16_t LOW_MASKS[4] = {
    0xf,    // CLUBS,    2^0 + 2^1 + 2^2 + 2^3
    0xf0,   // DIAMONDS, 2^4 + 2^5 + 2^6 + 2^7
    0xf00,  // HEARTS,   2^8 + 2^9 + 2^10 + 2^11
    0xf000  // SPADES,   2^12 + 2^13 + 2^14 + 2^15
  };

  static uint16_t getHighMask(ps::Suit suit);  // accessor of HIGH_MASKS
  static uint16_t getLowMask(ps::Suit suit);  // accessor of LOW_MASKS

  SuitMask removeHigh(ps::Suit suit) const;
  SuitMask removeLow(ps::Suit suit) const;

  uint8_t size() const { return __builtin_popcount(_mask); }
};
static_assert(sizeof(SuitMask)==2);

#include "SuitMaskINLINES.cpp"
