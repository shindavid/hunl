#include "strategy/JointHoldingDistribution.h"

/*
 * Implements AbstractJointWeightedHolding
 */
class RankedJointWeightedHolding {
private:
  Holding _holding;
  ps::PokerEvaluation _eval;
  float _weight[2];
  float _equity[2];

public:
  void init(Holding holding, float weight1, float weight2);

  Holding getHolding() const;
  void setHolding(Holding holding);

  float getWeight(int index) const;
  void setWeight(int index, float weight);

  float getEquity(int index) const;
  void setEquity(int index, float equity);
  
  ps::PokerEvaluation getEval() const;
  void setEval(ps::PokerEvaluation eval);
};

struct RankCompare_t {
  bool operator() (const RankedJointWeightedHolding& r1, const RankedJointWeightedHolding& r2) {
    return r1.getEval() < r2.getEval();
  }
} RankCompare;

typedef JointHoldingDistribution<RankedJointWeightedHolding> RankedJointHoldingDistribution;

namespace rangecalc {
  /*
   * Sets equity values for each item in dist.
   */
  void computeEquities_naive(RankedJointHoldingDistribution& dist);
  void computeEquities_smart(RankedJointHoldingDistribution& dist);
}

#include "strategy/RangeCalculationsINLINES.cpp"

