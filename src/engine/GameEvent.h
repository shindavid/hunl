#pragma once

#include "pokerstove/peval/Card.h"
#include "engine/PublicHandState.h"
#include "engine/TypeDefs.h"

namespace ps = pokerstove;

/*
 * GameEvent is the base class of all event types.
 *
 * subclass DealEvent:
 * - HoleCardDealEvent
 * - FlopDealEvent
 * - TurnDealEvent
 * - RiverDealEvent
 *
 * subclass PlayerEvent:
 * - BetEvent
 * - RaiseEvent
 * - FoldEvent
 * - CheckEvent
 * - CallEvent
 */
class GameEvent {
protected:
  const PublicHandState& _public_state;
  const uint64_t _id;

public:
  GameEvent(const PublicHandState& public_state) : _public_state(public_state), _id(__next_id++) {}

  uint64_t getID() const { return _id; }
  hand_id_t getHandID() const { return _hand_id; }
  const PublicHandState& getPublicHandState() const { return _public_state; }

private:
  static uint64_t __next_id;
};

extern uint64_t GameEvent::__next_id = 1;

/*
 * A base class. Any event associated with a specific player.
 */
class PlayerEvent {
private:
  player_id_t _player_id;

public:
  PlayerEvent(player_id_t player_id) : _player_id(player_id) {}

  player_id_t getPlayerID() const { return _player_id; }
};

template<int n>
class DealEvent {
private:
  ps::Card _cards[n];

public:
  DealEvent(ps::Card* cards) {
    for (int i=0; i<n; ++i) {
      _cards[i] = cards[i];
    }
  }

  ps::Card getCard(int i) const { return _cards[i]; }

  static const int NUM_CARDS = n;
};

class HoleCardDealEvent : public GameEvent, public DealEvent<2> {
private:
  player_id_t _player_id;

public:
  HoleCardDealEvent(const PublicHandState& public_state, 
      player_id_t player_id, ps::Card cards[2]) :
    GameEvent(public_state), DealEvent<2>(cards), _player_id(player_id) {}
  
  player_id_t getPlayerID() const { return _player_id; }
};

class FlopDealEvent : public GameEvent, public DealEvent<3> {
public:
  FlopDealEvent(const PublicHandState& public_state, ps::Card cards[3]) :
    GameEvent(public_state), DealEvent<3>(cards) {}
};

class TurnDealEvent : public GameEvent, public DealEvent<1> {
public:
  TurnDealEvent(const PublicHandState& public_state, ps::Card card) :
    GameEvent(public_state), DealEvent<3>(&card) {}
};

class RiverDealEvent : public GameEvent, public DealEvent<1> {
public:
  RiverDealEvent(const PublicHandState& public_state, ps::Card card) :
    GameEvent(public_state), DealEvent<3>(&card) {}
};

class BettingDecision : public GameEvent, public PlayerEvent {
public:
  BettingDecision(const PublicHandState& public_state, player_id_t player_id) :
    GameEvent(public_state), PlayerEvent(player_id) {}
};

template <action_type_t action_type>
class BettingDecision_Impl : public BettingDecision {
private:
  chip_amount_t _chip_amount;

public:
  BettingDecision_Impl(const PublicHandState& public_state, player_id_t player_id, 
      chip_amount_t chip_amount=0) :
    BettingDecision(public_state, player_id), _chip_amount(chip_amount) {}
  
  chip_amount_t getChipAmount() const { return _chip_amount; }
  static const action_type ACTION_TYPE = action_type;
};

typedef BettingDecision_Impl<ACTION_BET> BetDecision;
typedef BettingDecision_Impl<ACTION_RAISE> RaiseDecision;
typedef BettingDecision_Impl<ACTION_CHECK> CheckDecision;
typedef BettingDecision_Impl<ACTION_CALL> CallDecision;
typedef BettingDecision_Impl<ACTION_FOLD> FoldDecision;

class BettingDecisionRequest : public GameEvent, public PlayerEvent {
public:
  BettingDecisionRequest(const PublicHandState& public_state, player_id_t player_id) :
    GameEvent(public_state), PlayerEvent(player_id) {}

  template<typename Event> void validate(const Event& decision) const;
  bool facingBet() const;
  chip_amount_t betAmountFaced() const;  // this is the amount you would call with, only defined if facingBet() == true
  chip_amount_t minLegalRaiseAmount() const;  // only defined if facingBet() == true
  chip_amount_t maxLegalRaiseAmount() const;  // only defined if facingBet() == true

  bool canCheck() const { return !facingBet(); }

  bool canBet() const { return !facingBet(); }
  chip_amount_t minLegalBetAmount() const;
  chip_amount_t maxLegalBetAmount() const;
  
  bool canCall() const { return facingBet(); }
  chip_amount_t legalCallAmount() const;
  
  bool canFold() const { return facingBet(); }

  bool canRaise() const;
  chip_amount_t minLegalRaiseAmount() const;  // only defined if facingBet() == true
  chip_amount_t maxLegalRaiseAmount() const;  // only defined if facingBet() == true

  chip_amount_t legalizeBet(chip_amount_t amount) const;
  chip_amount_t legalizeRaise(chip_amount_t amount) const;
};

class BlindPostEvent {
private:
  chip_amount_t _amount;

public:
  BlindPostEvent(const PublicHandState& public_state, player_id_t player_id, chip_amount_t amount) :
    GameEvent(public_state), PlayerEvent(player_id), _amount(amount) {}

  chip_amount_t getAmount() const { return _amount; }
};

class BlindPostRequest : public GameEvent, public PlayerEvent {
private:
  chip_amount_t _amount;

public:
  BlindPostRequest(const PublicHandState& public_state, player_id_t player_id, chip_amount_t amount) :
    GameEvent(public_state), PlayerEvent(player_id), _amount(amount) {}

  chip_amount_t getAmount() const { return _amount; }

  void validate(const BlindPostEvent& event) const {
    assert(_amount == event.getAmount());
  }
};

#include "GameEventINLINES.cpp"

