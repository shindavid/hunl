#pragma once

#include "engine/Player.h"
#include "SimpleWeightedDice.h"

struct random_params_t {
  std::string _name;

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

class RandomPlayer : public Player {
private:
  SimpleWeightedDice<random_params_t::NUM_CAN_CHECK_RESPONSES> _check_dice;
  SimpleWeightedDice<random_params_t::NUM_CAN_CALL_RESPONSES> _call_dice;

public:
  RandomPlayer(const random_params_t& random_params) :
    Player(random_params._name.c_str()),
    _check_dice(random_params._check_probs),
    _call_dice(random_params._call_probs) {}

  virtual BettingDecision handleRequest(const BettingDecisionRequest& request);
};

#include "RandomPlayerINLINES.cpp"

