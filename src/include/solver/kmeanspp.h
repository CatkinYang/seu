#pragma once
#include "marco.h"
#include "task_manager.h"

namespace seu {

class kmeanspp {
  public:
    // 计算两个任务之间的欧式距离
    static auto euclideanDistance(const TaskRef &t1, const TaskRef &t2)
        -> double;

    // K-Means++算法初始化聚类中心，选择初始的k个聚类中心
    static auto
    initCentroidsKMeansPlusPlus(const std::unordered_map<int, TaskRef> &m_tasks,
                                std::vector<TaskRef> &centroids, int k) -> void;
    // 将任务分配到距离最近的聚类中心所在的类
    static auto assignTasks(const std::unordered_map<int, TaskRef> &m_tasks,
                            const std::vector<TaskRef> &centroids,
                            std::unordered_map<int, int> &assignments) -> void;

    // 更新聚类中心（计算每个类中任务的资源使用量平均值作为新的聚类中心
    static auto updateCentroids(const std::unordered_map<int, TaskRef> &m_tasks,
                                const std::unordered_map<int, int> &assignments,
                                std::vector<TaskRef> &centroids) -> void;

    // 判断聚类中心是否收敛（这里简单判断前后两次聚类中心的资源使用量是否变化不大）
    static auto isConverged(const std::vector<TaskRef> &old_centroids,
                            const std::vector<TaskRef> &new_centroids,
                            double tolerance) -> bool;

    // 执行K-Means++聚类算法
    static auto
    kMeansPlusPlusClustering(const std::unordered_map<int, TaskRef> &m_tasks,
                             std::vector<TaskRef> &centroids,
                             std::unordered_map<int, int> &assignments,
                             double tolerance, int max_iterations) -> void;
};

} // namespace seu
