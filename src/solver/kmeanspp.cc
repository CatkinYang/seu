#include "solver/kmeanspp.h"
#include <Eigen/Dense>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

namespace seu {

auto kmeanspp::euclideanDistance(const TaskRef &t1, const TaskRef &t2)
    -> double {
    // if (!t1 || !t2) {
    //     throw std::runtime_error(
    //         "Null pointer encountered in euclideanDistance");
    // }
    Eigen::Vector3d v1(t1->getClb(), t1->getDsp(), t1->getBram());
    Eigen::Vector3d v2(t2->getClb(), t2->getDsp(), t2->getBram());
    return (v1 - v2).norm();
}

// K-Means++算法初始化聚类中心，选择初始的k个聚类中心
auto kmeanspp::initCentroidsKMeansPlusPlus(
    const std::unordered_map<int, TaskRef> &m_tasks,
    std::vector<TaskRef> &centroids, int k) -> void {
    std::vector<int> keys;
    for (const auto &pair : m_tasks) {
        keys.push_back(pair.first);
    }
    // 随机选择第一个聚类中心
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int first_index = std::rand() % keys.size();
    centroids.push_back(m_tasks.at(keys[first_index]));

    for (int i = 1; i < k; ++i) {
        std::vector<double> distances;
        double sum_distances = 0.0;
        for (const auto &pair : m_tasks) {
            auto task = pair.second;
            if (!task) {
                throw std::runtime_error(
                    "Null task encountered in initCentroidsKMeansPlusPlus");
            }
            double min_dist = std::numeric_limits<double>::max();
            for (auto centroid : centroids) {
                if (!centroid) {
                    throw std::runtime_error("Null centroid encountered in "
                                             "initCentroidsKMeansPlusPlus");
                }
                double dist = euclideanDistance(task, centroid);
                min_dist = std::min(min_dist, dist);
            }
            distances.push_back(min_dist);
            sum_distances += min_dist;
        }

        // 根据距离权重随机选择下一个聚类中心
        double random_value =
            static_cast<double>(std::rand()) / RAND_MAX * sum_distances;
        double cumulative_distance = 0.0;
        int chosen_index = 0;
        for (size_t j = 0; j < distances.size(); ++j) {
            cumulative_distance += distances[j];
            if (cumulative_distance >= random_value) {
                chosen_index = j;
                break;
            }
        }
        centroids.push_back(m_tasks.at(keys[chosen_index]));
    }
}

auto kmeanspp::assignTasks(const std::unordered_map<int, TaskRef> &m_tasks,
                           const std::vector<TaskRef> &centroids,
                           std::unordered_map<int, int> &assignments) -> void {
    for (const auto &pair : m_tasks) {
        auto task = pair.second;
        double min_dist = std::numeric_limits<double>::max();
        int cluster_idx = -1;
        for (size_t i = 0; i < centroids.size(); ++i) {
            double dist = euclideanDistance(task, centroids[i]);
            if (dist < min_dist) {
                min_dist = dist;
                cluster_idx = i;
            }
        }
        assignments[pair.first] = cluster_idx;
    }
}

auto kmeanspp::updateCentroids(const std::unordered_map<int, TaskRef> &m_tasks,
                               const std::unordered_map<int, int> &assignments,
                               std::vector<TaskRef> &centroids) -> void {
    int num_clusters = centroids.size();
    std::vector<int> count(num_clusters, 0);
    std::vector<TaskRef> new_centroids;
    new_centroids.resize(num_clusters);
    for (int i = 0; i < num_clusters; ++i) {
        new_centroids[i] = std::make_shared<Task>(i, 0, 0, 0, 0);
    }

    for (const auto &pair : m_tasks) {
        int cluster_idx = assignments.at(pair.first);
        auto task = pair.second;
        new_centroids[cluster_idx]->addClb(task->getClb());
        new_centroids[cluster_idx]->addDsp(task->getDsp());
        new_centroids[cluster_idx]->addBram(task->getBram());
        count[cluster_idx]++;
    }

    for (int i = 0; i < num_clusters; ++i) {
        if (count[i] > 0) {
            centroids[i] =
                std::make_shared<Task>(i, new_centroids[i]->getClb() / count[i],
                                       new_centroids[i]->getDsp() / count[i],
                                       new_centroids[i]->getBram() / count[i],
                                       0 // 执行时间暂时忽略
                );
        }
    }
}

auto kmeanspp::isConverged(const std::vector<TaskRef> &old_centroids,
                           const std::vector<TaskRef> &new_centroids,
                           double tolerance) -> bool {
    for (size_t i = 0; i < old_centroids.size(); ++i) {
        double dist = euclideanDistance(old_centroids[i], new_centroids[i]);
        if (dist > tolerance) {
            return false;
        }
    }
    return true;
}

auto kmeanspp::kMeansPlusPlusClustering(
    const std::unordered_map<int, TaskRef> &m_tasks,
    std::vector<TaskRef> &centroids, std::unordered_map<int, int> &assignments,
    double tolerance, int max_iterations) -> void {
    initCentroidsKMeansPlusPlus(m_tasks, centroids, centroids.size());
    int iteration = 0;
    std::vector<TaskRef> old_centroids;
    do {
        old_centroids = centroids;
        assignTasks(m_tasks, centroids, assignments);
        updateCentroids(m_tasks, assignments, centroids);
        iteration++;
    } while (!isConverged(old_centroids, centroids, tolerance) &&
             iteration < max_iterations);
}

} // namespace seu
