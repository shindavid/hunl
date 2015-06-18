
virtual BettingDecision RandomPlayer::handleRequest(const BettingDecisionRequest& request) {
  if (request.canCheck()) {
    CanCheckResponse choice = (CanCheckResponse)_check_dice.roll();
    switch (choice) {
      case CHECK:
        return _create_check(request);
      case MIN_BET:
        return _create_bet(request, request.minLegalBetAmount());
      case HALF_POT_SIZED_BET:
        return _create_bet(request, request.legalizeBet(request.getPotSize()/2));
      case POT_SIZED_BET:
        return _create_bet(request, request.legalizeBet(request.getPotSize()));
      case ALL_IN_BET:
        return _create_bet(request, request.maxLegalBetAmount());
      default:
        return _create_check(request);  // shouldn't get here
    }
  } else {  // canCall
    CanCallResponse choice = (CanCallResponse)_call_dice.roll();
    switch (choice) {
      case FOLD:
        return _create_fold(request);
      case CALL:
        return _create_call(request);
      case MIN_RAISE:
        return _create_raise(request, request.minLegalRaiseAmount());
      case POT_SIZED_RAISE:
        return _create_raise(request, request.legalizeRaise(request.getPotentialPotSize()*2));
      case ALL_IN_RAISE:
        return _create_raise(request, request.maxLegalRaiseAmount());
      default:
        return _create_call(request);  // shouldn't get here
    }
  }
}

