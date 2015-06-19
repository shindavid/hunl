void BettingDecisionRequest::validate(const BettingDecision& decision) {
  switch (decision.getActionType()) {
    case ACTION_BET: _validate(*((const BetDecision*)&decision));
    case ACTION_RAISE: _validate(*((const RaiseDecision*)&decision));
    case ACTION_CHECK: _validate(*((const CheckDecision*)&decision));
    case ACTION_CALL: _validate(*((const CallDecision*)&decision));
    case ACTION_FOLD: _validate(*((const FoldDecision*)&decision));
  }
}

void BettingDecisionRequest::_validate(const BetDecision& decision) const {
  assert(canBet());
  assert(decision.getChipAmount() >= minLegalBetAmount());
  assert(decision.getChipAmount() <= maxLegalBetAmount());
}

void BettingDecisionRequest::_validate(const RaiseDecision& decision) const {
  assert(canRaise());
  assert(decision.getChipAmount() >= minLegalRaiseAmount());
  assert(decision.getChipAmount() <= maxLegalRaiseAmount());
}

void BettingDecisionRequest::_validate(const CheckDecision& decision) const {
  assert(canCheck());
}

void BettingDecisionRequest::_validate(const CallDecision& decision) const {
  assert(canCall());
  assert(decision.getChipAmount() == legalCallAmount());
}

void BettingDecisionRequest::_validate(const FoldDecision& decision) const {
  assert(canFold());
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

