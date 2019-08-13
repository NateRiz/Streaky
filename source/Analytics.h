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
    , emp::DataMonitor<double>& funForkCountMonitor
    , emp::DataMonitor<double>& funTotalCallMonitor
  ) : dfile(
      emp::keyname::pack({
        {"treatment", cfg.TREATMENT()},
        {"seed", emp::to_string(cfg.SEED())},
        {"SEQ_A", emp::to_string(cfg.SEQ_A())},
        {"SEQ_B",emp::to_string(cfg.SEQ_B())},
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
    
    dfile.AddMin(funCallCountMonitor, "Minimum Function Calls");
    dfile.AddMax(funCallCountMonitor, "Maximum Function Calls");
    dfile.AddMean(funCallCountMonitor, "Mean Function Calls");
    
    dfile.AddMin(funForkCountMonitor, "Minimum Function Forks");
    dfile.AddMax(funForkCountMonitor, "Maximum Function Forks");
    dfile.AddMean(funForkCountMonitor, "Mean Function Forks");
 
    dfile.AddMin(funTotalCallMonitor, "Minimum Function Calls & Forks");
    dfile.AddMax(funTotalCallMonitor, "Maximum Function Calls & Forks");
    dfile.AddMean(funTotalCallMonitor, "Mean Function Calls & Forks");


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
