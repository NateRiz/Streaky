#pragma once

#include "base/vector.h"
#include "base/Ptr.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "ConfigBase.h"
#include "Trait.h"

template<typename Metric, typename Selector>
class ConfigHardware {

  public:

    static constexpr size_t TAG_WIDTH = 16;

    using TRAIT_TYPE = Trait;
    using matchbin_t = emp::MatchBin<size_t, Metric, Selector>;
    using hardware_t = emp::EventDrivenGP_AW<
      TAG_WIDTH
      , TRAIT_TYPE
      , matchbin_t 
      >;
    using inst_lib_t = emp::InstLib<hardware_t>;
    using inst_t = typename hardware_t::Instruction;
    using event_lib_t = emp::EventLib<hardware_t>;
    using state_t = typename hardware_t::State;
    using program_t = typename hardware_t::Program;
    using mutator_t = emp::SignalGPMutator<TAG_WIDTH, TRAIT_TYPE, matchbin_t>;

};
