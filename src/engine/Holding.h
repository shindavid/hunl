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

  int _code1() const;
  int _code2() const;

public:
  Holding() {}
  Holding(ps::CardSet cards);

  ps::CardSet getCardSet() const;
  ps::Card getCard1() const;
  ps::Card getCard2() const;
  bool contains(ps::Card c) const;
  bool intersects(const ps::CardSet& cs) const;
  bool intersects(const Holding& h) const;

  bool operator==(const Holding& h) const;
  uint64_t hash() const;
  std::string str() const;

  static const int sNumHoldings = 1326;  // 52-choose-2
  static const int sNumFlopHoldings = 1176;  // 49-choose-2
  static const int sNumTurnHoldings = 1128;  // 48-choose-2
  static const int sNumRiverHoldings = 1081;  // 47-choose-2
};

/*
 * A compact version of Holding
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
 
  bool operator==(const CompactHolding& h) const;
};
static_assert(sizeof(CompactHolding)==2, "CompactHolding should fit in 2 bytes");

#include "HoldingINLINES.cpp"

