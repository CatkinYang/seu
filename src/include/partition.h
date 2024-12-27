#pragma once

#include "marco.h"

namespace seu {

class Platform {
  public:
    Platform(unsigned int nres)
        : N_FPGA_RESOURCES(nres), maxFPGAResources(nres), recTimePerUnit(nres) {
    }

  public:
    unsigned int N_FPGA_RESOURCES;
    /* Number of FPGA resources (for each type) */
    vector<unsigned int> maxFPGAResources;
    /* Reconfiguration time for one unit of resource */
    vector<double> recTimePerUnit;
};

class HW_Task_t {
  public:
    HW_Task_t(Platform &p) { resDemand.resize(p.N_FPGA_RESOURCES); }

  public:
    double WCET;
    vector<double> resDemand;
    unsigned int SW_Task_ID;
};

class SW_Task_t {
  public:
    vector<unsigned int> H;
    // vector<double> WCETs;
};

class Taskset {
  public:
    Taskset(unsigned int nhwt, unsigned int nswt, Platform &p)
        : HW_Tasks(nhwt, HW_Task_t(p)), SW_Tasks(nswt) {
        maxHW_Tasks = nhwt;
        maxSW_Tasks = nswt;
        maxPartitions = nhwt;
        /* TODO: This can be improved */
        maxSlotsPerPartition = nhwt;
    }

  public:
    unsigned int maxPartitions;
    unsigned int maxSlotsPerPartition;
    unsigned int maxHW_Tasks;
    unsigned int maxSW_Tasks;

    vector<HW_Task_t> HW_Tasks;
    vector<SW_Task_t> SW_Tasks;

    void print();
};
} // namespace seu
