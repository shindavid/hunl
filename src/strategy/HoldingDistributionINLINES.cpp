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
  assert(_validate_weights());
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

/*
 * Groupings:
 *
 * A. Monotone case (e.g., 3 spades)
 *    1. ss
 *    2. s-
 *    3. -s
 *    4. --
 * B. Rainbow case (e.g., 1 diamond, 1 heart, 1 spade)
 *    1. dd
 *    2. hh
 *    3. ss
 *    4. --
 * C. Two-of-one-suit case (e.g., 2 diamonds, 1 heart)
 *    1. dd
 *    2. d-
 *    3. -d
 *    4. hh
 *    5. --
 */
template<typename V, int N>
void regroupOnFlop(SuitCounts counts)
{
  assert(counts.size()==3);
  throw std::exception("TODO");
}

/*
 * Groupings:
 *
 * A. Monotone case (e.g., 4 spades)
 *    1. ss (distinct from s- because of straight-flush possibilities)
 *    2. s-
 *    3. -s
 *    4. --
 * B. Rainbow case (e.g., 1 diamond, 1 heart, 1 spade, 1 diamond)
 *    1. --
 * C. Three-of-one-suit case (e.g., 3 diamonds, 1 heart)
 *    1. dd
 *    2. d-
 *    3. -d
 *    4. --
 * D. Two-of-one-suit case (e.g., 2 diamonds, 1 heart, 1 spade)
 *    1. dd
 *    2. --
 * E. Two-of-two-suits case (e.g., 2 diamonds, 2 hearts)
 *    1. dd
 *    2. hh
 *    3. --
 */
template<typename V, int N>
void regroupOnTurn(SuitCounts counts)
{
  assert(counts.size()==4);
  throw std::exception("TODO");
}

/*
 * Groupings:
 *
 * A. 5-of-one-suit case (e.g, 5 spades)
 *    1. ss (distinct from s- because of straight-flush possibilities)
 *    2. s-
 *    3. -s
 *    4. --
 * B. 4-of-one-suit case (e.g, 4 spades)
 *    1. ss (distinct from s- because of straight-flush possibilities)
 *    2. s-
 *    3. -s
 *    4. --
 * C. 3-of-one-suit case (e.g, 3 spades)
 *    1. ss
 *    2. --
 * D. other
 *    1. --
 */
template<typename V, int N>
void regroupOnRiver(SuitCounts counts)
{
  assert(counts.size()==5);
  throw std::exception("TODO");
}

