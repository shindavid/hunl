#pragma once

#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/CardSet.h"

namespace ps = pokerstove;

/*
 * A set of 2 cards.
 */
class Holding {
private:
  const ps::CardSet _cards;

  int _code1() const;
  int _code2() const;

public:
  Holding() {}
  Holding(ps::CardSet cards);

  ps::CardSet getCardSet() const;
  ps::Card getCard1() const;
  ps::Card getCard2() const;

  bool operator==(const Holding& h) const;
  uint64_t hash() const;
};

#include "HoldingINLINES.cpp"

