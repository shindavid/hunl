
RandomFLPlayer::RandomFLPlayer(const BettingRules* betting_rules,
    const random_fl_params_t& random_params, const char* name)
  : Player(betting_rules, name)
  , _fcr_dice(random_params._probs) {}

BettingDecision RandomFLPlayer::makeDecision(const HandState& state) {
  chip_amount_t c = _betting_rules->getLegalCheckOrCall(state);
  random_fl_params_t::Response choice = (random_fl_params_t::Response)_fcr_dice.roll();
  switch (choice) {
    case random_fl_params_t::F: return _createBettingDecision(state, 0);
    case random_fl_params_t::C: return _createBettingDecision(state, c);
    default: return _createBettingDecision(state, _betting_rules->getMinLegalBetOrRaise(state));
  }
}

