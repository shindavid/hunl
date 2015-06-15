
BlindPostEvent Player::handleRequest(const BlindPostRequest& request) {
  BlindPostEvent event(request.getPublicHandState(), _id, request.getAmount());
  return event;
}

