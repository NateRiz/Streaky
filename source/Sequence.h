#pragma once

#include "base/vector.h"
#include "tools/Random.h"
#include "tools/BitSet.h"

#include "Config.h"

class Sequence {

private:

  const Config &cfg;

  emp::vector<int> seq;
  emp::Random &rand;

  const double p_switch;

public:

  Sequence(const Config & cfg_, emp::Random &rand_, const double p_switch_)
  : cfg(cfg_)
  , rand(rand_)
  , p_switch(p_switch_)
  {
    seq.reserve(cfg.TICKS_PER_TEST() + cfg.TICKS_NOISE());
    seq.push_back(rand.P(0.5));
  };

  int Get(const size_t i) {
    while (Size() <= i) Extend();
    return seq[i];
  }

  size_t Size() const { return seq.size(); }

  void Extend() { seq.push_back(seq.back() ^ rand.P(p_switch)); }

  double P() const { return p_switch <= 0.5001 ? 0 : 1; };



};
