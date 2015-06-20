PublicHandState::PublicHandState(const SessionParams& session_params, 
    const SessionState& session_state) :
  _session_params(session_params),
  _session_state(session_state)
{
  for (seat_t seat=0; seat<2; ++seat) {
    _wagered_current_round[seat] = 0;
    _wagered_prior_rounds[seat] = 0;
    _folded[seat] = false;
    _action_count[seat] = 0;
  }
  _global_action_count = 1;
  _action_on = session_state.getButton();
  _is_current_betting_round_done = false;
}
  
chip_amount_t PublicHandState::getPotentialPotSize() const {
  chip_amount_t pot = 0;
  for (seat_t seat=0; seat<2; ++seat) {
    pot += _wagered_prior_rounds[seat];
  }
  pot += std::max(_wagered_current_round[0], _wagered_current_round[1]);
  return pot;
}

chip_amount_t PublicHandState::getPotSize() const {
  chip_amount_t pot = 0;
  for (seat_t seat=0; seat<2; ++seat) {
    pot += _wagered_current_round[seat];
    pot += _wagered_prior_rounds[seat];
  }
  return pot;
}

bool PublicHandState::isCurrentBettingRoundDone() const {
  if (hasFolded(0) || hasFolded(1)) return true;

  for (seat_t seat=0; seat<2; ++seat) {
    if (!isAllIn(seat)) {
      if (!_folded[seat] && _action_count[seat] < _global_action_count) return false;
    }
  }
  return true;
}

bool PublicHandState::validate_chip_amounts() const {
  return (_wagered_prior_rounds[0] + _wagered_current_round[0] <= _get_starting_stack_size()) &&
    (_wagered_prior_rounds[1] + _wagered_current_round[1] <= _get_starting_stack_size());
}

void PublicHandState::setFolded(seat_t seat) {
  _folded[seat] = true;
  _is_current_betting_round_done = true;
}

void PublicHandState::add(ps::Card card) {
  _board.insert(card);
  _board_cards.push_back(card);
}

void PublicHandState::advanceBettingRound() {
  for (seat_t seat=0; seat<2; ++seat) {
    _wagered_prior_rounds[seat] += _wagered_current_round[seat];
    _wagered_current_round[seat] = 0;
    _action_count[seat] = 0;
  }
  
  _global_action_count = 1;
  _action_on = !_session_state.getButton();
  _is_current_betting_round_done = isAllIn(0) || isAllIn(1);  // technically only need to test 1
}

bool PublicHandState::isAllIn(seat_t seat) const {
  return _wagered_prior_rounds[seat] + _wagered_current_round[seat] == _get_starting_stack_size();
}
