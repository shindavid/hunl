
BettingDecision RandomPlayer::handleRequest(const BettingDecisionRequest* request) {
  //fprintf(stdout, "%s call=%d\n", getName(), request->legalCallAmount());
  if (request->legalCallAmount()==0) {
    random_params_t::CanCheckResponse choice = (random_params_t::CanCheckResponse)_check_dice.roll();
    switch (choice) {
      case random_params_t::CHECK: return _createBettingDecision(request, 0);
      case random_params_t::MIN_BET:
        return _createBettingDecision(request, request->minLegalAdditionalAmount());
      case random_params_t::HALF_POT_SIZED_BET:
        return _createBettingDecision(request, request->legalizeAdditionalAmount(request->getPotSize()/2));
      case random_params_t::POT_SIZED_BET:
        return _createBettingDecision(request, request->legalizeAdditionalAmount(request->getPotSize()));
      case random_params_t::ALL_IN_BET:
        return _createBettingDecision(request, request->maxLegalAdditionalAmount());
      default: return _createBettingDecision(request, 0);  // shouldn't get here
    }
  } else {  // canCall
    random_params_t::CanCallResponse choice = (random_params_t::CanCallResponse)_call_dice.roll();
    switch (choice) {
      case random_params_t::FOLD: return _createBettingDecision(request, 0);
      case random_params_t::CALL: return _createBettingDecision(request, request->legalCallAmount());
      case random_params_t::MIN_RAISE: 
        return _createBettingDecision(request, request->minLegalAdditionalAmount());
      case random_params_t::POT_SIZED_RAISE: 
        return _createBettingDecision(request, request->legalizeAdditionalAmount(
              request->getPotentialPotSize() + request->legalCallAmount()));
      case random_params_t::ALL_IN_RAISE:
        return _createBettingDecision(request, request->maxLegalAdditionalAmount());
      default: return _createBettingDecision(request, 0);  // shouldn't get here
    }
  }
}

