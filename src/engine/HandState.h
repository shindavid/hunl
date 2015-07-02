#pragma once

#include "engine/Board.h"
#include "engine/Holding.h"
#include "engine/TypeDefs.h"
#include "engine/SessionParams.h"
#include "engine/SessionState.h"
#include "pokerstove/peval/CardSet.h"

namespace ps = pokerstove;

/*
 * Combination of private and public hand state information.
 */
class HandState {
private:
  const SessionParams* _session_params;
  const SessionState* _session_state;
  Holding _holdings[NUM_PLAYERS];
  Board _board;

  chip_amount_t _wagered_prior_rounds[NUM_PLAYERS];
  chip_amount_t _wagered_current_round[NUM_PLAYERS];
  seat_t _action_on;
  int _global_action_count = 1;  // every bet/raise increments this counter
  int _action_count[NUM_PLAYERS];  // _action_count[i] set to _global_action_count when player i act
  bool _folded[NUM_PLAYERS];
  bool _is_current_betting_round_done = false;
  bool _showdown_performed = false;

public:
  HandState(const SessionParams& session_params,
      const SessionState& session_state);

  void setShowdownPerformed() { _showdown_performed = true; }
  Holding getHolding(seat_t seat) const {
    assert(_showdown_performed);
    return _holdings[seat];
  }

  void setHolding(seat_t seat, ps::CardSet set) { _holdings[seat] = Holding(set); }

  Player* getPlayer(seat_t seat) const { return _session_params->getPlayer(seat); }
  seat_t getButton() const { return _session_state->getButton(); }
  
  void setCurrentBettingRoundDone(bool x) { _is_current_betting_round_done = x; }

  bool isDone() const { return _folded[0] || _folded[1]; }
  bool isCurrentBettingRoundDone() const;
  
  seat_t getActionOn() const { return _action_on; }
  void setActionOn(seat_t seat) {
    _action_on = seat;
  }
  
  bool hasFolded(seat_t seat) const { return _folded[seat]; }
  void setFolded(seat_t seat, bool x) { _folded[seat] = x; }

  chip_amount_t getAmountWageredPriorRounds(seat_t seat) const {
    return _wagered_prior_rounds[seat];
  }
  chip_amount_t getAmountWageredCurrentRound(seat_t seat) const {
    return _wagered_current_round[seat];
  }
  chip_amount_t getMaxAmountWageredCurrentRound() const {
    chip_amount_t max = 0;
    for (seat_t seat=0; seat<2; ++seat) {
      max = std::max(max, _wagered_current_round[seat]);
    }
    return max;
  }
  void addWagerCurrentRound(seat_t seat, chip_amount_t amount) {
    _wagered_current_round[seat] += amount;
    //fprintf(stdout, "%s(%d,%d) -> %d\n", __func__, seat, amount, _wagered_current_round[seat]);
  }
  void incrementGlobalActionCount(bool x=true) { _global_action_count += x?1:0; }
  void setActionCount(seat_t seat) { _action_count[seat] = _global_action_count; }

  chip_amount_t getRemainingChips(seat_t seat) const {
    return _session_params->getStartingStackSize() - _wagered_current_round[seat] - 
      _wagered_prior_rounds[seat];
  }

  chip_amount_t getPotentialPotSize() const;  // if current bet called
  chip_amount_t getPotSize() const;

  const SessionParams& getSessionParams() const { return *_session_params; }
  const SessionState& getSessionState() const { return *_session_state; }
  
  void add(ps::Card card);
  void advanceBettingRound();

  bool isAllIn(seat_t seat) const { 
    return getRemainingChips(seat)==0; }
  
  const Board& getBoard() const { return _board; }
  Board& getBoard() { return _board; }
  bool isPreflop() const { return _board.getSize() == 0; }

  bool _validate_chip_amounts() const;  // for debugging
};

#include "engine/HandStateINLINES.cpp"

