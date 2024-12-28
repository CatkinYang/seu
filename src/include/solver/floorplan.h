#pragma once
#include "marco.h"
#include "milp_solver_interface.h"
#include "pynq/pynq.h"

namespace seu {

enum fpga_type { TYPE_ZYNQ = 0, TYPE_VIRTEX, TYPE_VIRTEX_5, TYPE_PYNQ };

using position_vec = vector<pos>;
using vec_2d = vector<vector<int>>;

struct slot {
    int clb;
    int bram;
    int dsp;
};
using slotRef = std::shared_ptr<slot>;

struct input_to_floorplan {
    int num_rm_partitions;
    string path_to_input;
    string path_to_output;
};
using itfRef = std::shared_ptr<input_to_floorplan>;

class floorplan {
  public:
    explicit floorplan(itfRef itf);
    ~floorplan();

    pynqRef pynq_inst;

    int num_rm_partitions = 0;

    fpga_type type;
    int connections = 0;
    msiRef solver;

    vector<int> clb_vector = vector<int>(MAX_SLOTS);
    vector<int> bram_vector = vector<int>(MAX_SLOTS);
    vector<int> dsp_vector = vector<int>(MAX_SLOTS);

    vector<int> clb_from_solver = vector<int>(MAX_SLOTS);
    vector<int> bram_from_solver = vector<int>(MAX_SLOTS);
    vector<int> dsp_from_solver = vector<int>(MAX_SLOTS);

    vector<string> cell_name = vector<string>(MAX_SLOTS);

    vector<slot> sl_array = vector<slot>(MAX_SLOTS);

    vec_2d connection_matrix =
        vector<vector<int>>(MAX_SLOTS, vector<int>(MAX_SLOTS, 0));

    vector<int> eng_x = vector<int>(MAX_SLOTS);
    vector<int> eng_y = vector<int>(MAX_SLOTS);
    vector<int> eng_w = vector<int>(MAX_SLOTS);
    vector<int> eng_h = vector<int>(MAX_SLOTS);

    vector<int> x_vector = vector<int>(MAX_SLOTS);
    vector<int> y_vector = vector<int>(MAX_SLOTS);
    vector<int> w_vector = vector<int>(MAX_SLOTS);
    vector<int> h_vector = vector<int>(MAX_SLOTS);

    vector<hw_task_allocation> alloc = vector<hw_task_allocation>(MAX_SLOTS);

    position_vec forbidden_region = position_vec(MAX_SLOTS);
    // position_vec forbidden_region_pynq = position_vec(MAX_SLOTS);
    // position_vec forbidden_region_virtex = position_vec(MAX_SLOTS);
    // position_vec forbidden_region_virtex_5 = position_vec(MAX_SLOTS);
    std::vector<int> get_units_per_task(int n, int n_units, int n_min,
                                        int n_max);
    itfRef floorplan_input;
    ptsRef param;
    pfsRef from_solver = std::make_shared<param_from_solver>(
        0, 0, &eng_x, &eng_y, &eng_w, &eng_h, &clb_from_solver,
        &bram_from_solver, &dsp_from_solver, &alloc);

    void clear_vectors();
    void resize_vectors(int n);
    void prep_input();
    void write_output(pfsRef from_solver);
    void start_optimizer();
    void generate_cell_name(int num_part, vector<string> *cell);
    void generate_xdc(string fplan_file_name);

    //    void init_fpga(enum fpga_type);
    //    void init_gui();
    //    void plot_rects(param_from_solver *);
    //    bool is_compatible(std::vector<slot> ptr, unsigned long slot_num, int
    //    max, unsigned long min, int type);
    vector<double> generate_slacks(Taskset &t, Platform &platform,
                                   double alpha);

    Taskset generate_taskset_one_HW_task_per_SW_task(
        uint n, Platform &p, const vector<double> &res_usage,
        double WCET_area_ratio, double max_area_usage);
};

} // namespace seu
