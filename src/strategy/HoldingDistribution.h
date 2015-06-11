#pragma once

#include "RankPair.h"
#include "SuitMask.h"
#include "pokerstove/peval/Card.h"

#include <exception>

namespace ps = pokerstove;

/*
 * A hash-map whose keys are HoldingSet's.
 *
 * Temporarily making it inherit from std::unordered_map, bdsun will change implementation later.
 */
template<typename V, int N> class HoldingDistribution 
{
private:
  /*
   * TODO: magic ordering of element_t fields based on sizeof(V) % 4?
   */
  struct element_t {
    V _v;
    float _weight = 0;
    SuitMask _suit_mask;

    element_t() = default;

    element_t(float weight, SuitMask suit_mask) :
      _weight(weight), _suit_mask(suit_mask) {}
  };

  struct bucket_t {
    element_t _elements[N];
  };

  bucket_t _buckets[RankPair::NUM_RANK_PAIRS];
  float _total_weight;

  bool _validate_weights() const;

public:
  /*
   * Default constructor intializes a uniform distribution over all (52-choose-2) possible holdings,
   * collapsing preflop-isomorphic holdings.
   */
  HoldingDistribution();

  /*
   * Removes all holdings that contains <card>.
   */
  void remove(ps::Card card) { throw std::exception("TODO"); }

  void regroupOnFlop(SuitCounts counts);
  void regroupOnTurn(SuitCounts counts);
  void regroupOnRiver(SuitCounts counts);
};

#include "HoldingDistributionINLINES.cpp"

