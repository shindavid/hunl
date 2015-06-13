#pragma once

typedef probability_t float;
typedef session_id_t uint64_t;
typedef player_id_t uint64_t;
typedef hand_id_t uint64_t;
typedef chip_amount_t uint32_t;
typedef seat_t bool;

enum action_type_t {
  ACTION_BET,
  ACTION_RAISE,
  ACTION_CHECK,
  ACTION_CALL,
  ACTION_FOLD
};

