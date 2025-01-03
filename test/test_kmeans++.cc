#include "solver/kmeanspp.h"
#include "task.h"
#include "task_manager.h"
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

int main() {
    auto TM = std::make_shared<seu::TaskManager>();
    // TM->init_from_random(20);
    // 随机生成
    TM->init_from_json("test.json");
    auto random_task_set = TM->getJsonTask();
    // seu::TaskManager::TaskInfoPrint(random_task_set);
    std::vector<seu::TaskRef> centroids;
    std::unordered_map<int, int> assignments;
    double tolerance = 0.1;
    int max_iterations = 100;
    int k = 5;

    centroids.resize(k);
    for (int i = 0; i < k; ++i) {
        centroids[i] =
            std::make_shared<seu::Task>(i, 0, 0, 0, 0); // 初始化每个聚类中心
    }
    std::cout << "begin cluster" << std::endl;
    seu::kmeanspp::kMeansPlusPlusClustering(
        random_task_set, centroids, assignments, tolerance, max_iterations);

    std::unordered_map<int, std::vector<int>> clusterToTask;
    for (const auto &pair : assignments) {
        clusterToTask[pair.second].push_back(pair.first);
    }

    for (const auto &[k, v] : clusterToTask) {
        std::cout << "cluster " << k << std::endl;
        for (auto i : v) {
            std::cout << "task id: " << i
                      << " resouces: " << random_task_set.at(i)->getClb() << " "
                      << random_task_set.at(i)->getDsp() << " "
                      << random_task_set.at(i)->getBram() << std::endl;
        }
    }
    auto cluster_info = seu::kmeanspp::getClusterMaxResourcesNumber(
        clusterToTask, random_task_set);

    std::ofstream file("output.csv");

    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    // 遍历cluster_info并写入文件
    for (const auto &pair : cluster_info) {
        int k = pair.first;
        const auto &v = pair.second;
        file << std::get<0>(v) << "," << std::get<1>(v) << "," << std::get<2>(v)
             << "," << k << std::endl;
    }

    // 关闭文件
    file.close();

    std::cout << "Data has been written to output.csv" << std::endl;

    return 0;
}
