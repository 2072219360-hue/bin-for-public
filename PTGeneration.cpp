// PTGeneration.cpp
#include "PTGeneration.h"
#include <cmath>

std::pair<double, double> PTGeneration::getPT(double h) {//单位为km
	h *= 1000;

	double temperature = -1;
	double pressure = -1;

	// 对流层内的温度和气压计算（0 到 11 km 高度）
	if (h >= 0 && h <= 11000) {
		// 计算温度（K）
		temperature = T0 - L * h;

		// 计算气压（Pa）
		double exponent = (g0 * M) / (R * L);
		pressure = P0 * pow(1 - (L * h) / T0, exponent);
	}
	// 平流层内的温度和气压计算（11 到 25 km 高度）
	else if (h > 11000 && h <= 25000) {
		// 平流层温度保持常值 T1
		temperature = T1;

		// 计算气压（Pa）
		double exponent = (g0 * M) / (R * L);
		pressure = P1 * pow(1 - (L * 11000) / T0, exponent);
	}
	// 中层的温度和气压计算（25 km 到 85 km）
	else if (h > 25000 && h <= 85000) {
		// 中层的温度递减公式（简化处理）
		temperature = T1 - (h - 25000) * 2.8 / 1000;

		// 计算气压（Pa）
		pressure = P1 * exp(-g0 * M * (h - 25000) / (R * T1));
	}
	// 热层的温度和气压计算（85 km 以上）
	else if (h > 85000) {
		// 热层的温度大幅上升（假设温度增加）
		temperature = 1800 + 0.002 * (h - 85000);

		// 计算气压（Pa），此范围内气压变化非常剧烈
		pressure = 0.0;  // 高度达到热层后，气压趋近于 0
	}
	else {
		std::cerr << "高度超出有效范围" << std::endl;
	}

	temperature = tempCal.kelvinToCelsius(temperature);
	pressure = pressure / 100;


	// 如果有指定高度的压力值，则使用映射中的值
	if (hp.find(h) != hp.end()) {
		pressure = hp[h];
	}

	// 如果有指定高度的温度值，则使用映射中的值
	if (ht.find(h) != ht.end()) {
		temperature = ht[h];
	}

	return std::make_pair(pressure, temperature);
}