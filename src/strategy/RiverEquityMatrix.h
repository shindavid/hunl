#pragma once

#include "engine/Holding.h"
#include "util/BitMatrix.h"
#include "strategy/Range.h"

/*
 * The equity matrix on the river can be stored much more compactly than for prior streets, since the
 * associated matrix only has 0/1 entries. Sort of.
 */
class RiverEquityMatrix {
public:
  static const int sN = Holding::sNumRiverHoldings;
  typedef BitMatrix<sN,sN> BitMatrix_t;
private:
  BitMatrix_t _wins;
  BitMatrix_t _ties;
  BitMatrix_t _denoms;

public:
  void set_win_block(int x, int yb, int64_t bits);
  void set_tie_block(int x, int yb, int64_t bits);
  void set_denom_block(int x, int yb, int64_t bits);

  void compute_equities(const HoldingMap<float>& range, HoldingMap<float>& equities);
};

#include "strategy/RiverEquityMatrixINLINES.cpp"

