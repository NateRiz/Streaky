#pragma once

#include "data/DataNode.h"
#include "Evolve/World.h"
#include "tools/Random.h"

#include "Analytics.h"
#include "Cell.h"
#include "Config.h"
#include "EventLibrary.h"
template<typename CH>
class StreakyWorld: public emp::World<Cell<CH>> {
  public:

    StreakyWorld(const Config & cfg);
    void Restart();
    void Tick();
    void CreatePopulation(const unsigned int SAMPLES);
    void Start();
    double GetFitness(Cell<CH>& cell);

  private:
    void ConfigureHardware();
    void ConfigureWorld();
    void PrintBestCell();
    void TestCellVerbose();

  private:
    const Config & cfg;
    emp::Random random;
    typename CH::inst_lib_t inst_lib;
    typename CH::event_lib_t event_lib;
    typename CH::mutator_t mutator;

    emp::vector<emp::DataMonitor<double, emp::data::Log>> guessMonitors;
    emp::DataMonitor<double, emp::data::Log> senseMonitor;
    emp::DataMonitor<double, emp::data::Log> fitnessMonitor;
    emp::DataMonitor<double, emp::data::Log> sizeMonitor;
    emp::DataMonitor<double, emp::data::Log> funCallCountMonitor; 
    emp::DataMonitor<double, emp::data::Log> funForkCountMonitor;
    emp::DataMonitor<double, emp::data::Log> funTotalCallMonitor;
    Analytics analytics;
    
    double bestFitness;
    Cell<CH>* bestCell;

};
