// Included by SessionHand.h

SessionHand::SessionHand(SessionLog& log, chip_amount_t stack_size, seat_t button) : 
  _log(log)
  _stack_size(stack_size)
{
  for (seat_type_t seat=0; seat<2; ++seat) {
    _wagered_current_round[seat] = 0;
    _wagered_prior_rounds[seat] = 0;
    _folded[seat] = false;
  }
  _action_on = button;
  _button = button;
  _is_current_betting_round_done = false;
}

template<>
void SessionHand::handleEvent(seat_t seat, const HoleCardDealEvent& event)
{
  _log.record(event);
  for (int i=0; i<HoleCardDealEvent::NUM_CARDS; ++i) {
    _hole_cards[seat].insert(event.getCard(i));
  }
}

template<int n>
void SessionHand::handleEvent(const DealEvent<n>& event)
{
  _log.record(event);

  for (int i=0; i<n; ++i) {
    _board.insert(event.getCard(i));
  }

  for (seat_type_t seat=0; seat<2; ++seat) {
    _wagered_prior_rounds[seat] += _wagered_current_round[seat];
    _wagered_current_round[seat] = 0;
  }

  _action_on = !_button;
  _is_current_betting_round_done = _all_in(0) || _all_in(1);  // technically only need to test 1
}

template<>
void SessionHand::handleEvent(seat_t seat, const BlindPostEvent& event)
{
  _log.record(event);
  assert(_action_on==seat);
  chip_amount_t amount = event.getPostAmount();
  _wagered_current_round[seat] = amount;
  _action_on = !_action_on;
  assert(_validate_chip_amounts());
}

template<>
void SessionHand::handleEvent(seat_t seat, const FoldDecision& event)
{
  _log.record(event);
  assert(_action_on==seat);
  assert(_wagered_current_round[!seat] > _wagered_current_round[seat]);
  _folded[seat] = true;
  _is_current_betting_round_done = true;
  assert(_validate_chip_amounts());
}

template<>
void SessionHand::handleEvent(seat_t seat, const CheckDecision& event)
{
  _log.record(event);
  assert(_action_on==seat);
  assert(_wagered_current_round[seat] == 0);
  assert(_wagered_current_round[!seat] == 0);
  _action_on = !_action_on;
  _is_current_betting_round_done = (_action_on == _button);
  assert(_validate_chip_amounts());
}

template<>
void SessionHand::handleEvent(seat_t seat, const CallDecision& event)
{
  _log.record(event);
  chip_amount_t amount = event.getChipAmount();
  
  assert(_action_on==seat);
  assert(_wagered_current_round[!seat] == _wagered_current_round[seat] + amount);

  _amounts_wagered_current_round[seat] += amount;  // only calls use +=, peculiarity of poker terms
  _action_on = !_action_on;
  _is_current_betting_round_done = true;
  assert(_validate_chip_amounts());
}

template<>
void SessionHand::handleEvent(seat_t seat, const BetDecision& event)
{
  _log.record(event);
  assert(_action_on==seat);
  assert(_wagered_current_round[seat] == 0);
  assert(_wagered_current_round[!seat] == 0);

  chip_amount_t amount = event.getChipAmount();
  _amounts_wagered_current_round[seat] = amount;
  _action_on = !_action_on;
  assert(_validate_chip_amounts());
}

template<>
void SessionHand::handleEvent(seat_t seat, const RaiseDecision& event)
{
  _log.record(event);
  chip_amount_t amount = event.getChipAmount();
  chip_amount_t delta = amount - _wagered_current_round[!seat];
  chip_amount_t last_delta = _wagered_current_round[!seat] - _wagered_current_round[seat];
  
  assert(_action_on==seat);
  assert(_wagered_current_round[!seat] > 0);
  assert(last_delta > 0);
  assert(delta >= last_delta);

  _amounts_wagered_current_round[seat] = amount;
  _action_on = !_action_on;
  assert(_validate_chip_amounts());
}

