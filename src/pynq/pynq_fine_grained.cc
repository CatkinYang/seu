#include "pynq/pynq_fine_grained.h"

namespace seu {

pynq_fine_grained::pynq_fine_grained() {
    m_name = "pynq_fine_grained";
    init_fine_grained();
}

void pynq_fine_grained::init_fine_grained() {
    m_fg.resize(PYNQ_WIDTH);
    m_fg = {
        {CLB, 0, 1},     {CLB, 2, 3},     {CLB, 4, 5},     {CLB, 6, 7},
        {BRAM, 0, 0},    {CLB, 8, 9},     {CLB, 10, 11},   {DSP, 0, 0},
        {CLB, 12, 13},   {CLB, 14, 15},   {CLB, 16, 17},   {CLB, 18, 19},
        {DSP, 1, 1},     {CLB, 20, 21},   {CLB, 22, 23},   {BRAM, 1, 1},
        {CLB, 24, 25},   {CLB, 26, 27},   {CLB, 28, 29},   {CLB, 30, 31},
        {BRAM, 2, 2},    {CLB, 32, 33},   {CLB, 34, 35},   {DSP, 2, 2},
        {CLB, 36, 37},   {CLB, 38, 39},   {CLB, 40, 41},   {CLB, 42, 43},
        {CLB, 44, 45},   {CLB, 46, 47},   {CLB, 48, 49},   {CENTRAL_CLK, 0, 1},
        {CLB, 50, 51},   {CLB, 52, 53},   {BRAM, 3, 3},    {CLB, 54, 55},
        {CLB, 56, 57},   {CLB, 58, 59},   {CLB, 60, 61},   {CLB, 62, 63},
        {CLB, 64, 65},   {CLB, 66, 67},   {CLB, 68, 69},   {CLB, 70, 71},
        {CLB, 72, 73},   {CLB, 74, 75},   {CLB, 76, 77},   {CLB, 78, 79},
        {FBDN, 0, 1},    {CLB, 80, 81},   {CLB, 82, 83},   {CLB, 84, 85},
        {CLB, 86, 87},   {CLB, 88, 89},   {BRAM, 4, 4},    {CLB, 90, 91},
        {CLB, 92, 93},   {DSP, 3, 3},     {CLB, 94, 95},   {CLB, 96, 97},
        {CLB, 98, 99},   {CLB, 100, 101}, {DSP, 4, 4},     {CLB, 102, 103},
        {CLB, 104, 105}, {BRAM, 5, 5},    {CLB, 106, 107}, {CLB, 108, 109},
        {CLB, 110, 111}, {CLB, 112, 113}};
}

} // namespace seu
