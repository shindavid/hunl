// Included by Session.h

SessionHand::SessionHand(chip_amount_t stack_size, bool button) {
  _pot_size = 0;
  _stack_sizes[0] = _stack_sizes[1] = stack_size;
  _action_on = button;
  _button = button;
}

Session::Session(Player* p0, Player* p1, chip_amount_t stack_size, uint64_t seed) {
  _id = __next_id++;
  _players[0] = p0;
  _players[1] = p1;

  _stack_size = stack_size;

  // srand() API is weird, passing in 1 gives you a weird error
  assert(seed>1);
  _base_seed = seed;
  srand(seed);
  _button = rand() % 2;
}

void Session::playHand(HandResult& result) {
  _current_hand_id++;
  _button = !_button;
  
  uint64_t seed = _base_seed + _current_hand_id + 1;
  srand(seed);
  _deck.shuffle();

  SessionHand hand(_stack_size, _button);

  // Pre-deal cards before any of the Player's start calling rand()
  ps::CardSet holding0 = _deck.deal(2);
  ps::CardSet holding1 = _deck.deal(2);

  ps::CardSet flop = _deck.deal(3);
  ps::CardSet turn = _deck.deal(1);
  ps::CardSet river = _deck.deal(1);

  HoleCardDealEvent hole_card_event0(_current_hand_id, _id, _players[0]->getID(), holding0);
  _players[0].handleEvent(hole_card_event0);
  hand.handleEvent(0, hole_card_event0);

  HoleCardDealEvent hole_card_event1(_current_hand_id, _id, _players[1]->getID(), holding1);
  _players[1].handleEvent(hole_card_event1);
  hand.handleEvent(1, hole_card_event2);

  hand.handleEvent(_button, _players[_button].requestSmallBlindPost());
  hand.handleEvent(!_button, _players[!_button].requestBigBlindPost());

  while (hand.isActive()) {
    bool action_on = hand.getActionOn();
    PlayerEvent event = _players[action_on]->handleActionRequest();
    if (hand.handleEvent(action_on, event)) break;
  }

  if (hand.isDone()) {
    result.log(hand);
    return;
  }
  
  hand.handleEvent(flop_event);
  while (hand.isActive()) {
    bool action_on = hand.getActionOn();
    PlayerEvent event = _players[action_on]->handleActionRequest();
    if (hand.handleEvent(action_on, event)) break;
  }
  if (hand.isDone()) {
    result.log(hand);
    return;
  }
  
  hand.handleEvent(turn_event);
  while (hand.isActive()) {
    bool action_on = hand.getActionOn();
    PlayerEvent event = _players[action_on]->handleActionRequest();
    if (hand.handleEvent(action_on, event)) break;
  }
  if (hand.isDone()) {
    result.log(hand);
    return;
  }
  
  hand.handleEvent(river_event);
  while (hand.isActive()) {
    bool action_on = hand.getActionOn();
    PlayerEvent event = _players[action_on]->handleActionRequest();
    if (hand.handleEvent(action_on, event)) break;
  }
  if (hand.isDone()) {
    result.log(hand);
    return;
  }
}

