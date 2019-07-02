#include "data/DataNode.h"
#include "data/DataFile.h"
#include <string>

class Analytics{

public:
  Analytics(emp::vector<emp::DataMonitor<double>>& guessMonitors, emp::DataMonitor<double>& senseMonitor, emp::DataMonitor<double>& fitnessMonitor){
    dfile.SetupLine("",",","\n");
   
    dfile.AddMin(senseMonitor, "Minimum Sense Count");
    dfile.AddMax(senseMonitor, "Maximum Sense Count");
    dfile.AddMean(senseMonitor, "Mean Sense Count");

    dfile.AddMin(fitnessMonitor, "Minimum Fitness");
    dfile.AddMax(fitnessMonitor, "Maximum Fitness");
    dfile.AddMean(fitnessMonitor, "Mean Fitness");

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
  emp::DataFile dfile = emp::DataFile("results.csv");

};
