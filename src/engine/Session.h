#pragma once

#include "TypeDefs.h"
#include "pokerstove/penum/SimpleDeck.hpp"
#include "pokerstove/peval/CardSet.hpp"

#include <cstdlib>
#include <assert.h>

class Player;

namespace ps = pokerstove;

class SessionHand {
private:
  ps::CardSet _hole_cards;
  ps::CardSet _board;
  chip_amount_t _pot_size;
  chip_amount_t _stack_sizes[2];
  bool _action_on : 1;
  bool _button : 1;

public:
  SessionHand(chip_amount_t stack_size, bool button);
};

/*
 * A session is initialized with 2 Player's and a random seed.
 *
 * The seed determines the cards dealt out per hand. At the start of the i'th hand, the random seed is
 * re-initialized to (base_random_seed + i), so that the same cards get dealt out no matter what the
 * players do.
 *
 * By running two session with the same seed, but with the order of the players switched, you can
 * run duplicate poker (http://en.wikipedia.org/wiki/Duplicate_poker).
 *
 * TODO: to reduce variance, you can handle pre-river all-in's by splitting the pot according to
 * pot-equity. That bool should probably be a Session template variable.
 */
class Session {
private:
  session_id_t _id;  // eventually get assigned from a database
  hand_id_t _current_hand_id = 0;

  ps::SimpleDeck _deck;
  uint64_t _base_seed;
  Player* _players[2];
  chip_amount_t _stack_size;  // every hand, stacks reset to this size
  chip_amount_t _score;  // positive means _players[0] is winning
  bool _button;

public:
  Session(Player* p0, Player* p1, chip_amount_t stack_size, uint64_t seed);

  void playHand(HandResult& result);

private:
  static session_id_t __next_id;
};

#include "SessionINLINES.cpp"

