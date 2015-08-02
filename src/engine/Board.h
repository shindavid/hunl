#pragma once

#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/CardSet.h"

#include <cstring>

namespace ps = pokerstove;

class Board {
private:
  ps::CardSet _card_set;
  ps::Card _cards[5];

public:
  void add(ps::Card card);
  ps::CardSet getCards() const;
  ps::Card getCard(int i) const;
  unsigned getSize() const;
  bool contains(ps::Card card) const;

  bool operator==(const Board& board) const;
};

#include "BoardINLINES.cpp"

