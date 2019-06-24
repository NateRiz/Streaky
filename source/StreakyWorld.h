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

    int bestFitness = -10;


};
