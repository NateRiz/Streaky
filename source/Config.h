#pragma once

#include "base/vector.h"
#include "base/Ptr.h"
#include "hardware/EventDrivenGP.h"
#include "hardware/signalgp_utils.h"
#include "ConfigBase.h"
#include "Trait.h"

class Config : public ConfigBase {

  public:

    Config(){};

    static constexpr size_t TAG_WIDTH = 16;

    using TRAIT_TYPE = Trait;
    using hardware_t = emp::EventDrivenGP_AW<
      TAG_WIDTH
      , TRAIT_TYPE
      , emp::MatchBin<
        size_t
        , emp::HammingMetric<16>
        , emp::RankedSelector<std::ratio<24,16>>
        >
      >;
    using inst_lib_t = emp::InstLib<hardware_t>;
    using inst_t = hardware_t::Instruction;
    using event_lib_t = emp::EventLib<hardware_t>;
    using state_t = hardware_t::State;
    using program_t = hardware_t::Program;
    using mutator_t = emp::SignalGPMutator<TAG_WIDTH, TRAIT_TYPE>;

    // TODO allow a variable number of streaky factors's
    double SEQS(const size_t idx) const {
      if (idx == 0) return SEQ_A();
      else if (idx ==1) return SEQ_B();
      else {
        emp_assert(false);
        return -1;
      }
    }

    size_t NSEQS() const { return 2; }

};
