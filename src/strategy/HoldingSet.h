#pragma once

#include "pokerstove/peval/Card.h"
#include "pokerstove/peval/Rank.h"
#include "pokerstove/peval/Suit.h"
#include <assert.h>

namespace ps = pokerstove;

/*
 * A HoldingSet represents a set of card pairs. The ranks of the cards in each pair will be the same,
 * but the suits can differ. For example, a HoldingSet can include AhKc and AsKs, but that same
 * HoldingSet cannot also include AhAs.
 *
 * Note then that for pocket-pair ranks, there are up to (4 choose 2) = 6 possible holdings
 * represented by a HoldingSet, while for a non pocket-pair, there are up to 4*4 = 16 possible
 * holdings represented by a HoldingSet. 
 *
 * This class is intended to be used in conjunction with HoldingSetDistribution, in order to 
 * represent a probability distribution over holdings. 
 */
class __attribute__ ((__packed__)) HoldingSet {
private:
  struct __attribute__ ((__packed__)) data_t {
    /*
     * The i'th lowest bit of this mask represents the holding with suit (i%4) on the high rank
     * and suit (i/4) on the low rank.
     *
     * For pocket pairs (where _high_rank == _low_rank), only bits where (i%4) > (i/4) will be valid.
     */
    const uint16_t _suit_mask;

    // If a pocket-pair, _high_rank == _low_rank
    // TODO: pack pokerstove::Rank to fit into 4 bits
    const ps::Rank _high_rank : 4;
    const ps::Rank _low_rank : 4;
    
    data_t(uint16_t suit_mask, ps::Rank high_rank, ps::Rank low_rank)
      : _suit_mask(suit_mask), _high_rank(high_rank), _low_rank(low_rank) {}
  };

  union {
    const data_t _data;
    const uint32_t _hash;
  }; 
  
  uint32_t hash() const { return _hash; }

  friend struct HoldingSetHashFunction;

public:
  static const uint16_t SUITED_MASK = 0x8421;  // 2^0 + 2^5 + 2^10 + 2^15
  static const uint16_t UNSUITED_MASK = 0x7bde;  // 0xffff - SUITED_MASK
  static const uint16_t POCKET_PAIR_MASK = 0x8ce;  // 2^1 + 2^2 + 2^3 + 2^6 + 2^7 + 2^11

  static const uint16_t HIGH_MASKS[4] = {
    0x1111,  // CLUBS,    2^0 + 2^4 + 2^8 + 2^12
    0x2222,  // DIAMONDS, 2^1 + 2^5 + 2^9 + 2^13
    0x3333,  // HEARTS,   2^2 + 2^6 + 2^10 + 2^14
    0x4444   // SPADES,   2^3 + 2^7 + 2^11 + 2^15
  };
  
  static const uint16_t LOW_MASKS[4] = {
    0xf,    // CLUBS,    2^0 + 2^1 + 2^2 + 2^3
    0xf0,   // DIAMONDS, 2^4 + 2^5 + 2^6 + 2^7
    0xf00,  // HEARTS,   2^8 + 2^9 + 2^10 + 2^11
    0xf000  // SPADES,   2^12 + 2^13 + 2^14 + 2^15
  };

  static uint16_t getHighMask(ps::Suit suit) {
    // TODO: code() is private so below won't compile. Either unprivatize or use friend.
    uint8_t code = suit.code();
    return HIGH_MASKS[code];
  }

  static uint16_t getLowMask(ps::Suit suit) {
    // TODO: code() is private so below won't compile. Either unprivatize or use friend.
    uint8_t code = suit.code();
    return LOW_MASKS[code];
  }

public:
  /*
   * If pocket-pair, high_rank == low_rank
   */
  HoldingSet(uint16_t suit_mask, ps::Rank high_rank, ps::Rank low_rank)
    : _data(suit_mask, high_rank, low_rank)
  {
    assert(high_rank>=low_rank);
    assert((high_rank!=low_rank) || !(suit_mask & ~POCKET_PAIR_MASK));
  }

  bool operator=(const HoldingSet& hset) const {
    return this->_hash == hset._hash;
  }

  int getSize() const { return __builtin_popcount(_data._suit_mask); }
  bool empty() const { return !_data._suit_mask; }

  /*
   * Remove all combinations of this that include the given card.
   *
   * Branchless operation.
   */
  HoldingSet remove(ps::Card card) const {
    ps::Rank rank = card.rank();
    uint16_t high_mask = getHighMask(rank);
    uint16_t low_mask = getLowMask(rank);

    // should be branchless ternary operators
    high_mask = (_data._high_rank == rank) ? high_mask : 0;
    low_mask = (_data._low_rank == rank) ? low_mask : 0;

    uint16_t suit_mask = _suit_mask & ~high_mask & ~low_mask;
    return HoldingSet(suit_mask, _high_rank, _low_rank);
  }
};
static_assert(sizeof(HoldingSet)<=4);

struct HoldingSetHashFunction {
  uint64_t operator()(const HoldingSet& set) const {
    return set.hash();
  }
};

