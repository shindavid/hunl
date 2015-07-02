player_id_t Player::__next_id = 0;

BettingDecision Player::_createBettingDecision(
    const BettingDecisionRequest* request, chip_amount_t amount) const
{
  const HandState& hand_state = *request->getHandState();

  seat_t seat = hand_state.getActionOn();
  chip_amount_t contributed = hand_state.getAmountWageredCurrentRound(seat);
  chip_amount_t max_amount = hand_state.getMaxAmountWageredCurrentRound();
  chip_amount_t call_amount = max_amount - contributed;
 
  if (call_amount==0) {
    if (amount==0) return BettingDecision(0, ACTION_CHECK);
    return BettingDecision(request->legalizeAdditionalAmount(amount), ACTION_BET);
  } else {
    if (amount==0) return BettingDecision(0, ACTION_FOLD);
    else if (amount==call_amount) return BettingDecision(amount, ACTION_CALL);
    else return BettingDecision(amount, ACTION_RAISE);
  }
}

BlindPostDecision Player::handleRequest(const BlindPostRequest* request) {
  return BlindPostDecision(request->getAmount(), request->getBlindType());
}

