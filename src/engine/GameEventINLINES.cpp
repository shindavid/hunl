inline void BettingDecisionRequest::validate(BettingDecision decision) const {
  seat_t seat = _hand_state->getActionOn();
  chip_amount_t amount = decision.getAmount();
  chip_amount_t contributed = _hand_state->getAmountWageredCurrentRound(seat);
  chip_amount_t remaining = _hand_state->getRemainingChips(seat) - amount;
  chip_amount_t max_amount = _hand_state->getMaxAmountWageredCurrentRound();
  chip_amount_t call_amount = max_amount - contributed;
  chip_amount_t additional = amount - call_amount;
  
  bool all_in = remaining==0;
  bool legal_more = all_in || (additional>=
      std::max(call_amount, _hand_state->getSessionParams().getBigBlindSize()));
  
  switch (decision.getActionType()) {
    case ACTION_BET: assert(call_amount==0 && legal_more); return;
    case ACTION_RAISE: assert(call_amount>0 && legal_more); return;
    case ACTION_CHECK: assert(amount==0 && additional==0); return;
    case ACTION_CALL: assert(amount>0 && (max_amount==contributed+amount || all_in)); return;
    case ACTION_FOLD: assert(amount==0 && max_amount>contributed); return;
  }
}

inline chip_amount_t BettingDecisionRequest::legalCallAmount() const {
  seat_t seat = _hand_state->getActionOn();
  
  chip_amount_t amount = _hand_state->getMaxAmountWageredCurrentRound() -
    _hand_state->getAmountWageredCurrentRound(seat);
  assert(amount>=0);

  return std::min(amount, maxLegalAdditionalAmount());
}

inline chip_amount_t BettingDecisionRequest::minLegalAdditionalAmount() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");

  chip_amount_t call = legalCallAmount();
  chip_amount_t big_blind = _hand_state->getSessionParams().getBigBlindSize();
  chip_amount_t amount = call + std::max(call, big_blind);  // assumes heads-up

  return std::min(amount, maxLegalAdditionalAmount());
}

inline chip_amount_t BettingDecisionRequest::maxLegalAdditionalAmount() const {
  seat_t seat = _hand_state->getActionOn();
  chip_amount_t stack = _hand_state->getRemainingChips(seat);
  assert(stack>0);
  return stack;
}

inline chip_amount_t BettingDecisionRequest::legalizeAdditionalAmount(chip_amount_t amount) const {
  return std::max(std::min(amount, maxLegalAdditionalAmount()), minLegalAdditionalAmount());
}

inline void BlindPostRequest::validate(BlindPostDecision decision) const {
  assert(decision.getAmount() == getAmount());
  assert(decision.getBlindType() == getBlindType());
}
  
inline chip_amount_t BlindPostRequest::getAmount() const {
  const SessionParams& params = _hand_state->getSessionParams();
  return getBlindType()==SMALL_BLIND ? params.getSmallBlindSize() : params.getBigBlindSize();
}

inline BlindType BlindPostRequest::getBlindType() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  return _hand_state->getActionOn()==_hand_state->getButton() ? SMALL_BLIND : BIG_BLIND;
}

chip_amount_t PotWinEvent::getCalledPotSize() const {
  return 2 * _hand_state->getAmountWageredPriorRounds(!getSeat());
}

chip_amount_t PotWinEvent::getUncalledBetSize() const {
  return _hand_state->getAmountWageredPriorRounds(getSeat()) -
    _hand_state->getAmountWageredPriorRounds(!getSeat());
}

chip_amount_t PotSplitEvent::getSplitAmount() const {
  return _hand_state->getAmountWageredPriorRounds(0);
}

