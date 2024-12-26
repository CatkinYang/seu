#include "utils.h"
#include <filesystem>
#include <fstream>
#include <limits.h>
#include <sstream>
#include <string>
#include <unistd.h>

namespace seu {

namespace fs = std::filesystem;

auto Utils::get_current_working_directory() -> std::string {
    char buf[PATH_MAX];
    if (getcwd(buf, sizeof(buf)) != NULL) {
        return std::string(buf);
    } else {
        return std::string();
    }
}

auto Utils::get_project_root() -> std::string {
    fs::path p = fs::current_path();
    while (p != p.root_path() && !fs::exists(p / "CMakeLists.txt")) {
        p = p.parent_path();
    }
    return p.string();
}

} // namespace seu
