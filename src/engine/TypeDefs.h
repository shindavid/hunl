#pragma once

#include <cstdint>

typedef float probability_t;
typedef uint64_t session_id_t;
typedef uint64_t player_id_t;
typedef uint64_t hand_id_t;
typedef int32_t chip_amount_t;
typedef uint8_t seat_t;

enum action_type_t {
  ACTION_BET,
  ACTION_RAISE,
  ACTION_CHECK,
  ACTION_CALL,
  ACTION_FOLD
};

enum BlindType {
  SMALL_BLIND,
  BIG_BLIND
};

enum BettingRound {
  PREFLOP,
  FLOP,
  TURN,
  RIVER
};

#define NUM_PLAYERS 2

