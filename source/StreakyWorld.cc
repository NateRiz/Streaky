#pragma once
#include <iostream>
#include <fstream>

#include "hardware/EventDrivenGP.h"

#include "Config.h"
#include "InstructionLibrary.h"
#include "StreakyWorld.h"


template <typename CH>
StreakyWorld<CH>::StreakyWorld(const Config & cfg_)
  : cfg(cfg_)
  , random(cfg_.SEED())
  , guessMonitors(cfg.NSEQS())
  , analytics(cfg_, guessMonitors, senseMonitor, fitnessMonitor, sizeMonitor, funCallCountMonitor, funForkCountMonitor, funTotalCallMonitor)
{
  bestFitness = -1*(double)(cfg.NSEQS()*cfg.SEQ_REPS())-1;
  bestCell = nullptr;
  this->MarkSynchronous(true);
  this->SetPopStruct_Mixed(true);

  InstructionLibrary<CH> il;
  inst_lib = il.CreateInstLib(cfg, random);
  
  EventLibrary<CH> el;
  event_lib = el.CreateEventLib();

  ConfigureWorld();
}

/// Entry point for the world. 
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
  }

  analytics.Update();//TODO why are there two of these? Does it need to udate one last time?
}

template <typename CH>
void StreakyWorld<CH>::PrintBestCell(){
  std::ofstream file;
  /// Write the best program to the output file
  file.open("program.gp");
  bestCell->hardware.PrintProgram(file);
  file.close();
}

template <typename CH>
void StreakyWorld<CH>::TestCellVerbose(){
  /// Run two tests on a single cell and write the state after each
  /// tick() to an output file.
  emp::vector<Sequence> tests;
  tests.emplace_back(cfg, StreakyWorld<CH>::random, cfg.SEQS(0));
  tests.emplace_back(cfg, StreakyWorld<CH>::random, cfg.SEQS(1));
  bestCell->CacheFitness(tests, true);
}

template <typename CH>
void StreakyWorld<CH>::CreatePopulation(const unsigned int SAMPLES){
  /// Create an initial population and generate random programs for each
  /// in compliance with the config args.
  /// Inject it into the world.
  for (unsigned int i = 0; i < SAMPLES; ++i){
    Cell<CH> cell{cfg, inst_lib, event_lib, random, mutator};
    typename CH::program_t prog = emp::GenRandSignalGPProgram(random
        , inst_lib
        , cfg.MIN_FUNC_CNT()
        , cfg.MAX_FUNC_CNT()
        , cfg.MIN_FUN_LEN()
        , cfg.MAX_FUN_LEN());
    cell.SetProgram(prog);
    cell.hardware.ResetHardware();
    this->Inject( cell );
  }
}

template <typename CH>
void StreakyWorld<CH>::ConfigureWorld(){

  this->SetFitFun([&](const Cell<CH>& cell)->double{
      /// The fitness fx is a getter for the cached fitness in the cell
      return cell.hardware.GetTrait().fitness;
  });

  mutator.ResetMutators();
  /// Set up mutator config options and
  /// assign world's mutator function
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
    /// Create {# tests}*{test types} different tests that will be given to each cell
    emp::vector<Sequence> tests;
    tests.reserve(cfg.SEQ_REPS() * cfg.NSEQS());
    for (size_t seq = 0; seq < cfg.NSEQS(); ++seq) {
      for (size_t rep = 0; rep < cfg.SEQ_REPS(); ++rep) {
        tests.emplace_back(cfg, StreakyWorld<CH>::random, cfg.SEQS(seq));
      }
    }

    /// We want the best cell relative to each generation
    /// in order to monitor its fitness
    /// If the fitness beats our absolute best we run it thru verbose tests.
    emp::vector<Cell<CH>*> curGenBestCells {};
    //Impossibly low fitness to achieve as default
    double curGenBestFitness = -1*(double)(cfg.SEQ_REPS() * cfg.NSEQS()) - 1;
    curGenBestCells.push_back(this->pop[0]);
    for(auto & cell : this->pop) {
      cell->CacheFitness(tests);
      double fitness = cell->hardware.GetTrait().fitness;
      
      /// Keep track of multiple cells that share a best fitness
      if (abs(fitness - curGenBestFitness) < cfg.EPSILON()){
        curGenBestCells.push_back(cell);
      }
      else if (fitness > curGenBestFitness){
        curGenBestCells.resize(1);
        curGenBestCells[0] = cell;  
      }

      /// Check if theres a new best cell and verbose test it.
      if (fitness > bestFitness){
        bestCell = cell;
        bestFitness = fitness;
        if(!cfg.FAST()){
          this->TestCellVerbose();
        }
        this->PrintBestCell();
        std::cout<<"New Best Fitness: "<<fitness <<std::endl;
      }

      curGenBestFitness = std::max(fitness, curGenBestFitness);
      fitnessMonitor.Add(fitness);
    }

    /// Update all our monitors once per gen unless FAST
    if(!cfg.FAST()){
      for(auto& cell: curGenBestCells){
        senseMonitor.Add((double)cell->hardware.GetTrait().senseCount / (double)cfg.SEQ_REPS());
        sizeMonitor.Add((double)cell->hardware.GetProgram().GetInstCnt());
        funCallCountMonitor.Add(cell->hardware.GetTrait().funCallCount);
        funForkCountMonitor.Add(cell->hardware.GetTrait().funForkCount);
        funTotalCallMonitor.Add(cell->hardware.GetTrait().funCallCount+cell->hardware.GetTrait().funForkCount);
        for(size_t i = 0; i < cfg.NSEQS(); ++i){
          guessMonitors[i].Add(
            (double)cell->hardware.GetTrait().guessCount[i] / (double)cfg.SEQ_REPS()
          );
        } 
      }

      /// After we update, reset because as we care about a per-generation monitor.
      analytics.Update();
      senseMonitor.Reset();
      fitnessMonitor.Reset();
      sizeMonitor.Reset();
      funCallCountMonitor.Reset();
      funForkCountMonitor.Reset();
      funTotalCallMonitor.Reset();
      for (size_t i = 0; i < guessMonitors.size(); ++i){
        guessMonitors[i].Reset();
      }
    }
    
    for(auto & cell : this->pop){
      /// Reset the counts in the cell exept fitness which is done after selection.
      cell->hardware.GetTrait().funCallCount = 0;
      cell->hardware.GetTrait().funForkCount = 0;
      cell->hardware.GetTrait().guessCount.clear();
      cell->hardware.GetTrait().senseCount = 0;
    }
    
    TournamentSelect(*this, 2, this->GetNumOrgs());
    for(auto & cell : this->pop) cell->hardware.GetTrait().fitness = 0;

  });
}
