
template<>
void BettingDecisionRequest::validate<BetDecision>(const BetDecision& decision) const {
  assert(canBet());
  assert(decision.getChipAmount() >= minLegalBetAmount());
  assert(decision.getChipAmount() <= maxLegalBetAmount());
}

template<>
void BettingDecisionRequest::validate<RaiseDecision>(const RaiseDecision& decision) const {
  assert(canRaise());
  assert(decision.getChipAmount() >= minLegalRaiseAmount());
  assert(decision.getChipAmount() <= maxLegalRaiseAmount());
}

template<>
void BettingDecisionRequest::validate<CheckDecision>(const CheckDecision& decision) const {
  assert(canCheck());
}

template<>
void BettingDecisionRequest::validate<CallDecision>(const CallDecision& decision) const {
  assert(canCall());
  assert(decision.getChipAmount() == legalCallAmount());
}

template<>
void BettingDecisionRequest::validate<FoldDecision>(const FoldDecision& decision) const {
  assert(canFold());
}

bool BettingDecisionRequest::facingBet() const {
  seat_type_t action_on = _public_state.getActionOn();
  return _public_state.getAmountWageredCurrentRound(!action_on) >
    _public_state.getAmountWageredCurrentRound(action_on);
}

chip_amount_t BettingDecisionRequest::betAmountFaced() const {
  seat_type_t action_on = _public_state.getActionOn();
  return _public_state.getAmountWageredCurrentRound(!action_on) -
    _public_state.getAmountWageredCurrentRound(action_on);
}

chip_amount_t BettingDecisionRequest::minLegalRaiseAmount() const {
  chip_amount_t call_amount = betAmountFaced();
  chip_amount_t min_additional_amount = std::max(call_amount, _public_state.getBigBlindAmount());
  chip_amount_t min_raise_amount = call_amount + min_additional_amount;

  return std::min(min_raise_amount, maxLegalRaiseAmount());
}

chip_amount_t BettingDecisionRequest::maxLegalRaiseAmount() const {
  return _public_state.getRemainingChips();
}

chip_amount_t BettingDecisionRequest::legalCallAmount() const {
  return std::min(betAmountFaced(), _public_state.getRemainingChips());
}

bool BettingDecisionRequest::canRaise() const {
  return maxLegalRaiseAmount() > legalCallAmount();
}

chip_amount_t BettingDecisionRequest::minLegalBetAmount() const {
  return std::min(_public_state.getRemainingChips(), _public_state.getBigBlindAmount());
}

chip_amount_t BettingDecisionRequest::maxLegalBetAmount() const {
  return _public_state.getRemainingChips();
}

