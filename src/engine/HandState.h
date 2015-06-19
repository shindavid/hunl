#pragma once

#include "engine/TypeDefs.h"
#include "engine/GameEvent.h"
#include "engine/SessionLog.h"
#include "pokerstove/peval/CardSet.hpp"

namespace ps = pokerstove;

/*
 * Combination of private and public hand state information.
 */
class HandState {
private:
  SessionLog& _log;
  PublicHandState _public_hand_state;
  ps::CardSet _hole_cards[2];
  ps::Card _hole_cards_ordered[2][2];

public:
  HandState(SessionLog& log, const SessionParams& session_params, const SessionState& session_state);

  const PublicHandState& getPublicState() const { return _public_hand_state; }

  typename<typename E> void handleEvent(seat_t seat, const E& event);
  typename<typename E> void broadcastEvent(seat_t seat, const E& event) {
    _public_hand_state.getPlayer(seat)->handleEvent(event);
  }
  
  typename<typename E> void handleEvent(seat_t seat, const E& event);
  typename<typename E> void broadcastEvent(const E& event) {
    for (seat_t seat=0; seat<2; ++seat) {
      _public_hand_state.getPlayer(seat)->handleEvent(event);
    }
  }

  const ps::CardSet getHoleCards(seat_t seat) const {
    return _hole_cards[seat];
  }
  
  const ps::CardSet getHoleCard(seat_t seat, int i) const {
    return _hole_cards_ordered[seat][i];
  }
};

#include "engine/HandStateINLINES.cpp"

