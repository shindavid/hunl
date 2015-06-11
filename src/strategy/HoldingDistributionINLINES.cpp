// Included by HoldingDistribution.h

#include "HoldingSet.h"

template<typename V, int N>
HoldingDistribution<V>::HoldingDistribution() {
  for (int i=0; i<RankPair::NUM_RANK_PAIRS; ++i) {
    RankPair pair = decode(i);
    if (pair.isPocketPair()) {
      SuitMask pocket_pair_mask(SuitMask::POCKET_PAIR_MASK);
      _buckets[i][0] = element_t(pocket_pair_mask.size(), pocket_pair_mask);
    } else {
      SuitMask suited_mask(SuitMask::SUITED_MASK);
      SuitMask unsuited_mask(SuitMask::UNSUITED_MASK);
      _buckets[i][0] = element_t(suited_mask.size(), suited_mask);
      _buckets[i][1] = element_t(unsuited_mask.size(), unsuited_mask);
    }
  }

  _total_weight = 52*51/2;  // 52-choose-2
}

template<typename V, int N>
bool HoldingDistribution<V>::_validate_weights() const
{
  float total_weight = 0.0;
  for (int i=0; i<RankPair::NUM_RANK_PAIRS; ++i) {
    const bucket_t& bucket = _buckets[i];
    for (int j=0; j<N; ++j) {
      const element_t& element = bucket._elements[j];
      if (element._suit_mask.size()==0) break;
      total_weight += element._weight;
    }
  }

  return approximately_equal(total_weight, _total_weight);
}

