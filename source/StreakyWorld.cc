#pragma once

#include "hardware/EventDrivenGP.h"

#include "Config.h"
#include "InstructionLibrary.h"
#include "StreakyWorld.h"

template <typename CH>
StreakyWorld<CH>::StreakyWorld(const Config & cfg_)
  : cfg(cfg_)
  , random(cfg_.SEED())
  , guessMonitors(cfg.NSEQS())
  , analytics(cfg_, guessMonitors, senseMonitor, fitnessMonitor, funCallCountMonitor, funForkCountMonitor, funTotalCallMonitor)
{
  this->MarkSynchronous(true);
  this->SetPopStruct_Mixed(true);

  InstructionLibrary<CH> il;
  inst_lib = il.CreateInstLib(cfg, random);

  ConfigureWorld();
}

template <typename CH>
void StreakyWorld<CH>::Start(){
  int gen = 0;
  while (gen < cfg.MAX_GENERATIONS() || cfg.MAX_GENERATIONS() == -1) {
    ++gen;
    if(!(gen%50)) { std::cout << "GEN: " << gen << std::endl; }
    for (auto & cell : this->pop){
      cell->hardware.GetTrait().fitness = 0;
    }
    this->Update();
    this->DoMutations();
    this->PrintBestCell();
  }
  analytics.Update();
}

template <typename CH>
void StreakyWorld<CH>::PrintBestCell(){
  Cell<CH>* bestCell = this->pop[0];
  for (unsigned int i = 0 ; i < this->GetNumOrgs(); ++i){
    double fitness = this->CalcFitnessOrg(*this->pop[i]);
    if (fitness > bestFitness){
      bestFitness = fitness;
      bestCell = this->pop[i];
      bestCell->hardware.PrintProgram();
      bestCell->hardware.PrintProgramFull();
      std::cout << "Finished with Fitness: " << bestFitness << std::endl;
    }
  }
}

template <typename CH>
void StreakyWorld<CH>::CreatePopulation(const unsigned int SAMPLES){
  for (unsigned int i = 0; i < SAMPLES; ++i){
    Cell<CH> cell{cfg, inst_lib, event_lib, random, mutator};
    typename CH::program_t prog = emp::GenRandSignalGPProgram(random, inst_lib, 1, 16, 5, 32); //TODO make these config params
    cell.SetProgram(prog);
    cell.hardware.ResetHardware();
    this->Inject( cell );
  }
}

template <typename CH>
void StreakyWorld<CH>::ConfigureWorld(){

  this->SetFitFun([&](const Cell<CH>& cell)->double{
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

  this->SetMutFun([&](Cell<CH>& cell, emp::Random& random)->size_t{
    return mutator.ApplyMutations(cell.hardware.GetProgram(), random);
  });

  this->OnUpdate([&](size_t update)->void{
    emp::vector<Sequence> tests;
    tests.reserve(cfg.SEQ_REPS() * cfg.NSEQS());
    for (size_t seq = 0; seq < cfg.NSEQS(); ++seq) {
      for (size_t rep = 0; rep < cfg.SEQ_REPS(); ++rep) {
        tests.emplace_back(
          cfg,
          StreakyWorld<CH>::random,
          cfg.SEQS(seq)
        );
      }
    }

    for(auto & cell : this->pop) {
      cell->CacheFitness(tests);
      senseMonitor.Add((double)cell->hardware.GetTrait().senseCount / (double)cfg.SEQ_REPS());
      funCallCountMonitor.Add(cell->hardware.GetTrait().funCallCount);
      funForkCountMonitor.Add(cell->hardware.GetTrait().funForkCount);
      funTotalCallMonitor.Add(cell->hardware.GetTrait().funCallCount+cell->hardware.GetTrait().funForkCount);
      fitnessMonitor.Add((double)cell->hardware.GetTrait().fitness);
      for(size_t i = 0; i < cfg.NSEQS(); ++i){
        guessMonitors[i].Add(
          (double)cell->hardware.GetTrait().guessCount[i] / (double)cfg.SEQ_REPS()
        );
      }

      /// Reset the counts in the cell exept fitness which is done after selection.
      cell->hardware.GetTrait().funCallCount = 0;
      cell->hardware.GetTrait().funForkCount = 0;
      cell->hardware.GetTrait().guessCount.clear();
      cell->hardware.GetTrait().senseCount = 0;
    }

    analytics.Update();
    senseMonitor.Reset();
    fitnessMonitor.Reset();
    funCallCountMonitor.Reset();
    funForkCountMonitor.Reset();
    funTotalCallMonitor.Reset();
    for (size_t i = 0; i < guessMonitors.size(); ++i){
      guessMonitors[i].Reset();
    }

    TournamentSelect(*this, 2, this->GetNumOrgs());
    for(auto & cell : this->pop) cell->hardware.GetTrait().fitness = 0;
  });
}
