
PublicDealEvent::PublicDealEvent(ps::Card* cards, int n) {
  assert(n>=0 && n<=4);
  for (int i=0; i<n; ++i) {
    _cards[i] = cards[i];
  }

  _n = n;
}

ps::Card PublicDealEvent::getCard(int i) const { return _cards[i]; }
int PublicDealEvent::getNumCards() const { return _n; }

template<int n>
PrivateDealEvent<n>::PrivateDealEvent(ps::Card cards[n], seat_t seat) : _seat(seat) {
  for (int i=0; i<n; ++i) {
    _cards[i] = cards[i];
  }
}

template<int n> seat_t PrivateDealEvent<n>::getSeat() const { return _seat; }
template<int n> ps::Card PrivateDealEvent<n>::getCard(int i) const { return _cards[i]; }

BettingDecision::BettingDecision(chip_amount_t amount, ActionType action_type) : 
  _amount(amount), _action_type(action_type) {}

chip_amount_t BettingDecision::getAmount() const { return _amount; }
ActionType BettingDecision::getActionType() const { return _action_type; }

BlindPostDecision::BlindPostDecision(chip_amount_t amount, BlindType btype)
  : _amount(amount), _btype(btype) {}

chip_amount_t BlindPostDecision::getAmount() const { return _amount; }
BlindType BlindPostDecision::getBlindType() const { return _btype; }

ShowdownEvent::ShowdownEvent(Holding holding, ps::PokerEvaluation eval, seat_t seat)
  : _holding(holding)
  , _eval(eval)
  , _seat(seat) {}

Holding ShowdownEvent::getHolding() const { return _holding; }
ps::PokerEvaluation ShowdownEvent::getEval() const { return _eval; }
seat_t ShowdownEvent::getSeat() const { return _seat; }

PotWinEvent::PotWinEvent(const HandState* hand_state, seat_t seat)
  : _hand_state(hand_state), _seat(seat)
{
  assert(getCalledPotSize() + getUncalledBetSize() == hand_state->getPotSize());
}

seat_t PotWinEvent::getSeat() const { return _seat; }

chip_amount_t PotWinEvent::getCalledPotSize() const {
  return 2 * _hand_state->getAmountWageredPriorRounds(!getSeat());
}

chip_amount_t PotWinEvent::getUncalledBetSize() const {
  return _hand_state->getAmountWageredPriorRounds(getSeat()) -
    _hand_state->getAmountWageredPriorRounds(!getSeat());
}

PotSplitEvent::PotSplitEvent(const HandState* hand_state) : _hand_state(hand_state) {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  assert(2*getSplitAmount() == _hand_state->getPotSize());
}

chip_amount_t PotSplitEvent::getSplitAmount() const {
  return _hand_state->getAmountWageredPriorRounds(0);
}

