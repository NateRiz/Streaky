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
  InstructionLibrary il;
  inst_lib = il.CreateInstLib(random);
  SetFitFun([&](const Cell& cell)->double{
    int guess = cell.hardware.GetTrait().guess;
    return (guess == -1 ? guess : guess == this->correctAnswer);
    });
}

void StreakyWorld::Restart(){
  ResetStreak();
  for (auto& cell : cells){
    cell.Restart(streakyFactor);
  }
}

void StreakyWorld::ResetStreak(int streakType){
  double streakyFactor = 0.5;
  correctAnswer = 1;
  if ((random.GetInt(2) == 0 && streakType == -1) || streakType == 0){
    streakyFactor+=random.GetDouble(0.1, 0.4);
    correctAnswer = 0;
  }
}

void StreakyWorld::Tick(){
  for (auto& cell : cells) { cell.Tick(); }
}

std::vector<int> StreakyWorld::GetFitness(){
  //TODO maybe change the guess to a vector. Better fitness would be
  //whichever vector has the most of correctAnswer.
  //TODO secondary fitness attribute could be program size
  std::vector<int> fitness;
  for (auto& cell : cells){
    int guess = cell.hardware.GetTrait().guess;
    fitness.push_back((guess == -1 ? guess : guess == correctAnswer));
  }
  return fitness;
}

