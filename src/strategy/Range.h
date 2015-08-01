#pragma once

#include "engine/Holding.h"

/*
 * A HoldingMap is a map with Holding's as keys. It does not support lookup's.
 */
template <size_t tSize, typename T> class HoldingMap {
private:
  struct unit_t {
    Holding holding;
    T t;
  };

  unit_t _units[tSize];

public:
  static const size_t sSize = tSize;

  HoldingMap() {}
  /*
   * Sets keys to be in same order as <map>. Values are not written.
   */
  template<typename T2> HoldingMap(const HoldingMap<tSize,T2>& map) { map.relabel(*this); }

  unit_t& get(int i) { return _units[i]; }
  const unit_t& get(int i) const { return _units[i]; }
  
  Holding getHolding(int i) const { return _units[i].holding; }
  void setHolding(int i, Holding h) { _units[i].holding = h; }
  
  const T& getValue(int i) const { return _units[i].t; }
  T& getValue(int i) { return _units[i].t; }
  void setValue(int i, const T& t) { _units[i].t = t; }
  
  template <class Compare> void sort(Compare comp) { std::sort(_units, &_units[tSize], comp); }

  /*
   * Rewrites <map> to have its Holding's in the same order as this. The T values of <map> are
   * set to garbage.
   */
  template<typename T2> void relabel(HoldingMap<tSize,T2>& map) const;

  /*
   * Rewrites <map> to have its Holding's in the same order as this. The T values of <map> are
   * preserved. 
   */
  template<typename T2> void reorder(HoldingMap<tSize,T2>& map) const;

  /*
   * Removes all Holding's from this that intersect <cards>, storing the result in <map>.
   * Preserves T values. It is the caller's responsibility to ensure that <map> is declared as the
   * right size.
   */
  template<size_t tSize2> void remove(ps::CardSet cards, HoldingMap<tSize2,T>& map) const;
};

template <typename T> using PreflopHoldingMap = HoldingMap<Holding::sNumHoldings, T>;
template <typename T> using FlopHoldingMap = HoldingMap<Holding::sNumFlopHoldings, T>;
template <typename T> using TurnHoldingMap = HoldingMap<Holding::sNumTurnHoldings, T>;
template <typename T> using RiverHoldingMap = HoldingMap<Holding::sNumRiverHoldings, T>;

struct JointWeights {
  float weight[2];
};
typedef PreflopHoldingMap<JointWeights> PreflopRange;
typedef FlopHoldingMap<JointWeights> FlopRange;
typedef TurnHoldingMap<JointWeights> TurnRange;
typedef RiverHoldingMap<JointWeights> RiverRange;

struct JointEquities {
  float equity[2];
};
typedef PreflopHoldingMap<JointEquities> PreflopEquities;
typedef FlopHoldingMap<JointEquities> FlopEquities;
typedef TurnHoldingMap<JointEquities> TurnEquities;
typedef RiverHoldingMap<JointEquities> RiverEquities;

typedef RiverHoldingMap<ps::PokerEvaluation> RiverEvals;

#include "strategy/RangeINLINES.cpp"

