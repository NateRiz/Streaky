#pragma once

#include "base/Ptr.h"
#include "Config.h"

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
    emp::vector<size_t> guessCount;
};
