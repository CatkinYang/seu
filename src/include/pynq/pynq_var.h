
#pragma once

#include "marco.h"
#include "milp_solver_interface.h"

namespace seu {

using GRBVarArray = vector<GRBVar>;
using GRBVar2DArray = vector<GRBVarArray>;
using GRBVar3DArray = vector<GRBVar2DArray>;
using GRBVar4DArray = vector<GRBVar3DArray>;

static int H, W;                // 高，宽
static int num_slots;           // 槽数
static int num_rows;            // 行数
static int num_forbidden_slots; // 禁区槽数
static int BIG_M = 100000000;
static int num_clk_regs; // 时钟区域

static int status;     // 状态
static int delta_size; // delta大小

static int clb_max = 30000;
static int bram_max = 1000;
static int dsp_max = 1000;

static int clb_per_tile;
static int bram_per_tile;
static int dsp_per_tile;

// 浪费的资源
static int wasted_clb_pynq, wasted_bram_pynq, wasted_dsp_pynq;

static vector<int> clb_req_pynq(MAX_SLOTS);
static vector<int> bram_req_pynq(MAX_SLOTS);
static vector<int> dsp_req_pynq(MAX_SLOTS);

static vector<vector<int>> conn_matrix_pynq =
    vector<vector<int>>(MAX_SLOTS, vector<int>(MAX_SLOTS, 0));

static int num_conn_slots_pynq;

const int num_fbdn_edge = 11;
vector<int> beta_fbdn = {0, 1, 1};
vector<int> forbidden_boundaries_right = {8,  13, 23, 58, 63, 5,
                                          16, 21, 35, 55, 66};
vector<int> forbidden_boundaries_left = {7,  12, 22, 57, 62, 4,
                                         15, 20, 34, 54, 65};

static Vecpos fs_pynq(MAX_SLOTS);

} // namespace seu
