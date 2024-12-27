#include "task_manager.h"
#include "utils.h"
#include "json/include/json/json.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>

namespace seu {

auto TaskManager::init_from_json(const std::string &filename) -> void {
    std::string root_path = Utils::get_project_root();
    std::string path = root_path + "/src/info/taskinfo/" + filename;
    std::ifstream jsonFile(path);
    if (!jsonFile.is_open()) {
        std::cerr << "TM Failed to open JSON file" << std::endl;
        return;
    }

    // 读取文件内容到字符串
    std::stringstream buffer;
    buffer << jsonFile.rdbuf();

    // 关闭文件
    jsonFile.close();

    // 解析 JSON 字符串
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(buffer.str(), root)) {
        std::cerr << "Failed to parse JSON: "
                  << reader.getFormattedErrorMessages() << std::endl;
        return;
    }

    const Json::Value &tasks = root["tasks"];
    for (const auto &task : tasks) {
        int id = task["id"].asInt();
        int clb = task["clb"].asInt();
        int dsp = task["dsp"].asInt();
        int bram = task["bram"].asInt();
        int exectime = task["exectime"].asInt();

        auto t = std::make_shared<Task>(id, clb, dsp, bram, exectime);
        m_tasks[id] = t;

        task_num++;
    }
    return;
}

auto TaskManager::init_from_random() -> void {
    auto n = random_int_gen(5, 25); // 任务节点数
    for (int i = 0; i < n; i++) {
        auto task = task_info_init();
        m_random_task.insert({m_taskid, task});
        m_taskid++;
    }
}

auto TaskManager::init_from_random(int n) -> void {
    for (int i = 0; i < n; i++) {
        auto task = task_info_init();
        m_random_task.insert({m_taskid, task});
        m_taskid++;
    }
}

auto TaskManager::task_info_init() -> TaskRef {
    auto clb = random_int_gen(2000, 3000);
    auto dsp = random_int_gen(0, 80);
    auto bram = random_int_gen(0, 80);
    auto exec = random_int_gen(5, 50);
    auto task = std::make_shared<Task>(m_taskid, clb, dsp, bram, exec);
    return task;
}

auto TaskManager::task_info_init_custom(std::pair<int, int> clb,
                                        std::pair<int, int> dsp,
                                        std::pair<int, int> bram,
                                        std::pair<int, int> exec) -> TaskRef {
    auto clbn = random_int_gen(clb.first, clb.second);
    auto dspn = random_int_gen(dsp.first, dsp.second);
    auto bramn = random_int_gen(bram.first, bram.second);
    auto execn = random_int_gen(exec.first, exec.second);
    auto task = std::make_shared<Task>(m_taskid, clbn, dspn, bramn, execn);
    m_taskid++;
    return task;
}

auto TaskManager::random_int_gen(int lower, int upper) -> int {
    std::random_device rd;  // 随机数生成器
    std::mt19937 gen(rd()); // 以随机设备作为种子的 Mersenne Twister 生成器
    std::uniform_int_distribution<> distr(lower,
                                          upper); //  均匀分布的整数分布
    return distr(gen);                            //
}

void TaskManager::TaskInfoPrint(std::unordered_map<int, TaskRef> TaskSet) {
    for (const auto &[k, v] : TaskSet) {
        std::cout << "task_id: " << k << "  resources: " << v->getClb() << " "
                  << v->getDsp() << " " << v->getBram() << std::endl;
    }
}

} // namespace seu
