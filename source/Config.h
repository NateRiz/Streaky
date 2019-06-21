#pragma once
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"

struct Trait{
  public:
    Trait():streakyFactor(), guess(-1), fitness(0){;
    }
    double streakyFactor;
    int guess; // STREAKY 0 : EVEN 1
    double fitness;
    int sequenceBit;
};

class Config{
  public:
    Config(){};
    static constexpr unsigned int TAG_WIDTH = 16;
    static constexpr size_t HW_MAX_THREADS = 16;
    static constexpr size_t HW_MAX_CALL_DEPTH = 128;
    static constexpr double HW_MIN_SIM_THRESH = 0.0;

    using TRAIT_TYPE = Trait;
    using hardware_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>;
    using inst_lib_t = emp::InstLib<hardware_t>;
    using inst_t = hardware_t::Instruction;
    using event_lib_t = emp::EventLib<hardware_t>;
    using state_t = hardware_t::State;
    using program_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>::Program;
    using mutator_t = emp::SignalGPMutator<Config::TAG_WIDTH, Config::TRAIT_TYPE>;



};
