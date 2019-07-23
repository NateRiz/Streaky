#include <iostream>
#include <fstream>
#include "../Trait.cc"
#include "../Sequence.h"
#include "../Config.h"
#include "../InstructionLibrary.h"


int main(){
    static constexpr unsigned int TAG_WIDTH = 16;

    using TRAIT_TYPE = Trait;
    using hardware_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>;
    using inst_lib_t = emp::InstLib<hardware_t>;
    using event_lib_t = emp::EventLib<hardware_t>;

    emp::Random random(1);
    InstructionLibrary il;
    inst_lib_t& inst_lib = il.CreateInstLib(random);
    event_lib_t event_lib;
    
    hardware_t hardware(inst_lib, event_lib, &random);
    std::ifstream prog ("program.txt");
    hardware.GetProgram().Load(prog);
    prog.close();



    for (int i = 0; i < 30; ++i){
      Trait trait;
      hardware.SetTrait(trait);
      hardware.ResetHardware();
      hardware.SpawnCore(0);
      Sequence sequence(random, i%2);
      hardware.GetTrait().seq = &sequence;

      for(unsigned int i = 0; i < 300000; ++i){
        hardware.SingleProcess();
      }
      
    }
}
