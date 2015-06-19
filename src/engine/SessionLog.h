#pragma once

#include "engine/GameEvent.h"
#include "engine/SessionState.h"
#include "engine/SessionParams.h"

class SessionLog {
public:
  void recordSessionStart(const SessionParams& state, uint64_t base_seed);
  void recordHandStart(const SessionState& state);

  void record(const HoleCardDealEvent& event);
  void record(const FlopDealEvent& event);
  void record(const TurnDealEvent& event);
  void record(const RiverDealEvent& event);
  
  void record(const FoldEvent& event);
  void record(const CheckEvent& event);
  void record(const CallEvent& event);
  void record(const BetEvent& event);
  void record(const RaiseEvent& event);
  
  void record(const ShowdownEvent& event);
  void record(const PotWinEvent& event);
  void record(const PotSplitEvent& event);
};

#include "engine/SessionLogINLINES.cpp"
