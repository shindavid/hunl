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
  void add(ps::Card card) {
    _cards[getSize()] = card;
    _card_set.insert(card);
  }

  ps::CardSet getCards() const { return _card_set; }
  ps::Card getCard(int i) const { return _cards[i]; }
  unsigned getSize() const { return _cards_set.size(); }

  bool operator==(const Board& board) const {
    return !memcmp(this->_cards, board._cards, sizeof(_cards));
  }
};

