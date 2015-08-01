#include "engine/Board.h"
#include "engine/Deck.h"
#include "pokerstove/peval/HoldemHandEvaluator.h"
#include "strategy/RangeCalculations.h"

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using nanoseconds = std::chrono::nanoseconds;

namespace ps = pokerstove;

int main() {
  srand(100);
  ps::HoldemHandEvaluator evaluator;
  for (int i=0; i<10; ++i) {
    PreflopRange preflop_range;
    rangecalc::init_uniform(preflop_range);

    Board board;
    Deck deck;
    deck.shuffle();
    for (int c=0; c<5; ++c) {
      ps::Card card = deck.deal();
      board.add(card);
    }

    RiverRange range;
    preflop_range.remove(board.getCards(), range);

    RiverComputationMap river_comp_naive(range);
    RiverComputationMap river_comp_smart(range);

    Clock::time_point t0 = Clock::now();

    for (size_t j=0; j<RiverRange::sSize; ++j) {
      ps::PokerEvaluation eval = evaluator.evaluateHand(range.getHolding(j).getCardSet(),
          board.getCards()).high();

      RiverComputationUnit unit;
      unit.weight[0] = 1.0;
      unit.weight[1] = 1.0;
      unit.eval = eval;
      river_comp_naive.setValue(j, unit);
      river_comp_smart.setValue(j, unit);
    }
    
    Clock::time_point t1 = Clock::now();
    rangecalc::computeEquities_naive(river_comp_naive);
    Clock::time_point t2 = Clock::now();
    rangecalc::computeEquities_smart(river_comp_smart);
    Clock::time_point t3 = Clock::now();
    /*
    rangecalc::computeRiverEquityMatrix(M, indexing, evals);
    Clock::time_point t4 = Clock::now();
    M.compute_equities(range, equities);
    Clock::time_point t5 = Clock::now();
*/
    
    nanoseconds elapsed01 = std::chrono::duration_cast<nanoseconds>(t1-t0);
    nanoseconds elapsed12 = std::chrono::duration_cast<nanoseconds>(t2-t1);
    nanoseconds elapsed23 = std::chrono::duration_cast<nanoseconds>(t3-t2);
    /*
    nanoseconds elapsed34 = std::chrono::duration_cast<nanoseconds>(t4-t3);
    nanoseconds elapsed45 = std::chrono::duration_cast<nanoseconds>(t5-t4);
    */
    
    fprintf(stdout, "RangeCalculation test #%d\n", i);
    fprintf(stdout, "hand eval: %.fus\n", (double)(0.001*elapsed01.count()));
    fprintf(stdout, "naive equity calc: %.fus\n", (double)(0.001*elapsed12.count()));
    fprintf(stdout, "smart equity calc: %.fus\n", (double)(0.001*elapsed23.count()));
    //fprintf(stdout, "river matrix calc: %.fus\n", (double)(0.001*elapsed34.count()));
    //fprintf(stdout, "river equity calc: %.fus\n", (double)(0.001*elapsed45.count()));

    fprintf(stdout, "Board [%s %s %s %s %s]\n",
        board.getCard(0).str().c_str(), board.getCard(1).str().c_str(), board.getCard(2).str().c_str(),
        board.getCard(3).str().c_str(), board.getCard(4).str().c_str());

    river_comp_naive.sort(PrintCompare);
    river_comp_smart.sort(PrintCompare);

    fprintf(stdout, "naive        | smart\n");
    for (int j=0; j<RiverRange::sSize; ++j) {
      Holding holding_naive = river_comp_naive.getHolding(j);
      Holding holding_smart = river_comp_smart.getHolding(j);
      float naive_equity = river_comp_naive.getValue(j).equity[0];
      float smart_equity = river_comp_smart.getValue(j).equity[0];
      assert(approximately_equal(naive_equity, smart_equity));
      fprintf(stdout, "%.6f %s | %.6f %s\n", naive_equity, holding_naive.str().c_str(), smart_equity,
          holding_smart.str().c_str());
    }
  }
  
  return 0;
}
