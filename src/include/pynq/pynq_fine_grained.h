#pragma once

#include "fine_grained.h"

namespace seu {

class pynq_fine_grained : public fine_grained {
  public:
    pynq_fine_grained();
    virtual void init_fine_grained() override;
    ~pynq_fine_grained() = default;
};

using pfgRef = std::shared_ptr<pynq_fine_grained>;

} // namespace seu
