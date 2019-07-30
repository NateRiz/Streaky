#pragma once

#include "data/DataNode.h"
#include "Evolve/World.h"
#include "tools/Random.h"

#include "Analytics.h"
#include "Cell.h"
#include "Config.h"

template<typename CH>
class StreakyWorld: public emp::World<Cell<CH>> {
  public:

    StreakyWorld(const Config & cfg);
    void Restart();
    void Tick();
    void PrintCurrentState();
    void CreatePopulation(const unsigned int SAMPLES);
    void Start();
    double GetFitness(Cell<CH>& cell);

  private:
    void ConfigureHardware();
    void ConfigureWorld();
    void PrintBestCell();

  private:
    const Config & cfg;
    emp::Random random;
    typename CH::inst_lib_t inst_lib;
    typename CH::event_lib_t event_lib;
    typename CH::mutator_t mutator;

    emp::vector<emp::DataMonitor<double>> guessMonitors;
    emp::DataMonitor<double> senseMonitor;
    emp::DataMonitor<double> fitnessMonitor;
    Analytics analytics;

    int bestFitness = -10;


};
