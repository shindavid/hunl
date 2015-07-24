#include "engine/Board.h"
#include "engine/Deck.h"
#include "pokerstove/peval/HoldemHandEvaluator.h"
#include "strategy/RangeCalculations.h"

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using nanoseconds = std::chrono::nanoseconds;

namespace ps = pokerstove;

struct PrintSort_t {
  bool operator() (const RankedJointWeightedHolding& r1, const RankedJointWeightedHolding& r2) {
    return (r1.getEquity(0) > r2.getEquity(0)) ||
      (r1.getEquity(0) == r2.getEquity(0) && r1.getEval() > r2.getEval());
  }
} PrintSort;

int main() {
  srand(100);
  ps::HoldemHandEvaluator evaluator;
  for (int i=0; i<10; ++i) {
    RankedJointHoldingDistribution dist;
    Board board;
    Deck deck;
    deck.shuffle();
    for (int c=0; c<5; ++c) {
      ps::Card card = deck.deal();
      dist.remove(card);
      board.add(card);
    }
  
    Clock::time_point t0 = Clock::now();

    for (uint32_t j=0; j<dist.size(); ++j) {
      RankedJointWeightedHolding& item = dist[j];
      item.setEval(evaluator.evaluateHand(item.getHolding().getCardSet(), board.getCards()).high());
    }
    
    Clock::time_point t1 = Clock::now();

    rangecalc::computeEquities_naive(dist);
    
    Clock::time_point t2 = Clock::now();
    
    nanoseconds elapsed01 = std::chrono::duration_cast<nanoseconds>(t1-t0);
    nanoseconds elapsed12 = std::chrono::duration_cast<nanoseconds>(t2-t1);
    
    fprintf(stdout, "RangeCalculation test #%d\n", i);
    fprintf(stdout, "hand eval: %.fus\n", (double)(0.001*elapsed01.count()));
    fprintf(stdout, "equity calc: %.fus\n", (double)(0.001*elapsed12.count()));

    fprintf(stdout, "Board [%s %s %s %s %s]\n",
        board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
        board.getCard(3).str().c_str(), board.getCard(4).str().c_str());

    dist.sort(PrintSort);
    for (uint32_t j=0; j<dist.size(); ++j) {
      const RankedJointWeightedHolding& item = dist[j];
      Holding holding = item.getHolding();
      float equity = item.getEquity(0);
      fprintf(stdout, "%.6f %s\n", equity, holding.str().c_str());
    }
  }
  
  return 0;
}
