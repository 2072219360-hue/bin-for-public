#include"computeLiutex.h"

namespace Liutex {
	Vector operator*(double scalar, const Vector& vec) {
		return Vector(vec.x_ * scalar, vec.y_ * scalar, vec.z_ * scalar);
	}
	
	double sign(double value) {
		return (value >= 0) ? 1.0 : -1.0;
	}

	double acosSafe(double value) {
		if (value < -1.0) return M_PI;
		if (value > 1.0) return 0.0;
		return std::acos(value);
	}

	Tensor rotation(const Vector& u, const Vector& v) {
		const double eps = 1.0e-10;
		Tensor q;
		Vector a = u.cross(v);

		if (a.magnitude() < eps) {
			q.xx() = q.yy() = q.zz() = 1.0;
		}
		else {
			a /= a.magnitude();
			double t = sign(u.dot(v));
			double alpha = acosSafe(t);
			double c = std::cos(alpha);
			double s = std::sin(alpha);

			q.xx() = a.x() * a.x() * (1 - c) + c;
			q.xy() = a.x() * a.y() * (1 - c) - a.z() * s;
			q.xz() = a.x() * a.z() * (1 - c) + a.y() * s;

			q.yx() = a.y() * a.x() * (1 - c) + a.z() * s;
			q.yy() = a.y() * a.y() * (1 - c) + c;
			q.yz() = a.y() * a.z() * (1 - c) - a.x() * s;

			q.zx() = a.z() * a.x() * (1 - c) - a.y() * s;
			q.zy() = a.z() * a.y() * (1 - c) + a.x() * s;
			q.zz() = a.z() * a.z() * (1 - c) + c;
		}
		return q;
	}

