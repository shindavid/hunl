#pragma once

#include "engine/TypeDefs.h"
#include "engine/Player.h"
#include "engine/Deck.h"
#include "pokerstove/peval/CardSet.hpp"

#include <cstdlib>
#include <assert.h>

namespace ps = pokerstove;

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

  Deck _deck;
  uint64_t _base_seed;
  Player* _players[2];
  chip_amount_t _stack_size;  // every hand, stacks reset to this size
  chip_amount_t _small_blind_size, _big_blind_size;
  chip_amount_t _score;  // positive means _players[0] is winning
  seat_t _button;

public:
  Session(Player* p0, Player* p1, chip_amount_t stack_size, chip_amount_t small_blind_size,
      chip_amount_t big_blind_size, uint64_t seed);

  void playHand(HandResult& result);

private:
  static session_id_t __next_id;

  void _init_hand();
  void _main_loop(SessionHand& hand);
  void _finish_hand(const SessionHand& hand);
  void _do_betting_round(SessionHand& hand);
};

#include "SessionINLINES.cpp"

