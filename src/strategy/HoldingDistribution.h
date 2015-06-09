#pragma once

#include "HoldingSet.h"
#include <unordered_map>

namespace ps = pokerstove;

struct HoldingSetHashFunction {
  uint64_t operator()(const HoldingSet& set) const {
    return set.hash();
  }
};

/*
 * A hash-map whose keys are HoldingSet's.
 *
 * Temporarily making it inherit from std::unordered_map, bdsun will change implementation later.
 *
 * Typename V must have the following properties:
 *
 * - Can be constructed from return value of HoldingSet::size()
 * - Can be casted to a float.
 */
template<typename V> class HoldingDistribution : 
  public std::unordered_map<HoldingSet, V, HoldingSetHashFunction>
{
private:
  float _total_weight;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  HoldingDistribution();
};

#include "HoldingDistributionINLINES.cpp"

