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
    double fitness;
    emp::Ptr<Sequence> seq;

    //Analytics
    size_t senseCount;
    std::unordered_map<size_t, size_t> guessCount;

};
