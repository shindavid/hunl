#pragma once

#include "pokerstove/peval/Rank.h"

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
  const ps::Rank _high;
  const ps::Rank _low;
  
public:
  RankPair(ps::Rank high, ps::Rank low);

  ps::Rank getHigh() const;
  ps::Rank getLow() const;
  bool isPocketPair() const;

  uint8_t code() const;  // returns an int in range [0, NUM_RANK_PAIRS-1]
  
  static const int NUM_RANK_PAIRS = 91;
};
static_assert(sizeof(RankPair)==2);

#include "strategy/RankPairINLINES.cpp"

