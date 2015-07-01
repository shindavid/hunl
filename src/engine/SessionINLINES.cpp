// Included by Session.h

Session::Session(Player* p0, Player* p1, chip_amount_t starting_stack_size,
    chip_amount_t small_blind_size, chip_amount_t big_blind_size, uint64_t base_seed) :
  _params(__next_id, p0, p1, starting_stack_size, small_blind_size, big_blind_size),
  _base_seed(base_seed)
{
  __next_id++;
  _log.recordSessionStart(_params, base_seed);
}

template<>
void Session::handleEvent(HandState& state, seat_t seat, const HoleCardDealEvent* event)
{
  ps::CardSet set;
  set.insert(event->getCard(0));
  set.insert(event->getCard(1));
  state.setHolding(event->getSeat(), set);
  _log.record(&state, event);
}

template<>
void Session::handleEvent(HandState& state, const PublicDealEvent* event)
{
  Board& board = state.getBoard();
  for (int i=0; i<event->getNumCards(); ++i) {
    board.add(event->getCard(i));
  }
  _log.record(&state, event);
}

template<>
void Session::handleEvent(HandState& state, seat_t seat, const BlindPostDecision* event)
{
  assert(state.getActionOn()==seat);
  chip_amount_t amount = event->getAmount();
  state.addWagerCurrentRound(seat, amount);
  state.setActionOn(!seat);
  assert(state._validate_chip_amounts());
  
  _log.record(&state, event);
}

template<>
void Session::handleEvent(HandState& state, seat_t seat, const BettingDecision* event)
{
  chip_amount_t amount = event->getAmount();
  action_type_t action_type = event->getActionType();
  bool all_in = state.isAllIn(seat);
  bool fold = action_type==ACTION_FOLD;

  state.setFolded(seat);
  state.setCurrentBettingRoundDone(fold || all_in);
  state.incrementGlobalActionCount(action_type==ACTION_BET || action_type==ACTION_RAISE);
  state.setActionCount(seat);
  state.addWagerCurrentRound(seat, amount);
  state.setActionOn(!seat);
  assert(state._validate_chip_amounts());

  _log.record(&state, event);
}

void Session::_do_betting_round(HandState& hand_state) {
  while (!hand_state.isCurrentBettingRoundDone()) {
    seat_t seat = hand_state.getActionOn();
    BettingDecisionRequest request(&hand_state);
    BettingDecision decision = _params.getPlayer(seat)->handleRequest(&request);
    request.validate(decision);

    handleEvent(hand_state, seat, &decision);
    broadcastEvent(hand_state, !seat, &decision);
    _log.record(&hand_state, &decision);
  }
}

void Session::_init_hand() {
  _state.incrementHandID();
  _state.moveButton();
  
  uint64_t seed = _base_seed + _state.getCurrentHandID();
  srand(seed);
  _deck.shuffle();
}

void Session::_main_loop(HandState& hand_state) {
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

  for (seat_t seat=0; seat<2; ++seat) {
    HoleCardDealEvent hole_card_event(holdings[seat], seat);
    _params.getPlayer(seat)->handleEvent(hand_state, &hole_card_event);
    handleEvent(hand_state, seat, &hole_card_event);
  }

  seat_t button = hand_state.getButton();
  BlindPostRequest small_blind_request(&hand_state);
  BlindPostDecision small_blind_post = _params.getPlayer(button)->handleRequest(&small_blind_request);
  small_blind_request.validate(small_blind_post);
  handleEvent(hand_state, button, &small_blind_post);

  BlindPostRequest big_blind_request(&hand_state);
  BlindPostDecision big_blind_post = _params.getPlayer(!button)->handleRequest(&big_blind_request);
  big_blind_request.validate(big_blind_post);
  handleEvent(hand_state, !button, &big_blind_post);

  _do_betting_round(hand_state);
  if (hand_state.isDone()) return;
 
  PublicDealEvent flop_event(flop, 3);
  handleEvent(hand_state, &flop_event);
  broadcastEvent(hand_state, &flop_event);
  _do_betting_round(hand_state);
  if (hand_state.isDone()) return;

  PublicDealEvent turn_event(&turn, 1);
  handleEvent(hand_state, &turn_event);
  broadcastEvent(hand_state, &turn_event);
  _do_betting_round(hand_state);
  if (hand_state.isDone()) return;
  
  PublicDealEvent river_event(&river, 1);
  handleEvent(hand_state, &river_event);
  broadcastEvent(hand_state, &river_event);
  _do_betting_round(hand_state);
}

void Session::_award_pot(const HandState& hand_state, seat_t seat) {
  chip_amount_t net_gain = hand_state.getPotSize() - hand_state.getAmountWageredPriorRounds(seat);
  chip_amount_t net_loss = - hand_state.getAmountWageredPriorRounds(!seat);

  assert(net_gain + net_loss == 0);
  _state.updateScore(seat, net_gain);
  _state.updateScore(!seat, net_loss);

  PotWinEvent win_event(&hand_state, seat);
  broadcastEvent(hand_state, &win_event);
  _log.record(&hand_state, &win_event);
}

void Session::_split_pot(const HandState& hand_state) {
  // no change to scores
  
  PotSplitEvent split_event(&hand_state);
  broadcastEvent(hand_state, &split_event);
  _log.record(&hand_state, &split_event);
}

void Session::_finish_hand(HandState& hand_state) {
  hand_state.advanceBettingRound();
  assert(!(hand_state.hasFolded(0) && hand_state.hasFolded(1)));
  assert(hand_state.getPotSize() == 
      hand_state.getAmountWageredPriorRounds(0) + hand_state.getAmountWageredPriorRounds(1));

  if (hand_state.hasFolded(0)) {
    _award_pot(hand_state, 1);
  } else if (hand_state.hasFolded(1)) {
    _award_pot(hand_state, 0);
  } else {
    ps::PokerEvaluation evals[2];
    seat_t showdown_order[2] = {!hand_state.getButton(), hand_state.getButton()};
    for (int i=0; i<2; ++i) {
      seat_t seat = showdown_order[i];
      Holding holding = hand_state.getHolding(seat);
      ps::PokerEvaluation eval = _evaluator.evaluateHand(holding.getCardSet(), 
          hand_state.getBoard().getCards()).high();
      evals[seat] = eval;
      ShowdownEvent showdown_event(holding.getCard1(), holding.getCard2(), eval, seat);
      broadcastEvent(hand_state, !seat, &showdown_event);
      _log.record(&hand_state, &showdown_event);
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

  HandState hand_state(_params, _state);
  _log.recordHandStart(&hand_state);
  _main_loop(hand_state);
  _finish_hand(hand_state);
}

