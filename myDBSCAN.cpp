#include "myDBSCAN.h"

namespace DBSCAN {
	double euclidean_distance(const Point& p1, const Point& p2) {
		return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p2.y - p1.y, 2) + std::pow(p2.z - p1.z, 2));
	}
	// 获取点p的ε邻域内的点
	std::vector<int> region_query(const std::vector<Point>& points, int p_idx, double eps) {
		std::vector<int> neighbors;
		for (int i = 0; i < points.size(); ++i) {
			if (euclidean_distance(points[p_idx], points[i]) <= eps) {
				neighbors.push_back(i);
			}
		}
		return neighbors;
	}
	// 扩展聚类
	void expand_cluster(std::vector<Point>& points, int p_idx, int cluster_id, double eps, int min_pts) {
		std::vector<int> seeds = region_query(points, p_idx, eps);
		if (seeds.size() < min_pts) {
			points[p_idx].cluster_id = 0;  // 标记为噪声
			return;
		}

		for (int i : seeds) {
			points[i].cluster_id = cluster_id;
		}
		//删除p_idx
		seeds.erase(std::remove(seeds.begin(), seeds.end(), p_idx), seeds.end());

		while (!seeds.empty()) {
			int current_p = seeds.back();
			seeds.pop_back();

			std::vector<int> result = region_query(points, current_p, eps);
			if (result.size() >= min_pts) {
				for (int i : result) {
					if (points[i].cluster_id == -1 || points[i].cluster_id == 0) {
						if (points[i].cluster_id == -1) {
							seeds.push_back(i);
						}
						points[i].cluster_id = cluster_id;
					}
				}
			}
		}
	}
	void myDBSCAN(vector<Point>& points, double eps, int min_pts) {
		int cluster_id = 1;
		for (int i = 0; i < points.size(); ++i) {
			if (points[i].cluster_id == -1) {
				expand_cluster(points, i, cluster_id, eps, min_pts);
				cluster_id++;
			}
		}
	}
}
