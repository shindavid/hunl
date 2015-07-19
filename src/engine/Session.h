#pragma once

#include "engine/BettingRules.h"
#include "engine/Deck.h"
#include "engine/HandState.h"
#include "engine/Player.h"
#include "engine/TypeDefs.h"
#include "pokerstove/peval/CardSet.h"
#include "pokerstove/peval/HoldemHandEvaluator.h"

#include <cstdlib>
#include <assert.h>

namespace ps = pokerstove;

class SessionLog;

class Session {
private:
  ps::HoldemHandEvaluator _evaluator;
  const BettingRules* const _betting_rules;
  Player* const _players[NUM_PLAYERS];
  Holding _holdings[NUM_PLAYERS];
  chip_amount_t _score[NUM_PLAYERS]; 
  hand_id_t _current_hand_id = 0;
  Deck _deck;
  seat_t _button = 0;
  
  /*
   * The seed determines the cards dealt out per hand. At the start of the i'th hand, the random seed is
   * re-initialized to (_base_seed + i), so that the same cards get dealt out no matter what the
   * players do.
   *
   * By running two session with the same seed, but with the order of the players switched, you can
   * run duplicate poker (http://en.wikipedia.org/wiki/Duplicate_poker).
   */
  const uint64_t _base_seed;

  void _set_holding(seat_t seat, ps::Card c1, ps::Card c2);
  Holding _get_holding(seat_t seat) const;
  void _update_score(seat_t seat, chip_amount_t delta);
  void _move_button();
  void _init_hand();
  void _main_loop(SessionLog& log, HandState& hand);
  void _finish_hand(SessionLog& log, HandState& hand);
  void _do_betting_round(SessionLog& log, HandState& hand);
  void _award_pot(SessionLog& log, const HandState& state, seat_t seat);
  void _split_pot(SessionLog& log, const HandState& state);
  void _validate_decision(const HandState& state, const BettingDecision& decision);

  template<typename E> void handleEvent(SessionLog& log, HandState& state, const E& event);
  template<typename E> void handleEvent(SessionLog& log, HandState& state, seat_t seat,
      const E& event);
  
  template<typename E> void broadcastEvent(const HandState& state, const E& event) const;
  template<typename E> void broadcastEvent(const HandState& state, seat_t seat, const E& event) const;

public:
  Session(Player* p0, Player* p1, const BettingRules* betting_rules, uint64_t base_seed);

  const Player* getPlayer(int i) const;
  const BettingRules& getBettingRules() const;
  uint64_t getBaseSeed() const;
  session_id_t getCurrentHandID() const;
  seat_t getButton() const;
  chip_amount_t getScore(seat_t seat) const;
  
  void playHand(SessionLog& log);
};

#include "engine/SessionINLINES.cpp"

