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

void Session::_doBettingRound(SessionHand& hand) {
  while (!hand.isCurrentBettingRoundDone()) {
    seat_t seat = hand.getActionOn();
    BettingDecision decision = _players[seat]->handleBettingDecisionRequest();
    hand.handleEvent(seat, decision);
  }
}

void Session::_init_hand() {
  _current_hand_id++;
  _button = !_button;
  
  uint64_t seed = _base_seed + _current_hand_id;
  srand(seed);
  _deck.shuffle();
}

void _main_loop(SessionHand& hand) {
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

  for (int p=0; p<2; ++p) {
    HoleCardDealEvent hole_card_event(_current_hand_id, _id, _players[p]->getID(), holdings[p]);
    _players[p].handleEvent(hole_card_event);
    hand.handleEvent(p, hole_card_event);
  }

  hand.handleEvent(_button, _players[_button].requestSmallBlindPost(_small_blind_size));
  hand.handleEvent(!_button, _players[!_button].requestBigBlindPost(_big_blind_size));

  _doBettingRound(hand);
  if (hand.isDone()) return;
  
  hand.handleEvent(FlopDealEvent(_current_hand_id, _id, flop));
  _doBettingRound(hand);
  if (hand.isDone()) return;
  
  hand.handleEvent(TurnDealEvent(_current_hand_id, _id, turn));
  _doBettingRound(hand);
  if (hand.isDone()) return;
  
  hand.handleEvent(RiverDealEvent(_current_hand_id, _id, river));
  _doBettingRound(hand);
}

void Session::_finish_hand(const SessionHand& hand) {
  /*
   * TODO: determine winner of hand, update scores
   */
  throw std::exception("implement me");
}

void Session::playHand(SessionLog& log) {
  _init_hand();

  SessionHand hand(log, _stack_size, _button);
  _main_loop(hand);
  _finish_hand(hand);
}

