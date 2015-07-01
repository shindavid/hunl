#pragma once

#include "engine/GameEvent.h"
#include "engine/SessionParams.h"
#include "engine/SessionState.h"

class SessionLog {
private:
  void _recordFlop(const HandState* state);
  void _recordTurn(const HandState* state);
  void _recordRiver(const HandState* state);

  void _recordBet(const HandState* state, const BettingDecision* event);
  void _recordRaise(const HandState* state, const BettingDecision* event);
  void _recordCheck(const HandState* state, const BettingDecision* event);
  void _recordCall(const HandState* state, const BettingDecision* event);
  void _recordFold(const HandState* state, const BettingDecision* event);

public:
  void recordSessionStart(const SessionParams& state, uint64_t base_seed);
  void recordHandStart(const HandState* state);
  
  template<typename E> void record(const HandState* state, const E* event);
};

#include "engine/SessionLogINLINES.cpp"
