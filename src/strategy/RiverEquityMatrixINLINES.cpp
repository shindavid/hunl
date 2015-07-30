
void RiverEquityMatrix::set_win_block(int x, int yb, int64_t bits) {
  _wins.set_block(x, yb, bits);
}

void RiverEquityMatrix::set_tie_block(int x, int yb, int64_t bits) {
  _ties.set_block(x, yb, bits);
}

void RiverEquityMatrix::set_denom_block(int x, int yb, int64_t bits) {
  _denoms.set_block(x, yb, bits);
}

void RiverEquityMatrix::compute_equities(const HoldingMap<float>& range, HoldingMap<float>& equities) {
  assert(sN==range.size());
  assert(sN==equities.size());

  HoldingMap<float> win_prod(sN);
  HoldingMap<float> tie_prod(sN);
  HoldingMap<float> denom_prod(sN);

  _wins.mult(range.getArray(), win_prod.getArray());
  _ties.mult(range.getArray(), tie_prod.getArray());
  _denoms.mult(range.getArray(), denom_prod.getArray());

  // TODO(dshin) use vectorized operations to do below:
  for (int i=0; i<sN; ++i) {
    equities[i] = (win_prod[i] + 0.5*tie_prod[i]) / denom_prod[i];
  }
}
