#pragma once

#include "engine/GameEvent.h"
#include "engine/TypeDefs.h"

#include <string>

class Player {
private:
  player_id_t _id;  // eventually retrieve from a database
  std::string _name;

protected:
  CheckDecision _create_check(const BettingDecisionRequest& request);
  CallDecision _create_call(const BettingDecisionRequest& request);
  FoldDecision _create_fold(const BettingDecisionRequest& request);
  BetDecision _create_bet(const BettingDecisionRequest& request, chip_amount_t amount);
  RaiseDecision _create_raise(const BettingDecisionRequest& request, chip_amount_t amount);

public:
  Player(const char* name) {
    _id = __next_id++;
    _name = name;
  }

  player_id_t getID() const { return _id; }
  const char* getName() const { return _name.c_str(); }

  virtual BettingDecision handleRequest(const BettingDecisionRequest& request) = 0;
  virtual BlindPostEvent handleRequest(const BlindPostRequest& request);
  virtual void handleEvent(const HoleCardDealEvent& event) {};
  virtual void handleEvent(const BettingDecision& event) {};
  virtual void handleEvent(const FlopDealEvent& event) {};
  virtual void handleEvent(const TurnDealEvent& event) {};
  virtual void handleEvent(const RiverDealEvent& event) {};
  virtual void handleEvent(const ShowdownEvent& event) {};
  virtual void handleEvent(const PotWinEvent& event) {};
  virtual void handleEvent(const PotSplitEvent& event) {};

private:
  static player_id_t __next_id;
};

#include "PlayerINLINES.cpp"

