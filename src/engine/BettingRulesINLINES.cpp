#include <assert.h>
#include <cstdio>

/*
 * BettingRules
 */
BettingRules::BettingRules(chip_amount_t small_blind, chip_amount_t big_blind,
    chip_amount_t stack_size)
  : _small_blind(small_blind)
  , _big_blind(big_blind)
  , _stack_size(stack_size) {}

chip_amount_t BettingRules::getLegalCheckOrCall(const HandState& state) const {
  seat_t seat = state.getActionOn();
  return state.getMaxAmountWageredCurrentRound() - state.getAmountWageredCurrentRound(seat);
}
  
chip_amount_t BettingRules::legalize(const HandState& state, chip_amount_t amount) const {
  return std::max(std::min(amount, getMaxLegalBetOrRaise(state)), getMinLegalBetOrRaise(state));
}

chip_amount_t BettingRules::getBlindAmount(BlindType btype) const {
  return btype==SMALL_BLIND ? _small_blind : _big_blind;
}
chip_amount_t BettingRules::getSmallBlind() const { return _small_blind; }
chip_amount_t BettingRules::getBigBlind() const { return _big_blind; }
chip_amount_t BettingRules::getStackSize() const { return _stack_size; }

/*
 * NoLimitRules
 */
NoLimitRules::NoLimitRules(chip_amount_t stack_size, chip_amount_t small_blind, 
    chip_amount_t big_blind)
  : BettingRules(small_blind, big_blind, stack_size)
{
  assert(small_blind*2 == big_blind);
  assert(big_blind < stack_size);
  assert(stack_size % big_blind == 0);
}

std::string NoLimitRules::descr() const {
  char buffer[128];
  sprintf(buffer, "%dBB-deep $%d/$%d NL", _stack_size/_big_blind, _small_blind, _big_blind);
  return std::string(buffer);
}

chip_amount_t NoLimitRules::getMinLegalBetOrRaise(const HandState& state) const {
  chip_amount_t call = this->getLegalCheckOrCall(state);
  chip_amount_t bound = call + std::max(call, _big_blind);  // SB 1st to act must raise at least 1.5BB
  return std::min(this->getMaxLegalBetOrRaise(state), bound);
}

chip_amount_t NoLimitRules::getMaxLegalBetOrRaise(const HandState& state) const {
  seat_t seat = state.getActionOn();
  chip_amount_t remaining = state.getRemainingChips(seat);
  chip_amount_t call = this->getLegalCheckOrCall(state);
  return remaining > call ? remaining : 0;
}

/*
 * FixedLimitRules
 */
FixedLimitRules::FixedLimitRules(chip_amount_t small_blind, chip_amount_t big_blind)
  : BettingRules(small_blind, big_blind, 100*big_blind)
{
  assert(small_blind*2 == big_blind);
}

std::string FixedLimitRules::descr() const {
  char buffer[128];
  sprintf(buffer, "$%d/$%d FL", _big_blind, 2*_big_blind);
  return std::string(buffer);
}

chip_amount_t FixedLimitRules::_getBetSize(BettingRound round) const {
  switch (round) {
    case ROUND_PREFLOP:
    case ROUND_FLOP: return _big_blind;
    default: return 2*_big_blind;
  }
}

chip_amount_t FixedLimitRules::_getLegalBetOrRaise(const HandState& state) const {
  chip_amount_t bet_size = _getBetSize(state.getBettingRound());
  chip_amount_t cap = 4*bet_size;
  chip_amount_t max_wagered = state.getMaxAmountWageredCurrentRound();

  return (max_wagered==cap) ? 0 : (this->getLegalCheckOrCall(state) + bet_size);
}

chip_amount_t FixedLimitRules::getMinLegalBetOrRaise(const HandState& state) const {
  return this->_getLegalBetOrRaise(state);
}

chip_amount_t FixedLimitRules::getMaxLegalBetOrRaise(const HandState& state) const {
  return this->_getLegalBetOrRaise(state);
}

