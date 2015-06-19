#pragma once

#include <algorithm>
#include <boost/array.hpp>
#include "pokerstove/peval/Card.h"

namespace ps = pokerstove;

/**
 * Adapted from ps::SimpleDeck
 */
class Deck
{
public:
    static const size_t DECK_SIZE = 52;

    /**
     * construct a deck that is in-order
     */
    Deck ()
    {
        for (uint8_t i=0; i<DECK_SIZE; i++)
        {
            _deck[i] = Card(i);
        }
        reset ();
    }

    /**
     * put all dealt cards back into deck, don't reorder
     */
    void reset ()
    {
        _current = DECK_SIZE;
    }

    /**
     * number of cards left in the deck
     */
    size_t size () const
    {
        return _current;
    }

    ps::Card deal ()
    {
        _current -= 1;
        ps::Card card = _deck[_current];
        return card;
    }

    void shuffle ()
    {
        std::random_shuffle(_deck.begin(), _deck.end());
        reset (); //_current = 0;
    }

private:
    // these are the data which track info about the deck
    boost::array<ps::Card,DECK_SIZE> _deck;
    size_t _current;
};

