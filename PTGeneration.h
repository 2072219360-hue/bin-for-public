// PTGeneration.h
#ifndef PT_GENERATION_H
#define PT_GENERATION_H

#include <map>
#include <utility>
#include <string>
#include <iostream>
#include"ISA_Calc.h"

class PTGeneration {
private:
	// 物理常数作为成员常量
	// const double T0 = 288.15;   // 海平面温度 (K)
	// const double P0 = 101325;   // 海平面压力 (Pa)
	// const double L = -0.0065;   // 温度递减率 (K/m)
	// const double g0 = 9.80665;  // 重力加速度 (m/s²)
	// const double M = 0.0289644; // 空气的摩尔质量 (kg/mol)
	// const double R = 8.31447;   // 通用气体常数 (J/(mol·K))

	const double T0 = 288.15;  // 海平面温度 288.15 K
	const double P0 = 101325;  // 海平面气压 101325 Pa
	const double L = 6.5 / 1000; // 温度递减率 6.5°C/km 转换为 K/m
	const double g0 = 9.80665; // 标准重力加速度 9.80665 m/s²
	const double M = 0.0289644; // 干空气的摩尔质量 0.0289644 kg/mol
	const double R = 8.31432;  // 理想气体常数 8.31432 J/(mol·K)
	const double T1 = 216.65;  // 平流层的温度 216.65 K
	const double P1 = 22632.1; // 平流层的海平面气压 22632.1 Pa

	// 温度计算器
	ISA::ISA_Calc tempCal;

	// 存储高度对应的压力和温度映射
	std::map<double, double> hp;  // 高度到压力的映射
	std::map<double, double> ht;  // 高度到温度的映射

public:
	// 构造函数
	PTGeneration() = default;

	// 可以添加带参数的构造函数，用于初始化映射
	PTGeneration(const std::map<double, double>& pressureMap,
		const std::map<double, double>& temperatureMap)
		: hp(pressureMap), ht(temperatureMap) {}

	// 计算指定高度的压力和温度
	std::pair<double, double> getPT(double H);

	// 提供设置映射的方法
	void setPressureMap(const std::map<double, double>& pressureMap) {
		hp = pressureMap;
	}

	void setTemperatureMap(const std::map<double, double>& temperatureMap) {
		ht = temperatureMap;
	}
};

#endif // PT_GENERATION_H