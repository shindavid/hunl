#pragma once

#include "engine/Player.h"
#include "SimpleWeightedDice.h"

struct random_nl_params_t {
  enum CanCheckResponse {
    CHECK,
    MIN_BET,
    HALF_POT_SIZED_BET,
    POT_SIZED_BET,
    ALL_IN_BET,
    NUM_CAN_CHECK_RESPONSES
  };

  enum CanCallResponse {
    FOLD,
    CALL,
    MIN_RAISE,
    POT_SIZED_RAISE,
    ALL_IN_RAISE,
    NUM_CAN_CALL_RESPONSES
  };

  double _check_probs[NUM_CAN_CHECK_RESPONSES];
  double _call_probs[NUM_CAN_CALL_RESPONSES];
};

class RandomNLPlayer : public Player {
private:
  SimpleWeightedDice<random_nl_params_t::NUM_CAN_CHECK_RESPONSES> _check_dice;
  SimpleWeightedDice<random_nl_params_t::NUM_CAN_CALL_RESPONSES> _call_dice;

public:
  RandomNLPlayer(const BettingRules* betting_rules, const random_nl_params_t& random_params,
      const char* name);

  virtual BettingDecision makeDecision(const HandState& hand_state);
};

#include "RandomNLPlayerINLINES.cpp"

