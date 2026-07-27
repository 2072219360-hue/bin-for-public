#pragma once
#include "ECHVStrategy.h"
#include"ECHVSignalInstance.h"
#include <vtkSmartPointer.h>
#include <vtkDataSetReader.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSet.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include<vtkCellArray.h>
#include <vtkCellLocator.h>
#include<vtkIdList.h>
#include<vtkDataArray.h>
#include<vtkType.h>
#include<vtkDoubleArray.h>
#include<vtkFloatArray.h>
#include<vtkIntArray.h>
#include <vtkStructuredGridWriter.h>
#include <vtkCellTreeLocator.h>
#include<vtkCellArray.h>
#include<vtkCell3D.h>
#include<vtkTetra.h>

#include<string>
#include<QDebug>
#include<vector>

using namespace std;

#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
namespace platforms {
	namespace echv {
		class transGrid :
			public ECHVStrategy
		{
		public:
			void AlgorithmInterface() override;
			transGrid(runData* run_data, argData* arg_data)
				:ECHVStrategy(run_data, arg_data) {}
			vtkSmartPointer<vtkStructuredGrid> UnstrToStr(vtkSmartPointer<vtkUnstructuredGrid> ungrid, int xSize=100, int ySize=100, int zSize=100,double interval=1);
			double volume3D(double *a, double *b, double *c, double *d);
			void sub3D(double *v1, double *v2,double *res);
			void cross3D(double *v1, double *v2,double *res);
			double dot3D(double *v1, double *v2);
			void transCell(vtkSmartPointer<vtkUnstructuredGrid> ungrid);
			void processDouble(vtkSmartPointer<vtkStructuredGrid> grid);
			double safeValue(double x);
		private:

		};
	}
}



