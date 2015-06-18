player_id_t Player::__next_id = 0;

BlindPostEvent Player::handleRequest(const BlindPostRequest& request) {
  BlindPostEvent event(request.getPublicHandState(), _id, request.getAmount(), request.getBlindType());
  return event;
}

CheckDecision Player::_create_check(const BettingDecisionRequest& request) {
  return CheckDecision(request.getPublicHandState(), getID());
}

CallDecision Player::_create_call(const BettingDecisionRequest& request) {
  return CallDecision(request.getPublicHandState(), getID(), request.legalCallAmount());
}

FoldDecision Player::_create_fold(const BettingDecisionRequest& request) {
  return FoldDecision(request.getPublicHandState(), getID());
}

BetDecision Player::_create_bet(const BettingDecisionRequest& request, chip_amount_t amount) {
  return BetDecision(request.getPublicHandState(), getID(), amount);
}

RaiseDecision Player::_create_raise(const BettingDecisionRequest& request, chip_amount_t amount) {
  return RaiseDecision(request.getPublicHandState(), getID(), amount);
}

