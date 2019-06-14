#pragma once
#include "Config.h"
#include "tools/Random.h"

class StreakyWorld{
  public:
    StreakyWorld();
    void Start();
    void Tick();
    
  private:
    void ConfigureHardware();
    void BindRandomProgram();

  private:
    emp::Random random = emp::Random(1);
    Config::inst_lib_t inst_lib;
    Config::event_lib_t event_lib;
    Config::hardware_t hardware = Config::hardware_t(&inst_lib, &event_lib, &random); 

};
