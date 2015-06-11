// Included by SuitMask.h

uint16_t SuitMask::getHighMask(ps::Suit suit) {
  // TODO: code() is private so below won't compile. Either unprivatize or use friend.
  uint8_t code = suit.code();
  return HIGH_MASKS[code];
}

uint16_t SuitMask::getLowMask(ps::Suit suit) {
  // TODO: code() is private so below won't compile. Either unprivatize or use friend.
  uint8_t code = suit.code();
  return LOW_MASKS[code];
}

SuitMask SuitMask::removeHigh(ps::Suit suit) const {
  uint16_t mask = getHighMask(suit);
  return SuitMask(_mask & ~mask);
}

SuitMask SuitMask::removeLow(ps::Suit suit) const {
  uint16_t mask = getLowMask(suit);
  return SuitMask(_mask & ~mask);
}