	Tensor computeGradient(std::vector<Vector>& field, const std::vector<int>& cell, double dx, double dy, double dz) {
		Tensor grad;
		if (cell.size() == 8) {
			//如果cell是八个顶点的情况如何计算梯度
			grad.xx() = (field[cell[1]].x() - field[cell[0]].x()) / dx;
			grad.xy() = (field[cell[2]].x() - field[cell[0]].x()) / dy;
			grad.xz() = (field[cell[4]].x() - field[cell[0]].x()) / dz;

			grad.yx() = (field[cell[1]].y() - field[cell[0]].y()) / dx;
			grad.yy() = (field[cell[2]].y() - field[cell[0]].y()) / dy;
			grad.yz() = (field[cell[4]].y() - field[cell[0]].y()) / dz;

			grad.zx() = (field[cell[1]].z() - field[cell[0]].z()) / dx;
			grad.zy() = (field[cell[2]].z() - field[cell[0]].z()) / dy;
			grad.zz() = (field[cell[4]].z() - field[cell[0]].z()) / dz;
		}

		if (cell.size() == 4){
			//计算四面体的梯度
			// 顶点坐标
		
			Vector& P0 = field[cell[0]];
			Vector& P1 = field[cell[1]];
			Vector& P2 = field[cell[2]];
			Vector& P3 = field[cell[3]];
			// 计算四面体体积
			double V = (1.0 / 6.0) * std::abs(
				(P1.x() - P0.x()) * ((P2.y() - P0.y()) * (P3.z() - P0.z()) - (P2.z() - P0.z()) * (P3.y() - P0.y())) -
				(P1.y() - P0.y()) * ((P2.x() - P0.x()) * (P3.z() - P0.z()) - (P2.z() - P0.z()) * (P3.x() - P0.x())) +
				(P1.z() - P0.z()) * ((P2.x() - P0.x()) * (P3.y() - P0.y()) - (P2.y() - P0.y()) * (P3.x() - P0.x()))
			);
			// 计算梯度
			grad.xx() = ((P1.x() - P0.x()) * (P2.y() - P0.y()) * (P3.z() - P0.z()) - (P2.x() - P0.x()) * (P1.y() - P0.y()) * (P3.z() - P0.z())
				+ (P2.x() - P0.x()) * (P3.y() - P0.y()) * (P1.z() - P0.z()) - (P3.x() - P0.x()) * (P2.y() - P0.y()) * (P1.z() - P0.z())) / (6.0 * V);

			grad.xy() = ((P1.y() - P0.y()) * (P2.z() - P0.z()) * (P3.x() - P0.x()) - (P2.y() - P0.y()) * (P1.z() - P0.z()) * (P3.x() - P0.x())
				+ (P2.y() - P0.y()) * (P3.z() - P0.z()) * (P1.x() - P0.x()) - (P3.y() - P0.y()) * (P2.z() - P0.z()) * (P1.x() - P0.x())) / (6.0 * V);

			grad.xz() = ((P1.z() - P0.z()) * (P2.x() - P0.x()) * (P3.y() - P0.y()) - (P2.z() - P0.z()) * (P1.x() - P0.x()) * (P3.y() - P0.y())
				+ (P2.z() - P0.z()) * (P3.x() - P0.x()) * (P1.y() - P0.y()) - (P3.z() - P0.z()) * (P2.x() - P0.x()) * (P1.y() - P0.y())) / (6.0 * V);

			grad.yx() = grad.xy(); // 对称张量
			grad.yy() = grad.xx(); // 对称张量
			grad.yz() = grad.xz(); // 对称张量
			grad.zx() = grad.xz(); // 对称张量
			grad.zy() = grad.yz(); // 对称张量
			grad.zz() = grad.xx(); // 对称张量
		}
		return grad;
	}
	std::vector<Tensor> computeGradients(std::vector<std::vector<int>>&  cells, std::vector<Vector>& field, double dx, double dy, double dz) {
		std::vector<Tensor> gradients(cells.size());

		for (size_t i = 0; i < cells.size(); ++i) {
			const auto& cell = cells[i];

			gradients[i] = computeGradient(field, cell, dx, dy, dz);
		}

		return gradients;
	}
	//U为点上的向量,cell为单元格
	std::vector<Vector> computRotex(std::vector<std::vector<int>>&  cells, std::vector<Vector>& U) {
		double dx = 1.0, dy = 1.0, dz = 1.0;
		qDebug() << "begin compute gradU";
		std::vector<Tensor> gradU = computeGradients(cells, U, dx, dy, dz);
		qDebug() << "end compute gradU";
		std::vector<Vector> Rotex(gradU.size());
		Vector z0(0, 0, 1);
		qDebug() << "begin computRotex for";
		for (size_t cellI = 0; cellI < gradU.size(); ++cellI) {
			//qDebug() << "cellI=" << cellI;
			Tensor a = gradU[cellI];
			//aa 是特征多项式一次项的系数,矩阵 a 的迹的负值
			double aa = -(a.xx() + a.yy() + a.zz());
			//tt 是矩阵 a 的自乘积
			Tensor tt = a & a;

			//bb 是特征多项式中的二次项系数
			double bb = -0.5 * (tt.xx() + tt.yy() + tt.zz() - std::pow((tt.xx() + tt.yy() + tt.zz()), 2));
			//cc 是特征多项式的常数项系数,矩阵 a 的行列式的负值。
			double cc = -(a.xx() * (a.yy() * a.zz() - a.yz() * a.zy())
				- a.xy() * (a.yx() * a.zz() - a.yz() * a.zx())
				+ a.xz() * (a.yx() * a.zy() - a.yy() * a.zx()));
			//三次特征多项式的判别式
			double delta = 18.0 * aa * bb * cc
				- 4.0 * std::pow(aa, 3) * cc
				+ std::pow(aa, 2) * std::pow(bb, 2)
				- 4.0 * std::pow(bb, 3)
				- 27.0 * std::pow(cc, 2);
			//卡尔达诺公式（Cardano's formula）的一部分，用于进一步求解特征值
			double qq = (std::pow(aa, 2) - 3 * bb) / 9.0;
			double rr = (2.0 * std::pow(aa, 3) - 9.0 * aa * bb + 27.0 * cc) / 54.0;

			delta = -delta / 108.0;
			//有一个实根和一对共轭复根
			if (delta > 0.0) {
				double aaaa = -sign(rr) * std::pow(std::abs(rr) + std::sqrt(delta), 1.0 / 3.0);
				double bbbb = 0;

				if (aaaa != 0.0)
					bbbb = qq / aaaa;
				//计算第一个复数根的实部和虚部
				double eig1c_r = -0.5 * (aaaa + bbbb) - aa / 3.0;
				double eig1c_i = 0.5 * std::sqrt(3.0) * (aaaa - bbbb);
				//计算第二个复数根的实部和虚部
				double eig2c_r = -0.5 * (aaaa + bbbb) - aa / 3.0;
				double eig2c_i = -0.5 * std::sqrt(3.0) * (aaaa - bbbb);
				//计算实根
				double eig3r = aaaa + bbbb - aa / 3.0;

				double delta1 = (a.xx() - eig3r) * (a.yy() - eig3r) - a.yx() * a.xy();
				double delta2 = (a.yy() - eig3r) * (a.zz() - eig3r) - a.yz() * a.zy();
				double delta3 = (a.xx() - eig3r) * (a.zz() - eig3r) - a.zx() * a.xz();

				if (delta1 == 0.0 && delta2 == 0.0 && delta3 == 0.0) {
					std::cerr << "Fatal error: delta1-3 are:" << delta1 << ", " << delta2 << ", " << delta3 << std::endl;
					//return -1;
				}

				Vector vr(0, 0, 0);

				if (std::abs(delta1) >= std::abs(delta2) && std::abs(delta1) >= std::abs(delta3)) {
					vr.x() = (-(a.yy() - eig3r) * a.xz() + a.xy() * a.yz()) / delta1;
					vr.y() = (a.yx() * a.xz() - (a.xx() - eig3r) * a.yz()) / delta1;
					vr.z() = 1.0;
				}
				else if (std::abs(delta2) >= std::abs(delta1) && std::abs(delta2) >= std::abs(delta3)) {
					vr.x() = 1.0;
					vr.y() = (-(a.zz() - eig3r) * a.yx() + a.yz() * a.zx()) / delta2;
					vr.z() = (a.zy() * a.yx() - (a.yy() - eig3r) * a.zx()) / delta2;
				}
				else if (std::abs(delta3) >= std::abs(delta1) && std::abs(delta3) >= std::abs(delta2)) {
					vr.x() = (-(a.zz() - eig3r) * a.xy() + a.xz() * a.zy()) / delta3;
					vr.y() = 1.0;
					vr.z() = (a.zx() * a.xy() - (a.xx() - eig3r) * a.zy()) / delta3;
				}
				else {
					std::cerr << "Fatal error: vr calculation error" << std::endl;
					//return -1;
				}

				vr.normalize();

				Tensor qqq = rotation(z0, vr);
				Tensor vg = qqq.transpose() & a;
				vg = vg & qqq;

				double alpha = 0.5 * std::sqrt(std::pow(vg.yy() - vg.xx(), 2) + std::pow(vg.yx() + vg.xy(), 2));
				double beta = 0.5 * (vg.yx() - vg.xy());

				if (beta * beta > alpha * alpha) {
					double rm = 0.0;
					if (beta > 0.0)
						rm = 2.0 * (beta - alpha);
					else
						rm = 2.0 * (beta + alpha);
					//rm对应公式(38)中的R，vr对应r
					Rotex[cellI] = rm * vr;
					//qDebug() << "cellId=" << cellI << " " << Rotex[cellI].x() << " " << Rotex[cellI].y() << " " << Rotex[cellI].z();
				}
			}
		}
		
		return Rotex;
	}
}