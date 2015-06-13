#pragma once

#include "TypeDefs.h"
#include "GameEvent.h"
#include "pokerstove/peval/CardSet.hpp"

namespace ps = pokerstove;

/*
 * A single hand.
 */
class SessionHand {
private:
  SessionLog& _log;
  ps::CardSet _hole_cards[2];
  ps::CardSet _board;
  const chip_amount_t _stack_size;
  chip_amount_t _wagered_prior_rounds[2];
  chip_amount_t _wagered_current_round[2];
  seat_t _action_on;
  seat_t _button;
  bool _folded[2];
  bool _is_current_betting_round_done;

  bool _validate_chip_amounts() const {
    return (_wagered_prior_rounds[0] + _wagered_current_round[0] <= _stack_size) &&
      (_wagered_prior_rounds[1] + _wagered_current_round[1] <= _stack_size);
  }

  bool _all_in(seat_type_t seat) const {
    return _wagered_prior_rounds[seat] + _wagered_current_round[seat] == _stack_size;
  }

public:
  SessionHand(SessionLog& log, chip_amount_t stack_size, seat_t button);

  bool isDone() const { throw std::exception("implement me"); }
  bool isCurrentBettingRoundDone() const { return _is_current_betting_round_done; }
  seat_t getActionOn() const { return _action_on; }

  typename<typename E>
    void handleEvent(seat_t seat, const E& event);
};

#include "SessionHandINLINES.cpp"
