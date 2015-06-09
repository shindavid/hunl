// Included by HoldingSet.h

uint16_t HoldingSet::getHighMask(ps::Suit suit) {
  // TODO: code() is private so below won't compile. Either unprivatize or use friend.
  uint8_t code = suit.code();
  return HIGH_MASKS[code];
}

uint16_t HoldingSet::getLowMask(ps::Suit suit) {
  // TODO: code() is private so below won't compile. Either unprivatize or use friend.
  uint8_t code = suit.code();
  return LOW_MASKS[code];
}

HoldingSet::HoldingSet(uint16_t suit_mask, ps::Rank high_rank, ps::Rank low_rank)
  : _data(suit_mask, high_rank, low_rank)
{
  assert(high_rank>=low_rank);
  assert((high_rank!=low_rank) || !(suit_mask & ~POCKET_PAIR_MASK));
}

HoldingSet HoldingSet::remove(ps::Card card) const {
  ps::Rank rank = card.rank();
  uint16_t high_mask = getHighMask(rank);
  uint16_t low_mask = getLowMask(rank);

  // should be branchless ternary operators
  high_mask = (_data._high_rank == rank) ? high_mask : 0;
  low_mask = (_data._low_rank == rank) ? low_mask : 0;

  uint16_t suit_mask = _suit_mask & ~high_mask & ~low_mask;
  return HoldingSet(suit_mask, _high_rank, _low_rank);
}
