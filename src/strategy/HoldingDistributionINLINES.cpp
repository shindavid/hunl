// Included by HoldingDistribution.h

template<typename Unit>
HoldingDistribution<Unit>::HoldingDistribution() {
  this->_size = 0;
  for (int i=0; i<52; ++i) {
    for (int j=i+1; j<52; ++j) {
      uint64_t mask = (1LL<<i) | (1LL<<j);
      _units[_size++].init(Holding(ps::CardSet(mask)), 1.0);
    }
  }

  _total_weight = 52*51/2;  // 52-choose-2
  assert(_validate());
}

template<typename Unit>
bool HoldingDistribution<Unit>::_validate() const
{
  assert(_size <= sMaxSize);

  float total_weight = 0.0;
  for (int i=0; i<_size; ++i) {
    total_weight += units[i].getWeight();
  }

  return approximately_equal(total_weight, _total_weight);
}
  
template<typename Unit>
void HoldingDistribution<Unit>::remove(ps::Card card) {
  size_t r = 0;
  size_t w = 0;
  size_t N = _size;

  for (; r<N; ++r) {
    const Unit& unit = _units[r];
    bool contains = unit.getHolding().contains(card);

    _units[w] = unit;
    w += contains ? 0 : 1;
    _size -= contains ? 1 : 0;
    _total_weight -= contains ? unit.getWeight() : 0;
  }

  _validate();
}

template<typename Unit, class Compare>
void HoldingDistribution<Unit>::sort(Compare comp) {
  std::sort(_units, &_units[_size], comp);
}

