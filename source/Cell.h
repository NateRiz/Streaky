#pragma once

#include "hardware/signalgp_utils.h"

#include "Config.h"
#include "Sequence.h"
#include "Trait.h"
#include <fstream>
#include <iostream>
#include <string>

template <typename CH>
struct Cell{
private:
  const Config & cfg;

public:
  Cell(
    const Config & cfg_,
    typename CH::inst_lib_t& il,
    typename CH::event_lib_t& el,
    emp::Random& random,
    typename CH::mutator_t& mut
  ) : cfg(cfg_)
  , rand(random)
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

  void SetProgram(typename CH::program_t program){
    hardware.SetProgram(program);
  }

  int EvalSequence(Sequence & seq, bool verbose) {
    Restart();
    hardware.GetTrait().seq = &seq;
    std::ofstream file;

    if (verbose){ 
      file.open("Verbose_" + std::to_string(seq.P()) + ".txt"); 
      hardware.PrintState(file);
      file << "===============================\n";
    }

    size_t cpu_cycles = cfg.TICKS_PER_TEST() + rand.GetInt(cfg.TICKS_NOISE()); //TODO
    emp::BitSet<16> affinity;
    const emp::vector<size_t> funcAffinities = {0, (size_t)pow(2, 16)-1}; // 000... || 111...

    for ( size_t t = 0; t < cpu_cycles; ++t){
      if(cfg.EVENT_DRIVEN()>0 && !(t % cfg.CYCLES_PER_EVENT())){
        affinity.SetUInt(0, funcAffinities[seq.Get(t / cfg.CYCLES_PER_EVENT())]);
        hardware.TriggerEvent("NextBit", affinity);
      }
      Tick();
      if (verbose){
        hardware.PrintState(file);
        file << "===============================\n";
      }
    }

    if (verbose){ file.close(); }

    const double idx_guess = hardware.GetTrait().guess;
    const double p_guess = (idx_guess == -1) ? idx_guess : cfg.SEQS(idx_guess);

    return (p_guess == -1 ? p_guess : p_guess == seq.P());
  }

  int EvalSequences(emp::vector<Sequence> & seqs, bool verbose) {

    int sum = 0;
    for(Sequence & seq : seqs) sum += EvalSequence(seq, verbose);
    return sum;

  }

  void CacheFitness(emp::vector<Sequence> & seqs, bool verbose = false) {
    int fit = EvalSequences(seqs, verbose);
    hardware.GetTrait().fitness = fit;
  
    //Prunes down the program to only the needed instructions
    /*
    if ( fit == (int) seqs.size() ) {
      hardware.GetTrait().fitness += (512.0 - (double)hardware.GetProgram().GetInstCnt()) / 100.0;
    }
    */
  }

public:
    emp::Random &rand;
    typename CH::hardware_t hardware;
    typename CH::mutator_t mutator;

private:
  // Create a way for the hardware to print our traits.
    std::function<void(std::ostream&, typename CH::TRAIT_TYPE)> trait_printer = [](
    std::ostream& os, typename CH::TRAIT_TYPE trait
  ){
    os << "[P: " << trait.seq->P()
       << " -- Guess: " << trait.guess << "]"
       << std::endl;
  };

  void ConfigureHardware(){
    hardware.SetMinBindThresh(cfg.HW_MIN_SIM_THRESH());
    hardware.SetMaxCores(cfg.HW_MAX_THREADS());
    hardware.SetMaxCallDepth(cfg.HW_MAX_CALL_DEPTH());
    hardware.SetTraitPrintFun(trait_printer);
  }

};
