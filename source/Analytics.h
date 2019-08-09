#include <string>

#include "data/DataNode.h"
#include "data/DataFile.h"
#include "tools/keyname_utils.h"

#include "Config.h"

class Analytics{

public:
  Analytics(
    const Config & cfg
    , emp::vector<emp::DataMonitor<double>>& guessMonitors
    , emp::DataMonitor<double>& senseMonitor
    , emp::DataMonitor<double>& fitnessMonitor
    , emp::DataMonitor<double>& funCallCountMonitor
  ) : dfile(
      emp::keyname::pack({
        {"treatment", cfg.TREATMENT()},
        {"seed", emp::to_string(cfg.SEED())},
        // {"_emp_hash=", STRINGIFY(EMPIRICAL_HASH_)},
        // {"_source_hash=", STRINGIFY(DISHTINY_HASH_)},
        {"ext", ".csv"}
      })
    )
  {
    dfile.SetupLine("",",","\n"); //TODO

    dfile.AddMin(senseMonitor, "Minimum Sense Count");
    dfile.AddMax(senseMonitor, "Maximum Sense Count");
    dfile.AddMean(senseMonitor, "Mean Sense Count");

    dfile.AddMin(fitnessMonitor, "Minimum Fitness");
    dfile.AddMax(fitnessMonitor, "Maximum Fitness");
    dfile.AddMean(fitnessMonitor, "Mean Fitness");
    
    dfile.AddMin(funCallCountMonitor, "Min Fx Calls");
    dfile.AddMax(funCallCountMonitor, "Max Fx Calls");
    dfile.AddMean(funCallCountMonitor, "Mean Fx Calls");

    for (size_t i = 0; i < guessMonitors.size(); ++i){
      dfile.AddMin(guessMonitors[i], "Minimum Guess"+std::to_string(i)+" Count");
      dfile.AddMax(guessMonitors[i], "Maximum Guess"+std::to_string(i)+" Count" );
      dfile.AddMean(guessMonitors[i], "Mean Guess"+std::to_string(i)+" Count");
    }
    dfile.PrintHeaderKeys();

  }
  Analytics()=default;
  ~Analytics()=default;

  void Update(){
    dfile.Update();
  }

private:
  emp::DataFile dfile;

};
