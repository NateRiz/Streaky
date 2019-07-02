#pragma once

#include "hardware/signalgp_utils.h"

#include "Config.h"
#include "Sequence.h"
#include "Trait.h"

struct Cell{
public:
  Cell(Config::inst_lib_t& il, Config::event_lib_t& el, emp::Random& random, Config::mutator_t& mut)
  : rand(random)
  , hardware(il, el, &random)
  , mutator(mut)
  {
    ConfigureHardware();
  }

  void Restart(){
    hardware.ResetHardware();
    hardware.GetTrait().guess = -1;
    hardware.GetTrait().seq = nullptr;
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

  int EvalSequence(Sequence & seq) {
    Restart();
    hardware.GetTrait().seq = &seq;

    for (
      size_t t = 0;
      t < Config::TICKS + rand.GetInt(Config::TICKS_NOISE);
      ++t
    ) Tick();

    const double idx_guess = hardware.GetTrait().guess;
    const double p_guess = (idx_guess == -1) ? idx_guess : Config::SEQS[idx_guess];

    return (p_guess == -1 ? p_guess : p_guess == seq.P());
  }

  int EvalSequences(emp::vector<Sequence> & seqs) {

    int sum = 0;
    for(Sequence & seq : seqs) sum += EvalSequence(seq);
    return sum;

  }

  void CacheFitness(emp::vector<Sequence> & seqs) {
    int fit = EvalSequences(seqs);
    if ( fit == (int) seqs.size() ) {
      fit += 10000 - hardware.GetConstProgram().GetInstCnt();
    }
    hardware.GetTrait().fitness = fit;
  }

public:
    emp::Random &rand;
    Config::hardware_t hardware;
    Config::mutator_t mutator;

private:
  void ConfigureHardware(){
    hardware.SetMinBindThresh(Config::HW_MIN_SIM_THRESH);
    hardware.SetMaxCores(Config::HW_MAX_THREADS);
    hardware.SetMaxCallDepth(Config::HW_MAX_CALL_DEPTH);

    // Create a way for the hardware to print our traits.
    auto trait_printer = [](std::ostream& os, Config::TRAIT_TYPE trait){
      os << "[SF: " << trait.seq->P()
         << " -- G: " << trait.guess << "]"
         << std::endl;
    };
    hardware.SetTraitPrinter(trait_printer);
  }

};
