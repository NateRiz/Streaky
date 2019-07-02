#pragma once
#include "Trait.h"
#include "Config.h"

Trait::Trait() : guess(-1), fitness(0), senseCount(0), guessCount(emp::vector<size_t>(Config::SEQS.size(), 0)){;}
