// Included by HandState.h

HandState::HandState(SessionLog& log, const SessionParams& session_params,
    const SessionState& session_state) :
  _log(log),
  _public_hand_state(session_params, session_state)
{}

template<>
void HandState::handleEvent(seat_t seat, const HoleCardDealEvent& event)
{
  for (int i=0; i<HoleCardDealEvent::NUM_CARDS; ++i) {
    _hole_cards[seat].insert(event.getCard(i));
    _hole_cards_ordered[seat][i] = event.getCard(i);
  }
  _log.record(event);
}

template<int n>
void HandState::handleEvent(const DealEvent<n>& event)
{
  for (int i=0; i<n; ++i) {
    _public_hand_state.add(event.getCard(i));
  }

  _public_hand_state.advanceBettingRound();
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const BlindPostEvent& event)
{
  seat_t action_on = _public_hand_state.getActionOn();
  assert(action_on==seat);
  chip_amount_t amount = event.getPostAmount();
  _public_hand_state.addWagerCurrentRound(seat, amount);
  _public_hand_state.setActionOn(!action_on);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const FoldDecision& event)
{
  seat_t action_on = _public_hand_state.getActionOn();
  assert(action_on==seat);
  assert(_public_hand_state.getAmountWageredCurrentRound(!seat) >
      _public_hand_state.getAmountWageredCurrentRound(seat));
 
  _public_hand_state.setFolded(seat);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const CheckDecision& event)
{
  seat_t action_on = _public_hand_state.getActionOn();
  assert(action_on==seat);
  assert(_public_hand_state.getAmountWageredCurrentRound(seat) == 0);
  assert(_public_hand_state.getAmountWageredCurrentRound(!seat) == 0);
  _public_hand_state.setActionOn(!action_on);
  _public_hand_state.setCurrentBettingRoundDone(_action_on == _button);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const CallDecision& event)
{
  chip_amount_t amount = event.getChipAmount();
  
  seat_t action_on = _public_hand_state.getActionOn();
  assert(action_on==seat);
  assert(_public_hand_state.getAmountWageredCurrentRound(!seat) ==
      _public_hand_state.getAmountWageredCurrentRound(seat) + amount);

  _public_hand_state.addWagerCurrentRound(seat, amount);
  _public_hand_state.setActionOn(!action_on);
  _public_hand_state.setCurrentBettingRoundDone(true);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const BetDecision& event)
{
  seat_t action_on = _public_hand_state.getActionOn();
  
  assert(action_on==seat);
  assert(_public_hand_state.getAmountWageredCurrentRound(seat) == 0);
  assert(_public_hand_state.getAmountWageredCurrentRound(!seat) == 0);

  chip_amount_t amount = event.getChipAmount();
  _public_hand_state.addWagerCurrentRound(seat, amount);
  _public_hand_state.setActionOn(!action_on);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

template<>
void HandState::handleEvent(seat_t seat, const RaiseDecision& event)
{
  seat_t action_on = _public_hand_state.getActionOn();
  chip_amount_t amount = event.getChipAmount();
  chip_amount_t opp_wager_amount_current_round =
    _public_hand_state.getAmountWageredCurrentRound(!seat);
  chip_amount_t delta = amount - opp_wager_amount_current_round;
  chip_amount_t last_delta = opp_wager_amount_current_round -
    _public_hand_state.getAmountWageredCurrentRound(seat);
  
  assert(action_on==seat);
  assert(opp_wager_amount_current_round > 0);
  assert(last_delta > 0);
  assert(delta >= last_delta);

  _public_hand_state.addWagerCurrentRound(seat, delta);
  _public_hand_state.setActionOn(!action_on);
  assert(_public_hand_state.validate_chip_amounts());
  _log.record(event);
}

