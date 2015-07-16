
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

