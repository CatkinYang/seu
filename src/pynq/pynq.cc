#include "pynq/pynq.h"
#include <cstddef>

namespace seu {

pynq::pynq() {
    m_name = "pynq";
    m_clb_pertile = PYNQ_CLB_PER_TILE;
    m_bram_pertile = PYNQ_BRAM_PER_TILE;
    m_dsp_pertile = PYNQ_DSP_PER_TILE;

    m_num_clk_reg = PYNQ_CLK_REG;
    m_width = PYNQ_WIDTH;

    m_num_forbidden_slots = PYNQ_FORBIDDEN;
    m_num_rows = PYNQ_NUM_ROWS;

    m_clk_reg.resize(PYNQ_CLK_REG);
    clk_reg_pos.resize(PYNQ_CLK_REG);
    bram_in_reg.resize(PYNQ_CLK_REG);
    dsp_in_reg.resize(PYNQ_CLK_REG);
    bram_pos.resize(PYNQ_CLK_REG);
    dsp_pos.resize(PYNQ_CLK_REG);
    forbidden_pos.resize(PYNQ_CLK_REG);

    for (size_t i = 0; i < bram_pos.size(); i++) {
        bram_pos[i].resize(3);
    }

    for (size_t i = 0; i < dsp_pos.size(); i++) {
        dsp_pos[i].resize(3);
    }

    clk_reg_pos = {{0, 0, 31, 50},  {0, 50, 31, 50},  {0, 100, 31, 50},
                   {32, 0, 38, 50}, {32, 50, 38, 50}, {32, 100, 38, 50}};

    bram_in_reg = {1, 1, 3, 3, 3, 3};

    bram_pos = {{21, 0, 0},   {21, 0, 0},   {5, 16, 21},
                {35, 55, 66}, {35, 55, 66}, {35, 55, 66}};

    dsp_in_reg = {1, 1, 3, 2, 2, 2};

    dsp_pos = {{24, 0, 0},  {24, 0, 0},  {8, 13, 24},
               {58, 63, 0}, {58, 63, 0}, {58, 63, 0}};
    forbidden_pos = {{0, 10, 17, 20}, {42, 10, 6, 20}, {47, 0, 2, 10}};

    initialize_clk_reg();
}

void pynq::initialize_clk_reg() {
    unsigned int i = 0;

    for (i = 0; i < PYNQ_CLK_REG; i++) {
        init_clk_reg(i, clk_reg_pos[i], m_clb_pertile, m_bram_pertile,
                     m_dsp_pertile, bram_in_reg[i], dsp_in_reg[i], bram_pos[i],
                     dsp_pos[i]);
    }
}
} // namespace seu
