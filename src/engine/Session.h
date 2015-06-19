#pragma once

#include "engine/Deck.h"
#include "engine/HandState.h"
#include "engine/Player.h"
#include "engine/SessionLog.h"
#include "engine/SessionParams.h"
#include "engine/TypeDefs.h"
#include "pokerstove/peval/CardSet.h"
#include "pokerstove/peval/HoldemHandEvaluator.h"

#include <cstdlib>
#include <assert.h>

namespace ps = pokerstove;

class Session : public SessionParams {
private:
  ps::HoldemHandEvaluator _evaluator;
  SessionLog _log;
  const SessionParams _params;
  SessionState _state;
  Deck _deck;
  
  /*
   * The seed determines the cards dealt out per hand. At the start of the i'th hand, the random seed is
   * re-initialized to (_base_seed + i), so that the same cards get dealt out no matter what the
   * players do.
   *
   * By running two session with the same seed, but with the order of the players switched, you can
   * run duplicate poker (http://en.wikipedia.org/wiki/Duplicate_poker).
   */
  const uint64_t _base_seed;

  void _init_hand();
  void _main_loop(HandState& hand);
  void _finish_hand(HandState& hand);
  void _do_betting_round(HandState& hand);
  void _award_pot(const HandState& public_state, seat_t seat);
  void _split_pot(const HandState& public_state);

public:
  Session(Player* p0, Player* p1, chip_amount_t stack_size, chip_amount_t small_blind_size,
      chip_amount_t big_blind_size, uint64_t base_seed);

  void logSessionStart();
  void logHandStart();
  void playHand();
  uint64_t getBaseSeed() const { return _base_seed; }

private:
  static session_id_t __next_id;
};

#include "engine/SessionINLINES.cpp"

