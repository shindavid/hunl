
BettingDecision RandomPlayer::handleRequest(const BettingDecisionRequest* request) {
  if (request->legalCallAmount()==0) {
    random_params_t::CanCheckResponse choice = (random_params_t::CanCheckResponse)_check_dice.roll();
    switch (choice) {
      case random_params_t::CHECK: return BettingDecision(0, ACTION_CHECK);
      case random_params_t::MIN_BET:
        return BettingDecision(request->minLegalAdditionalAmount(), ACTION_BET);
      case random_params_t::HALF_POT_SIZED_BET:
        return BettingDecision(request->legalizeAdditionalAmount(request->getPotSize()/2), ACTION_BET);
      case random_params_t::POT_SIZED_BET:
        return BettingDecision(request->legalizeAdditionalAmount(request->getPotSize()), ACTION_BET);
      case random_params_t::ALL_IN_BET:
        return BettingDecision(request->maxLegalAdditionalAmount(), ACTION_BET);
      default: return BettingDecision(0, ACTION_CHECK);  // shouldn't get here
    }
  } else {  // canCall
    random_params_t::CanCallResponse choice = (random_params_t::CanCallResponse)_call_dice.roll();
    switch (choice) {
      case random_params_t::FOLD: return BettingDecision(0, ACTION_FOLD);
      case random_params_t::CALL: return BettingDecision(request->legalCallAmount(), ACTION_CALL);
      case random_params_t::MIN_RAISE: 
        return BettingDecision(request->minLegalAdditionalAmount(), ACTION_RAISE);
      case random_params_t::POT_SIZED_RAISE: 
        return BettingDecision(request->legalizeAdditionalAmount(
              request->getPotentialPotSize() + request->legalCallAmount()), ACTION_RAISE);
      case random_params_t::ALL_IN_RAISE:
        return BettingDecision(request->maxLegalAdditionalAmount(), ACTION_RAISE);
      default: return BettingDecision(0, ACTION_FOLD);  // shouldn't get here
    }
  }
}

