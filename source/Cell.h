#pragma once
#include "Config.h"
#include "hardware/signalgp_utils.h"

struct Cell{
public:
  Cell(Config::inst_lib_t& il, Config::event_lib_t& el, emp::Random& random, Config::mutator_t& mut)
  : hardware(il, el, &random)
  , mutator(mut)
  {
    ConfigureHardware();  
  }

  void Restart(double &streakyFactor){
    hardware.ResetHardware();
    hardware.GetTrait().guess = -1;
    hardware.GetTrait().streakyFactor = streakyFactor;
    hardware.SpawnCore(0);
  }

  void Tick(){
    hardware.SingleProcess();
  }
    
  void PrintCurrentState(){
    hardware.PrintProgramFull();
    hardware.PrintState();
    std::cout <<"\n\n";
  }

  void SetProgram(Config::program_t program){
    hardware.SetProgram(program);
  }

  /*size_t DoMutations(emp::Random& random){
    if (random.GetDouble() < 0.5 ){
      std::cout<<"Doing Mutations"<<std::endl;
      return mutator.ApplyMutations(hardware.GetProgram(), random);   
    }std::cout <<"No Mutations\n";
    return 0;
  }*/

public:
    Config::hardware_t hardware;
    Config::mutator_t mutator;

private:
  void ConfigureHardware(){
    hardware.SetMinBindThresh(Config::HW_MIN_SIM_THRESH);
    hardware.SetMaxCores(Config::HW_MAX_THREADS);
    hardware.SetMaxCallDepth(Config::HW_MAX_CALL_DEPTH);

    // Create a way for the hardware to print our traits.
    auto trait_printer = [](std::ostream& os, Config::TRAIT_TYPE trait){
      os << "[SF: "<<trait.streakyFactor << " -- G: "<<trait.guess<<"]"<<std::endl;
    };
    hardware.SetTraitPrinter(trait_printer);
  }

};
