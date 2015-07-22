#pragma once

#include "engine/Holding.h"
#include "pokerstove/peval/Card.h"

#include <exception>

namespace ps = pokerstove;

/*
 * This is not an actual class to be used. Only here to serve as a spec for the class to be used
 * as a template parameter for HoldingDistribution.
 */
struct AbstractJointWeightedHolding {
  void init(Holding holding, float weight1, float weight2);

  Holding getHolding() const;
  void setHolding(Holding holding);

  float getWeight(int index) const;
  void setWeight(int index, float weight);
};

/*
 * A joint probability distribution over Holding's.
 *
 * Does not support O(1) lookups. 
 *
 * Item must implement the class signature of AbstractJoitgWeightedHolding above.
 */
template<typename Unit> class JointHoldingDistribution 
{
private:
  static const size_t sMaxSize = 52*51/2;
  uint32_t _size;
  float _total_weight[2];
  Unit _units[sMaxSize];

  bool _validate() const;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  JointHoldingDistribution();

  /*
   * Removes all holdings that contains <card>.
   */
  void remove(ps::Card card);
};

#include "HoldingDistributionINLINES.cpp"


