#pragma once

#include "engine/HandState.h"
#include "engine/TypeDefs.h"

#include <string>

class BettingRules {
protected:
  const chip_amount_t _small_blind;
  const chip_amount_t _big_blind;
  const chip_amount_t _stack_size;

public:
  BettingRules(chip_amount_t small_blind, chip_amount_t big_blind, chip_amount_t stack_size);
  virtual ~BettingRules() {}

  virtual std::string descr() const = 0;

  /*
   * The minimum # of chips the current player can put into the pot, if betting/raising.
   *
   * Note that if facing a bet/raise, the returned amount includes the call amount.
   *
   * If the player cannot legally bet/raise, returns 0.
   */
  virtual chip_amount_t getMinLegalBetOrRaise(const HandState& state) const = 0;

  /*
   * The maximum # of chips the current player can put into the pot, if betting/raising.
   *
   * Note that if facing a bet/raise, the returned amount includes the call amount.
   * 
   * If the player cannot legally bet/raise, returns 0.
   */
  virtual chip_amount_t getMaxLegalBetOrRaise(const HandState& state) const = 0;

  /*
   * The # of chips the current player can put into the pot, if checking/calling.
   */
  chip_amount_t getLegalCheckOrCall(const HandState& state) const;

  /*
   * Rounds amount to the nearest value that falls in the range
   * [getMinLegalBetOrRaise(), getMaxLegalBetOrRaise()]
   */
  chip_amount_t legalize(const HandState& state, chip_amount_t amount) const;

  chip_amount_t getBlindAmount(BlindType btype) const;
  chip_amount_t getSmallBlind() const;
  chip_amount_t getBigBlind() const;
  chip_amount_t getStackSize() const;
};

/*
 * Assumes equal stack sizes.
 */
class NoLimitRules : public BettingRules {
public:
  NoLimitRules(chip_amount_t stack_size, chip_amount_t small_blind, chip_amount_t big_blind);
  virtual std::string descr() const;
 
  virtual chip_amount_t getMinLegalBetOrRaise(const HandState& state) const;
  virtual chip_amount_t getMaxLegalBetOrRaise(const HandState& state) const;
};

/*
 * Assumes stack size of 100*big_blind (essentially infinite), each round capped at 4 bets.
 */
class FixedLimitRules : public BettingRules {
private:
  chip_amount_t _getBetSize(BettingRound round) const;
  chip_amount_t _getLegalBetOrRaise(const HandState& state) const;

public:
  FixedLimitRules(chip_amount_t small_blind, chip_amount_t big_blind);
  virtual std::string descr() const;
  
  virtual chip_amount_t getMinLegalBetOrRaise(const HandState& state) const;
  virtual chip_amount_t getMaxLegalBetOrRaise(const HandState& state) const;
};

#include "engine/BettingRulesINLINES.cpp"

