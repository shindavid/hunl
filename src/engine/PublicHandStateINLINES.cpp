
PublicHandState::PublicHandState(session_id_t id, chip_amount_t starting_stack_size, seat_t button) :
  _id(id), _starting_stack_size(starting_stack_size), _button(button)
{
  for (seat_type_t seat=0; seat<2; ++seat) {
    _wagered_current_round[seat] = 0;
    _wagered_prior_rounds[seat] = 0;
    _folded[seat] = false;
  }
  _action_on = button;
  _is_current_betting_round_done = false;
}
