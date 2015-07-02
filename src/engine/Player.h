#pragma once

#include "engine/GameEvent.h"
#include "engine/HandState.h"
#include "engine/TypeDefs.h"

#include <string>

class Player {
protected:
  player_id_t _id;  // eventually retrieve from a database
  std::string _name;

  BettingDecision _createBettingDecision(
      const BettingDecisionRequest* request, chip_amount_t amount) const;

public:
  Player(const char* name) {
    _id = __next_id++;
    _name = name;
  }

  player_id_t getID() const { return _id; }
  const char* getName() const { return _name.c_str(); }

  virtual BettingDecision handleRequest(const BettingDecisionRequest* request) = 0;
  virtual BlindPostDecision handleRequest(const BlindPostRequest* request);

  virtual void handleEvent(const HandState& hand_state, const HoleCardDealEvent* event) {};
  virtual void handleEvent(const HandState& hand_state, const BettingDecision* event) {};
  virtual void handleEvent(const HandState& hand_state, const PublicDealEvent* event) {};
  virtual void handleEvent(const HandState& hand_state, const ShowdownEvent* event) {};
  virtual void handleEvent(const HandState& hand_state, const PotWinEvent* event) {};
  virtual void handleEvent(const HandState& hand_state, const PotSplitEvent* event) {};

private:
  static player_id_t __next_id;
};

#include "PlayerINLINES.cpp"

