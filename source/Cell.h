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

  /// Reset a cell for the beginning of another test. (Not fitness)
  /// Spawns a core in the hardware if this isn't an event driven test.
  void Restart(){
    hardware.ResetHardware();
    hardware.GetTrait().guess = -1;
    hardware.GetTrait().seq = nullptr;
    if (cfg.EVENT_DRIVEN() == 0){
      hardware.SpawnCore(0);
    }
  }


  /// Run a single CPU intruction in the hardware on each core.
  void Tick(){
    hardware.SingleProcess();
  }

  /// Decays once every event to ensure set/adj_regs instructions are being used.                                         
  void DecayRegulators() {
    if(cfg.DECAY_TEST() == "COUNTDOWN"){
      for (const auto & uid : hardware.GetMatchBin().ViewUIDs()) {
        if (hardware.GetMatchBin().GetVal(uid)) {
          --hardware.GetMatchBin().GetVal(uid);
        } else {
          hardware.GetMatchBin().SetRegulator(uid, 1.0);
        }
      }
    }
    else if(cfg.DECAY_TEST() == "FRACTION"){
      const double decayRate = 0.2;
      for (const auto & uid : hardware.GetMatchBin().ViewUIDs()){
        double newRegulator = hardware.GetMatchBin().ViewRegulator(uid);
        newRegulator -= ((hardware.GetMatchBin().ViewRegulator(uid) - 1.0) * decayRate);
        hardware.GetMatchBin().SetRegulator(uid, newRegulator);
      }
    }
  }
  

  void SetProgram(typename CH::program_t program){
    hardware.SetProgram(program);
  }


  int EvalSequence(Sequence & seq, bool verbose) {
    Restart();
    hardware.GetTrait().seq = &seq;
    std::ofstream file;

    emp::BitSet<16> affinity;
    const emp::vector<size_t> funcAffinities = {0, (size_t)pow(2, 16)-1}; // 000... || 111...

    if (verbose){
      file.open("Verbose_" + std::to_string(seq.P()) + ".txt");
      if (cfg.EVENT_DRIVEN()){
        for(size_t j=0; j<2; ++j){
          std::unordered_map<int, double> funCounter;
          affinity.SetUInt(0, funcAffinities[j]);
          for (size_t i = 0; i < 100; ++i){
            emp::vector<size_t> match = hardware.FindBestFuncMatch(affinity, 1, 0.5);
            if(match.size()>0){
              if(funCounter.find(match[0]) == funCounter.end()){
                funCounter[match[0]]=0;
              }
              ++funCounter[match[0]];
            }
          }
          emp::vector<std::pair<double, size_t>>bestMatches;
          for(auto &[id, count] : funCounter){bestMatches.emplace_back(count, id);}
          std::sort(bestMatches.begin(), bestMatches.end());
          std::reverse(bestMatches.begin(), bestMatches.end());
          file << "Event [" << affinity << "]:\n";
          for(auto &[prob, id] : bestMatches){file << "      Fn-"<< id << ": " << prob <<"%\n";}
        }
      }
      file <<"Tick: -1"<<"\n";
      hardware.PrintState(file);
      file << "===============================\n";
    }

    size_t cpu_cycles = cfg.TICKS_PER_TEST() + rand.GetInt(cfg.TICKS_NOISE()); //TODO


    for ( size_t t = 0; t < cpu_cycles; ++t){
      if(cfg.EVENT_DRIVEN()>0 && (t % cfg.CYCLES_PER_EVENT() == 0)){
        affinity.SetUInt(0, funcAffinities[seq.Get(t / cfg.CYCLES_PER_EVENT())]);
        hardware.TriggerEvent("NextBit", affinity);
      }
      Tick();
      if (
        cfg.DECAY_TEST() != "NO_DECAY"
        && cfg.EVENT_DRIVEN()
        && t % cfg.CYCLES_PER_EVENT() == 0
      ){
        DecayRegulators();
      }
      if (verbose){
        file <<"Tick: "<<t<<"\n";
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
    if ( fit == (int) seqs.size() ) {
      hardware.GetTrait().fitness += (512.0 - (double)hardware.GetProgram().GetInstCnt()) / 100.0;
      for(unsigned int i = 0; i < hardware.GetProgram().GetSize(); ++i){
        for(unsigned int j = 0; j < hardware.GetProgram()[i].GetSize(); ++j){
          if(hardware.GetProgram().GetInstLib()->GetName(hardware.GetProgram()[i][j].id) == "Nop"){
           hardware.GetTrait().fitness += 0.0001;
          }
       }
      }
    }
    

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
