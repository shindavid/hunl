#pragma once

#include "TypeDefs.h"
#include "GameEvent.h"
#include "pokerstove/peval/CardSet.hpp"

namespace ps = pokerstove;

/*
 * A single hand.
 */
class HandState {
private:
  SessionLog& _log;
  
  ps::CardSet _hole_cards[2];
  PublicHandState _public_hand_state;

public:
  HandState(SessionLog& log, session_id_t session_id, hand_id_t id,
      chip_amount_t starting_stack_size, seat_t button);

  const PublicHandState& getPublicState() const { return _public_hand_state; }

  typename<typename E>
    void handleEvent(seat_t seat, const E& event);
};

#include "SessionHandINLINES.cpp"

