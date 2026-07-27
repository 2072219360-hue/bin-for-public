#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include<vector>
#include<qdebug.h>
namespace Liutex {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
	//向量
	class Vector {
	public:
		double x_, y_, z_;

		Vector(double x = 0, double y = 0, double z = 0) : x_(x), y_(y), z_(z) {}

		double& x() { return x_; }
		double& y() { return y_; }
		double& z() { return z_; }

		double magnitude() const {
			return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
		}

		Vector operator/(double scalar) const {
			if (scalar == 0) throw std::runtime_error("Division by zero");
			return Vector(x_ / scalar, y_ / scalar, z_ / scalar);
		}

		double dot(const Vector& other) const {
			return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
		}

		Vector cross(const Vector& other) const {
			return Vector(
				y_ * other.z_ - z_ * other.y_,
				z_ * other.x_ - x_ * other.z_,
				x_ * other.y_ - y_ * other.x_
			);
		}

		Vector operator-(const Vector& other) const {
			return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
		}

		Vector operator+(const Vector& other) const {
			return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
		}

		Vector operator*(double scalar) const {
			return Vector(x_ * scalar, y_ * scalar, z_ * scalar);
		}
		Vector& operator/=(double scalar) {
			if (scalar == 0) throw std::runtime_error("Division by zero");
			x_ /= scalar;
			y_ /= scalar;
			z_ /= scalar;
			return *this;
		}

		void normalize() {
			double mag = magnitude();
			if (mag > 0) {
				x_ /= mag;
				y_ /= mag;
				z_ /= mag;
			}
		}
	};
	Vector operator*(double scalar, const Vector& vec);
	//张量
	class Tensor {
	public:
		double data[3][3];

		Tensor() {
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					data[i][j] = 0.0;
		}

		double& xx() { return data[0][0]; }
		double& xy() { return data[0][1]; }
		double& xz() { return data[0][2]; }
		double& yx() { return data[1][0]; }
		double& yy() { return data[1][1]; }
		double& yz() { return data[1][2]; }
		double& zx() { return data[2][0]; }
		double& zy() { return data[2][1]; }
		double& zz() { return data[2][2]; }

		Tensor operator&(const Tensor& other) const {
			Tensor result;
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					result.data[i][j] = data[i][j] * other.data[i][j];
			return result;
		}

		Tensor transpose() const {
			Tensor result;
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					result.data[i][j] = data[j][i];
			return result;
		}

		//friend std::ostream& operator<<(std::ostream& os, const Tensor& tensor);
	};

	double sign(double value);
	double acosSafe(double value);
	Tensor rotation(const Vector& u, const Vector& v);
	std::vector<Vector> computRotex(std::vector<std::vector<int>>&  cells, std::vector<Vector>& U);
}
