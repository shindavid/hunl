
template<int N> EquityMatrix<N>::EquityMatrix() : _M(N, N), _W(N, 2), _P(N, 2) {}

template<int N> template<typename T>
void EquityMatrix<N>::compute(HoldingMap<N,T>& map) {
  /*
   * TODO(dshin) The order of for-loops here is optimized for cache-write access, but not for
   * cache-read access. There might be a better way to do this if we dig into the Eigen API.
   */
  for (int p=0; p<2; ++p) {
    for (int i=0; i<N; ++i) {
      _W(i,p) = map.getValue(i).weight[p];
    }
  }

  _P = _M * W;
  
  // See above TODO comment.
  for (int p=0; p<2; ++p) {
    for (int i=0; i<N; ++i) {
      map.getValue(i).equity[p] = _P(i,p);
    }
  }
}

