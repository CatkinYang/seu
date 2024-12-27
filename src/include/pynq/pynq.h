#pragma once
#include "fpga.h"
#include "marco.h"

namespace seu {

class pynq : public fpga {
  public:
    pynq();
    virtual void initialize_clk_reg() override;
    ~pynq() {};
};
using pynqRef = std::shared_ptr<pynq>;

} // namespace seu
