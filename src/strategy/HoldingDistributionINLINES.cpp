// Included by HoldingDistribution.h

HoldingDistribution::HoldingDistribution() {
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
  _total_weight = 52*51/2;  // 52 choose 2
}

