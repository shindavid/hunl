#pragma once

#include "pokerstove/peval/Rank.h"

#include <exception>

namespace ps = pokerstove;

/*
 * A RankPair is an ordered pair of (ps::Rank, ps::Rank).
 *
 * There are 91 possible RankPair's: 13 pocket pairs and 13-choose-2 non-pocket-pairs.
 */ 
class __attribute__ ((__packed__)) RankPair {
private:
  /*
   * _high >= _low
   */
  const ps::Rank _high : 4;
  const ps::Rank _low : 4;
  
public:
  RankPair(ps::Rank high, ps::Rank low) :
    _high(high), _low(low) {}

  uint8_t code() const {
    uint8_t H = _high.code();
    uint8_t L = _low.code();
    return H*(H+1)/2 + L;
  }

  ps::Rank getHigh() const { return _high; }
  ps::Rank getLow() const { return _low; }
  bool isPocketPair() const { return _high==_low; }

  static RankPair decode(uint8_t code) {
    // TODO: best approach here is look-up table in 91-element array?
    throw std::exception("implement me");
  }
  static const int NUM_RANK_PAIRS = 91;
};
static_assert(sizeof(RankPair)==1);

