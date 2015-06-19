/*
 * ~/hunl/src/launch$ g++ -std=c++11 -I .. main.cpp
 */

#include "engine/Session.h"
#include "strategy/RandomPlayer.h"

int main() {
  random_params_t params;
  params._check_probs[random_params::CHECK] = 0.5;
  params._check_probs[random_params::MIN_BET] = 0.05;
  params._check_probs[random_params::HALF_POT_SIZED_BET] = 0.3;
  params._check_probs[random_params::POT_SIZED_BET] = 0.12;
  params._check_probs[random_params::ALL_IN_BET] = 0.03;
  params._call_probs[random_params::FOLD] = 0.2;
  params._call_probs[random_params::CALL] = 0.6;
  params._call_probs[random_params::MIN_RAISE] = 0.1;
  params._call_probs[random_params::POT_SIZED_RAISE] = 0.07;
  params._call_probs[random_params::ALL_IN_RAISE] = 0.03;

  RandomPlayer david(params, "david");
  RandomPlayer bryan(params, "bryan");

  chip_amount_t stack_size = 1000;
  chip_amount_t small_blind_size = 10;
  chip_amount_t big_blind_size = 20;
  uint64_t seed = 100;
  Session session(david, bryan, stack_size, small_blind_size, big_blind_size, seed);

  int N = 100;
  for (int i=0; i<N; ++i) {
    session.playHand();
  }
}
