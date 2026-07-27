#pragma once
#include "ECHVStrategy.h"
#include"ECHVSignalInstance.h"
#include"runData.h"
#include"argData.h"
#include"computeLiutex.h"

#include <vtkArrayDispatch.h>
#include <vtkSMPTools.h>
#include <vtkDataSetReader.h>
#include <vtkGradientFilter.h>
#include <vtkDataObject.h>
#include <vtkDataArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkDataSet.h>
#include <vtkStructuredGrid.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkDataSetWriter.h>
#include<vtkIdFilter.h>
//#include <vtkGenerateGlobalIds.h>
#include <vtk_eigen.h>
#include VTK_EIGEN(Eigenvalues)
#include VTK_EIGEN(Geometry)


#include<qDebug>
#include <array>
#include <iostream>

using namespace std;
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
namespace
{
	// Computes A*b = x given a 3-matrix A and a 3-vector b.
	template <typename AArrayType, typename BArrayType, typename XArrayType>
	class MatrixVectorMultiplyFunctor
	{
		AArrayType* AArray;
		BArrayType* BArray;
		XArrayType* XArray;

	public:
		MatrixVectorMultiplyFunctor(AArrayType* aArray, BArrayType* bArray, XArrayType* xArray)
			: AArray(aArray)
			, BArray(bArray)
			, XArray(xArray)
		{
		}

		void operator()(vtkIdType begin, vtkIdType end)
		{
			
			
			for (vtkIdType idx = begin; idx < end; ++idx)
			{
				double a[9];
				double b[3];
				double x[3];

				this->AArray->GetTuple(idx, a);
				this->BArray->GetTuple(idx, b);

				for (vtkIdType i = 0; i < 3; ++i)
				{
					x[i] = a[0 + i * 3] * b[0] + a[1 + i * 3] * b[1] + a[2 + i * 3] * b[2];
				}

				this->XArray->SetTuple(idx, x);
			}
			/*const auto aRange = vtk::DataArrayTupleRange<9>(this->AArray, begin, end);
			const auto bRange = vtk::DataArrayTupleRange<3>(this->BArray, begin, end);
			auto xRange = vtk::DataArrayTupleRange<3>(this->XArray, begin, end);

			auto a = aRange.cbegin();
			auto b = bRange.cbegin();
			auto x = xRange.begin();

			for (; a != aRange.cend(); ++a, ++b, ++x)
			{
				for (vtkIdType i = 0; i < 3; ++i)
				{
					(*x)[i] =
						((*a)[0 + i * 3] * (*b)[0] + (*a)[1 + i * 3] * (*b)[1] + (*a)[2 + i * 3] * (*b)[2]);
				}
			}*/
		}
	};

	struct MatrixVectorMultiplyWorker
	{
		template <typename AArrayType, typename BArrayType, typename XArrayType>
		void operator()(AArrayType* aArray, BArrayType* bArray, XArrayType* xArray)
		{
			MatrixVectorMultiplyFunctor<AArrayType, BArrayType, XArrayType> functor(aArray, bArray, xArray);
			vtkSMPTools::For(0, xArray->GetNumberOfTuples(), functor);
		}
	};


