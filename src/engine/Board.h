#pragma once

#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/CardSet.h"

namespace ps = pokerstove;

class Board {
private:
  ps::CardSet _card_set;
  std::vector<ps::Card> _cards;

public:
  void add(ps::Card card) {
    _card_set.insert(card);
    _cards.push_back(card);
  }

  ps::CardSet getCards() const { return _card_set; }
  ps::Card getCard(int i) const { return _cards[i]; }
  unsigned getSize() const { return _cards.size(); }
};

