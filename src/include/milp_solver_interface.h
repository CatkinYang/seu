#pragma once
#include "fpga.h"
#include "marco.h"
#include "partition.h"

namespace seu {

using Vec = vector<int>;
using Vec2d = vector<Vec>;
using Vecpos = vector<pos>;

struct hw_task_allocation {
    int num_tasks_in_part;
    int num_hw_tasks_in_part;
    vector<int> task_id;
};

struct param_to_solver {
  public:
    param_to_solver(int num_rm_partitions, int num_forbidden_slots,
                    int num_rows, int width, int num_connected_slots,
                    int num_clk_regs, int clb_per_tile, int bram_per_tile,
                    int dsp_per_tile, Vec *clb, Vec *bram, Vec *dsp,
                    Vec2d *conn_vector, Vecpos *fbdn_slot, Taskset *task_set,
                    Platform *platform, vector<double> *slacks)
        : num_rm_partitions(num_rm_partitions),
          num_forbidden_slots(num_forbidden_slots), num_rows(num_rows),
          width(width), num_connected_slots(num_connected_slots),
          num_clk_regs(num_clk_regs), clb_per_tile(clb_per_tile),
          bram_per_tile(bram_per_tile), dsp_per_tile(dsp_per_tile), clb(clb),
          bram(bram), dsp(dsp), conn_vector(conn_vector), fbdn_slot(fbdn_slot),
          task_set(task_set), platform(platform), slacks(slacks) {}
    param_to_solver() = default;

  public:
    int num_rm_partitions;
    int num_forbidden_slots;
    int num_rows;
    int width;
    int num_connected_slots;
    int num_clk_regs;
    int clb_per_tile;
    int bram_per_tile;
    int dsp_per_tile;
    Vec *clb;
    Vec *bram;
    Vec *dsp;
    Vec2d *conn_vector;
    Vecpos *fbdn_slot;
    Taskset *task_set;
    Platform *platform;
    vector<double> *slacks;
};
using ptsRef = std::shared_ptr<param_to_solver>;

struct param_from_solver {
  public:
    param_from_solver(int num_partition, int max_modules_per_partition,
                      vector<int> *x, vector<int> *y, vector<int> *w,
                      vector<int> *h, vector<int> *clb_from_solver,
                      vector<int> *bram_from_solver,
                      vector<int> *dsp_from_solver,
                      vector<hw_task_allocation> *task_alloc)
        : num_partition(num_partition),
          max_modules_per_partition(max_modules_per_partition), x(x), y(y),
          w(w), h(h), clb_from_solver(clb_from_solver),
          bram_from_solver(bram_from_solver), dsp_from_solver(dsp_from_solver),
          task_alloc(task_alloc) {}

  public:
    int num_partition;
    int max_modules_per_partition;
    vector<int> *x;
    vector<int> *y;
    vector<int> *w;
    vector<int> *h;
    vector<int> *clb_from_solver;
    vector<int> *bram_from_solver;
    vector<int> *dsp_from_solver;
    vector<hw_task_allocation> *task_alloc;
};
using pfsRef = std::shared_ptr<param_from_solver>;

class milp_solver_interface {
  public:
    virtual int start_optimizer(pfsRef pfs, ptsRef pts) = 0;
    virtual ~milp_solver_interface() = default;

    pfsRef m_pfs;
    ptsRef m_pts;
};
using msiRef = shared_ptr<milp_solver_interface>;

class milp_solver_pynq : public milp_solver_interface {
  public:
    virtual int start_optimizer(pfsRef pfs, ptsRef pts) override;
    int solve_milp_pynq(pfsRef to_sim);
};

// class milp_solver_zynq : public milp_solver_interface {
//   public:
//     virtual int start_optimizer(pfsRef pfs, ptsRef pts) override;
//     int solve_milp_zynq(pfsRef to_sim);
// };

// class milp_solver_vc707 : public milp_solver_interface {
//   public:
//     virtual int start_optimizer(pfsRef pfs, ptsRef pts) override;
//     int solve_milp_vc707(pfsRef to_sim);
// };

// class milp_solver_vcu118 : public milp_solver_interface {
//   public:
//     virtual int start_optimizer(pfsRef pfs, ptsRef pts) override;
//     int solve_milp_vcu118(pfsRef to_sim);
// };

} // namespace seu
