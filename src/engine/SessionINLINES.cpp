// Included by Session.h
#include "SessionLog.h"

const BettingRules& Session::getBettingRules() const { return *_betting_rules; }
uint64_t Session::getBaseSeed() const { return _base_seed; }
const Player* Session::getPlayer(int i) const { return _players[i]; }
session_id_t Session::getCurrentHandID() const { return _current_hand_id; }
seat_t Session::getButton() const { return _button; }
chip_amount_t Session::getScore(seat_t seat) const { return _score[seat]; }

Session::Session(Player* p0, Player* p1, const BettingRules* betting_rules, uint64_t base_seed)
  : _players{p0, p1}
  , _score{0,0}
  , _betting_rules(betting_rules)
  , _base_seed(base_seed)
{}

template<typename E>
void Session::broadcastEvent(const HandState& state, seat_t seat, const E& event) const {
  _players[seat]->handleEvent(state, event);
}

template<typename E>
void Session::broadcastEvent(const HandState& state, const E& event) const {
  for (seat_t seat=0; seat<NUM_PLAYERS; ++seat) {
    _players[seat]->handleEvent(state, event);
  }
}

template<>
void Session::handleEvent(SessionLog& log, HandState& state, const PublicDealEvent& event)
{
  state.setActionOn(!_button);
  Board& board = state.getBoard();
  for (int i=0; i<event.getNumCards(); ++i) {
    board.add(event.getCard(i));
  }
  log.record(*this, state, event);
}

template<>
void Session::handleEvent(SessionLog& log, HandState& state, seat_t seat,
    const BlindPostDecision& event)
{
  assert(state.getActionOn()==seat);
  chip_amount_t amount = event.getAmount();
  state.addWagerCurrentRound(seat, amount);
  assert(state._validate_chip_amounts());
  
  log.record(*this, state, event);
}

template<>
void Session::handleEvent(SessionLog& log, HandState& state, seat_t seat, const BettingDecision& event)
{
  chip_amount_t amount = event.getAmount();
  ActionType action_type = event.getActionType();
  bool all_in = state.isAllIn(seat);
  bool fold = action_type==ACTION_FOLD;

  state.setFolded(seat, fold);
  state.setCurrentBettingRoundDone(fold || all_in);
  state.incrementGlobalActionCount(action_type==ACTION_BET || action_type==ACTION_RAISE);
  state.setActionCount(seat);
  state.addWagerCurrentRound(seat, amount);
  state.setActionOn(!seat);
  assert(state._validate_chip_amounts());

  log.record(*this, state, event);
}

void Session::_validate_decision(const HandState& hand_state, const BettingDecision& decision) {
  chip_amount_t amount = decision.getAmount();
  chip_amount_t legal_call = _betting_rules->getLegalCheckOrCall(hand_state);
  chip_amount_t min_legal_raise = _betting_rules->getMinLegalBetOrRaise(hand_state);
  chip_amount_t max_legal_raise = _betting_rules->getMaxLegalBetOrRaise(hand_state);
  bool legal_more = amount >= min_legal_raise && amount <= max_legal_raise;

  switch (decision.getActionType()) {
    case ACTION_BET: assert(legal_call==0 && amount>0 && legal_more); return;
    case ACTION_RAISE: assert(legal_call>0 && amount>legal_call && legal_more); return;
    case ACTION_CHECK: assert(amount==0 && legal_call==0); return;
    case ACTION_CALL: assert(amount>0 && amount==legal_call); return;
    case ACTION_FOLD: assert(amount==0 && legal_call>0); return;
  }
}

void Session::_set_holding(seat_t seat, ps::Card c1, ps::Card c2) {
  ps::CardSet set;
  set.insert(c1);
  set.insert(c2);
  new (&_holdings[seat]) Holding(set);
}

Holding Session::_get_holding(seat_t seat) const {
  return _holdings[seat];
}

void Session::_do_betting_round(SessionLog& log, HandState& hand_state) {
  while (!hand_state.isCurrentBettingRoundDone()) {
    seat_t seat = hand_state.getActionOn();
    BettingDecision decision = _players[seat]->makeDecision(hand_state);
    _validate_decision(hand_state, decision);

    handleEvent(log, hand_state, seat, decision);
    broadcastEvent(hand_state, !seat, decision);
  }
  hand_state.advanceBettingRound();
}

void Session::_move_button() {
  _button = (_button + 1) % NUM_PLAYERS;
}

void Session::_init_hand() {
  _current_hand_id++;
  _move_button();
  
  uint64_t seed = _base_seed + _current_hand_id;
  srand(seed);
  _deck.shuffle();
}

