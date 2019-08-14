//  This file is part of Streaky
//  Copyright (C) Nathan Rizik, 2019.
//  Released under GNU General Public License v3; see LICENSE

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "config/command_line.h"
#include "tools/MatchBin.h"
#include "config/ArgManager.h"

#include "../Config.h"
#include "../ConfigHardware.h"
#include "../StreakyWorld.cc"
#include "../Trait.cc"

// This is the main function for the NATIVE version of Streaky.
int main(int argc, char* argv[])
{

  // Read configs.
  Config cfg;
  cfg.Read(cfg.CONFIG_FILENAME());

  emp::ArgManager am(
    argc,
    argv,
    emp::ArgManager::make_builtin_specs(&cfg)
  );

  if (!am.ProcessBuiltin(&cfg)) return 1;

  std::cout << "==============================" << std::endl;
  std::cout << "|    How am I configured?    |" << std::endl;
  std::cout << "==============================" << std::endl;
  cfg.Write(std::cout);
  std::cout << "==============================\n"
           << std::endl;

  auto res = am.UseArg("_positional");

  if (!res) {
    std::cout << "no run type provided" << std::endl;
    std::exit(1);
  }

  // need to reverse to read left to right while popping off back
  std::reverse(res->begin(), res->end());

  while (res->size()) {

    if (res->back() == "evolve-ranked") {

      std::cout << "running mode: " << res->back() << std::endl;
      StreakyWorld<
        ConfigHardware<
          emp::HammingMetric<16>,
          emp::RankedSelector<std::ratio<24,16>>
        >
      > rankedWorld(cfg);
      rankedWorld.CreatePopulation(cfg.POP_SIZE());
      rankedWorld.Start();
      std::cout << "DONE." << std::endl;

    } else if (res->back() == "evolve-roulette") {

      std::cout << "running mode: " << res->back() << std::endl;
       StreakyWorld<
         ConfigHardware<
           emp::StreakMetric<16>,
           emp::RouletteSelector<>
         >
       > rouletteWorld(cfg);
       rouletteWorld.CreatePopulation(cfg.POP_SIZE());
       rouletteWorld.Start();
      std::cout << "DONE." << std::endl;

    } else if (res->back() == "run-program") {

      std::cout << "running mode: " << res->back() << std::endl;
      emp::Random random(1);
      using CH = ConfigHardware<
        emp::HammingMetric<16>,
        emp::RankedSelector<std::ratio<24,16>>
      >;
      InstructionLibrary<CH> il;
      CH::inst_lib_t& inst_lib = il.CreateInstLib(cfg, random);
      CH::event_lib_t event_lib;

      CH::hardware_t hardware(inst_lib, event_lib, &random);
      std::ifstream prog (cfg.PROGRAM_FILENAME());
      hardware.GetProgram().Load(prog);
      prog.close();

      for (size_t i = 0; i < cfg.RUN_REPS(); ++i){
        Trait trait;
        hardware.SetTrait(trait);
        hardware.ResetHardware();
        hardware.SpawnCore(0);
        Sequence sequence(cfg, random, cfg.SEQS(i%cfg.NSEQS()));
        hardware.GetTrait().seq = &sequence;

        for(size_t j = 0; j < cfg.RUN_TICKS(); ++j){
          hardware.SingleProcess();
        }

      }

    } else std::cout << "unknown running mode: " << res->back() << std::endl;

    res->pop_back();

  }

}
