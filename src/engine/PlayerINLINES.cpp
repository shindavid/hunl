player_id_t Player::__next_id = 0;

Player::Player(const BettingRules* betting_rules, const char* name)
  : _betting_rules(betting_rules)
  , _id(__next_id++)
  , _name(name) {}

BettingDecision Player::_createBettingDecision(const HandState& hand_state,
    chip_amount_t amount) const
{
  seat_t seat = hand_state.getActionOn();
  chip_amount_t contributed = hand_state.getAmountWageredCurrentRound(seat);
  chip_amount_t max_amount = hand_state.getMaxAmountWageredCurrentRound();
  chip_amount_t call_amount = max_amount - contributed;
 
  if (call_amount==0) {
    if (amount==0) return BettingDecision(0, ACTION_CHECK);
    chip_amount_t legal_amount = _betting_rules->legalize(hand_state, amount);

    return legal_amount ? BettingDecision(legal_amount, ACTION_BET) : BettingDecision(0, ACTION_CHECK);
  } else {
    if (amount==0) return BettingDecision(0, ACTION_FOLD);
    else if (amount==call_amount) return BettingDecision(amount, ACTION_CALL);
    else {
      chip_amount_t legal_amount = _betting_rules->legalize(hand_state, amount);

      return legal_amount ? BettingDecision(legal_amount, ACTION_RAISE) : 
        BettingDecision(call_amount, ACTION_CALL);
    }
  }
}

BlindPostDecision Player::handleBlindRequest(BlindType btype) {
  return BlindPostDecision(_betting_rules->getBlindAmount(btype), btype);
}

