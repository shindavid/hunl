#pragma once

#include "engine/Player.h"
#include "SimpleWeightedDice.h"

struct random_fl_params_t {
  enum Response {
    F,
    C,
    R
  };

  double _probs[3];
};

class RandomFLPlayer : public Player {
private:
  SimpleWeightedDice<3> _fcr_dice;

public:
  RandomFLPlayer(const BettingRules* betting_rules, const random_fl_params_t& random_params,
      const char* name);

  virtual BettingDecision makeDecision(const HandState& hand_state);
};

#include "RandomFLPlayerINLINES.cpp"

