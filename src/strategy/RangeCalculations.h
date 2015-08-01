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

namespace rangecalc {
  void init_uniform(PreflopRange& range);

  /*
   * Sets equity values for each item in dist.
   */
  void computeEquities_naive(RiverComputationMap& map);
  void computeEquities_smart(RiverComputationMap& map);
}

#include "strategy/RangeCalculationsINLINES.cpp"

