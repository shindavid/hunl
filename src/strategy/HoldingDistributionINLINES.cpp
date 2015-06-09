// Included by HoldingDistribution.h

#include "HoldingSet.h"

template<typename V>
HoldingDistribution<V>::HoldingDistribution() {
  _total_weight = 0;
  for (unsigned int r1=0; r1<ps::Rank::NUM_RANK; ++r1) {
    ps::Rank high_rank(r1);  // TODO: this Rank constructor stupidly branches. Fix pokerstove.
    HoldingSet pocket_pair(HoldingSet::POCKET_PAIR_MASK, high_rank, high_rank);
    V v1(pocket_pair.size());
    _total_weight += (float)v1;
    (*this)[pocket_pair] = v1;
    for (unsigned int r2=0; r2<r1; ++r2) {
      ps::Rank low_rank(r2);  // TODO: this Rank constructor stupidly branches. Fix pokerstove.
      HoldingSet suited(HoldingSet::SUITED_MASK, high_rank, low_rank);
      HoldingSet unsuited(HoldingSet::UNSUITED_MASK, high_rank, low_rank);
      V v2_s(suited.size());
      V v2_u(unsuited.size());
      _total_weight += v2_s;
      _total_weight += v2_u;
      (*this)[suited] = (float)v2_s;
      (*this)[unsuited] = (float)v2_u;
    }
  }
}

