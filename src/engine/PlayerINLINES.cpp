player_id_t Player::__next_id = 0;

BlindPostEvent_ptr Player::handleRequest(const BlindPostRequest* request) {
  return BlindPostEvent_ptr(new BlindPostEvent(request->getPublicHandState(), _id,
        request->getAmount(), request->getBlindType()));
}

CheckDecision_ptr Player::_create_check(const BettingDecisionRequest* request) {
  return CheckDecision_ptr(new CheckDecision(request->getPublicHandState(), getID()));
}

CallDecision_ptr Player::_create_call(const BettingDecisionRequest* request) {
  return CallDecision_ptr(new CallDecision(request->getPublicHandState(), getID(),
        request->legalCallAmount()));
}

FoldDecision_ptr Player::_create_fold(const BettingDecisionRequest* request) {
  return FoldDecision_ptr(new FoldDecision(request->getPublicHandState(), getID()));
}

BetDecision_ptr Player::_create_bet(const BettingDecisionRequest* request, chip_amount_t amount) {
  return BetDecision_ptr(new BetDecision(request->getPublicHandState(), getID(), amount));
}

RaiseDecision_ptr Player::_create_raise(const BettingDecisionRequest* request, chip_amount_t amount) {
  return RaiseDecision_ptr(new RaiseDecision(request->getPublicHandState(), getID(), amount));
}

