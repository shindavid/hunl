#pragma once

#include "engine/TypeDefs.h"

class SessionState {
private:
  seat_t _button = 0;
  chip_amount_t _score[NUM_PLAYERS]; 
  hand_id_t _current_hand_id = 0;

public:
  SessionState() {
    for (int i=0; i<NUM_PLAYERS; ++i) _score[i] = 0;
  }
  
  session_id_t getCurrentHandID() const { return _current_hand_id; }
  seat_t getButton() const { return _button; }
  chip_amount_t getScore(seat_t seat) const { return _score[seat]; }

  void moveButton() { _button = (_button + 1) % NUM_PLAYERS; }
  void updateScore(seat_t seat, chip_amount_t delta) { _score[seat] += delta; }
  void incrementHandID() { _current_hand_id++; }
};

