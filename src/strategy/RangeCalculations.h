#include "strategy/JointHoldingDistribution.h"
#include "strategy/Range.h"

struct RankCompare_t {
  template<typename T>
  bool operator() (const T& t1, const T& t2) { return t1.t < t2.t; }
} RankCompare;

namespace rangecalc {
  void init_uniform(PreflopRange& range);

  /*
   * Sets equity values for each item in dist.
   */
  void computeEquities_naive(const RiverRange& range, const RiverEvals& evals,
      RiverEquities& equities);
  void computeEquities_smart(RiverRange& range, RiverEvals& evals,
      RiverEquities& equities);
  /*void computeRiverEquityMatrix(RiverEquityMatrix& M, const HoldingIndexing& indexing,
      const HoldingMap<ps::PokerEvaluation>& evals);*/
}

#include "strategy/RangeCalculationsINLINES.cpp"

