#pragma once

#include "engine/GameEvent.h"
#include "engine/TypeDefs.h"

#include <string>

class Player {
private:
  player_id_t _id;  // eventually retrieve from a database
  std::string _name;

public:
  Player(const char* name) {
    _id = __next_id++;
    _name = name;
  }

  player_id_t getID() const { return _id; }
  const char* getName() const { return _name.c_str(); }

  virtual BettingDecision handleRequest(const BettingDecisionRequest& request) = 0;
  virtual BlindPostEvent handleRequest(const BlindPostRequest& request);
  virtual void handleEvent(const HoleCardDealEvent& event) = 0;
  virtual void handleEvent(const BettingDecision& event) = 0;
  virtual void handleEvent(const FlopDealEvent& event) = 0;
  virtual void handleEvent(const TurnDealEvent& event) = 0;
  virtual void handleEvent(const RiverDealEvent& event) = 0;

private:
  static player_id_t __next_id;
};

#include "PlayerINLINES.cpp"

