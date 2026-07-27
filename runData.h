#pragma once

#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include<vector>
using namespace std;
namespace platforms {
	namespace echv {
		//该类用来负责保存运行时的各种数据结构
		class runData
		{
			friend class Strategy;
		public:
			runData();
			~runData();
			vtkSmartPointer<vtkStructuredGrid> getGrid() { return grid; }
			vtkSmartPointer<vtkUnstructuredGrid> getUnGrid() { return ungrid; }
			vector<vector<int>>* getJT() { return JT; }
			vector<vector<int>>* getST() { return ST; }
			vector<vector<int>>* getCT() { return CT; }
			void setGird(vtkSmartPointer<vtkStructuredGrid> arg_grid) { grid=arg_grid; }
			void setUnGird(vtkSmartPointer<vtkUnstructuredGrid> arg_grid) { ungrid=arg_grid; }
			void setJT(vector<vector<int>>* new_JT) {
				if (JT != nullptr) delete JT;
				JT = new_JT;
			}
			void setST(vector<vector<int>>* new_ST) {
				if (ST != nullptr) delete ST;
				ST = new_ST;
			}
			void setCT(vector<vector<int>>* new_CT) {
				if (CT != nullptr) delete CT;
				CT = new_CT;
			}
		private:
			vtkSmartPointer<vtkStructuredGrid> grid;
			vtkSmartPointer<vtkUnstructuredGrid> ungrid;
			vector<vector<int>>* JT;
			vector<vector<int>>* ST;
			vector<vector<int>>* CT;
		};
	}
}