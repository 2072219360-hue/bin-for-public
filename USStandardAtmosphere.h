#ifndef US_STANDARD_ATMOSPHERE_H
#define US_STANDARD_ATMOSPHERE_H

#include <utility>

class USStandardAtmosphere {
public:
	// 返回 {pressure_Pa, temperature_K}
	// 输入：height_km（几何高度，km），有效范围 0..1000
	// 如果超出范围将抛出 std::out_of_range
	std::pair<double, double> getPT(double height_km) const;

private:
	// 温度（K），按 MATLAB atmo_temp 的分段实现（参数与行为保持一致）
	double atmo_temp(double Z_km) const;

	// 压强（Pa），对 Z<=86 使用分层解析公式（与 Matlab atmo_p 一致），
	// 对 Z>86 使用从 86 km 起的数值积分（基于对流静力方程）
	double atmo_p(double Z_km) const;

	// 将几何高度 Z(km) -> 地势高度 H(km)
	static double geopotential_height(double Z_km);

	// 常量
	static constexpr double R_univ = 8.31432e3; // J/(kmol*K)
	static constexpr double M0 = 28.9644;      // kg/kmol (mean molar mass)
	static constexpr double g0 = 9.80665;      // m/s^2
	static constexpr double r_E = 6.356766e3;  // Earth's radius in km (as in MATLAB)
};

#endif // US_STANDARD_ATMOSPHERE_H
