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

    RiverEvals evals(range);
    RiverEquities equities_naive(range);
    RiverEquities equities_smart(range);

    Clock::time_point t0 = Clock::now();

    for (size_t j=0; j<RiverRange::sSize; ++j) {
      ps::PokerEvaluation eval = evaluator.evaluateHand(evals.getHolding(j).getCardSet(),
          board.getCards()).high();
      evals.setValue(j, eval);
    }
    
    Clock::time_point t1 = Clock::now();
    rangecalc::computeEquities_naive(range, evals, equities_naive);
    Clock::time_point t2 = Clock::now();
    rangecalc::computeEquities_smart(range, evals, equities_smart);
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

    int permutation[RiverRange::sSize];
    int permutation2[RiverRange::sSize];
    for (int j=0; j<RiverRange::sSize; ++j) {
      permutation[j] = j;
      permutation2[j] = j;
    }
    std::sort(permutation, &permutation[RiverRange::sSize], 
        [&](const int& a, const int& b) {
        float eqa = equities_naive.getValue(a).equity[0];
        float eqb = equities_naive.getValue(b).equity[0];
        ps::PokerEvaluation evala = evals.getValue(a);
        ps::PokerEvaluation evalb = evals.getValue(b);
        return eqa>eqb || (eqa==eqb && evala>evalb);
        });
    std::sort(permutation2, &permutation2[RiverRange::sSize], 
        [&](const int& a, const int& b) {
        float eqa = equities_smart.getValue(a).equity[0];
        float eqb = equities_smart.getValue(b).equity[0];
        ps::PokerEvaluation evala = evals.getValue(a);
        ps::PokerEvaluation evalb = evals.getValue(b);
        return eqa>eqb || (eqa==eqb && evala>evalb);
        });

    RiverEquities equities_naive_sorted;
    RiverEquities equities_smart_sorted;
    for (int j=0; j<RiverRange::sSize; ++j) {
      int pj = permutation[j];
      equities_naive_sorted.setHolding(j, equities_naive.getHolding(pj));
      equities_naive_sorted.setValue(j, equities_naive.getValue(pj));
      
      int pj2 = permutation2[j];
      equities_smart_sorted.setHolding(j, equities_smart.getHolding(pj2));
      equities_smart_sorted.setValue(j, equities_smart.getValue(pj2));
    }

    fprintf(stdout, "naive    smart    hand\n");
    for (int j=0; j<RiverRange::sSize; ++j) {
      Holding holding = equities_naive_sorted.getHolding(j);
      float naive_equity = equities_naive_sorted.getValue(j).equity[0];
      float smart_equity = equities_smart_sorted.getValue(j).equity[0];
      assert(approximately_equal(naive_equity, smart_equity));
      fprintf(stdout, "%.6f %.6f %s\n", naive_equity, smart_equity, holding.str().c_str());
    }
  }
  
  return 0;
}