	void computeVortexCriteria(const float s[9], const float omega[9], float vortexCriteria[4])
	{

		Eigen::Matrix<double, 3, 3> S, Omega, J;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				const double& s_ij = s[3 * i + j];
				const double& omega_ij = omega[3 * i + j];
				S(i, j) = s_ij;
				Omega(i, j) = omega_ij;
				J(i, j) = (s_ij + omega_ij) / 2.;
			}
		}

		// The Q-criterion is defined as
		// Q = \frac{1}{2} \left[ | \Omega |^2 - | S |^2 \right] > 0
		float& Q = vortexCriteria[0];
		Q = (Omega.operatorNorm() - S.operatorNorm()) / 2.;

		// The delta-criterion is defined as
		// \Delta = \left( \frac{Q}{3} \right)^3 + \left( \frac{\det J}{2} \right)^2 > 0
		float& delta = vortexCriteria[1];
		delta = std::pow(Q / 3., 3) + std::pow(J.determinant() / 2., 2);

		// The lambda_2-criterion is defined as
		// \lambda_2 \left( S^2 + \Omega^2 \right) < 0
		// where $\lambda_2$ is the intermediate eigenvalue
		float& lambda_2 = vortexCriteria[2];
		{
			Eigen::Matrix<double, 3, 3> A = S * S + Omega * Omega;
			auto Eigenvalues = A.eigenvalues();
			// Matrix A is symmetric, so its eigenvalues are all real.
			std::array<double, 3> eigenvalues = { Eigenvalues[0].real(), Eigenvalues[1].real(),
			  Eigenvalues[2].real() };
			std::nth_element(eigenvalues.begin(), eigenvalues.begin() + 1, eigenvalues.end());
			lambda_2 = eigenvalues[1];
		}

		// The lambda_ci-criterion is defined as the imaginary component of the
	  // eigenvalues of the complex conjugate pair of eigenvalues of J
		float& lambda_ci = vortexCriteria[3];
		{
			Eigen::EigenSolver<Eigen::Matrix<double, 3, 3>> eigensolver(J);
			auto eigenvalues = eigensolver.eigenvalues();

			if (std::abs(eigenvalues[0].imag()) > VTK_DBL_EPSILON)
			{
				if ((std::abs(eigenvalues[0].real() - eigenvalues[1].real()) < VTK_DBL_EPSILON &&
					std::abs(eigenvalues[0].imag() + eigenvalues[1].imag()) < VTK_DBL_EPSILON) ||
					(std::abs(eigenvalues[0].real() - eigenvalues[2].real()) < VTK_DBL_EPSILON &&
						std::abs(eigenvalues[0].imag() + eigenvalues[2].imag()) < VTK_DBL_EPSILON))
				{
					lambda_ci = std::abs(eigenvalues[0].imag());
				}
			}
			else if (std::abs(eigenvalues[1].imag()) > VTK_DBL_EPSILON)
			{
				if (std::abs(eigenvalues[1].real() - eigenvalues[2].real()) < VTK_DBL_EPSILON &&
					std::abs(eigenvalues[1].imag() + eigenvalues[2].imag()) < VTK_DBL_EPSILON)

				{
					lambda_ci = std::abs(eigenvalues[1].imag());
				}
			}
		}
	}


	template <typename JacobianArrayType, typename QArrayType, typename DeltaArrayType,
		typename Lambda2ArrayType, typename LambdaciArrayType>
		class ComputeCriteriaFunctor
	{
		JacobianArrayType* JacobianArray;
		QArrayType* QArray;
		DeltaArrayType* DeltaArray;
		Lambda2ArrayType* Lambda2Array;
		LambdaciArrayType* LambdaciArray;

	public:
		ComputeCriteriaFunctor(JacobianArrayType* jacobianArray, QArrayType* qArray,
			DeltaArrayType* deltaArray, Lambda2ArrayType* lambda2Array, LambdaciArrayType* lambdaciArray)
		{
			JacobianArray = jacobianArray;
			QArray = qArray;
			DeltaArray = deltaArray;
			Lambda2Array = lambda2Array;
			LambdaciArray = lambdaciArray;
		}

		void operator() (vtkIdType begin, vtkIdType end)
		{
			for (vtkIdType idx = begin; idx < end; ++idx)
			{
				// 获取 JacobianArray 的元组 (9 个组件)
				double jacobianTuple[9];
				this->JacobianArray->GetTuple(idx, jacobianTuple);

				// 获取 QArray, DeltaArray, Lambda2Array, LambdaciArray 的元组 (1 个组件)
				double qTuple[1];
				double deltaTuple[1];
				double lambda2Tuple[1];
				double lambdaciTuple[1];

				this->QArray->GetTuple(idx, qTuple);
				this->DeltaArray->GetTuple(idx, deltaTuple);
				this->Lambda2Array->GetTuple(idx, lambda2Tuple);
				this->LambdaciArray->GetTuple(idx, lambdaciTuple);
				
				// 计算 S 和 Omega
				float S[9];
				float Omega[9];
				static const std::array<int, 9> idxT = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };

				for (int i = 0; i < 9; i++)
				{
					double j_i = jacobianTuple[i];
					double jt_i = jacobianTuple[idxT[i]];

					S[i] = (j_i + jt_i) / 2.0;
					Omega[i] = (j_i - jt_i) / 2.0;
				}
				
				// 计算涡旋准则
				std::array<float, 4> vortexCriteria;
				computeVortexCriteria(S, Omega, vortexCriteria.data());

				// 设置 QArray, DeltaArray, Lambda2Array, LambdaciArray 的值
				qTuple[0] = vortexCriteria[0];
				deltaTuple[0] = vortexCriteria[1];
				lambda2Tuple[0] = vortexCriteria[2];
				lambdaciTuple[0] = vortexCriteria[3];

				this->QArray->SetTuple(idx, qTuple);
				this->DeltaArray->SetTuple(idx, deltaTuple);
				this->Lambda2Array->SetTuple(idx, lambda2Tuple);
				this->LambdaciArray->SetTuple(idx, lambdaciTuple);
			}
			/*const auto jacobianRange = vtk::DataArrayTupleRange<9>(this->JacobianArray, begin, end);
			auto qRange = vtk::DataArrayTupleRange<1>(this->QArray, begin, end);
			auto deltaRange = vtk::DataArrayTupleRange<1>(this->DeltaArray, begin, end);
			auto lambda2Range = vtk::DataArrayTupleRange<1>(this->Lambda2Array, begin, end);
			auto lambdaciRange = vtk::DataArrayTupleRange<1>(this->LambdaciArray, begin, end);

			auto j = jacobianRange.cbegin();
			auto q = qRange.begin();
			auto delta = deltaRange.begin();
			auto lambda2 = lambda2Range.begin();
			auto lambdaci = lambdaciRange.begin();

			for (; j != jacobianRange.cend(); ++j, ++q, ++delta, ++lambda2, ++lambdaci)
			{
				std::array<float, 4> vortexCriteria;
				float S[9];
				float Omega[9];
				static const std::array<typename decltype(j)::value_type::size_type, 9> idxT = { 0, 3, 6, 1,
					4, 7, 2, 5, 8 };
				for (int i = 0; i < 9; i++)
				{
					double j_i = (*j)[i];
					double jt_i = (*j)[idxT[i]];

					S[i] = (j_i + jt_i) / 2.;
					Omega[i] = (j_i - jt_i) / 2.;
				}

				computeVortexCriteria(S, Omega, vortexCriteria.data());
				(*q)[0] = vortexCriteria[0];
				(*delta)[0] = vortexCriteria[1];
				(*lambda2)[0] = vortexCriteria[2];
				(*lambdaci)[0] = vortexCriteria[3];
			}*/
		}
	};

	struct ComputeCriteriaWorker
	{
		template <typename JacobianArrayType, typename QArrayType, typename DeltaArrayType,
			typename Lambda2ArrayType, typename LambdaciArrayType>
			void operator()(JacobianArrayType* jacobianArray, QArrayType* qArray,
				DeltaArrayType* deltaArray, Lambda2ArrayType* lambda2Array, LambdaciArrayType* lambdaciArray)
		{
			ComputeCriteriaFunctor<JacobianArrayType, QArrayType, DeltaArrayType, Lambda2ArrayType, LambdaciArrayType> functor(
				jacobianArray, qArray, deltaArray, lambda2Array, lambdaciArray);
			vtkSMPTools::For(0, jacobianArray->GetNumberOfTuples(), functor);
		}
	};
}


namespace platforms {
	namespace echv {
		class CreateFullDataset :
			public ECHVStrategy {
		public:
			void AlgorithmInterface() override;
			CreateFullDataset(runData* run_data, argData* arg_data)
				:ECHVStrategy(run_data, arg_data) {}
			void calculateOyf(int xDims, int yDims, int zDims, vtkDataSet* input, vtkFloatArray* oyf_array);
			void calculateVelUf(int xDims, int yDims, int zDims, vtkDataSet* input,
				vtkFloatArray* vel_uf_array);
			void processDouble(vtkSmartPointer<vtkStructuredGrid> grid);
			void createLiutex(vtkSmartPointer<vtkStructuredGrid> newGrid, string scalar_name);
		private:
			string vectorName;

		};
	}
}

