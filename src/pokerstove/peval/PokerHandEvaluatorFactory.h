#pragma once

#include "PokerHandEvaluator.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace pokerstove
{
class PokerHandEvaluatorFactory {
public:
  /**
   * Construction of PokerHandEvaluator subclasses must be done through this Factory.
   * The subclass is identified by the first letter of the input
   * string according to the table below
   *
   * supported games:
   * - 'h'    hold'em (or high if no board)
   * - 'k'    Kansas City lowball (2-7)
   * - 'l'    lowball (A-5)
   * - '3'    three card poker
   * - 'O'    omaha high
   * - 'r'    razz
   * - 's'    stud
   * - 'q'    stud high/low no qualifier
   * - 'd'    draw high
   * - 't'    triple draw lowball (2-7)
   * - 'T'    triple draw lowball (A-5)
   * - 'o'    omaha/high low
   * - 'e'    stud/8
   * - 'b'    badugi
   */
  typedef boost::shared_ptr<PokerHandEvaluator> eval_ptr;
  static eval_ptr alloc(const std::string& strid);
};
}

#include "PokerHandEvaluatorINLINES.cpp"

