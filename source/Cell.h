#pragma once

#include "hardware/signalgp_utils.h"

#include "Config.h"
#include "Sequence.h"
#include "Trait.h"

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

  int EvalSequence(Sequence & seq) {
    Restart();
    hardware.GetTrait().seq = &seq;

    for (
      size_t t = 0;
      t < cfg.TICKS_PER_TEST() + rand.GetInt(cfg.TICKS_NOISE()); //TODO
      ++t
    ) Tick();

    const double idx_guess = hardware.GetTrait().guess;
    const double p_guess = (idx_guess == -1) ? idx_guess : cfg.SEQS(idx_guess);

    return (p_guess == -1 ? p_guess : p_guess == seq.P());
  }

  int EvalSequences(emp::vector<Sequence> & seqs) {

    int sum = 0;
    for(Sequence & seq : seqs) sum += EvalSequence(seq);
    return sum;

  }

  void CacheFitness(emp::vector<Sequence> & seqs) {
    int fit = EvalSequences(seqs);
    hardware.GetTrait().fitness = fit;

    if ( fit == (int) seqs.size() ) {
      hardware.GetTrait().fitness += (512.0 - (double)hardware.GetProgram().GetInstCnt()) / 100.0;
    }
  }

public:
    emp::Random &rand;
    typename CH::hardware_t hardware;
    typename CH::mutator_t mutator;

private:
  void ConfigureHardware(){
    hardware.SetMinBindThresh(cfg.HW_MIN_SIM_THRESH());
    hardware.SetMaxCores(cfg.HW_MAX_THREADS());
    hardware.SetMaxCallDepth(cfg.HW_MAX_CALL_DEPTH());

    // Create a way for the hardware to print our traits.
    auto trait_printer = [](
      std::ostream& os, typename CH::TRAIT_TYPE trait
    ){
      os << "[SF: " << trait.seq->P()
         << " -- G: " << trait.guess << "]"
         << std::endl;
    };
    hardware.SetTraitPrintFun(trait_printer);
  }

};
