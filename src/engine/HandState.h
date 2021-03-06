#pragma once

#include "engine/Board.h"
#include "engine/Holding.h"
#include "engine/TypeDefs.h"
#include "pokerstove/peval/CardSet.h"

namespace ps = pokerstove;

/*
 * Combination of private and public hand state information.
 */
class HandState {
private:
  const chip_amount_t _starting_stack_size;
  Board _board;

  chip_amount_t _stack_size[NUM_PLAYERS];
  chip_amount_t _wagered_prior_rounds[NUM_PLAYERS];
  chip_amount_t _wagered_current_round[NUM_PLAYERS];
  const seat_t _button;
  seat_t _action_on;
  int _global_action_count = 1;  // every bet/raise increments this counter
  int _action_count[NUM_PLAYERS];  // _action_count[i] set to _global_action_count when player i act
  BettingRound _betting_round;
  bool _folded[NUM_PLAYERS];
  bool _is_current_betting_round_done = false;

public:
  HandState(chip_amount_t starting_stack_size, seat_t button);

  void payBlinds(seat_t button, chip_amount_t small_blind, chip_amount_t big_blind);
  seat_t getButton() const;
  void setCurrentBettingRoundDone(bool x);
  bool isDone() const;
  bool isCurrentBettingRoundDone() const;
  
  seat_t getActionOn() const;
  void setActionOn(seat_t seat);

  BettingRound getBettingRound() const;
  void setBettingRound(BettingRound x);

  bool hasFolded(seat_t seat) const;
  void setFolded(seat_t seat, bool x);

  chip_amount_t getAmountWageredPriorRounds(seat_t seat) const;
  chip_amount_t getAmountWageredCurrentRound(seat_t seat) const;
  chip_amount_t getMaxAmountWageredCurrentRound() const;
  void addWagerCurrentRound(seat_t seat, chip_amount_t amount);
  void incrementGlobalActionCount(bool x=true);
  void setActionCount(seat_t seat);

  chip_amount_t getRemainingChips(seat_t seat) const;
  chip_amount_t getPotentialPotSize() const;  // if current bet called
  chip_amount_t getPotSize() const;

  void add(ps::Card card);
  void advanceBettingRound();

  bool isAllIn(seat_t seat) const;
  const Board& getBoard() const;
  Board& getBoard();
  bool isPreflop() const;

  bool _validate_chip_amounts() const;  // for debugging
};

#include "engine/HandStateINLINES.cpp"

