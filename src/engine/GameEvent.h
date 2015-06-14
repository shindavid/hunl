#pragma once

#include "pokerstove/peval/Card.h"
#include "TypeDefs.h"

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
private:
  uint64_t _id;  // unique per-session
  hand_id_t _hand_id;
  session_id_t _session_id;

public:
  GameEvent(hand_id_t hand_id, session_id_t session_id) {
    this->_id = __next_id++;
    this->_hand_id = hand_id;
    this->_session_id = session_id;
  }

  uint64_t getID() const { return _id; }
  hand_id_t getHandID() const { return _hand_id; }
  session_id_t getSessionID() const { return _session_id; }
  
private:
  static uint64_t __next_id;
};

extern uint64_t GameEvent::__next_id = 1;

/*
 * A base class. Any event performed by a specific player.
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
  HoleCardDealEvent(hand_id_t hand_id, session_id_t session_id, 
      player_id_t player_id, ps::Card cards[2]) :
    GameEvent(hand_id, session_id), DealEvent<2>(cards), _player_id(player_id) {}
  
  player_id_t getPlayerID() const { return _player_id; }
};

class FlopDealEvent : public GameEvent, public DealEvent<3> {
public:
  FlopDealEvent(hand_id_t hand_id, session_id_t session_id, ps::Card cards[3]) :
    GameEvent(hand_id, session_id), DealEvent<3>(cards) {}
};

class TurnDealEvent : public GameEvent, public DealEvent<1> {
public:
  TurnDealEvent(hand_id_t hand_id, session_id_t session_id, ps::Card card) :
    GameEvent(hand_id, session_id), DealEvent<3>(&card) {}
};

class RiverDealEvent : public GameEvent, public DealEvent<1> {
public:
  RiverDealEvent(hand_id_t hand_id, session_id_t session_id, ps::Card card) :
    GameEvent(hand_id, session_id), DealEvent<3>(&card) {}
};

class BettingDecision_Base : public GameEvent, public PlayerEvent {
public:
  BettingDecision_Base(hand_id_t hand_id, session_id_t session_id, player_id_t player_id) :
    GameEvent(hand_id, session_id), PlayerEvent(player_id) {}
};

template <action_type_t action_type>
class BettingDecision : public BettingDecision_Base {
private:
  chip_amount_t _chip_amount;

public:
  BettingDecision(hand_id_t hand_id, session_id_t session_id, player_id_t player_id, 
      chip_amount_t chip_amount=0) :
    BettingDecision_Base(hand_id, session_id, player_id), _chip_amount(chip_amount)
  {
    if (action_type == ACTION_CHECK || action_type == ACTION_FOLD) {
      assert(chip_amount==0);
    } else {
      assert(chip_amount>0);
    }
  }
  
  chip_amount_t getChipAmount() const { return _chip_amount; }
  static const action_type ACTION_TYPE = action_type;
};

typedef BettingDecision<ACTION_BET> BetDecision;
typedef BettingDecision<ACTION_RAISE> RaiseDecision;
typedef BettingDecision<ACTION_CHECK> CheckDecision;
typedef BettingDecision<ACTION_CALL> CallDecision;
typedef BettingDecision<ACTION_FOLD> FoldDecision;

