#include "solver/floorplan.h"
#include "csvdata.h"
#include "generate_xdc.h"
#include "milp_solver_interface.h"
#include "pynq/pynq_fine_grained.h"
#include <memory>
#include <yaml-cpp/yaml.h>

namespace seu {

extern YAML::Node config;

char const *fpga_board_name[] = {
    "Zybo board", "Pynq board", "ultrascale ZCU-102 board", "ultra96-v2 board"};

char const *fpga_device_name[] = {"xc7z010clg400-1", "xc7z020clg400-1",
                                  "xczu9eg-ffvb1156-2-e",
                                  "xczu3eg-sbva484-1-i"};
// const fpga_type ftype = TYPE_PYNQ;

floorplan::floorplan() {
    num_rm_modules = config["dart"]["hw_ips"].size();
    cout << endl << "PR_TOOL: reading inputs " << num_rm_modules << endl;
    platform = new Platform(3);
    task_set = new Taskset(num_rm_modules, num_rm_modules, *platform);
    cout << "FLORA: num of slots **** " << num_rm_modules << endl;
}

floorplan::~floorplan() { cout << "floorplan: destruction " << endl; }

// Prepare the input
void floorplan::clear_vectors() {
    clb_vector.clear();
    bram_vector.clear();
    dsp_vector.clear();
    cell_name.clear();

    eng_x.clear();
    eng_y.clear();
    eng_w.clear();
    eng_h.clear();

    x_vector.clear();
    y_vector.clear();
    w_vector.clear();
    h_vector.clear();
}

void floorplan::resize_vectors(int n) {

    clb_vector.resize(n);
    bram_vector.resize(n);
    dsp_vector.resize(n);
    cell_name.resize(n);

    eng_x.resize(n);
    eng_y.resize(n);
    eng_w.resize(n);
    eng_h.resize(n);

    x_vector.resize(n);
    y_vector.resize(n);
    w_vector.resize(n);
    h_vector.resize(n);
}

void floorplan::prep_input() {
    int i, clbs, brams, dsps;

    cout << endl << "FLORA: resource requirement of the input slots " << endl;
    cout << "\t clb " << " \t bram " << "\t dsp " << endl;

    for (i = 0; i < num_rm_modules; i++) {
        clbs = config["dart"]["hw_ips"][i]["CLBs"].as<int>();
        brams = config["dart"]["hw_ips"][i]["BRAMs"].as<int>();
        dsps = config["dart"]["hw_ips"][i]["DSPs"].as<int>();

        cout << "slot " << i;
        clb_vector[i] = clbs;
        bram_vector[i] = brams;
        dsp_vector[i] = dsps;

        HW_WCET[i] = config["dart"]["hw_ips"][i]["wcet"].as<int>();
        slacks[i] = config["dart"]["hw_ips"][i]["slack_time"].as<int>();

        cout << "\t " << clb_vector[i] << "\t " << bram_vector[i] << "\t "
             << dsp_vector[i] << endl;
    }
}

void floorplan::start_optimizer() {
    int i;
    param->bram = &bram_vector;
    param->clb = &clb_vector;
    param->dsp = &dsp_vector;
    param->num_rm_modules = num_rm_modules;

    param->num_connected_slots = connections;
    param->conn_vector = &connection_matrix;

    for (i = 0; i < num_rm_modules; i++) {
        task_set->HW_Tasks[i].resDemand[CLB] = clb_vector[i];
        task_set->HW_Tasks[i].resDemand[BRAM] = bram_vector[i];
        task_set->HW_Tasks[i].resDemand[DSP] = dsp_vector[i];
        task_set->HW_Tasks[i].WCET = HW_WCET[i];
        task_set->SW_Tasks[i].H.push_back(i);
        task_set->HW_Tasks[i].SW_Task_ID = i;
    }

    double alpha = 2.0;
    double tmp2[] = {alpha, alpha, alpha};
    vector<double> res_usage(tmp2, tmp2 + 3);

    task_set->print();

    // slacks = generate_slacks(*task_set, *platform, utilization);
    param->task_set = task_set;
    param->platform = platform;
    param->slacks = &slacks;

    pynq_inst = std::make_shared<pynq>();
    solver = std::make_shared<milp_solver_pynq>();
    for (i = 0; i < pynq_inst->m_num_forbidden_slots; i++) {
        forbidden_region[i] = pynq_inst->forbidden_pos[i];
        // cout<< " fbdn" << forbidden_region[i].x << endl;
    }

    param->num_forbidden_slots = pynq_inst->m_num_forbidden_slots;
    param->num_rows = pynq_inst->m_num_rows;
    param->width = pynq_inst->m_width;
    param->fbdn_slot = &forbidden_region;
    param->num_clk_regs = pynq_inst->m_num_clk_reg / 2;
    param->clb_per_tile = PYNQ_CLB_PER_TILE;
    param->bram_per_tile = PYNQ_BRAM_PER_TILE;
    param->dsp_per_tile = PYNQ_DSP_PER_TILE;

    platform->maxFPGAResources[CLB] = PYNQ_CLB_TOT;
    platform->maxFPGAResources[BRAM] = PYNQ_BRAM_TOT;
    platform->maxFPGAResources[DSP] = PYNQ_DSP_TOT;

    platform->recTimePerUnit[CLB] = 1.0 / 4500.0;
    platform->recTimePerUnit[BRAM] = 1.0 / 4500.0;
    platform->recTimePerUnit[DSP] = 1.0 / 4000.0;

    cout << "FLORA: starting PYNQ MILP optimizer " << endl;
    solver->start_optimizer(from_solver, param);
    cout << "FLORA: finished MILP optimizer " << endl;
}

void floorplan::generate_cell_name(unsigned long num_part) {
    int i;
    vector<std::string> *cell = &(this->cell_name);
    for (i = 0; i < (int)num_part; i++)
        (*cell)[i] = "dart_i/acc_" + to_string(i) + "/inst";
}

void floorplan::generate_xdc(std::string fplan_xdc_file) {
    auto from_sol_ptr = from_solver;
    pfgRef fg_pynq_instance = std::make_shared<pynq_fine_grained>();

    generate_xdc_file(fg_pynq_instance, from_sol_ptr, param,
                      from_solver->num_partition, cell_name, fplan_xdc_file);
}

vector<int> floorplan::get_units_per_task(int n, int n_units, int n_min,
                                          int n_max) {
    vector<int> ret;
    double rand_dbl;

    uint n_units_sum = n_units, n_units_next = 0;

    for (uint i = 0; i < (uint)n - 1; i++) {
        srand(time(0));
        rand_dbl = pow(MY_RAND(), (1.0 / (double)(n - i - 1)));
        n_units_next = floor((double)n_units_sum * rand_dbl);
        // cout << n_units_next << " " << rand_dbl << endl;
        //  --------- LIMIT Task Utilization --------------
        if (n_units_next > (n_units_sum - n_min))
            n_units_next = n_units_sum - n_min;

        if (n_units_next < ((n - i - 1) * n_min))
            n_units_next = (n - i - 1) * n_min;

        if ((n_units_sum - n_units_next) > (uint)n_max)
            n_units_next = n_units_sum - n_max;
        // ------------------------------------------------

        ret.push_back(n_units_sum - n_units_next);
        n_units_sum = n_units_next;
    }

    ret.push_back(n_units_sum);
    return ret;
}

vector<double> floorplan::generate_slacks(Taskset &t, Platform &platform,
                                          double alpha) {
    std::vector<double> maxSlotSizePerResource;

    for (uint x = 0; x < platform.N_FPGA_RESOURCES; x++) {
        double tmp = 0;
        for (uint a = 0; a < t.maxHW_Tasks; a++)
            tmp = max(tmp, t.HW_Tasks[a].resDemand[x]);

        maxSlotSizePerResource.push_back(tmp);
        // cout << "MAX_SIZE[" << x << "] = " << tmp << endl;
    }

    std::vector<double> SLACK_BOUND;

    double max_delay = 1;

    for (uint a = 0; a < t.maxHW_Tasks; a++)
        for (uint x = 0; x < platform.N_FPGA_RESOURCES; x++)
            max_delay += maxSlotSizePerResource[x] * platform.recTimePerUnit[x];

    for (uint b = 0; b < t.maxHW_Tasks; b++)
        max_delay += t.HW_Tasks[b].WCET;

    // cout << "MAX DELAY = " << max_delay << endl;

    for (uint a = 0; a < t.maxHW_Tasks; a++) {
        const double min_delay =
            t.HW_Tasks[a].WCET + alpha * (max_delay - t.HW_Tasks[a].WCET);
        SLACK_BOUND.push_back(min_delay + (max_delay - min_delay) * MY_RAND());
        cout << "slack " << a << " " << SLACK_BOUND[a] << endl;
    }

    return SLACK_BOUND;
}

vector<uint> get_units_per_task(uint n, uint n_units, uint n_min, uint n_max) {
    vector<uint> ret;

    uint n_units_sum = n_units, n_units_next = 0;

    for (uint i = 0; i < n; i++) {

        n_units_next = floor((double)n_units_sum *
                             pow(MY_RAND(), (1.0 / (double)(n - i - 1))));

        // --------- LIMIT Task Utilization --------------
        if (n_units_next > (n_units_sum - n_min))
            n_units_next = n_units_sum - n_min;

        if (n_units_next < ((n - i - 1) * n_min))
            n_units_next = (n - i - 1) * n_min;

        if ((n_units_sum - n_units_next) > n_max)
            n_units_next = n_units_sum - n_max;
        // ------------------------------------------------

        ret.push_back(n_units_sum - n_units_next);
        n_units_sum = n_units_next;
    }

    //    ret.push_back(n_units_sum);

    return ret;
}

vector<HW_Task_t> generate_HW_tasks(uint n, Platform &p,
                                    const vector<double> &res_usage,
                                    double WCET_area_ratio,
                                    double max_area_usage) {
    const uint n_res = p.maxFPGAResources.size();
    uint temp[5] = {10, 10, 0};

    vector<HW_Task_t> HW_Tasks;

    // cout <<"in generate HW task "<< p.maxFPGAResources.size() << " " <<
    // res_usage.size() <<endl;
    if (res_usage.size() != n_res)
        return HW_Tasks;

    // Prepare task set
    for (uint i = 0; i < n; i++) {
        HW_Task_t new_task(p);
        HW_Tasks.push_back(new_task);
    }

    // For each FPGA resource
    for (uint x = 0; x < n_res; x++) {
        const uint n_units = floor(res_usage[x] * p.maxFPGAResources[x]);

        vector<uint> res_per_task = get_units_per_task(
            n, n_units, temp[x], max_area_usage * p.maxFPGAResources[x]);

        cout << endl << endl << "pritning res reqmt " << endl;
        for (uint i = 0; i < n; i++) {
            HW_Tasks[i].resDemand[x] = res_per_task[i];
            // cout << " Res"<<i<< " " <<res_per_task[i];
        }

        cout << endl;
    }

    // For each HW-task
    for (uint i = 0; i < n; i++) {
        uint acc = 0;
        for (uint x = 0; x < n_res; x++)
            acc += (uint)HW_Tasks[i].resDemand[x];
        // FIXME
        // const uint lb = (uint)floor(acc/WCET_area_ratio*(1.0-VAR));
        // const uint ub = (uint)floor(acc/WCET_area_ratio*(1.0+VAR));
        const uint lb = 5;
        const uint ub = 500;
        HW_Tasks[i].WCET = lb + (ub - lb) * MY_RAND();
        cout << "acc: " << acc << endl;
        cout << "WCET " << i << HW_Tasks[i].WCET << endl;
    }

    return HW_Tasks;
}

Taskset floorplan::generate_taskset_one_HW_task_per_SW_task(
    uint n, Platform &p, const vector<double> &res_usage,
    double WCET_area_ratio, double max_area_usage)

{
    Taskset t(n, n, p);
    vector<HW_Task_t> HW_Tasks =
        generate_HW_tasks(n, p, res_usage, WCET_area_ratio, max_area_usage);

    if (HW_Tasks.size() != n)
        return t;

    // Fill the task set with the HW-tasks

    for (uint i = 0; i < n; i++) {
        t.HW_Tasks[i] = HW_Tasks[i];
        // i=th HW-Task <---> i-th SW-Task
        t.SW_Tasks[i].H.push_back(i);
        t.HW_Tasks[i].SW_Task_ID = i;
    }

    cout << "in generate HW task" << endl;
    return t;
}

} // namespace seu
