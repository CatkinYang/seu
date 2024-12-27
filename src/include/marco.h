#pragma once
#include <array>
#include <fstream>
#include <gurobi_c++.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

using ul = unsigned long;
using ui = unsigned int;
using std::array;
using std::cout;
using std::endl;
using std::fstream;
using std::ofstream;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

using GRBVarArray = vector<GRBVar>;
using GRBVar2DArray = vector<GRBVarArray>;
using GRBVar3DArray = vector<GRBVar2DArray>;
using GRBVar4DArray = vector<GRBVar3DArray>;

#define MAX_SLOTS 10

#define CLB 0
#define BRAM 1
#define DSP 2
#define FBDN 3
#define CENTRAL_CLK 4

// parameters for vc707
#define VC707_CLK_REG 14
#define VC707_WIDTH 152
#define VC707_NUM_ROWS 10
#define VC707_FORBIDDEN 3
#define VC707_CLB_PER_TILE 50
#define VC707_BRAM_PER_TILE 10
#define VC707_DSP_PER_TILE 20
#define VC707_CLB_TOT 45525
#define VC707_BRAM_TOT 1030
#define VC707_DSP_TOT 2800

// parameters for pynq
#define PYNQ_CLK_REG 6
#define PYNQ_WIDTH 70
#define PYNQ_NUM_ROWS 10
#define PYNQ_FORBIDDEN 3
#define PYNQ_CLB_PER_TILE 50
#define PYNQ_BRAM_PER_TILE 10
#define PYNQ_DSP_PER_TILE 20
#define PYNQ_CLB_TOT 6650
#define PYNQ_BRAM_TOT 140
#define PYNQ_DSP_TOT 220

// parameters for VCU118
#define VCU118_CLK_REG 28
#define VCU118_WIDTH 188
#define VCU118_NUM_ROWS 20
#define VCU118_FORBIDDEN 3
#define VCU118_CLB_PER_TILE 60
#define VCU118_BRAM_PER_TILE 12
#define VCU118_DSP_PER_TILE 24
#define VCU118_CLB_TOT 45525
#define VCU118_BRAM_TOT 1030
#define VCU118_DSP_TOT 2800

// parameters for virtex_7
#define VIRTEX_CLB_TOT 21487
#define VIRTEX_BRAM_TOT 500
#define VIRTEX_DSP_TOT 900
#define VIRTEX_CLB_MIN 10
#define VIRTEX_BRAM_MIN 0
#define VIRTEX_DSP_MIN 0
#define VIRTEX_FORBIDDEN 4
#define VIRTEX_NUM_ROWS 10
#define VIRTEX_WIDTH 103
#define VIRTEX_CLK_REG 14
#define VIRTEX_CLB_PER_TILE 50
#define VIRTEX_BRAM_PER_TILE 10
#define VIRTEX_DSP_PER_TILE 20

// bram descritpion on virtex
#define VIRTEX_CLK00_BRAM 4
#define VIRTEX_CLK01_BRAM 4
#define VIRTEX_CLK02_BRAM 4
#define VIRTEX_CLK03_BRAM 4
#define VIRTEX_CLK04_BRAM 4
#define VIRTEX_CLK05_BRAM 2
#define VIRTEX_CLK06_BRAM 2
#define VIRTEX_CLK10_BRAM 4
#define VIRTEX_CLK11_BRAM 4
#define VIRTEX_CLK12_BRAM 4
#define VIRTEX_CLK13_BRAM 4
#define VIRTEX_CLK14_BRAM 5
#define VIRTEX_CLK15_BRAM 5
#define VIRTEX_CLK16_BRAM 5
#define VIRTEX_CLK00_DSP 4
#define VIRTEX_CLK01_DSP 4
#define VIRTEX_CLK02_DSP 4
#define VIRTEX_CLK03_DSP 4
#define VIRTEX_CLK04_DSP 4
#define VIRTEX_CLK05_DSP 2
#define VIRTEX_CLK06_DSP 2
#define VIRTEX_CLK10_DSP 3
#define VIRTEX_CLK11_DSP 3
#define VIRTEX_CLK12_DSP 3
#define VIRTEX_CLK13_DSP 3
#define VIRTEX_CLK14_DSP 3
#define VIRTEX_CLK15_DSP 3
#define VIRTEX_CLK16_DSP 3

