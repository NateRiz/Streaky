#pragma once
#include "StreakyWorld.h"
#include "hardware/EventDrivenGP.h"
#include "InstructionLibrary.h"
#include "Cell.h"

StreakyWorld::StreakyWorld()
  : World("StreakyWorld")
  , random(Config::SEED)
{
  MarkSynchronous(true);
  SetPopStruct_Mixed(true);

  InstructionLibrary il;
  inst_lib = il.CreateInstLib(random);

  SetFitFun([&](const Cell& cell)->double{
      return cell.hardware.GetTrait().fitness;
  });

  mutator.ResetMutators();

    // Setup other SignalGP functions.
  mutator.ARG_SUB__PER_ARG(Config::ARG_SUB__PER_ARG);
  mutator.INST_SUB__PER_INST(Config::INST_SUB__PER_INST);
  mutator.INST_INS__PER_INST(Config::INST_INS__PER_INST);
  mutator.INST_DEL__PER_INST(Config::INST_DEL__PER_INST);
  mutator.SLIP__PER_FUNC(Config::SLIP__PER_FUNC);
  mutator.FUNC_DUP__PER_FUNC(Config::FUNC_DUP__PER_FUNC);
  mutator.FUNC_DEL__PER_FUNC(Config::FUNC_DEL__PER_FUNC);
  mutator.TAG_BIT_FLIP__PER_BIT(Config::TAG_BIT_FLIP__PER_BIT);
  
  SetMutFun([&](Cell& cell, emp::Random& random)->size_t{
    return mutator.ApplyMutations(cell.hardware.GetProgram(), random);
  });

  OnUpdate([&](size_t update)->void{
    emp::vector<Sequence> tests;
    tests.reserve(Config::SEQ_REPS * Config::SEQS.size());
    for (size_t seq = 0; seq < Config::SEQS.size(); ++seq) {
      for (size_t rep = 0; rep < Config::SEQ_REPS; ++rep) {
        tests.emplace_back(
          StreakyWorld::random,
          Config::SEQS[seq]
        );
      }
    }

    for(auto & cell : pop) cell->CacheFitness(tests);
    TournamentSelect(*this, 2, GetNumOrgs());
    for(auto & cell : pop) cell->hardware.GetTrait().fitness = 0;
  });
}

void StreakyWorld::Start(){
  unsigned int gen = 0;
  while (true) {
    if(!(gen%100)){std::cout << "GEN: " << gen++ << std::endl;}
    for (auto cell : pop){
      cell->hardware.GetTrait().fitness = 0;
    }
    Update();
    DoMutations();
    PrintBestCell();
  }
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
    Cell cell{inst_lib, event_lib, random, mutator};
    Config::program_t prog = GenRandSignalGPProgram(random, inst_lib, 1, 16, 5, 32);
    cell.SetProgram(prog);
    cell.hardware.ResetHardware();
    Inject( cell );
  }
}
