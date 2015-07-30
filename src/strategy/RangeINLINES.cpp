
CompactHolding::CompactHolding(uint16_t code) : _code(code) {}

CompactHolding::CompactHolding(Holding holding)
  : _code(ordered_ints::encode(holding.getCard2().code(), holding.getCard1().code())) {}

Holding CompactHolding::toHolding() const {
  std::pair<int,int> xy = ordered_ints::decode(_code);
  ps::CardSet cards;
  cards.insert(ps::Card(xy.first));
  cards.insert(ps::Card(xy.second));
  return Holding(cards);
}

uint16_t CompactHolding::code() const { return _code; }
