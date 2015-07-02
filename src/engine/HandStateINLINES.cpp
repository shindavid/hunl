// Included by HandState.h

HandState::HandState(const SessionParams& session_params, 
    const SessionState& session_state)
  : _session_params(&session_params)
  , _session_state(&session_state)
{
  for (int i=0; i<NUM_PLAYERS; ++i) {
    _wagered_prior_rounds[i] = 0;
    _wagered_current_round[i] = 0;
    _action_count[i] = 0;
    _folded[i] = false;
  }
}

chip_amount_t HandState::getPotSize() const {
  chip_amount_t total = 0;
  for (int i=0; i<NUM_PLAYERS; ++i) {
    total += _wagered_prior_rounds[i];
    total += _wagered_current_round[i];
  }
  return total;
}

chip_amount_t HandState::getPotentialPotSize() const {
  chip_amount_t total = 0;
  for (int i=0; i<NUM_PLAYERS; ++i) {
    total += _wagered_prior_rounds[i];
  }
  total += getMaxAmountWageredCurrentRound() * NUM_PLAYERS;
  return total;
}

bool HandState::_validate_chip_amounts() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  return
    _wagered_prior_rounds[0] + _wagered_current_round[0] <= _session_params->getStartingStackSize() &&
    _wagered_prior_rounds[1] + _wagered_current_round[1] <= _session_params->getStartingStackSize();
}

bool HandState::isCurrentBettingRoundDone() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  if (hasFolded(0) || hasFolded(1)) return true;

  for (seat_t seat=0; seat<2; ++seat) {
    if (!isAllIn(seat)) {
      if (_action_count[seat] < _global_action_count) return false;
    }
  }
  return true;
}

void HandState::advanceBettingRound() {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  for (seat_t seat=0; seat<2; ++seat) {
    _wagered_prior_rounds[seat] += _wagered_current_round[seat];
    _wagered_current_round[seat] = 0;
    _action_count[seat] = 0;
  }
      
  seat_t button = _session_state->getButton();
  _global_action_count += 1;
  _action_on = _board.getSize()==0 ? button : !button;
  _is_current_betting_round_done = isAllIn(0) || isAllIn(1);  // technically only need to test 1
}

