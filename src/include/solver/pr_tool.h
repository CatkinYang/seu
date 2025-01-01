#pragma once

#include "fine_grained.h"
#include "floorplan.h"
#include "fpga.h"
#include "gen_wrapper.h"
#include "marco.h"
#include "milp_solver_interface.h"
#include <filesystem>

namespace seu {

class pr_tool {
  public:
    std::string wrapper_top_name = "acc";
    unsigned int re_synthesis_after_wrap = 0;
    unsigned long num_rm_modules = 0;
    unsigned long num_rm_partitions;

    fpga_type type;
    string dart_path;

    // variables to manage project directory
    std::string Project_dir; //= "/home/holmes/test_pr_dir";
    std::string Src_path;    // = Project_dir + "/Sources";
    std::string ip_repo_path;
    std::string hdl_copy_path;    //= Src_path + "/hdl";
    std::string fplan_xdc_file;   // = Src_path + "/constraints/pblocks.xdc";
    std::string tcl_project;      // = Project_dir + "/Tcl";
    std::string synthesis_script; // = Project_dir + "/ooc_synth.tcl" ;
    std::string impl_script;      // = Project_dir + "/impl.tcl";
    std::string static_hw_script;
    std::string static_dir;
    std::string fred_dir;
    // pointer to an instance of flora
    floorplan *fl_inst = NULL;
    // input_to_flora in_flora;

    // void prep_input();
    void init_dir_struct();
    void prep_proj_directory();
    void create_vivado_project();
    string module_attributes(string ip_name, string module_name);
    void generate_synthesis_tcl(floorplan *fl);
    void run_vivado(std::string synth_script);
    void add_debug_probes(void);
    void parse_synthesis_report();
    void generate_impl_tcl(floorplan *fl);
    void generate_fred_files(floorplan *fptr);
    void generate_static_part(floorplan *fl);
    void synthesize_static();
    void generate_wrapper(floorplan *fptr);
    void generate_fred_device_tree(floorplan *fptr);

    // explicit pr_tool(input_to_pr *);
    explicit pr_tool(string);
    ~pr_tool();

  private:
    string dir_separator() {
        string s;
        s.assign(1, std::filesystem::path::preferred_separator);
        return s;
    }

    // split a string, or a path, by the delimiter
    vector<string> split(const string &text, char delimiter) {
        string tmp;
        vector<string> stk;
        std::stringstream ss(text);
        while (getline(ss, tmp, delimiter)) {
            stk.push_back(tmp);
        }
        return stk;
    }

    std::string exec(const char *cmd);
};

} // namespace seu
