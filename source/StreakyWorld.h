#pragma once
#include "Config.h"
#include "tools/Random.h"
#include "Evolve/World.h"
#include "Cell.h"

class StreakyWorld: public emp::World<Cell>{
  public:
    StreakyWorld();
    void Restart();
    void Tick();
    std::vector<int> GetFitness();
    void ResetStreak(int streakType = -1);
    void PrintCurrentState();
  
  private:
    void ConfigureHardware();
    void BindRandomProgram();

  private:
    emp::Random random;
    Config::inst_lib_t inst_lib;
    int streakyFactor;
    int correctAnswer;
    std::vector<Cell> cells{};

};
