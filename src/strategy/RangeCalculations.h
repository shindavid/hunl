#include "strategy/EquityMatrix.h"
#include "strategy/JointHoldingDistribution.h"
#include "strategy/Range.h"

struct RankCompare_t {
  template<typename T>
  bool operator() (const T& t1, const T& t2) { return t1.t.eval < t2.t.eval; }
} RankCompare;

struct PrintCompare_t {
  template<typename T>
  bool operator() (const T& t1, const T& t2) {
    float eq1 = t1.t.equity[0];
    float eq2 = t2.t.equity[0];
    ps::PokerEvaluation eval1 = t1.t.eval;
    ps::PokerEvaluation eval2 = t2.t.eval;
    return eq1>eq2 || (eq1==eq2 && eval1>eval2);
  }
} PrintCompare;

struct PrintCompare2_t {
  template<typename T>
  bool operator() (const T& t1, const T& t2) {
    float eq1 = t1.t.equity[0];
    float eq2 = t2.t.equity[0];
    return eq1>eq2;
  }
} PrintCompare2;

namespace rangecalc {
  void init_uniform(PreflopRange& range);

  void computeTurnEquityMatrix(TurnEquityMatrix& matrix, const Board& board, const TurnEvalMap& evals);

  /*
   * Sets equity values for each item in map.
   *
   * NOTE(dshin): currently, the pre-river computeEquities*() functions assume that every unknown card
   * is equally like to get dealt from the deck. In other words, they ignore the effect of range
   * updates on the deck distribution. If we choose to get more sophisticated, we can relax this
   * assumption in the future by applying a joint distribution estimation algorithm like Gibb's
   * sampling. My personal feeling is that the assumption is mild enough to not have a serious impact
   * on the computations.
   *
   * ADDENDUM(dshin): On the other hand, there are faster algo's than Gibb's that probably work
   * decently enough here. Can compute a probability weight per-card for each player from the range
   * weights, and then maybe estimate that the probability weight of that card from the deck is 
   * proportional to (1-w0)*(1-w1). Later, can try to retrofit algo's to use this approximation.
   */
  void computeEquities_naive(RiverComputationMap& map, const TurnEvalMap& turn_evals,
      const Board& board);
  void computeEquities_naive(RiverComputationMap& map);
  void computeEquities_smart(RiverComputationMap& map);
}

#include "strategy/RangeCalculationsINLINES.cpp"

