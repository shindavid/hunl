#pragma once

#include "HoldingSet.h"
#include "pokerstove/peval/Rank.h"

#include <unordered_map>

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
   * collapsing suit-isomorphic holdings for maximum compactness.
   */
  HoldingDistribution() {
    for (unsigned int r1=0; r1<ps::Rank::NUM_RANK; ++r1) {
      ps::Rank high_rank(r1);  // TODO: this Rank constructor stupidly branches. Fix pokerstove.
      HoldingSet pocket_pair(HoldingSet::POCKET_PAIR_MASK, high_rank, high_rank);
      _weights[pocket_pair] = pocket_pair.size();
      for (unsigned int r2=0; r2<r1; ++r2) {
        ps::Rank low_rank(r2);  // TODO: this Rank constructor stupidly branches. Fix pokerstove.
        HoldingSet suited(HoldingSet::SUITED_MASK, high_rank, low_rank);
        HoldingSet unsuited(HoldingSet::UNSUITED_MASK, high_rank, low_rank);
        _weights[suited] = suited.size();
        _weights[unsuited] = unsuited.size();
      }
    }
  }
};

