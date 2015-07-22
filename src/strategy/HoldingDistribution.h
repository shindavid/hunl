#pragma once

#include "engine/Holding.h"
#include "pokerstove/peval/Card.h"

#include <exception>

namespace ps = pokerstove;

/*
 * This is not an actual class to be used. Only here to serve as a spec for the class to be used
 * as a template parameter for HoldingDistribution.
 */
struct AbstractWeightedHolding {
  void init(Holding holding, float weight);

  Holding getHolding() const;
  void setHolding(Holding holding);

  float getWeight() const;
  void setWeight(float weight);
};

/*
 * A probability distribution over Holding's.
 *
 * Does not support O(1) lookups. 
 *
 * Item must implement the class signature of AbstractWeightedHolding above.
 */
template<typename Unit> class HoldingDistribution 
{
private:
  static const size_t sMaxSize = 52*51/2;
  uint32_t _size;
  float _total_weight;
  Unit _units[sMaxSize];

  bool _validate() const;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  HoldingDistribution();

  /*
   * Removes all holdings that contains <card>.
   */
  void remove(ps::Card card);
};

#include "HoldingDistributionINLINES.cpp"

