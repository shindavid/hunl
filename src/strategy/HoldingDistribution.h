#pragma once

#include "HoldingSet.h"
#include "pokerstove/peval/Rank.h"

#include <unordered_map>

namespace ps = pokerstove;

/*
 * A HoldingDistribution represents a probability distribution over HoldingSet objects.
 */
class HoldingDistribution {
private:
  std::unordered_map<HoldingSet, double, HoldingSetHashFunction> _weights;
  double _total_weight;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  HoldingDistribution();
};

#include "HoldingDistributionINLINES.cpp"

