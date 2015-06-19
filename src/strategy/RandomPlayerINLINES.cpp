
virtual BettingDecision RandomPlayer::handleRequest(const BettingDecisionRequest& request) {
  if (request.canCheck()) {
    CanCheckResponse choice = (CanCheckResponse)_check_dice.roll();
    switch (choice) {
      case random_params_t::CHECK:
        return _create_check(request);
      case random_params_t::MIN_BET:
        return _create_bet(request, request.minLegalBetAmount());
      case random_params_t::HALF_POT_SIZED_BET:
        return _create_bet(request, request.legalizeBet(request.getPotSize()/2));
      case random_params_t::POT_SIZED_BET:
        return _create_bet(request, request.legalizeBet(request.getPotSize()));
      case random_params_t::ALL_IN_BET:
        return _create_bet(request, request.maxLegalBetAmount());
      default:
        return _create_check(request);  // shouldn't get here
    }
  } else {  // canCall
    CanCallResponse choice = (CanCallResponse)_call_dice.roll();
    switch (choice) {
      case random_params_t::FOLD:
        return _create_fold(request);
      case random_params_t::CALL:
        return _create_call(request);
      case random_params_t::MIN_RAISE:
        return _create_raise(request, request.minLegalRaiseAmount());
      case random_params_t::POT_SIZED_RAISE:
        return _create_raise(request, request.legalizeRaise(request.getPotentialPotSize()*2));
      case random_params_t::ALL_IN_RAISE:
        return _create_raise(request, request.maxLegalRaiseAmount());
      default:
        return _create_call(request);  // shouldn't get here
    }
  }
}

