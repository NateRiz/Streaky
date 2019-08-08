#pragma once

#include "hardware/InstLib.h"

#include "Config.h"
#include "Sequence.h"

template <typename CH>
class InstructionLibrary{
  public:
  InstructionLibrary()=default;

  typename CH::inst_lib_t& CreateInstLib(
    const Config & cfg,
    emp::Random& random
  ){
    inst_lib = typename CH::inst_lib_t();
    InitializeDefault();
    InitializeCustom(cfg, random);
    return inst_lib;
  }

  private:
    void InitializeDefault(){
    inst_lib.AddInst("Inc", CH::hardware_t::Inst_Inc, 1, "Increment value in local memory Arg1");
    inst_lib.AddInst("Dec", CH::hardware_t::Inst_Dec, 1, "Decrement value in local memory Arg1");
    inst_lib.AddInst("Not", CH::hardware_t::Inst_Not, 1, "Logically toggle value in local memory Arg1");
    inst_lib.AddInst("Add", CH::hardware_t::Inst_Add, 3, "Local memory: Arg3 = Arg1 + Arg2");
    inst_lib.AddInst("Sub", CH::hardware_t::Inst_Sub, 3, "Local memory: Arg3 = Arg1 - Arg2");
    inst_lib.AddInst("Mult", CH::hardware_t::Inst_Mult, 3, "Local memory: Arg3 = Arg1 * Arg2");
    inst_lib.AddInst("Div", CH::hardware_t::Inst_Div, 3, "Local memory: Arg3 = Arg1 / Arg2");
    inst_lib.AddInst("Mod", CH::hardware_t::Inst_Mod, 3, "Local memory: Arg3 = Arg1 % Arg2");
    inst_lib.AddInst("TestEqu", CH::hardware_t::Inst_TestEqu, 3, "Local memory: Arg3 = (Arg1 == Arg2)");
    inst_lib.AddInst("TestNEqu", CH::hardware_t::Inst_TestNEqu, 3, "Local memory: Arg3 = (Arg1 != Arg2)");
    inst_lib.AddInst("TestLess", CH::hardware_t::Inst_TestLess, 3, "Local memory: Arg3 = (Arg1 < Arg2)");
    inst_lib.AddInst("If", CH::hardware_t::Inst_If, 1, "Local memory: If Arg1 != 0, proceed; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
    inst_lib.AddInst("While", CH::hardware_t::Inst_While, 1, "Local memory: If Arg1 != 0, loop; else, skip block.", emp::ScopeType::BASIC, 0, {"block_def"});
    inst_lib.AddInst("Countdown", CH::hardware_t::Inst_Countdown, 1, "Local memory: Countdown Arg1 to zero.", emp::ScopeType::BASIC, 0, {"block_def"});
    inst_lib.AddInst("Close", CH::hardware_t::Inst_Close, 0, "Close current block if there is a block to close.", emp::ScopeType::BASIC, 0, {"block_close"});
    inst_lib.AddInst("Break", CH::hardware_t::Inst_Break, 0, "Break out of current block.");
    inst_lib.AddInst("Call", CH::hardware_t::Inst_Call, 0, "Call function that best matches call affinity.", emp::ScopeType::BASIC, 0, {"affinity"});
    inst_lib.AddInst("Return", CH::hardware_t::Inst_Return, 0, "Return from current function if possible.");
    inst_lib.AddInst("SetMem", CH::hardware_t::Inst_SetMem, 2, "Local memory: Arg1 = numerical value of Arg2");
    inst_lib.AddInst("CopyMem", CH::hardware_t::Inst_CopyMem, 2, "Local memory: Arg1 = Arg2");
    inst_lib.AddInst("SwapMem", CH::hardware_t::Inst_SwapMem, 2, "Local memory: Swap values of Arg1 and Arg2.");
    inst_lib.AddInst("Input", CH::hardware_t::Inst_Input, 2, "Input memory Arg1 => Local memory Arg2.");
    inst_lib.AddInst("Output", CH::hardware_t::Inst_Output, 2, "Local memory Arg1 => Output memory Arg2.");
    inst_lib.AddInst("Commit", CH::hardware_t::Inst_Commit, 2, "Local memory Arg1 => Shared memory Arg2.");
    inst_lib.AddInst("Pull", CH::hardware_t::Inst_Pull, 2, "Shared memory Arg1 => Shared memory Arg2.");
    inst_lib.AddInst("Fork", CH::hardware_t::Inst_Fork, 0, "Fork a new thread, using tag-based referencing to determine which function to call on the new thread.", emp::ScopeType::BASIC, 0, {"affinity"});
    inst_lib.AddInst("Terminate", CH::hardware_t::Inst_Terminate, 0, "Terminate current thread.");
    inst_lib.AddInst("Nop", CH::hardware_t::Inst_Nop, 0, "No operation.");
    }

    void InitializeCustom(const Config & cfg, emp::Random& random){
    inst_lib.AddInst("SetReg", CH::hardware_t::Inst_SetRegulator, 1, "Set Matchbin Regulator to Arg1");
    inst_lib.AddInst("AdjReg", CH::hardware_t::Inst_AdjRegulator, 1, "Adjust Matchbin Regulator to Arg1");
    inst_lib.AddInst(
     "Sense",
      [&](
        typename CH::hardware_t & hw,
        const typename CH::inst_t & inst
      ) {
        typename CH::state_t& state = hw.GetCurState();
        const int cur = hw.GetTrait().seq->Get(hw.GetTrait().senseCount++);
        state.SetLocal(inst.args[0], cur);
      },
      1,
      "Arg1 = Next in sequence."
    );

    for (size_t idx = 0; idx < cfg.NSEQS(); ++idx) {

      inst_lib.AddInst(
        emp::to_string("GS_", idx),
        [idx](
          typename CH::hardware_t & hw,
          const typename CH::inst_t & inst
        ){
          hw.GetTrait().guess = idx;
          hw.GetTrait().guessCount[idx]+=1;
        },
        0,
        "Guesses the identity of a sequence."
      );

    }

    //inst_lib.AddInst("Debug",[](CH::hardware_t& hw, const CH::inst_t& inst){CH::state_t& state = hw.GetCurState(); std::cout << state.GetLocal(inst.args[0])<<std::endl;},1,"debug");
    }
  private:
    typename CH::inst_lib_t inst_lib;

};
