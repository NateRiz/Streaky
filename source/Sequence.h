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
    //Reserve space equal to the max amount of senses
    seq.reserve(cfg.TICKS_PER_TEST() + cfg.TICKS_NOISE());
    //start with a random first bit 50% chance each.
    seq.push_back(rand.P(0.5)); 
  };

  int Get(const size_t i) {
    while (Size() <= i) Extend();
    return seq[i];
  }

  size_t Size() const { return seq.size(); }

  /// Easy Problem: p_switch is the odds that the next bit will be a 1. Else 0
  /// Hard Problem: p_switch is the odds that the next bit 
  ///  will be the same as the last bit.
  void Extend() {
    if (cfg.EASY_PROBLEM()) {
      seq.push_back(rand.P(p_switch));
    } else {
      seq.push_back(seq.back() ^ rand.P(p_switch));
    }
  }

  double P() const { return p_switch; }

};
