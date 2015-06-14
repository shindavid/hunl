// Included by Session.h

Session::Session(Player* p0, Player* p1, chip_amount_t stack_size, chip_amount_t small_blind_size,
    chip_amount_t big_blind_size, uint64_t seed)
{
  _id = __next_id++;
  _players[0] = p0;
  _players[1] = p1;

  _stack_size = stack_size;
  _small_blind_size = small_blind_size;
  _big_blind_size = big_blind_size;

  assert(_small_blind_size <= _big_blind_size);
  assert(_big_blind_size <= _stack_size);

  // srand() API is weird, passing in 1 gives you a weird error
  assert(seed>1);
  _base_seed = seed;
  srand(seed);
  _button = rand() % 2;
}

void Session::_do_betting_round(HandState& hand_state) {
  const PublicHandState& public_state = hand_state.getPublicState();
  while (!public_state.isCurrentBettingRoundDone()) {
    seat_t seat = public_state.getActionOn();
    BettingDecisionRequest request(public_state, seat);
    BettingDecision_Base decision = _players[seat]->handleBettingDecisionRequest(request);
    request.validate(decision);
    hand_state.handleEvent(seat, decision);
  }
}

void Session::_init_hand() {
  _current_hand_id++;
  _button = !_button;
  
  uint64_t seed = _base_seed + _current_hand_id;
  srand(seed);
  _deck.shuffle();
}

void _main_loop(HandState& hand_state) {
  // Pre-deal cards before any of the Player's start calling rand()
  ps::Card holdings[2][2];
  ps::Card flop[3];
  ps::Card turn;
  ps::Card river;

  for (int p=0; p<2; ++p) {
    for (int i=0; i<2; ++i ) {
      holdings[p][i] = _deck.deal();
    }
  }
  for (int i=0; i<3; ++i ) {
    flop[i] = _deck.deal();
  }
  turn = _deck.deal();
  river = _deck.deal();

  const PublicHandState& public_state = hand_state.getPublicState();
  for (int p=0; p<2; ++p) {
    HoleCardDealEvent hole_card_event(public_state, _players[p]->getID(), holdings[p]);
    _players[p].handleEvent(hole_card_event);
    hand_state.handleEvent(p, hole_card_event);
  }

  BlindPostRequest small_blind_request(public_state, _small_blind_size, _button);
  BlindPostEvent small_blind_post = _players[_button].handleBlindPostRequest(small_blind_request);
  small_blind_request.validate(small_blind_post);
  hand_state.handleEvent(_button, small_blind_post);

  BlindPostRequest big_blind_request(public_state, _big_blind_size, !_button);
  BlindPostEvent big_blind_post = _players[!_button].handleBlindPostRequest(big_blind_request);
  big_blind_request.validate(big_blind_post);
  hand_state.handleEvent(_button, big_blind_post);

  _do_betting_round(hand_state);
  if (public_state.isDone()) return;
  
  hand_state.handleEvent(FlopDealEvent(public_state, flop));
  _do_betting_round(hand_state);
  if (public_state.isDone()) return;
  
  hand_state.handleEvent(TurnDealEvent(public_state, turn));
  _do_betting_round(hand_state);
  if (public_state.isDone()) return;
  
  hand_state.handleEvent(RiverDealEvent(public_state, river));
  _do_betting_round(hand_state);
}

void Session::_finish_hand(const HandState& hand_state) {
  /*
   * TODO: determine winner of hand, update scores
   */
  throw std::exception("implement me");
}

void Session::playHand(SessionLog& log) {
  _init_hand();

  HandState hand_state(log, _current_hand_id, _stack_size, _button);
  _main_loop(hand_state);
  _finish_hand(hand_state);
}

