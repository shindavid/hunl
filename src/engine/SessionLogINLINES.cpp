#include <cstdio>

#include "Player.h"

void SessionLog::recordSessionStart(const Session& session) {
  fprintf(stdout, "Starting %s session, %s vs %s (seed=%lu)\n",
      session.getBettingRules().descr().c_str(), session.getPlayer(0)->getName(),
      session.getPlayer(1)->getName(), session.getBaseSeed());
}

void SessionLog::recordHandStart(const Session& session, const HandState& state) {
  fprintf(stdout, "**********************************\n");
  fprintf(stdout, "Hand #%lu\n", session.getCurrentHandID());
  for (seat_t seat=0; seat<2; ++seat) {
    chip_amount_t score = session.getScore(seat);
    fprintf(stdout, "%s score: %s$%d\n", session.getPlayer(seat)->getName(),
      score==0 ? "" : (score>0 ? "+" : "-"), abs(score));
  }
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const HoleCardDealEvent& event)
{
  fprintf(stdout, "Dealt to %s: [%s %s]\n", session.getPlayer(event.getSeat())->getName(),
      event.getCard(0).str().c_str(), event.getCard(1).str().c_str());
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const BlindPostDecision& event)
{
  fprintf(stdout, "%s posts %cB ($%d).\n", session.getPlayer(state.getActionOn())->getName(),
      event.getBlindType() == SMALL_BLIND ? 'S' : 'B', event.getAmount());
}

void SessionLog::_recordFlop(const HandState& state) {
  const Board& board = state.getBoard();
  fprintf(stdout, "Flop: [%s %s %s] (pot:$%d)\n",
      board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
      state.getPotSize());
}

void SessionLog::_recordTurn(const HandState& state) {
  const Board& board = state.getBoard();
  fprintf(stdout, "Turn: [%s %s %s][%s] (pot:$%d)\n",
      board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
      board.getCard(3).str().c_str(), state.getPotSize());
}

void SessionLog::_recordRiver(const HandState& state) {
  const Board& board = state.getBoard();
  fprintf(stdout, "River: [%s %s %s][%s][%s] (pot:$%d)\n",
      board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
      board.getCard(3).str().c_str(), board.getCard(4).str().c_str(), state.getPotSize());
}

void SessionLog::_recordBet(const Session& session, const HandState& state,
    const BettingDecision& event)
{
  seat_t seat = !state.getActionOn();
  fprintf(stdout, "%s bets $%d%s.\n", 
      session.getPlayer(seat)->getName(),
      event.getAmount(),
      state.isAllIn(seat) ? " (all-in)" : "");
}

void SessionLog::_recordRaise(const Session& session, const HandState& state,
    const BettingDecision& event)
{
  seat_t seat = !state.getActionOn();
  fprintf(stdout, "%s raises to $%d%s.\n", 
      session.getPlayer(seat)->getName(),
      state.getAmountWageredCurrentRound(seat),
      state.isAllIn(seat) ? " (all-in)" : "");
}

void SessionLog::_recordCheck(const Session& session, const HandState& state,
    const BettingDecision& event)
{
  seat_t seat = !state.getActionOn();
  fprintf(stdout, "%s checks.\n", session.getPlayer(seat)->getName());
}

void SessionLog::_recordCall(const Session& session, const HandState& state,
    const BettingDecision& event)
{
  seat_t seat = !state.getActionOn();
  fprintf(stdout, "%s calls $%d%s.\n", 
      session.getPlayer(seat)->getName(), event.getAmount(),
      state.isAllIn(seat) ? " (all-in)" : "");
}

void SessionLog::_recordFold(const Session& session, const HandState& state,
    const BettingDecision& event)
{
  fprintf(stdout, "%s folds.\n", 
      session.getPlayer(!state.getActionOn())->getName());
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const PublicDealEvent& event)
{
  switch (state.getBoard().getSize()) {
    case 3: _recordFlop(state); return;
    case 4: _recordTurn(state); return;
    case 5: _recordRiver(state); return;
    default: throw std::exception();
  }
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const BettingDecision& event)
{
  switch (event.getActionType()) {
    case ACTION_BET: _recordBet(session, state, event); return;
    case ACTION_RAISE: _recordRaise(session, state, event); return;
    case ACTION_CHECK: _recordCheck(session, state, event); return;
    case ACTION_CALL: _recordCall(session, state, event); return;
    case ACTION_FOLD: _recordFold(session, state, event); return;
  }
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const ShowdownEvent& event)
{
  fprintf(stdout, "%s shows [%s] (%s).\n", 
      session.getPlayer(event.getSeat())->getName(),
      event.getHolding().str().c_str(), event.getEval().str().c_str());
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const PotWinEvent& event) {
  chip_amount_t uncalled = event.getUncalledBetSize();
  chip_amount_t pot_size = event.getCalledPotSize();

  if (pot_size == 2 * session.getBettingRules().getSmallBlind()) {
    // (dshin) printing hack
    pot_size += uncalled;
    uncalled = 0;
  }

  fprintf(stdout, "%s wins pot of $%d", session.getPlayer(event.getSeat())->getName(), pot_size);
  if (uncalled) {
    fprintf(stdout, " (uncalled $%d returned)", uncalled);
  }
  fprintf(stdout, ".\n");
}

template<>
void SessionLog::record(const Session& session, const HandState& state, const PotSplitEvent& event) {
  fprintf(stdout, "%s and %s each win split pot of $%d.\n", 
      session.getPlayer(!state.getButton())->getName(),
      session.getPlayer(state.getButton())->getName(),
      event.getSplitAmount());
}

