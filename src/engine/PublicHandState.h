#pragma once

#include "engine/TypeDefs.h"
#include "engine/SessionState.h"
#include "engine/SessionParams.h"
#include "pokerstove/peval/CardSet.hpp"

/*
 * Hand state, only the public part of it.
 */
class PublicHandState {
protected:
  const SessionParams& _session_params;
  const SessionState& _session_state;
  
  chip_amount_t _wagered_prior_rounds[2];
  chip_amount_t _wagered_current_round[2];
  seat_t _action_on;
  bool _folded[2];
  bool _is_current_betting_round_done;

  ps::CardSet _board;
  std::vector<ps::Card> _board_cards;  // maintains ordering

  chip_amount_t _get_starting_stack_size() const { return _session_params.getStartinStackSize(); }

public:
  PublicHandState(const SessionParams& session_params, const SessionState& session_state);
  
  bool isDone() const { return _folded[0] || _folded[1]; }
  bool isCurrentBettingRoundDone() const { return _is_current_betting_round_done; }
  void setCurrentBettingRoundDone(bool x) { _is_current_betting_round_done = x; }
  
  seat_t getActionOn() const { return _action_on; }
  void setActionOn(seat_t seat) { _action_on = seat; }
  
  bool hasFolded(seat_t seat) const { return _folded[seat]; }
  void setFolded(seat_t seat) { _folded[seat] = true; }

  chip_amount_t getAmountWageredPriorRounds(seat_t seat) const {
    return _wagered_prior_rounds[seat];
  }
  chip_amount_t getAmountWageredCurrentRound(seat_t seat) const {
    return _wagered_current_round[seat];
  }
  void addWagerCurrentRound(seat_t seat, chip_amount_t amount) {
    _wagered_current_round[seat] += amount;
  }

  chip_amount_t getRemainingChips(seat_t seat) const {
    return _get_starting_stack_size() - _wagered_current_round[seat] - _wagered_prior_rounds[seat];
  }

  chip_amount_t getPotentialPotSize() const;  // if current bet called
  chip_amount_t getPotSize() const;

  const SessionParams& getSessionParams() const { return _session_params; }
  const SessionState& getSessionState() const { return _session_state; }
  
  bool validate_chip_amounts() const;  // for debugging
  void add(ps::Card card);
  void advanceBettingRound();

  ps::CardSet getBoard() const { return _board; }
  ps::Card getBoardCard(int i) const { return _board_cards[i]; }
  
  bool isAllIn(seat_t seat) const;
};

#include "engine/PublicHandStateINLINES.cpp"

