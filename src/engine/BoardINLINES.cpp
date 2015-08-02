void Board::add(ps::Card card) {
  _cards[getSize()] = card;
  _card_set.insert(card);
}

ps::CardSet Board::getCards() const { return _card_set; }
ps::Card Board::getCard(int i) const { return _cards[i]; }
unsigned Board::getSize() const { return _card_set.size(); }
bool Board::contains(ps::Card card) const { return _card_set.contains(card); }

bool Board::operator==(const Board& board) const {
  return !memcmp(this->_cards, board._cards, sizeof(_cards));
}

