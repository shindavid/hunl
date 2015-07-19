#pragma once

#include "engine/BettingRules.h"
#include "engine/GameEvent.h"
#include "engine/HandState.h"
#include "engine/TypeDefs.h"

#include <string>

class Player {
protected:
  const BettingRules* const _betting_rules;
  const player_id_t _id;  // eventually retrieve from a database
  const std::string _name;

  BettingDecision _createBettingDecision(const HandState& hand_state, chip_amount_t amount) const;

public:
  Player(const BettingRules* betting_rules, const char* name);

  player_id_t getID() const;
  const char* getName() const;

  virtual BettingDecision makeDecision(const HandState& hand_state) = 0;
  virtual BlindPostDecision handleBlindRequest(BlindType btype);

  virtual void handleEvent(const HandState& hand_state, const HoleCardDealEvent& event) {};
  virtual void handleEvent(const HandState& hand_state, const BettingDecision& event) {};
  virtual void handleEvent(const HandState& hand_state, const PublicDealEvent& event) {};
  virtual void handleEvent(const HandState& hand_state, const ShowdownEvent& event) {};
  virtual void handleEvent(const HandState& hand_state, const PotWinEvent& event) {};
  virtual void handleEvent(const HandState& hand_state, const PotSplitEvent& event) {};

private:
  static player_id_t __next_id;
};

#include "PlayerINLINES.cpp"

