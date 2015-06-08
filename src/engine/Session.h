#pragma once

#include "TypeDefs.h"

#include <cstdlib>
#include <assert.h>

class Player;

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

  uint64_t _base_seed;
  Player* _players[2];
  bool _button : 1;

public:
  Session(Player* p0, Player* p1, uint64_t seed) {
    _id = __next_id++;
    _players[0] = p0;
    _players[1] = p1;

    // srand() API is weird, passing in 1 gives you a weird error
    assert(seed>1);
    _base_seed = seed;
    srand(seed);
    _button = rand() % 2;
  }

  void playHand(HandResult& result) {
    uint64_t seed = _base_seed + _current_hand_id + 1;
    srand(seed);

    // ...
    while () {
      PlayerEvent event = _players[i]->handleActionRequest();
      updateGameState(event);
      i = !i;
    }

    _current_hand_id++;
    _button = !_button;
  }

private:
  static session_id_t __next_id;
};

