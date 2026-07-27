#pragma once
#include "ECHVStrategy.h"
#include"ECHVSignalInstance.h"
#include"runData.h"
#include"argData.h"
#include"unionFindCT.h"
#include<vtkStructuredGrid.h>
#include<vtkPointData.h>
#include<vtkDataSet.h>
#include<vtkPoints.h>
#include<vtkDataArray.h>
#include<vtkArray.h>
#include<qDebug>
#include<string>
#include<cmath>
#include<algorithm>
#include<vector>
#include<set>


using namespace std;

#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()


namespace platforms {
	namespace echv {
		class CreateTrees:
			public ECHVStrategy
		{
		public:
			void AlgorithmInterface() override;
			CreateTrees(runData* run_data, argData* arg_data)
				:ECHVStrategy(run_data, arg_data) {}
			vector<pair<int, double>> getPointsMagnitude(vtkDataArray* scalarArray);
			vector <vector<pair<int, int>>>   getEdges(vector<pair<int, double>>& points_magnitude, vtkSmartPointer<vtkStructuredGrid> grid);
			
			vector<vector<int>> creatJST(vector<pair<int, double>>& points_magnitude, vector <vector<pair<int, int>>>& edges, string treeType);
			vector<vector<int>> creatBranch(vector<double>& points_value, vector<vector<int>>& JST, string treeType);
			void write_vtk(vtkSmartPointer<vtkStructuredGrid> grid, const vector<vector<int>>& edges,
				const vector<double>& points_value, string fileName, string scalarName);
			void addBranchToPQ(vector<vector<int>>& tree, vector<vector<int>>& otree, set<vector<int>>& PQ);
			bool canSimplify(vector<int>& branch, vector<vector<int>>& tree, vector<vector<int>>& otree);
			string mergeBranch(vector<int>& branch, vector<vector<int>>& JT, vector<vector<int>>& ST);
			vector<vector<int>>  createCT(vector<vector<int>>& JT_Branch, vector<vector<int>>& ST_Branch);
			void test();
		private:
		};
	}
}



