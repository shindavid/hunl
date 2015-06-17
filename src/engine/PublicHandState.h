#pragma once

#include "engine/TypeDefs.h"
#include "pokerstove/peval/CardSet.hpp"

/*
 * Hand state, only the public part of it.
 */
class PublicHandState {
private:
  const session_id_t _session_id;
  const hand_id_t _id;
  const chip_amount_t _starting_stack_size;  // assume same for both players
  const chip_amount_t _small_blind_amount;
  const chip_amount_t _big_blind_amount;
  const seat_t _button;
  
  Player* _players[2];
  chip_amount_t _wagered_prior_rounds[2];
  chip_amount_t _wagered_current_round[2];
  seat_t _action_on;
  bool _folded[2];
  bool _is_current_betting_round_done;

  ps::CardSet _board;
  
  bool _validate_chip_amounts() const {
    return (_wagered_prior_rounds[0] + _wagered_current_round[0] <= _starting_stack_size) &&
      (_wagered_prior_rounds[1] + _wagered_current_round[1] <= _starting_stack_size);
  }

  bool _all_in(seat_type_t seat) const {
    return _wagered_prior_rounds[seat] + _wagered_current_round[seat] == _starting_stack_size;
  }

public:
  PublicHandState(Player* p0, Player* p1, session_id_t session_id, hand_id_t id,
      chip_amount_t starting_stack_size, chip_amount_t small_blind_amount,
      chip_amount_t big_blind_amount, seat_t button);
  
  bool isDone() const { return _folded[0] || _folded[1]; }
  bool isCurrentBettingRoundDone() const { return _is_current_betting_round_done; }
  seat_t getActionOn() const { return _action_on; }
  session_id_t getSessionID() const { return _session_id; }
  hand_id_t getHandID() const { return _hand_id; }
  Player* getPlayer(seat_type_t seat) const { return _players[seat]; }

  chip_amount_t getAmountWageredCurrentRound(seat_type_t seat) const {
    return _wagered_current_round[seat];
  }

  chip_amount_t getRemainingChips(seat_type_t seat) const {
    return _starting_stack_size - _wagered_current_round[seat] - _wagered_prior_rounds[seat];
  }

  chip_amount_t getStartingStackSize() const { return _starting_stack_size; }
  chip_amount_t getSmallBlindAmount() const { return _small_blind_amount; }
  chip_amount_t getBigBlindAmount() const { return _big_blind_amount; }
};

#include "PublicHandStateINLINES.cpp"

