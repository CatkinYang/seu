#include "solver/floorplan.h"
#include "csvdata.h"
#include "generate_xdc.h"
#include "milp_solver_interface.h"
#include "pynq/pynq_fine_grained.h"
#include <memory>

namespace seu {

floorplan::floorplan(itfRef itf) {
    floorplan_input = itf;
    if (floorplan_input->num_rm_partitions > 0) {
        num_rm_partitions = floorplan_input->num_rm_partitions;
        //        type = floorplan_input->type_of_fpga;
        num_rm_partitions = floorplan_input->num_rm_partitions;

        cout << "floorplan: num of partitions **** " << num_rm_partitions
             << endl;
        //        cout << "floorplan: type of FPGA **** " << type <<endl;
        cout << "floorplan: path for input **** "
             << floorplan_input->path_to_input << endl;
    } else {
        cout << "floorplan: The number of Reconfigurable modules > 0";
        exit(-1);
    }
    param = std::make_shared<param_to_solver>();
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
    // int row, col;
    int i, k;
    int ptr;
    string str;
    CSVData csv_data(floorplan_input->path_to_input);

    // row = csv_data.rows();
    // col = csv_data.columns();

    cout << endl
         << "floorplan: resource requirement of the input slots " << endl;
    cout << "\t clb " << " \t bram " << "\t dsp " << endl;
    // 确保向量容量足够

    if (clb_vector.size() < (size_t)num_rm_partitions ||
        bram_vector.size() < (size_t)num_rm_partitions ||
        dsp_vector.size() < (size_t)num_rm_partitions ||
        cell_name.size() < (size_t)num_rm_partitions) {
        std::cerr << "Error: Vector capacity is not sufficient." << std::endl;
        return;
    }

    for (i = 0, ptr = 0, k = 0; i < num_rm_partitions; i++, ptr++) {
        str = csv_data.get_value(i, k++);
        clb_vector[ptr] = std::stoi(str);

        str = csv_data.get_value(i, k++);
        bram_vector[ptr] = std::stoi(str);

        str = csv_data.get_value(i, k++);
        dsp_vector[ptr] = std::stoi(str);

        str = csv_data.get_value(i, k++);
        cell_name[i] = str;
        k = 0;

        cout << "\t " << clb_vector[ptr] << "\t " << bram_vector[ptr] << "\t "
             << dsp_vector[ptr] << endl;
    }
}

void floorplan::write_output(pfsRef from_solver) {
    // int row, col;
    int i, k;
    unsigned int ptr;
    string str;
    CSVData csv_data_in(floorplan_input->path_to_input);
    CSVData csv_data_out(floorplan_input->path_to_input);

    cout << endl << "floorplan: writing resource inside slots " << endl;
    cout << "\t clb " << " \t bram " << "\t dsp " << endl;
    for (i = 0, ptr = 0, k = 0; i < num_rm_partitions; i++, ptr++) {
        str = to_string((*from_solver->clb_from_solver)[i]);
        csv_data_out.set_value(i, k++, str);
        // cout << "clb after floorplan " << str <<endl;

        str = to_string((*from_solver->bram_from_solver)[i]);
        csv_data_out.set_value(i, k++, " " + str);
        // cout << "bram after floorplan " << str <<endl;

        str = to_string((*from_solver->dsp_from_solver)[i]);
        csv_data_out.set_value(i, k++, " " + str);
        // cout << "dsp after floorplan " << str <<endl;

        str = csv_data_in.get_value(i, k);
        csv_data_out.set_value(i, k++, " " + str);
        // cout << "cell after floorplan " << str <<endl;

        str = csv_data_in.get_value(i, k);
        csv_data_out.set_value(i, k++, " " + str);
        // cout << "cell after floorplan " << str <<endl;

        k = 0;
    }
    csv_data_out.write_data(floorplan_input->path_to_output);
}

void floorplan::start_optimizer() {
    param->bram = &bram_vector;
    param->clb = &clb_vector;
    param->dsp = &dsp_vector;
    param->num_rm_partitions = num_rm_partitions;
    param->num_connected_slots = connections;
    param->conn_vector = &connection_matrix;

    pynq_inst = std::make_shared<pynq>();
    solver = std::make_shared<milp_solver_pynq>();
    for (int i = 0; i < pynq_inst->m_num_forbidden_slots; i++) {
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

    cout << "floorplan: starting PYNQ MILP optimizer " << endl;
    solver->start_optimizer(from_solver, param);
    cout << "floorplan: finished MILP optimizer " << endl;
}

void floorplan::generate_cell_name(int num_part, vector<string> *cell) {
    int i;
    for (i = 0; i < num_part; i++)
        //        (*cell)[i] = "design_1_i/hw_task_0_" + to_string(i) + "/inst";
        //        (*cell)[i] = "hdmi_out_i/slot_" + to_string(i) + "_0";
        (*cell)[i] = "system_i/slot_p0_s" + to_string(i);
}

void floorplan::generate_xdc(std::string fplan_xdc_file) {
    pfsRef from_sol_ptr = from_solver;

    pfgRef fg_pynq_instance = std::make_shared<pynq_fine_grained>();
    // generate_cell_name(num_rm_partitions, &cell_name);
    generate_xdc_file(fg_pynq_instance, from_sol_ptr, param, num_rm_partitions,
                      cell_name, fplan_xdc_file);
}
} // namespace seu
