#pragma once

#include "base/Ptr.h"

// forward declaration
class Sequence;

struct Trait{

  public:

    Trait()
    : guess(-1)
    , fitness(0)
    , sense_idx(0)
    { ;}

    int guess;
    double fitness;
    emp::Ptr<Sequence> seq;
    size_t sense_idx;

};
