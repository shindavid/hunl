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
    RiverEquityMatrix M;
    HoldingIndexing indexing(Holding::sNumRiverHoldings);
    HoldingMap<ps::PokerEvaluation> evals(Holding::sNumRiverHoldings);
    HoldingMap<float> range(Holding::sNumRiverHoldings);
    HoldingMap<float> equities(Holding::sNumRiverHoldings);

    RankedJointHoldingDistribution dist;
    RankedJointHoldingDistribution dist2;
    Board board;
    Deck deck;
    deck.shuffle();
    for (int c=0; c<5; ++c) {
      ps::Card card = deck.deal();
      dist.remove(card);
      dist2.remove(card);
      board.add(card);
    }
  
    Clock::time_point t0 = Clock::now();

    for (uint32_t j=0; j<dist.size(); ++j) {
      RankedJointWeightedHolding& item = dist[j];
      ps::PokerEvaluation eval = evaluator.evaluateHand(item.getHolding().getCardSet(),
          board.getCards()).high();
      item.setEval(eval);
      dist2[j].setEval(eval);

      indexing[j] = CompactHolding(item.getHolding());
      evals[j] = eval;
      range[j] = 1.0;
    }
    
    Clock::time_point t1 = Clock::now();
    rangecalc::computeEquities_naive(dist);
    Clock::time_point t2 = Clock::now();
    rangecalc::computeEquities_smart(dist2);
    Clock::time_point t3 = Clock::now();
    rangecalc::computeRiverEquityMatrix(M, indexing, evals);
    Clock::time_point t4 = Clock::now();
    M.compute_equities(range, equities);
    Clock::time_point t5 = Clock::now();

    nanoseconds elapsed01 = std::chrono::duration_cast<nanoseconds>(t1-t0);
    nanoseconds elapsed12 = std::chrono::duration_cast<nanoseconds>(t2-t1);
    nanoseconds elapsed23 = std::chrono::duration_cast<nanoseconds>(t3-t2);
    nanoseconds elapsed34 = std::chrono::duration_cast<nanoseconds>(t4-t3);
    nanoseconds elapsed45 = std::chrono::duration_cast<nanoseconds>(t5-t4);
    
    fprintf(stdout, "RangeCalculation test #%d\n", i);
    fprintf(stdout, "hand eval: %.fus\n", (double)(0.001*elapsed01.count()));
    fprintf(stdout, "naive equity calc: %.fus\n", (double)(0.001*elapsed12.count()));
    fprintf(stdout, "smart equity calc: %.fus\n", (double)(0.001*elapsed23.count()));
    fprintf(stdout, "river matrix calc: %.fus\n", (double)(0.001*elapsed34.count()));
    fprintf(stdout, "river equity calc: %.fus\n", (double)(0.001*elapsed45.count()));

    fprintf(stdout, "Board [%s %s %s %s %s]\n",
        board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
        board.getCard(3).str().c_str(), board.getCard(4).str().c_str());

    dist.sort(PrintSort);
    dist2.sort(PrintSort);
    
    std::vector<RankedJointWeightedHolding> dist3;
    for (uint32_t j=0; j<dist.size(); ++j) {
      RankedJointWeightedHolding r;
      r.init(indexing[j].toHolding(), 1.0, 1.0);
      r.setEval(evals[j]);
      r.setEquity(0, equities[j]);
      r.setEquity(1, equities[j]);
      dist3.push_back(r);
    }
    std::sort(dist3.begin(), dist3.end(), PrintSort);

    fprintf(stdout, "naive  smart  hand\n");
    for (uint32_t j=0; j<dist.size(); ++j) {
      const RankedJointWeightedHolding& item = dist[j];
      Holding holding = item.getHolding();
      float naive_equity = item.getEquity(0);
      float smart_equity = dist2[j].getEquity(0);
      float river_equity = dist3[j].getEquity(0);
      assert(approximately_equal(naive_equity, smart_equity));
      //assert(approximately_equal(naive_equity, river_equity));
      fprintf(stdout, "%.4f %.4f %.4f %s\n", naive_equity, smart_equity, river_equity,
          holding.str().c_str());
    }
  }
  
  return 0;
}
