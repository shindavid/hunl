#include "util/Arithmetic.h"

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
bool Holding::intersects(const ps::CardSet& cs) const { return _cards.intersects(cs); }
bool Holding::intersects(const Holding& h) const { return _cards.intersects(h._cards); }

bool Holding::operator==(const Holding& h) const { return _cards==h._cards; }

uint64_t Holding::hash() const {
  int n = _code1();
  int m = _code2();
  return m*(m-1)/2 + n;
}

std::string Holding::str() const {
  ps::Card c[2] = {getCard1(), getCard2()};

  int first = c[0].rank() > c[1].rank() ? 0 : 1;
  int second = 1-first;
  return c[first].str() + c[second].str();
}

CompactHolding::CompactHolding(uint16_t code) : _code(code) {}
 
CompactHolding::CompactHolding(Holding holding)
  : _code(ordered_ints::encode(holding.getCard2().code(), holding.getCard1().code())) {}
 
Holding CompactHolding::toHolding() const {
  std::pair<int,int> xy = ordered_ints::decode(_code);
  ps::CardSet cards;
  cards.insert(ps::Card(xy.first));
  cards.insert(ps::Card(xy.second));
  Holding h = Holding(cards);
  assert(*this == CompactHolding(h));
  return h;
}
 
uint16_t CompactHolding::code() const { return _code; }
bool CompactHolding::operator==(const CompactHolding& h) const { return _code==h._code; }
