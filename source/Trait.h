#pragma once

#include <unordered_map>

#include "base/Ptr.h"

// forward declaration
class Sequence;

struct Trait{
public:

    Trait();
    ~Trait()=default;

    int guess;
    /// A cell's fitness is given by 1 point per test case they get right.
    /// If a cell doesn't guess they are deducted a point.
    /// Upon completion of all tests, gains reward inverse to program length
    double fitness; 
    /// Current test being run on the owning cell.
    emp::Ptr<Sequence> seq;
    
    //Analytics
    size_t senseCount;
    std::unordered_map<size_t, size_t> guessCount;
    size_t funCallCount;
    size_t funForkCount;
};
