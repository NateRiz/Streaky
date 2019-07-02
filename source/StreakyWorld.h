#pragma once
#include "Config.h"
#include "tools/Random.h"
#include "Evolve/World.h"
#include "Cell.h"
#include "Analytics.h"
#include "data/DataNode.h"

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
    void ConfigureWorld();
    void PrintBestCell();

  private:
    emp::Random random;
    Config::inst_lib_t inst_lib;
    Config::event_lib_t event_lib;
    Config::mutator_t mutator;
  
    emp::vector<emp::DataMonitor<double>> guessMonitors = emp::vector<emp::DataMonitor<double>>(Config::SEQS.size());
    emp::DataMonitor<double> senseMonitor;
    emp::DataMonitor<double> fitnessMonitor;
    Analytics analytics;

    int bestFitness = -10;


};
