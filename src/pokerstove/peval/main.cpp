/*
 * ~/hunl/src/pokerstove/peval$ g++ -std=c++11 -I ../.. main.cpp
 */
#include "Card.h"
#include "CardSet.h"
#include "HoldemHandEvaluator.h"
#include "PokerEvaluation.h"
#include <algorithm>
#include <cstdio>
#include <vector>

namespace ps = pokerstove;

int main() {
  std::vector<ps::Card> deck;
  for (uint8_t i=0; i<52; ++i) {
    deck.push_back(Card(i));
  }

  ps::HoldemHandEvaluator heval;

  int N = 1000;
  for (int i=0; i<N; ++i) {
    std::random_shuffle(deck.begin(), deck.end());
    
    int j=0;

    ps::CardSet hand1;
    hand1.insert(deck[j++]);
    hand1.insert(deck[j++]);
    
    ps::CardSet hand2;
    hand2.insert(deck[j++]);
    hand2.insert(deck[j++]);

    ps::CardSet board;
    board.insert(deck[j++]);
    board.insert(deck[j++]);
    board.insert(deck[j++]);

    ps::PokerEvaluation eval1 = heval.evaluateHand(hand1, board).high();
    ps::PokerEvaluation eval2 = heval.evaluateHand(hand2, board).high();

    fprintf(stdout, "Test %d\n", i);
    fprintf(stdout, "  Board: %s\n", board.str().c_str());
    fprintf(stdout, "  Hand 1: %s %s %s\n", hand1.str().c_str(), eval1.str().c_str(), 
        eval1>=eval2 ? "(WINNER)" : "");
    fprintf(stdout, "  Hand 2: %s %s %s\n", hand2.str().c_str(), eval2.str().c_str(), 
        eval1<=eval2 ? "(WINNER)" : "");
  }
  return 0;
}

