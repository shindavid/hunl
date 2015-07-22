int Holding::_code1() const { return __builtin_ffsll(_cards.mask()) - 1; }
int Holding::_code2() const { return 63 - __builtin_clzll(_cards.mask()); }

Holding::Holding(ps::CardSet cards) : _cards(cards) {
  assert(_cards.size() == 2);
  assert(_code1() < _code2());
  assert(0 <= _code1());
  assert(cards.contains(getCard1()));
  assert(cards.contains(getCard2()));
}

ps::CardSet Holding::getCardSet() const { return _cards; }
ps::Card Holding::getCard1() const { return ps::Card(_code1()); }
ps::Card Holding::getCard2() const { return ps::Card(_code2()); }
bool Holding::contains(ps::Card c) const { return _cards.contains(c); }
bool Holding::intersects(const Holding& h) const { return _cards.intersects(h._cards); }

bool Holding::operator==(const Holding& h) const { return _cards==h._cards; }

uint64_t Holding::hash() const {
  int n = _code1();
  int m = _code2();
  return m*(m-1)/2 + n;
}