void Session::_main_loop(SessionLog& log, HandState& hand_state) {
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
    HoleCardDealEvent event(holdings[seat], seat);
    _players[seat]->handleEvent(hand_state, event);
    _set_holding(event.getSeat(), event.getCard(0), event.getCard(1));
    log.record(*this, hand_state, event);
  }

  hand_state.setActionOn(_button);
  BlindPostDecision small_blind = _players[_button]->handleBlindRequest(SMALL_BLIND);
  log.record(*this, hand_state, small_blind);
  
  hand_state.setActionOn(!_button);
  BlindPostDecision big_blind = _players[!_button]->handleBlindRequest(BIG_BLIND);
  log.record(*this, hand_state, big_blind);

  hand_state.payBlinds(_button, _betting_rules->getSmallBlind(), _betting_rules->getBigBlind());

  hand_state.setActionOn(_button);
  hand_state.setBettingRound(ROUND_PREFLOP);
  _do_betting_round(log, hand_state);
  if (hand_state.isDone()) return;
 
  PublicDealEvent flop_event(flop, 3);
  handleEvent(log, hand_state, flop_event);
  broadcastEvent(hand_state, flop_event);
  hand_state.setBettingRound(ROUND_FLOP);
  _do_betting_round(log, hand_state);
  if (hand_state.isDone()) return;

  PublicDealEvent turn_event(&turn, 1);
  handleEvent(log, hand_state, turn_event);
  broadcastEvent(hand_state, turn_event);
  hand_state.setBettingRound(ROUND_TURN);
  _do_betting_round(log, hand_state);
  if (hand_state.isDone()) return;
  
  PublicDealEvent river_event(&river, 1);
  handleEvent(log, hand_state, river_event);
  broadcastEvent(hand_state, river_event);
  hand_state.setBettingRound(ROUND_RIVER);
  _do_betting_round(log, hand_state);
}

void Session::_update_score(seat_t seat, chip_amount_t delta) {
  _score[seat] += delta;
}

void Session::_award_pot(SessionLog& log, const HandState& hand_state, seat_t seat) {
  chip_amount_t net_gain = hand_state.getPotSize() - hand_state.getAmountWageredPriorRounds(seat);
  chip_amount_t net_loss = - hand_state.getAmountWageredPriorRounds(!seat);

  assert(net_gain + net_loss == 0);
  _update_score(seat, net_gain);
  _update_score(!seat, net_loss);

  PotWinEvent win_event(&hand_state, seat);
  broadcastEvent(hand_state, win_event);
  log.record(*this, hand_state, win_event);
}

void Session::_split_pot(SessionLog& log, const HandState& hand_state) {
  // no change to scores
  
  PotSplitEvent split_event(&hand_state);
  broadcastEvent(hand_state, split_event);
  log.record(*this, hand_state, split_event);
}

void Session::_finish_hand(SessionLog& log, HandState& hand_state) {
  assert(!(hand_state.hasFolded(0) && hand_state.hasFolded(1)));
  assert(hand_state.getPotSize() == 
      hand_state.getAmountWageredPriorRounds(0) + hand_state.getAmountWageredPriorRounds(1));

  if (hand_state.hasFolded(0)) {
    _award_pot(log, hand_state, 1);
  } else if (hand_state.hasFolded(1)) {
    _award_pot(log, hand_state, 0);
  } else {
    ps::PokerEvaluation evals[2];
    seat_t showdown_order[2] = {!hand_state.getButton(), hand_state.getButton()};
    for (int i=0; i<2; ++i) {
      seat_t seat = showdown_order[i];
      Holding holding = _get_holding(seat);
      ps::PokerEvaluation eval = _evaluator.evaluateHand(holding.getCardSet(), 
          hand_state.getBoard().getCards()).high();
      evals[seat] = eval;
      ShowdownEvent showdown_event(holding, eval, seat);
      broadcastEvent(hand_state, !seat, showdown_event);
      log.record(*this, hand_state, showdown_event);
    }
    if (evals[0] > evals[1]) {
      _award_pot(log, hand_state, 0);
    } else if (evals[0] < evals[1]) {
      _award_pot(log, hand_state, 1);
    } else {
      _split_pot(log, hand_state);
    }
  }
}

void Session::playHand(SessionLog& log) {
  _init_hand();
  HandState hand_state(_betting_rules->getStackSize(), _button);
  log.recordHandStart(*this, hand_state);
  _main_loop(log, hand_state);
  _finish_hand(log, hand_state);
}

