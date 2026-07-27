#pragma once
#include "ECHVStrategy.h"
#include"ECHVSignalInstance.h"
#include"runData.h"
#include"argData.h"
#include"myDBSCAN.h"
#include<vtkStructuredGrid.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include<vtkDataArray.h>
#include <vtkAppendPolyData.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataWriter.h>
#include <vtkStructuredGridWriter.h>
#include <vtkCubeSource.h>
#include<vtkDoubleArray.h>
#include<vtkCellData.h>
#include<vtkThreshold.h>
#include <vtkSmartPointer.h>
#include <vtkGeometryFilter.h>
#include <vtkSmoothPolyDataFilter.h>

#include <iostream>
#include<set>
#include<vector>
#include<qDebug>
#include<map>
#include<algorithm>
#include<string>
#include<queue>
using namespace std;

#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()

namespace platforms {
	namespace echv {
		class relevance:
			public ECHVStrategy
		{
		public:
			void AlgorithmInterface() override;
			relevance(runData* run_data, argData* arg_data)
				:ECHVStrategy(run_data, arg_data) {}
			vector<vector<double>> getPoints(vtkSmartPointer<vtkStructuredGrid> grid);
			vector<double> getScalars(vtkSmartPointer<vtkStructuredGrid> grid, string scalar_name);
			set<int> count_elements(vector<vector<int>>& lines);
			vector<double> get_Bounding_Box(vector<vector<double>>& nodes, vector<int> &id_list);
			double relevance::caculate_volume(vector<double> bound);
			bool judge_qRanges(string scalar_name, double min_q, double max_q);
			void write_extreme_outline(const vector<vector<int>>& extreme_point_branch, const vector<vector<double>>& bound_list,string file_name);
			vtkSmartPointer<vtkThreshold> region_growing(string file_name, string scalar_name, vtkSmartPointer<vtkStructuredGrid> grid,
				const vector<double>& scalar_data, const vector<int>& seed_point_ids, double min_value, double max_value);
			bool judge_region_growing(string scalar_name, double min_value, double max_value,
				const vector<double>& scalar_data, vtkSmartPointer<vtkIdList> point_ids);
			int compute_neighbors_count(vtkSmartPointer<vtkStructuredGrid> grid, vtkIdType cell_id, vtkSmartPointer<vtkIdTypeArray> region_ids);
			vtkSmartPointer<vtkPolyData> extractSurface(vtkSmartPointer<vtkUnstructuredGrid> dataset);
			void cluster_cell(vtkSmartPointer<vtkUnstructuredGrid> regionData, double interval);
		private:

		};
	}
}



