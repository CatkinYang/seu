#include "task.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

namespace seu {

Task::Task(int id, int clb, int dsp, int bram)
    : m_id(id), m_clb(clb), m_dsp(dsp), m_bram(bram) {
    m_children.clear();
    m_parent.clear();
    m_status = TaskStatus::WAITING;
}

Task::Task(int id, int clb, int dsp, int bram, int exec)
    : m_id(id), m_clb(clb), m_dsp(dsp), m_bram(bram), m_exectime(exec) {
    m_children.clear();
    m_parent.clear();
    m_status = TaskStatus::WAITING;
}

} // namespace seu
