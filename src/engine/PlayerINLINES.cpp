player_id_t Player::__next_id = 0;

BlindPostDecision Player::handleRequest(const BlindPostRequest* request) {
  return BlindPostDecision(request->getAmount(), request->getBlindType());
}

