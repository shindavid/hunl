#pragma once

#include "pokerstove/peval/Card.h"

namespace ps = pokerstove;

/*
 * GameEvent is the base class of all event types.
 *
 * Subclasses:
 * - HoleCardDealEvent
 * - FlopDealEvent
 * - TurnDealEvent
 * - RiverDealEvent
 *
 * - BetEvent
 * - RaiseEvent
 * - FoldEvent
 * - CheckEvent
 * - CallEvent
 */
class GameEvent {
private:
  uint64_t _id;  // unique across all GameEvent's
  session_id_t _session_id;

public:
  GameEvent(session_id_t session_id) {
    this->_id = __next_id++;
    this->_session_id = session_id;
  }

  uint64_t getID() const { return _id; }
  session_id_t getSessionID() const { return _session_id; }
  
private:
  static uint64_t __next_id;
};

/*
 * A base class. Any event that is particular to a specific player.
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
}

class HoleCardDealEvent : public GameEvent, public DealEvent<2>, public PlayerEvent {
public:
  HoleCardDealEvent(session_id_t session_id, player_id_t player_id, ps::Card cards[2]) :
    GameEvent(session_id), DealEvent<2>(cards), PlayerEvent(player_id) {}
};

class FlopDealEvent : public GameEvent, public DealEvent<3> {
public:
  FlopDealEvent(session_id_t session_id, ps::Card cards[3]) :
    GameEvent(session_id), DealEvent<3>(cards) {}
};

class TurnDealEvent : public GameEvent, public DealEvent<1> {
public:
  TurnDealEvent(session_id_t session_id, ps::Card card) :
    GameEvent(session_id), DealEvent<3>(&card) {}
};

class RiverDealEvent : public GameEvent, public DealEvent<1> {
public:
  RiverDealEvent(session_id_t session_id, ps::Card card) :
    GameEvent(session_id), DealEvent<3>(&card) {}
};

class BetEvent : public GameEvent, public PlayerEvent {
private:
  chip_amount_t _size;

public:
  BetEvent(session_id_t session_id, player_id_t player_id, chip_amount_t size) :
    GameEvent(session_id), PlayerEvent(player_id), _size(size) {}

  chip_amount_t getSize() const { return _size; }
};

/*
 * A size of 10 means an additional 10 on top of opponent's bet/raise.
 */
class RaiseEvent : public GameEvent, public PlayerEvent {
private:
  chip_amount_t _size;

public:
  RaiseEvent(session_id_t session_id, player_id_t player_id, chip_amount_t size) :
    GameEvent(session_id), PlayerEvent(player_id), _size(size) {}

  chip_amount_t getSize() const { return _size; }
};

class FoldEvent : public GameEvent, public PlayerEvent {
public:
  FoldEvent(session_id_t session_id, player_id_t player_id) :
    GameEvent(session_id), PlayerEvent(player_id) {}
};

class CheckEvent : public GameEvent, public PlayerEvent {
public:
  CheckEvent(session_id_t session_id, player_id_t player_id) :
    GameEvent(session_id), PlayerEvent(player_id) {}
};

class CallEvent : public GameEvent, public PlayerEvent {
public:
  CallEvent(session_id_t session_id, player_id_t player_id) :
    GameEvent(session_id), PlayerEvent(player_id) {}
};

