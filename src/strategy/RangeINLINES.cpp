
template <size_t tSize, typename T> 
void HoldingMap<tSize,T>::normalizeWeights() {
  float sum[2] = {};
  // TODO: vectorize
  for (size_t i=0; i<tSize; ++i) {
    sum[0] += _units[i].t.weight[0];
    sum[1] += _units[i].t.weight[1];
  }

  float inv_sum[2] = {1.0f/sum[0], 1.0f/sum[1]};

  // TODO: vectorize
  for (size_t i=0; i<tSize; ++i) {
    _units[i].t.weight[0] *= inv_sum[0];
    _units[i].t.weight[1] *= inv_sum[1];
  }
}

template <size_t tSize, typename T> template <typename T2>
void HoldingMap<tSize,T>::relabel(HoldingMap<tSize,T2>& map) const {
  for (size_t i=0; i<tSize; ++i) map.setHolding(i, _units[i].holding);
}

template <size_t tSize, typename T> template <typename T2>
void HoldingMap<tSize,T>::reorder(HoldingMap<tSize,T2>& map) const {
  T2 reverse_map[52][52];

  for (size_t i=0; i<tSize; ++i) {
    Holding holding = map.getHolding(i);
    const T2 t2 = map.getValue(i);
    reverse_map[holding.getCard1().code()][holding.getCard2().code()] = t2;
  }

  for (size_t i=0; i<tSize; ++i) {
    Holding holding = this->getHolding(i);
    map.setHolding(i, holding);
    map.setValue(i, reverse_map[holding.getCard1().code()][holding.getCard2().code()]);
  }
}

template <size_t tSize, typename T> template<size_t tSize2>
void HoldingMap<tSize,T>::remove(ps::CardSet cards, HoldingMap<tSize2,T>& map) const {
  size_t r=0;
  size_t w=0;

  for (; r<tSize; ++r) {
    const unit_t& unit = _units[r];
    bool intersects = unit.holding.intersects(cards);
    map.setHolding(w, unit.holding);
    map.setValue(w, unit.t);
    w += intersects ? 0 : 1;
  }

  assert(w==tSize2);
}
