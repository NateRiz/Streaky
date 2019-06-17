#pragma once
#include "Config.h"
#include "hardware/signalgp_utils.h"

struct Cell{
public:
  Cell(const Config::inst_lib_t& il, const Config::event_lib_t& el, emp::Random& random)
  : hardware(il, el, &random)
  , random(random)
  {
    inst_lib = il;
    event_lib = el;
    ConfigureHardware();  
  }

  void Restart(double streakyFactor){
    hardware.ResetHardware();
    hardware.GetTrait().guess = -1;
    hardware.GetTrait().streakyFactor = streakyFactor;
    BindRandomProgram();
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

public:
    Config::inst_lib_t inst_lib;
    Config::event_lib_t event_lib;
    Config::hardware_t hardware; 
    emp::Random random;

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

  void BindRandomProgram(){
    emp::EventDrivenGP_AW<Config::TAG_WIDTH, Config::TRAIT_TYPE>::Program program = GenRandSignalGPProgram(random, inst_lib); //Using all defaults.
    hardware.SetProgram(program);
    //hardware.PrintProgramFull();
  }

};
