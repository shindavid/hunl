#pragma once

#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/CardSet.h"

namespace ps = pokerstove;

/*
 * A set of 2 cards.
 */
class Holding {
private:
  ps::CardSet _cards;

  int _code1() const { return __builtin_ffsll(_cards.mask()) - 1; }
  int _code2() const { return 63 - __builtin_clzll(_cards.mask()); }

public:
  Holding() {}
  Holding(ps::CardSet cards) : _cards(cards) {
    assert(_cards.size() == 2);
    assert(_code1() < _code2());
    assert(0 <= _code1());
    assert(cards.contains(getCard1()));
    assert(cards.contains(getCard2()));
  }

  ps::CardSet getCardSet() const { return _cards; }
  ps::Card getCard1() const { return ps::Card(_code1()); }
  ps::Card getCard2() const { return ps::Card(_code2()); }

  bool operator==(const Holding& h) const { return _cards==h._cards; }
  uint64_t hash() const {
    int n = _code1();
    int m = _code2();
    return m*(m-1)/2 + n;
  }
};

