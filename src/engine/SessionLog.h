#pragma once

#include "engine/GameEvent.h"
#include "engine/BettingRules.h"
#include "engine/Session.h"

class SessionLog {
private:
  void _recordFlop(const HandState& state);
  void _recordTurn(const HandState& state);
  void _recordRiver(const HandState& state);

  void _recordBet(const Session& session, const HandState& state, const BettingDecision& event);
  void _recordRaise(const Session& session, const HandState& state, const BettingDecision& event);
  void _recordCheck(const Session& session, const HandState& state, const BettingDecision& event);
  void _recordCall(const Session& session, const HandState& state, const BettingDecision& event);
  void _recordFold(const Session& session, const HandState& state, const BettingDecision& event);

public:
  void recordSessionStart(const Session& session);
  void recordHandStart(const Session& session, const HandState& state);
  
  template<typename E> void record(const Session& session, const HandState& state, const E& event);
};

#include "engine/SessionLogINLINES.cpp"
