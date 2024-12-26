#pragma once

#include <limits.h>
#include <string>
#include <unistd.h>

namespace seu {

class Utils {
  public:
    static auto get_current_working_directory() -> std::string;
    static auto get_project_root() -> std::string;
};

} // namespace seu
