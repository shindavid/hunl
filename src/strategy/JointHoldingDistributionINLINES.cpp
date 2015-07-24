// Included by JointHoldingDistribution.h

#include "util/Arithmetic.h"

template<typename Unit>
JointHoldingDistribution<Unit>::JointHoldingDistribution() {
  this->_size = 0;
  for (int i=0; i<52; ++i) {
    for (int j=i+1; j<52; ++j) {
      uint64_t mask = (1LL<<i) | (1LL<<j);
      _units[_size++].init(Holding(ps::CardSet(mask)), 1.0, 1.0);
    }
  }

  _total_weight[0] = _total_weight[1] = 52*51/2;  // 52-choose-2
  assert(_validate());
}

template<typename Unit>
bool JointHoldingDistribution<Unit>::_validate() const
{
  assert(_size <= sMaxSize);

  float total_weight[2] = {};
  for (int i=0; i<_size; ++i) {
    for (int j=0; j<2; ++j) {
      total_weight[j] += _units[i].getWeight(j);
    }
  }

  return approximately_equal(total_weight[0], _total_weight[0]) && 
    approximately_equal(total_weight[1], _total_weight[1]);
}
  
template<typename Unit>
void JointHoldingDistribution<Unit>::remove(ps::Card card) {
  size_t r = 0;
  size_t w = 0;
  size_t N = _size;

  for (; r<N; ++r) {
    const Unit& unit = _units[r];
    bool contains = unit.getHolding().contains(card);

    _units[w] = unit;
    w += contains ? 0 : 1;
    _size -= contains ? 1 : 0;
    _total_weight[0] -= contains ? unit.getWeight(0) : 0;
    _total_weight[1] -= contains ? unit.getWeight(1) : 0;
  }

  _validate();
}

template<typename Unit> template<class Compare>
void JointHoldingDistribution<Unit>::sort(Compare comp) {
  std::sort(_units, &_units[_size], comp);
}

