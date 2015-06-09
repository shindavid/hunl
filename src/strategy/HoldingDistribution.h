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
 */
template<typename V> class HoldingDistribution : 
  public std::unordered_map<HoldingSet, std::pair<V,float>, HoldingSetHashFunction>
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

