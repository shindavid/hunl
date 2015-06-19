// Included by Session.h

Session::Session(Player* p0, Player* p1, chip_amount_t starting_stack_size,
    chip_amount_t small_blind_size, chip_amount_t big_blind_size, uint64_t base_seed) :
  _params(__next_id++, p0, p1, starting_stack_size, small_blind_size, big_blind_size)
  _base_seed(base_seed)
{
  _log.recordSessionStart(_params, base_seed);
}

void Session::_do_betting_round(HandState& hand_state) {
  const PublicHandState& public_state = hand_state.getPublicState();
  while (!public_state.isCurrentBettingRoundDone()) {
    seat_t seat = public_state.getActionOn();
    BettingDecisionRequest request(public_state, seat);
    BettingDecision decision = _players[seat]->handleRequest(request);
    request.validate(decision);
    hand_state.handleEvent(seat, decision);
    hand_state.broadcastEvent(!seat, decision);
  }
}

void Session::_init_hand() {
  _state.incrementHandID();
  _state.moveButton();
  
  uint64_t seed = _base_seed + _state.getCurrentHandID();
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
  for (seat_t seat=0; seat<2; ++seat) {
    HoleCardDealEvent hole_card_event(public_state, seat, holdings[p]);
    _players[p].handleEvent(hole_card_event);
    hand_state.handleEvent(p, hole_card_event);
  }

  BlindPostRequest small_blind_request(public_state, _small_blind_size, _button, SMALL_BLIND);
  BlindPostEvent small_blind_post = _players[_button].handleRequest(small_blind_request);
  small_blind_request.validate(small_blind_post);
  hand_state.handleEvent(_button, small_blind_post);

  BlindPostRequest big_blind_request(public_state, _big_blind_size, !_button, BIG_BLIND);
  BlindPostEvent big_blind_post = _players[!_button].handleRequest(big_blind_request);
  big_blind_request.validate(big_blind_post);
  hand_state.handleEvent(_button, big_blind_post);

  _do_betting_round(hand_state);
  if (public_state.isDone()) return;
 
  FlopDealEvent flop_event(public_state, flop);
  hand_state.handleEvent(flop_event);
  hand_state.broadcastEvent(flop_event);
  _do_betting_round(hand_state);
  if (public_state.isDone()) return;

  TurnDealEvent turn_event(public_state, turn);
  hand_state.handleEvent(turn_event);
  hand_state.broadcastEvent(turn_event);
  _do_betting_round(hand_state);
  if (public_state.isDone()) return;
  
  RiverDealEvent river_event(public_state, river);
  hand_state.handleEvent(river_event);
  hand_state.broadcastEvent(river_event);
  _do_betting_round(hand_state);
}

void Session::_award_pot(const HandState& hand_state, seat_t seat) {
  const PublicHandState& public_state = hand_state.getPublicState();
  
  chip_amount_t net_gain = public_state.getPotSize() - public_state.getAmountWageredPriorRounds(seat);
  chip_amount_t net_loss = - public_state.getAmountWageredPriorRounds(!seat);

  assert(net_gain + net_loss == 0);
  _state.updateScore(seat, net_gain);
  _state.updateScore(!seat, net_loss);

  PotWinEvent win_event(public_state, seat);
  hand_state.broadcastEvent(win_event);
  _log.record(win_event);
}

void Session::_split_pot(const HandState& hand_state) {
  const PublicHandState& public_state = hand_state.getPublicState();
  
  // no change to scores
  
  PotSplitEvent split_event(public_state);
  hand_state.broadcastEvent(split_event);
  _log.record(split_event);
}

void Session::_finish_hand(HandState& hand_state) {
  PublicHandState& phs = hand_state.getPublicState();
  phs.advanceBettingRound();
  assert(!phs.hasFolded(0) && !phs.hasFolded(1));
  assert(phs.getPotSize() == 
      phs.getAmountWageredPriorRounds(0) + phs.getAmountWageredPriorRounds(1));
  assert(!phs.hasFolded(0) && !phs.hasFolded(1));

  if (phs.hasFolded(0)) {
    _award_pot(hand_state, 1);
  } else if (phs.hasFolded(1)) {
    _award_pot(hand_state, 0);
  } else {
    ps::PokerEvaluation evals[2];
    seat_t showdown_order[2] = {!phs.getButton(), phs.getButton()};
    for (int i=0; i<2; ++it) {
      seat_t seat = showdown_order[i];
      ps::CardSet holding = hand_state.getHoleCards(seat);
      ps::PokerEvaluation eval = _evaluator.evaluateHand(holding, phs.getBoard());
      evals[seat] = eval;
      ShowdownEvent showdown_event(phs, hand_state.getHoleCard(seat,0), hand_state.getHoleCard(seat,1),
          eval, seat);
      hand_state.broadcastEvent(!seat, showdown_event);
      _log.record(showdown_event);
    }
    if (evals[0] > evals[1]) {
      _award_pot(hand_state, 0);
    } else if (evals[0] < evals[1]) {
      _award_pot(hand_state, 1);
    } else {
      _split_pot(hand_state);
    }
  }
}

void Session::playHand() {
  _init_hand();

  HandState hand_state(_log, _params, _state);
  _log.recordHandStart(_state);
  _main_loop(hand_state);
  _finish_hand(hand_state);
}

