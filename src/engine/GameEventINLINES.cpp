template<>
void BettingDecisionRequest::validate(const BetDecision* decision) const {
  assert(canBet());
  assert(decision->getChipAmount() >= minLegalBetAmount());
  assert(decision->getChipAmount() <= maxLegalBetAmount());
}

template<>
void BettingDecisionRequest::validate(const RaiseDecision* decision) const {
  assert(canRaise());
  assert(decision->getChipAmount() >= minLegalRaiseAmount());
  assert(decision->getChipAmount() <= maxLegalRaiseAmount());
}

template<>
void BettingDecisionRequest::validate(const CheckDecision* decision) const {
  assert(canCheck());
}

template<>
void BettingDecisionRequest::validate(const CallDecision* decision) const {
  assert(canCall());
  assert(decision->getChipAmount() == legalCallAmount());
}

template<>
void BettingDecisionRequest::validate(const FoldDecision* decision) const {
  assert(canFold());
}

template<>
void BettingDecisionRequest::validate(const BettingDecision* decision) const {
  if (const BetDecision* bet = dynamic_cast<const BetDecision*>(decision)) { validate(bet); return; }
  if (const CallDecision* call = dynamic_cast<const CallDecision*>(decision)) { validate(call); return; }
  if (const CheckDecision* check = dynamic_cast<const CheckDecision*>(decision)) { validate(check); return; }
  if (const FoldDecision* fold = dynamic_cast<const FoldDecision*>(decision)) { validate(fold); return; }
  if (const RaiseDecision* raise = dynamic_cast<const RaiseDecision*>(decision)) { validate(raise); return; }
}

bool BettingDecisionRequest::facingBet() const {
  seat_t action_on = _public_state.getActionOn();
  return _public_state.getAmountWageredCurrentRound(!action_on) >
    _public_state.getAmountWageredCurrentRound(action_on);
}

chip_amount_t BettingDecisionRequest::betAmountFaced() const {
  seat_t action_on = _public_state.getActionOn();
  return _public_state.getAmountWageredCurrentRound(!action_on) -
    _public_state.getAmountWageredCurrentRound(action_on);
}

chip_amount_t BettingDecisionRequest::minLegalRaiseAmount() const {
  chip_amount_t call_amount = betAmountFaced();
  chip_amount_t min_additional_amount = std::max(call_amount,
      _public_state.getSessionParams().getBigBlindSize());
  chip_amount_t min_raise_amount = call_amount + min_additional_amount;

  return std::min(min_raise_amount, maxLegalRaiseAmount());
}

chip_amount_t BettingDecisionRequest::maxLegalRaiseAmount() const {
  return _public_state.getRemainingChips(getSeat());
}

chip_amount_t BettingDecisionRequest::legalCallAmount() const {
  return std::min(betAmountFaced(), _public_state.getRemainingChips(getSeat()));
}

bool BettingDecisionRequest::canRaise() const {
  return maxLegalRaiseAmount() > legalCallAmount();
}

chip_amount_t BettingDecisionRequest::minLegalBetAmount() const {
  return std::min(_public_state.getRemainingChips(getSeat()),
      _public_state.getSessionParams().getBigBlindSize());
}

chip_amount_t BettingDecisionRequest::maxLegalBetAmount() const {
  return _public_state.getRemainingChips(getSeat());
}

chip_amount_t BettingDecisionRequest::legalizeBet(chip_amount_t amount) const {
  return std::max(std::min(amount, maxLegalBetAmount()), minLegalBetAmount());
}

chip_amount_t BettingDecisionRequest::legalizeRaise(chip_amount_t amount) const {
  return std::max(std::min(amount, maxLegalRaiseAmount()), minLegalRaiseAmount());
}

chip_amount_t PotWinEvent::getCalledPotSize() const {
  return 2 * _public_state.getAmountWageredPriorRounds(!getSeat());
}

chip_amount_t PotWinEvent::getUncalledBetSize() const {
  return _public_state.getAmountWageredPriorRounds(getSeat()) -
    _public_state.getAmountWageredPriorRounds(!getSeat());
}

chip_amount_t PotSplitEvent::getSplitAmount() const {
  return _public_state.getAmountWageredPriorRounds(0);
}
