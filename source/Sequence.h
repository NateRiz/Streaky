#pragma once

#include "base/vector.h"
#include "tools/Random.h"
#include "tools/BitSet.h"

#include "Config.h"

class Sequence {

private:

  emp::vector<int> seq;
  emp::Random rand;

  const double p_switch;

public:

  Sequence(const size_t seed, const double p_switch_)
  : rand(seed)
  , p_switch(p_switch_)
  {
    emp_assert(seed > 0);
    seq.reserve(Config::TICKS + Config::TICKS_NOISE);
    seq.push_back(rand.P(0.5));
  };

  int Get(const size_t i) {
    while (Size() <= i) Extend();
    return seq[i];
  }

  size_t Size() const { return seq.size(); }

  void Extend() { seq.push_back(seq.back() ^ rand.P(p_switch)); }

  double P() const { return p_switch; };



};
