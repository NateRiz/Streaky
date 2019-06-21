#pragma once
#include "StreakyWorld.h"
#include "hardware/EventDrivenGP.h"
#include "InstructionLibrary.h"
#include "Cell.h"

StreakyWorld::StreakyWorld()
  : World("StreakyWorld")
  , random(1)
  , correctAnswer(-1)
{
  MarkSynchronous(true);
  SetPopStruct_Mixed(true);

  InstructionLibrary il;
  inst_lib = il.CreateInstLib(random);

  SetFitFun([&](const Cell& cell)->double{
      double fitness =  cell.hardware.GetTrait().fitness; 
      return fitness;
  });

  mutator.ResetMutators();
  SetMutFun([&](Cell& cell, emp::Random& random)->size_t{
      if(random.GetDouble() < 0.5){
        return mutator.ApplyMutations(cell.hardware.GetProgram(), random);
      }
      return 0;
      });

  OnUpdate([&](size_t updates)->void{
    for(auto cell : pop){
      cell->hardware.GetTrait().fitness = 0; 
    }
    for (unsigned int i = 0; i < TESTS; ++i){
      unsigned int extraTicksNoise = random.GetInt(1,30);
      ResetStreak(i%2);
      unsigned int sequenceBit = random.GetInt(0,2);
      for (auto cell : pop){
        cell->hardware.ResetHardware();
        cell->hardware.SpawnCore(0);
        cell->hardware.GetTrait().streakyFactor = streakyFactor;
        cell->hardware.GetTrait().guess = -1;
        cell->hardware.GetTrait().sequenceBit = sequenceBit;
        for(unsigned int t = 0; t < TICKS+extraTicksNoise; ++t){
          cell->Tick();
        }

        cell->hardware.GetTrait().fitness+=GetFitness(*cell);
        if ( cell->hardware.GetTrait().fitness == (int)TESTS){
          cell->hardware.GetTrait().fitness += 10000 - cell->hardware.GetConstProgram().GetInstCnt();
        }

      }
    }
    TournamentSelect(*this, 2, GetNumOrgs());
    });
}

double StreakyWorld::GetFitness(Cell& cell){
  int guess = cell.hardware.GetTrait().guess;
  return (guess == -1 ? guess : guess == correctAnswer);
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

void StreakyWorld::ResetStreak(int streakType){
  streakyFactor = 0.5;
  correctAnswer = 1;
  if (streakType == 0){
    streakyFactor+=random.GetDouble(0.1, 0.4);
    correctAnswer = 0;
  }
}

void StreakyWorld::CreatePopulation(const unsigned int SAMPLES){
  for (unsigned int i = 0; i < SAMPLES; ++i){
    Cell cell{inst_lib, event_lib, random, mutator};
    Config::program_t prog = GenRandSignalGPProgram(random, inst_lib, 1, 2);
    cell.SetProgram(prog);
    cell.hardware.ResetHardware();
    Inject( cell );
  }
}

