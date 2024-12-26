#pragma once
#include "task.h"
#include <unordered_map>
#include <vector>

namespace seu {

class TaskInit {
  public:
    TaskInit() = default;

    // 从json环境中生成任务
    auto init_from_json(const std::string &filename) -> void;

    auto init_from_random() -> void;
    auto task_info_init() -> TaskRef;
    auto task_info_init_custom(std::pair<int, int> clb, std::pair<int, int> dsp,
                               std::pair<int, int> bram,
                               std::pair<int, int> exec) -> TaskRef;
    static auto random_int_gen(int lower, int upper) -> int;
    static auto generate_normal_distribution(double mean, double stddev,
                                             int size) -> std::vector<double>;

    auto getRandomTask() -> std::unordered_map<int, TaskRef> {
        return m_random_task;
    };
    auto getJsonTask() -> std::unordered_map<int, TaskRef> { return m_tasks; };

  private:
    // init from json
    int task_num = 0;
    std::unordered_map<int, TaskRef> m_tasks; // 存储id和Task之间的映射关系
    // init from random
    int m_taskid = 0;
    std::unordered_map<int, TaskRef> m_random_task; // key:id, value:task
};

} // namespace seu
