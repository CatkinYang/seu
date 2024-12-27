#pragma once
#include "marco.h"

namespace seu {

struct pos {
    int x;
    int y;
    int w;
    int h;
};
using posRef = shared_ptr<pos>;

struct fpga_clk_reg {
    pos clk_reg_pos;
    int clb_per_column;
    int bram_per_column;
    int dsp_per_column;
    int clb_num;
    int bram_num;
    int dsp_num;
    int forbidden_num;
    std::vector<int> bram_pos;
    std::vector<int> dsp_pos;
};

using fcrRef = shared_ptr<fpga_clk_reg>;
using fcr = fpga_clk_reg;

// fpga 基类,抽象类
class fpga {
  public:
    virtual ~fpga() = default;
    virtual void initialize_clk_reg() = 0;

  public:
    string m_name = "fpga";

    int m_clb_pertile = 0;
    int m_bram_pertile = 0;
    int m_dsp_pertile = 0;

    int m_num_clk_reg = 0;
    int m_width = 0;

    int m_num_forbidden_slots = 0;
    int m_num_rows = 0;

    vector<fcr> m_clk_reg;

    vector<pos> forbidden_pos;

    vector<pos> clk_reg_pos;

    vector<int> bram_in_reg;
    vector<vector<int>> bram_pos;

    vector<int> dsp_in_reg;
    vector<vector<int>> dsp_pos;
};

} // namespace seu
