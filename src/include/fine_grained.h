#pragma once
#include "marco.h"

namespace seu {

struct finegrained_res_description {
    int type_of_res;
    int slice_1;
    int slice_2;
};

using frd = finegrained_res_description;
using frdRef = unique_ptr<finegrained_res_description>;

struct slice_addres {
    unsigned long slice_x1;
    unsigned long slice_x2;
    unsigned long slice_y1;
    unsigned long slice_y2;
};

using slice = array<slice_addres, 4>;

class fine_grained {
  public:
    virtual ~fine_grained() = default;
    virtual void init_fine_grained() = 0;

    vector<frd> m_fg = {};
    string m_name = "fine_grained";
};

} // namespace seu
