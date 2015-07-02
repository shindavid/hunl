#pragma once

#include "pokerstove/peval/Card.h"
#include "engine/HandState.h"
#include "engine/TypeDefs.h"

#include <assert.h>
#include <boost/shared_ptr.hpp>

namespace ps = pokerstove;

class PublicDealEvent {
private:
  ps::Card _cards[4];
  int _n;

public:
  PublicDealEvent(ps::Card* cards, int n) {
    assert(n>=0 && n<=4);
    for (int i=0; i<n; ++i) {
      _cards[i] = cards[i];
    }

    _n = n;
  }

  ps::Card getCard(int i) const { return _cards[i]; }
  int getNumCards() const { return _n; }
};
static_assert(sizeof(PublicDealEvent)<=8, "oversized PublicDealEvent");

template<int n>
class PrivateDealEvent {
private:
  ps::Card _cards[n];
  const seat_t _seat;

public:
  PrivateDealEvent(ps::Card cards[n], seat_t seat) : _seat(seat) {
    for (int i=0; i<n; ++i) {
      _cards[i] = cards[i];
    }
  }
  
  seat_t getSeat() const { return _seat; }
  ps::Card getCard(int i) const { return _cards[i]; }
};

typedef PrivateDealEvent<2> HoleCardDealEvent;

class BettingDecision {
private:
  /*
   * How many additional chips to put into the pot on the current action.
   */
  const chip_amount_t _amount;
  const action_type_t _action_type;

public:
  BettingDecision(chip_amount_t amount, action_type_t action_type) : 
    _amount(amount), _action_type(action_type) {}

  chip_amount_t getAmount() const { return _amount; }
  action_type_t getActionType() const { return _action_type; }
};

class BettingDecisionRequest {
private:
  const HandState* _hand_state;

public:
  BettingDecisionRequest(const HandState* hand_state) : _hand_state(hand_state) {}

  void validate(BettingDecision decision) const;
  
  /*
   * If betting/raising, the minimum/maximum amount of chips that can be put in.
   *
   * Accounts for stack sizes.
   */
  chip_amount_t minLegalAdditionalAmount() const;
  chip_amount_t maxLegalAdditionalAmount() const;
  chip_amount_t getPotSize() const { return _hand_state->getPotSize(); }
  chip_amount_t getPotentialPotSize() const { return _hand_state->getPotentialPotSize(); }

  /*
   * 0 means you can check.
   */
  chip_amount_t legalCallAmount() const;

  chip_amount_t legalizeAdditionalAmount(chip_amount_t amount) const;

  const HandState* getHandState() const { return _hand_state; }
};

class BlindPostDecision {
private:
  const chip_amount_t _amount;
  const BlindType _btype;

public:
  BlindPostDecision(chip_amount_t amount, BlindType btype) : _amount(amount), _btype(btype) {}

  chip_amount_t getAmount() const { return _amount; }
  BlindType getBlindType() const { return _btype; }
};

class BlindPostRequest {
private:
  const HandState* _hand_state;

public:
  BlindPostRequest(const HandState* hand_state) : _hand_state(hand_state) {}

  void validate(BlindPostDecision decision) const;
  
  chip_amount_t getAmount() const;
  BlindType getBlindType() const;
};

class ShowdownEvent {
private:
  const ps::Card _hole_cards[2];
  const ps::PokerEvaluation _eval;
  const seat_t _seat;

public:
  ShowdownEvent(ps::Card c0, ps::Card c1, ps::PokerEvaluation eval, seat_t seat)
    : _hole_cards{c0, c1}
    , _eval(eval)
    , _seat(seat) {}

  ps::Card getCard(int i) const { return _hole_cards[i]; }
  ps::PokerEvaluation getEval() const { return _eval; }
  seat_t getSeat() const { return _seat; }
};

class PotWinEvent {
private:
  const HandState* _hand_state;
  seat_t _seat;

public:
  PotWinEvent(const HandState* hand_state, seat_t seat) : _hand_state(hand_state), _seat(seat) {
    assert(getCalledPotSize() + getUncalledBetSize() == hand_state->getPotSize());
  }

  seat_t getSeat() const { return _seat; }
  chip_amount_t getCalledPotSize() const;
  chip_amount_t getUncalledBetSize() const;
};

class PotSplitEvent {
private:
  const HandState* _hand_state;

public:
  PotSplitEvent(const HandState* hand_state) : _hand_state(hand_state) {
    static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
    assert(2*getSplitAmount() == _hand_state->getPotSize());
  }

  chip_amount_t getSplitAmount() const;
};

#include "engine/GameEventINLINES.cpp"

