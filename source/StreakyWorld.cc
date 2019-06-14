#pragma once
#include "StreakyWorld.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "InstructionLibrary.h"

StreakyWorld::StreakyWorld(){
  InstructionLibrary il;
  inst_lib = il.CreateInstLib(hardware, random);
  ConfigureHardware();
  BindRandomProgram();
}

void StreakyWorld::ConfigureHardware(){
  hardware.SetMinBindThresh(Config::HW_MIN_SIM_THRESH);
  hardware.SetMaxCores(Config::HW_MAX_THREADS);
  hardware.SetMaxCallDepth(Config::HW_MAX_CALL_DEPTH);
 
  // Create a way for the hardware to print our traits.
  auto trait_printer = [](std::ostream& os, Config::TRAIT_TYPE trait){
    os << "[SF: "<<trait.streakyFactor << "G: "<<trait.guess<<"]"<<std::endl; 
  };
  hardware.SetTraitPrinter(trait_printer);

  double streakyFactor = 0.5;
  if (random.GetInt(2)){
    streakyFactor+=random.GetDouble(0.1, 0.4);
  }
  hardware.GetTrait().streakyFactor = streakyFactor;
}

void StreakyWorld::BindRandomProgram(){
  emp::EventDrivenGP_AW<Config::TAG_WIDTH, Config::TRAIT_TYPE>::Program program = GenRandSignalGPProgram(random, inst_lib); //Using all defaults.
  hardware.SetProgram(program);
  //hardware.PrintProgramFull();
}

void StreakyWorld::Start(){
  hardware.ResetHardware();
  hardware.SpawnCore(0);
}

void StreakyWorld::Tick(){
  hardware.SingleProcess();
  hardware.PrintState();
  std::cout<<"\n\n";
}
