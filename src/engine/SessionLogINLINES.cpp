#include <cstdio>

#include "Player.h"

void SessionLog::recordSessionStart(const SessionParams& params, uint64_t base_seed) {
  fprintf(stdout, "Starting session %lu of %d-%d HUNL with %d stack sizes, %s vs %s (seed=%lu)\n",
      params.getID(), params.getSmallBlindSize(), params.getBigBlindSize(),
      params.getStartingStackSize(), params.getPlayer(0)->getName(), params.getPlayer(1)->getName(),
      base_seed);
}

void SessionLog::recordHandStart(const SessionState& state) {
  fprintf(stdout, "**********************************\n");
  fprintf(stdout, "Hand #%lu\n", state.getCurrentHandID());
}

template<>
void SessionLog::record(const HoleCardDealEvent* event) {
  fprintf(stdout, "Dealt to %s: [%s %s]\n",
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getCard(0).str().c_str(), event->getCard(1).str().c_str());
}

template<>
void SessionLog::record(const BlindPostEvent* event) {
  fprintf(stdout, "%s posts %cB ($%d).\n",
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getBlindType() == SMALL_BLIND ? 'S' : 'B',
      event->getAmount());
}

template<>
void SessionLog::record(const FlopDealEvent* event) {
  fprintf(stdout, "Flop: [%s %s %s] (pot:$%d)\n",
      event->getPublicHandState().getBoardCard(0).str().c_str(),
      event->getPublicHandState().getBoardCard(1).str().c_str(),
      event->getPublicHandState().getBoardCard(2).str().c_str(),
      event->getPublicHandState().getPotSize());
}

template<>
void SessionLog::record(const TurnDealEvent* event) {
  fprintf(stdout, "Turn: [%s %s %s][%s] (pot:$%d)\n",
      event->getPublicHandState().getBoardCard(0).str().c_str(),
      event->getPublicHandState().getBoardCard(1).str().c_str(),
      event->getPublicHandState().getBoardCard(2).str().c_str(),
      event->getPublicHandState().getBoardCard(3).str().c_str(),
      event->getPublicHandState().getPotSize());
}

template<>
void SessionLog::record(const RiverDealEvent* event) {
  fprintf(stdout, "River: [%s %s %s][%s][%s] (pot:$%d)\n",
      event->getPublicHandState().getBoardCard(0).str().c_str(),
      event->getPublicHandState().getBoardCard(1).str().c_str(),
      event->getPublicHandState().getBoardCard(2).str().c_str(),
      event->getPublicHandState().getBoardCard(3).str().c_str(),
      event->getPublicHandState().getBoardCard(4).str().c_str(),
      event->getPublicHandState().getPotSize());
}

template<>
void SessionLog::record(const FoldDecision* event) {
  fprintf(stdout, "%s folds.\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName());
}

template<>
void SessionLog::record(const CheckDecision* event) {
  fprintf(stdout, "%s checks.\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName());
}

template<>
void SessionLog::record(const CallDecision* event) {
  fprintf(stdout, "%s calls $%d%s.\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getChipAmount(),
      event->getPublicHandState().isAllIn(event->getSeat()) ? " (all-in)" : "");
}

template<>
void SessionLog::record(const BetDecision* event) {
  fprintf(stdout, "%s bets $%d%s.\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getChipAmount(),
      event->getPublicHandState().isAllIn(event->getSeat()) ? " (all-in)" : "");
}

template<>
void SessionLog::record(const RaiseDecision* event) {
  fprintf(stdout, "%s raises to $%d%s.\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getChipAmount(),
      event->getPublicHandState().isAllIn(event->getSeat()) ? " (all-in)" : "");
}

template<>
void SessionLog::record(const ShowdownEvent* event) {
  fprintf(stdout, "%s shows [%s %s] (%s).\n", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getCard(0).str().c_str(), event->getCard(1).str().c_str(),
      event->getEvaluation().str().c_str());
}

template<>
void SessionLog::record(const PotWinEvent* event) {
  fprintf(stdout, "%s wins pot of $%d", 
      event->getPublicHandState().getPlayer(event->getSeat())->getName(),
      event->getCalledPotSize());
  chip_amount_t uncalled = event->getUncalledBetSize();
  if (uncalled) {
    fprintf(stdout, " (uncalled $%d returned)", uncalled);
  }
  fprintf(stdout, ".\n");
}

template<>
void SessionLog::record(const PotSplitEvent* event) {
  fprintf(stdout, "%s and %s each win split pot of $%d.", 
      event->getPublicHandState().getPlayer(!event->getPublicHandState().getButton())->getName(),
      event->getPublicHandState().getPlayer(event->getPublicHandState().getButton())->getName(),
      event->getSplitAmount());
}

