#pragma once

#include "engine/Player.h"
#include "engine/TypeDefs.h"

class SessionParams {
protected:
  const session_id_t _id;  // eventually get assigned from a database

  Player* const _players[2];
  const chip_amount_t _starting_stack_size;  // every hand, stacks reset to this size
  const chip_amount_t _small_blind_size;
  const chip_amount_t _big_blind_size;

  SessionParams(session_id_t id, Player* p0, Player* p1, chip_amount_t starting_stack_size,
      chip_amount_t small_blind_size, chip_amount_t big_blind_size, uint64_t seed);

public:
  session_id_t getID() const { return _id; }
  Player* getPlayer(seat_type_t seat) const { return _players[seat]; }
  chip_amount_t getStartingStackSize() const { return _starting_stack_size; }
  chip_amount_t getSmallBlindSize() const { return _small_blind_size; }
  chip_amount_t getBigBlindSize() const { return _big_blind_size; }
};

#include "engine/SessionParamsINLINES.cpp"

