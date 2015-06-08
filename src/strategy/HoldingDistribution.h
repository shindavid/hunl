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
  /*
   * TODO: Create a specialized hash map that is initialized with 91 hash buckets, one for each
   * possible pair of (ps::Rank high, ps::Rank low). There are 91 such pairs because there are 13
   * pocket pairs and (13 choose 2) = 78 possible non-pocket-pair rank-pairs. 
   *
   * Then, for each bucket, use a size-n array of (suit_mask, float) pairs. I'm pretty sure the max
   * value of n that will be needed for our strategy will be something like n=4.
   *
   * With this specialization, we'll never have map resizings.
   */
  typedef std::unordered_map<HoldingSet, float, HoldingSetHashFunction> holding_map_t;
  holding_map_t _weights;
  float _total_weight;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  HoldingDistribution();
};

#include "HoldingDistributionINLINES.cpp"

