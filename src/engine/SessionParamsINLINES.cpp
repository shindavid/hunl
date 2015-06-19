#include <assert.h>

SessionParams::SessionParams(session_id_t id, Player* p0, Player* p1,
    chip_amount_t starting_stack_size, chip_amount_t small_blind_size, chip_amount_t big_blind_size) :
  _id(id),
  _starting_stack_size(starting_stack_size),
  _small_blind_size(small_blind_size),
  _big_blind_size(big_blind_size)
{
  _players[0] = p0;
  _players[1] = p1;
  assert(_small_blind_size <= _big_blind_size);
  assert(_big_blind_size <= _starting_stack_size);
}

