#include <assert.h>

SessionState::SessionState(session_id_t id, Player* p0, Player* p1, chip_amount_t stack_size,
    chip_amount_t small_blind_size, chip_amount_t big_blind_size, uint64_t seed)
{
  _id = __next_id++;
  _players[0] = p0;
  _players[1] = p1;

  _stack_size = stack_size;
  _small_blind_size = small_blind_size;
  _big_blind_size = big_blind_size;

  assert(_small_blind_size <= _big_blind_size);
  assert(_big_blind_size <= _stack_size);

  // srand() API is weird, passing in 1 gives you a weird error
  assert(seed>1);
  _base_seed = seed;
  srand(seed);
  _button = rand() % 2;
}

