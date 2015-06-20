#pragma once

#include "engine/GameEvent.h"
#include "engine/SessionState.h"
#include "engine/SessionParams.h"

class SessionLog {
public:
  void recordSessionStart(const SessionParams& state, uint64_t base_seed);
  void recordHandStart(const SessionState& state);

  template<typename E> void record(const E* event);
};

#include "engine/SessionLogINLINES.cpp"
