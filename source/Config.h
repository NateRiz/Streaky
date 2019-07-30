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