// parameters for zynq
#define ZYNQ_CLB_TOT 2200
#define ZYNQ_BRAM_TOT 60
#define ZYNQ_DSP_TOT 80
#define ZYNQ_CLB_MIN 10
#define ZYNQ_BRAM_MIN 0
#define ZYNQ_DSP_MIN 0
#define ZYNQ_FORBIDDEN 2
#define ZYNQ_NUM_ROWS 10
#define ZYNQ_WIDTH 29
#define ZYNQ_CLK_REG 4
#define ZYNQ_CLK00_BRAM 1
#define ZYNQ_CLK01_BRAM 1
#define ZYNQ_CLK10_BRAM 2
#define ZYNQ_CLK11_BRAM 2
#define ZYNQ_CLK00_DSP 1
#define ZYNQ_CLK01_DSP 1
#define ZYNQ_CLK10_DSP 1
#define ZYNQ_CLK11_DSP 1
#define ZYNQ_CLB_PER_TILE 50
#define ZYNQ_BRAM_PER_TILE 10
#define ZYNQ_DSP_PER_TILE 20

// parameters for virtex_5
#define VIRTEX_5_CLB_TOT 8140
#define VIRTEX_5_BRAM_TOT 160
#define VIRTEX_5_DSP_TOT 64
#define VIRTEX_5_CLB_MIN 10
#define VIRTEX_5_BRAM_MIN 0
#define VIRTEX_5_DSP_MIN 0
#define VIRTEX_5_CLK_REG 16
#define VIRTEX_5_FORBIDDEN 2
#define VIRTEX_5_NUM_ROWS 4
#define VIRTEX_5_WIDTH 62
#define VIRTEX_5_CLB_PER_TILE 20
#define VIRTEX_5_BRAM_PER_TILE 4
#define VIRTEX_5_DSP_PER_TILE 8
#define VIRTEX_5_CLK00_BRAM 2
#define VIRTEX_5_CLK01_BRAM 2
#define VIRTEX_5_CLK02_BRAM 2
#define VIRTEX_5_CLK03_BRAM 2
#define VIRTEX_5_CLK04_BRAM 2
#define VIRTEX_5_CLK05_BRAM 2
#define VIRTEX_5_CLK06_BRAM 2
#define VIRTEX_5_CLK07_BRAM 2
#define VIRTEX_5_CLK10_BRAM 3
#define VIRTEX_5_CLK11_BRAM 3
#define VIRTEX_5_CLK12_BRAM 3
#define VIRTEX_5_CLK13_BRAM 3
#define VIRTEX_5_CLK14_BRAM 3
#define VIRTEX_5_CLK15_BRAM 3
#define VIRTEX_5_CLK16_BRAM 3
#define VIRTEX_5_CLK17_BRAM 3
#define VIRTEX_5_CLK00_DSP 1
#define VIRTEX_5_CLK01_DSP 1
#define VIRTEX_5_CLK02_DSP 1
#define VIRTEX_5_CLK03_DSP 1
#define VIRTEX_5_CLK04_DSP 1
#define VIRTEX_5_CLK05_DSP 1
#define VIRTEX_5_CLK06_DSP 1
#define VIRTEX_5_CLK07_DSP 1
#define VIRTEX_5_CLK10_DSP 0
#define VIRTEX_5_CLK11_DSP 0
#define VIRTEX_5_CLK12_DSP 0
#define VIRTEX_5_CLK13_DSP 0
#define VIRTEX_5_CLK14_DSP 0
#define VIRTEX_5_CLK15_DSP 0
#define VIRTEX_5_CLK16_DSP 0
#define VIRTEX_5_CLK17_DSP 0

#ifdef FPGA_VCU118
#define US_FPGA 1
#else
#define US_FPGA 0
#endif

#define MY_RAND() ((double)((double)rand() / (double)RAND_MAX))

#define init_clk_reg(id, pos, clb, bram, dsp, num_bram, num_dsp, pos_bram,     \
                     pos_dsp)                                                  \
    m_clk_reg[id].clk_reg_pos = pos;                                           \
    m_clk_reg[id].clb_per_column = clb;                                        \
    m_clk_reg[id].bram_per_column = bram;                                      \
    m_clk_reg[id].dsp_per_column = dsp;                                        \
    m_clk_reg[id].bram_num = num_bram;                                         \
    m_clk_reg[id].dsp_num = num_dsp;                                           \
    m_clk_reg[id].bram_pos = pos_bram;                                         \
    m_clk_reg[id].dsp_pos = pos_dsp;
