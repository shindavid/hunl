#pragma once

#include <cstdint>

typedef float probability_t;
typedef uint64_t session_id_t;
typedef uint64_t player_id_t;
typedef uint64_t hand_id_t;
typedef uint32_t chip_amount_t;
typedef bool seat_t;

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

