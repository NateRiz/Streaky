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
    void ResetStreak(int streakType = -1);
    void PrintCurrentState();
    void CreatePopulation(const unsigned int SAMPLES);
    void Start();
    double GetFitness(Cell& cell);

  private:
    void ConfigureHardware();
    void PrintBestCell();

  private:
    emp::Random random;
    Config::inst_lib_t inst_lib;
    Config::event_lib_t event_lib;
    Config::mutator_t mutator;
    double streakyFactor;
    int correctAnswer;
    int bestFitness = -10;
    const unsigned int TICKS = 100;
    const unsigned int TESTS = 100;


};
