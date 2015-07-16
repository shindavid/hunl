
RandomNLPlayer::RandomNLPlayer(const BettingRules* betting_rules, const random_nl_params_t& random_params,
    const char* name)
  : Player(betting_rules, name)
  , _check_dice(random_params._check_probs)
  , _call_dice(random_params._call_probs) {}

BettingDecision RandomNLPlayer::makeDecision(const HandState& state) {
  chip_amount_t call_amount = _betting_rules->getLegalCheckOrCall(state);
  if (call_amount==0) {
    random_nl_params_t::CanCheckResponse choice = (random_nl_params_t::CanCheckResponse)_check_dice.roll();
    switch (choice) {
      case random_nl_params_t::CHECK: return _createBettingDecision(state, 0);
      case random_nl_params_t::MIN_BET:
        return _createBettingDecision(state, _betting_rules->getMinLegalBetOrRaise(state));
      case random_nl_params_t::HALF_POT_SIZED_BET:
        return _createBettingDecision(state, _betting_rules->legalize(state, state.getPotSize()/2));
      case random_nl_params_t::POT_SIZED_BET:
        return _createBettingDecision(state, _betting_rules->legalize(state, state.getPotSize()));
      case random_nl_params_t::ALL_IN_BET:
        return _createBettingDecision(state, _betting_rules->getMaxLegalBetOrRaise(state));
      default: return _createBettingDecision(state, 0);  // shouldn't get here
    }
  } else {  // canCall
    random_nl_params_t::CanCallResponse choice = (random_nl_params_t::CanCallResponse)_call_dice.roll();
    switch (choice) {
      case random_nl_params_t::FOLD: return _createBettingDecision(state, 0);
      case random_nl_params_t::CALL: return _createBettingDecision(state, call_amount);
      case random_nl_params_t::MIN_RAISE: 
        return _createBettingDecision(state, _betting_rules->getMinLegalBetOrRaise(state));
      case random_nl_params_t::POT_SIZED_RAISE: 
        return _createBettingDecision(state, _betting_rules->legalize(state,
              state.getPotentialPotSize() + call_amount));
      case random_nl_params_t::ALL_IN_RAISE:
        return _createBettingDecision(state, _betting_rules->getMaxLegalBetOrRaise(state));
      default: return _createBettingDecision(state, 0);  // shouldn't get here
    }
  }
}

