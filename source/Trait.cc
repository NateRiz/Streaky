#pragma once

#include "Trait.h"
#include "Config.h"

Trait::Trait()
  : guess(-1)
  , fitness(0)
  , funCallCount(0)
  , funForkCount(0)
  , seq(nullptr)
  , senseCount(0)
  { ; }
