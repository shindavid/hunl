// Included by HandState.h

HandState::HandState(chip_amount_t starting_stack_size, seat_t button)
  : _starting_stack_size(starting_stack_size)
  , _button(button)
{
  for (int i=0; i<NUM_PLAYERS; ++i) {
    _stack_size[i] = _starting_stack_size;
    _wagered_prior_rounds[i] = 0;
    _wagered_current_round[i] = 0;
    _action_count[i] = 0;
    _folded[i] = false;
  }
}

bool HandState::isAllIn(seat_t seat) const { return getRemainingChips(seat)==0; }
const Board& HandState::getBoard() const { return _board; }
Board& HandState::getBoard() { return _board; }
bool HandState::isPreflop() const { return _board.getSize() == 0; }

chip_amount_t HandState::getAmountWageredPriorRounds(seat_t seat) const {
  return _wagered_prior_rounds[seat];
}

chip_amount_t HandState::getAmountWageredCurrentRound(seat_t seat) const {
  return _wagered_current_round[seat];
}

chip_amount_t HandState::getMaxAmountWageredCurrentRound() const {
  chip_amount_t max = 0;
  for (seat_t seat=0; seat<2; ++seat) {
    max = std::max(max, _wagered_current_round[seat]);
  }
  return max;
}

void HandState::addWagerCurrentRound(seat_t seat, chip_amount_t amount) {
  _wagered_current_round[seat] += amount;
  _stack_size[seat] -= amount;
  assert(_stack_size[seat]>=0);
}

void HandState::payBlinds(seat_t button, chip_amount_t small_blind, chip_amount_t big_blind) {
  addWagerCurrentRound(button, small_blind);
  addWagerCurrentRound(!button, big_blind);
}

void HandState::setHolding(seat_t seat, ps::Card c1, ps::Card c2) {
  ps::CardSet set;
  set.insert(c1);
  set.insert(c2);
  new (&_holdings[seat]) Holding(set);
}

chip_amount_t HandState::getPotSize() const {
  chip_amount_t total = 0;
  for (int i=0; i<NUM_PLAYERS; ++i) {
    total += _wagered_prior_rounds[i];
    total += _wagered_current_round[i];
  }
  return total;
}

chip_amount_t HandState::getPotentialPotSize() const {
  chip_amount_t total = 0;
  for (int i=0; i<NUM_PLAYERS; ++i) {
    total += _wagered_prior_rounds[i];
  }
  total += getMaxAmountWageredCurrentRound() * NUM_PLAYERS;
  return total;
}

bool HandState::_validate_chip_amounts() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  return
    _wagered_prior_rounds[0] + _wagered_current_round[0] <= _starting_stack_size &&
    _wagered_prior_rounds[1] + _wagered_current_round[1] <= _starting_stack_size;
}

bool HandState::isCurrentBettingRoundDone() const {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  if (hasFolded(0) || hasFolded(1)) return true;

  for (seat_t seat=0; seat<2; ++seat) {
    if (!isAllIn(seat)) {
      if (_action_count[seat] < _global_action_count) return false;
    }
  }
  return true;
}

void HandState::advanceBettingRound() {
  static_assert(NUM_PLAYERS == 2, "multi-player games not supported");
  for (seat_t seat=0; seat<2; ++seat) {
    _wagered_prior_rounds[seat] += _wagered_current_round[seat];
    _wagered_current_round[seat] = 0;
    _action_count[seat] = 0;
  }
      
  _global_action_count += 1;
  _action_on = _board.getSize()==0 ? _button : !_button;
  _is_current_betting_round_done = isAllIn(0) || isAllIn(1);  // technically only need to test 1
}

