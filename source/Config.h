#pragma once

#include "base/vector.h"
#include "base/Ptr.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"

// forward declaration
class Trait;

class Config{
  public:
    Config(){};
    static constexpr unsigned int TAG_WIDTH = 16;
    static constexpr size_t HW_MAX_THREADS = 16;
    static constexpr size_t HW_MAX_CALL_DEPTH = 128;
    static constexpr double HW_MIN_SIM_THRESH = 0.0;

    static constexpr unsigned int TICKS = 75;
    static constexpr unsigned int TICKS_NOISE = 30;
    static constexpr unsigned int SEQ_REPS = 50;
    static constexpr int SEED = 1;
    static constexpr size_t POP_SIZE = 1000;

    static constexpr emp::array<double, 2> SEQS{0.0, 1.0};

    using TRAIT_TYPE = Trait;
    using hardware_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>;
    using inst_lib_t = emp::InstLib<hardware_t>;
    using inst_t = hardware_t::Instruction;
    using event_lib_t = emp::EventLib<hardware_t>;
    using state_t = hardware_t::State;
    using program_t = emp::EventDrivenGP_AW<TAG_WIDTH, TRAIT_TYPE>::Program;
    using mutator_t = emp::SignalGPMutator<Config::TAG_WIDTH, Config::TRAIT_TYPE>;


};
