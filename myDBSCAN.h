#pragma once

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
namespace DBSCAN {
	struct Point {
		double x, y, z;
		int cluster_id;  // -1 表示未分类, 0 表示噪声, 其他正数表示聚类ID
		Point(double x, double y, double z) : x(x), y(y), z(z), cluster_id(-1) {}
	};
	

	void myDBSCAN(vector<Point>& points, double eps, int min_pts);

}

