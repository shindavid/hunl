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
};

#include "SessionLogINLINES.cpp"
