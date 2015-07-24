#pragma once

#include "pokerstove/peval/Card.h"
#include "engine/HandState.h"
#include "engine/Holding.h"
#include "engine/TypeDefs.h"

#include <assert.h>
#include <boost/shared_ptr.hpp>

namespace ps = pokerstove;

class PublicDealEvent {
private:
  ps::Card _cards[4];
  int _n;

public:
  PublicDealEvent(ps::Card* cards, int n);

  ps::Card getCard(int i) const;
  int getNumCards() const;
};
static_assert(sizeof(PublicDealEvent)<=8, "oversized PublicDealEvent");

template<int n>
class PrivateDealEvent {
private:
  ps::Card _cards[n];
  const seat_t _seat;

public:
  PrivateDealEvent(ps::Card cards[n], seat_t seat);
  
  seat_t getSeat() const;
  ps::Card getCard(int i) const;
};

typedef PrivateDealEvent<2> HoleCardDealEvent;

class BettingDecision {
private:
  /*
   * How many additional chips to put into the pot on the current action.
   */
  const chip_amount_t _amount;
  const ActionType _action_type;

public:
  BettingDecision(chip_amount_t amount, ActionType action_type);

  chip_amount_t getAmount() const;
  ActionType getActionType() const;
};

class BlindPostDecision {
private:
  const chip_amount_t _amount;
  const BlindType _btype;

public:
  BlindPostDecision(chip_amount_t amount, BlindType btype);

  chip_amount_t getAmount() const;
  BlindType getBlindType() const;
};

class ShowdownEvent {
private:
  const Holding _holding;
  const ps::PokerEvaluation _eval;
  const seat_t _seat;

public:
  ShowdownEvent(Holding holding, ps::PokerEvaluation eval, seat_t seat);

  Holding getHolding() const;
  ps::Card getCard(int i) const;
  ps::PokerEvaluation getEval() const;
  seat_t getSeat() const;
};

class PotWinEvent {
private:
  const HandState* _hand_state;
  seat_t _seat;

public:
  PotWinEvent(const HandState* hand_state, seat_t seat);

  seat_t getSeat() const;
  chip_amount_t getCalledPotSize() const;
  chip_amount_t getUncalledBetSize() const;
};

class PotSplitEvent {
private:
  const HandState* _hand_state;

public:
  PotSplitEvent(const HandState* hand_state);

  chip_amount_t getSplitAmount() const;
};

#include "engine/GameEventINLINES.cpp"

