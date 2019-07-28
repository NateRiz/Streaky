#pragma once

#include "hardware/EventDrivenGP.h"

#include "Config.h"
#include "InstructionLibrary.h"
#include "StreakyWorld.h"

StreakyWorld::StreakyWorld(const Config & cfg_)
  : World("StreakyWorld")
  , cfg(cfg_)
  , random(cfg_.SEED())
  , guessMonitors(cfg.NSEQS())
  , analytics(cfg_, guessMonitors, senseMonitor, fitnessMonitor)
{
  MarkSynchronous(true);
  SetPopStruct_Mixed(true);

  InstructionLibrary il;
  inst_lib = il.CreateInstLib(cfg, random);

  ConfigureWorld();
}

void StreakyWorld::Start(){
  int gen = 0;
  while (gen < cfg.MAX_GENERATIONS() || cfg.MAX_GENERATIONS() == -1) {
    ++gen;
    if(!(gen%50)) { std::cout << "GEN: " << gen << std::endl; }
    for (auto cell : pop){
      cell->hardware.GetTrait().fitness = 0;
    }
    Update();
    DoMutations();
    PrintBestCell();
  }
  analytics.Update();
}

void StreakyWorld::PrintBestCell(){
  Cell* bestCell = pop[0];
  for (unsigned int i = 0 ; i < GetNumOrgs(); ++i){
    double fitness = CalcFitnessOrg(*pop[i]);
    if (fitness > bestFitness){
      bestFitness = fitness;
      bestCell = pop[i];
      bestCell->hardware.PrintProgram();
      bestCell->hardware.PrintProgramFull();
      std::cout << "Finished with Fitness: " << bestFitness << std::endl;
    }
  }
}

void StreakyWorld::CreatePopulation(const unsigned int SAMPLES){
  for (unsigned int i = 0; i < SAMPLES; ++i){
    Cell cell{cfg, inst_lib, event_lib, random, mutator};
    Config::program_t prog = emp::GenRandSignalGPProgram(random, inst_lib, 1, 16, 5, 32);
    cell.SetProgram(prog);
    cell.hardware.ResetHardware();
    Inject( cell );
  }
}

void StreakyWorld::ConfigureWorld(){

  SetFitFun([&](const Cell& cell)->double{
      return cell.hardware.GetTrait().fitness;
  });

  mutator.ResetMutators();
  // Setup other SignalGP functions.
  mutator.ARG_SUB__PER_ARG(cfg.ARG_SUB__PER_ARG());
  mutator.INST_SUB__PER_INST(cfg.INST_SUB__PER_INST());
  mutator.INST_INS__PER_INST(cfg.INST_INS__PER_INST());
  mutator.INST_DEL__PER_INST(cfg.INST_DEL__PER_INST());
  mutator.SLIP__PER_FUNC(cfg.SLIP__PER_FUNC());
  mutator.FUNC_DUP__PER_FUNC(cfg.FUNC_DUP__PER_FUNC());
  mutator.FUNC_DEL__PER_FUNC(cfg.FUNC_DEL__PER_FUNC());
  mutator.TAG_BIT_FLIP__PER_BIT(cfg.TAG_BIT_FLIP__PER_BIT());

  SetMutFun([&](Cell& cell, emp::Random& random)->size_t{
    return mutator.ApplyMutations(cell.hardware.GetProgram(), random);
  });

  OnUpdate([&](size_t update)->void{
    emp::vector<Sequence> tests;
    tests.reserve(cfg.SEQ_REPS() * cfg.NSEQS());
    for (size_t seq = 0; seq < cfg.NSEQS(); ++seq) {
      for (size_t rep = 0; rep < cfg.SEQ_REPS(); ++rep) {
        tests.emplace_back(
          cfg,
          StreakyWorld::random,
          cfg.SEQS(seq)
          // Uncomment the next line to change 0/1 to .5/.7-.9
          // Harder sol't for genome
          //(seq % 2 == 0 ? random.GetDouble(0.2, 0.4) + 0.5 : 0.5)
          // 30/70 10/90% each bit is 0/1
        );
      }
    }

    for(auto & cell : pop) {
      cell->CacheFitness(tests);
      senseMonitor.Add((double)cell->hardware.GetTrait().senseCount / (double)cfg.SEQ_REPS());
      fitnessMonitor.Add(cell->hardware.GetTrait().fitness);
      for(size_t i = 0; i < cfg.NSEQS(); ++i){
        guessMonitors[i].Add(
          (double)cell->hardware.GetTrait().guessCount[i] / (double)cfg.SEQ_REPS()
        );
      }

      /// Reset the counts in the cell exept fitness which is done after selection.
      cell->hardware.GetTrait().guessCount.clear();
      cell->hardware.GetTrait().senseCount = 0;
    }

    analytics.Update();
    senseMonitor.Reset();
    fitnessMonitor.Reset();
    for (size_t i = 0; i < guessMonitors.size(); ++i){
      guessMonitors[i].Reset();
    }


    TournamentSelect(*this, 2, GetNumOrgs());
    for(auto & cell : pop) cell->hardware.GetTrait().fitness = 0;
  });
}
