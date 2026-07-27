#pragma once
#include <QDebug>
#include <QFile>
#include <QMenu>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <iostream>
#include <string>
#include <chrono>  
#include <stdlib.h>
#include <stdio.h>
using namespace std;
typedef long long int ll;
const double eps = 1e-16;
const double epsBase = 1e6;
struct firstI {
	int id;
	double x;
	bool operator<(const firstI&b)const {
		return x < b.x;
	}
};
struct lastI {
	int id;
	double x;
	bool operator<(const lastI&b)const {
		return x < b.x;
	}
};
typedef struct block {
	int id = 0;
	int ni = 0, nj = 0, nk = 0;
	double *x = NULL, *y = NULL, *z = NULL;
	double min_x, max_x;
	double min_y, max_y;
	double min_z, max_z;
	double min_index, max_index;
	int numOfPoints;
	int numOfCells;//num of tetra
	int *sectId = NULL;// sectId[i] means sect id of line i
	int sectOffset;
	int sectFlag;

	double* X(int i, int j, int k) {
		return x + i * nj*nk + j * nk + k;
	}
	double* Y(int i, int j, int k) {
		return y + i * nj*nk + j * nk + k;
	}
	double* Z(int i, int j, int k) {
		return z + i * nj*nk + j * nk + k;
	}
	void reBuild() {
		if (ni != 0 && nj != 0 && nk != 0) {
			x = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(x, 0, sizeof(x));
		}
		if (ni != 0 && nj != 0 && nk != 0) {
			y = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(y, 0, sizeof(y));
		}
		if (ni != 0 && nj != 0 && nk != 0) {
			z = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(z, 0, sizeof(z));
		}
		sectId = (int*)malloc(ni * sizeof(int));
	}
	void build() {
		numOfPoints = 0;
		numOfCells = 0;
		min_x = 0;
		max_x = 0;
		min_index = 0;
		max_index = 0;
		sectId = (int*)malloc(ni * sizeof(int));
		sectOffset = 0;
		sectFlag = 0;
		if (ni != 0 && nj != 0 && nk != 0) {
			x = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(x, 0, sizeof(x));
		}
		if (ni != 0 && nj != 0 && nk != 0) {
			y = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(y, 0, sizeof(y));
		}
		if (ni != 0 && nj != 0 && nk != 0) {
			z = (double*)malloc(sizeof(double)*ni*nj*nk);
			memset(z, 0, sizeof(z));
		}
	}
	void clear() {
		if (x != NULL) free(x);
		if (y != NULL) free(y);
		if (z != NULL) free(z);
		x = NULL;
		y = NULL;
		z = NULL;
		if (sectId != NULL) free(sectId);
		sectId = NULL;
	}
	bool get_info() {
		if (x == NULL || y == NULL || z == NULL) return false;
		min_x = *X(0, 0, 0), max_x = *X(0, 0, 0);
		min_y = *Y(0, 0, 0), max_y = *Y(0, 0, 0);
		min_z = *Z(0, 0, 0), max_z = *Z(0, 0, 0);
		min_index = 0, max_index = 0;
		for (int k = 0; k < nk; k++) {
			for (int j = 0; j < nj; j++) {
				for (int i = 0; i < ni; i++) {
					//printf("(%lf,%lf,%lf)\n", *X(i, j, k), *Y(i, j, k), *Z(i, j, k));
					if (*X(i, j, k) < min_x) min_x = *X(i, j, k), min_index = i;
					if (*X(i, j, k) > max_x) max_x = *X(i, j, k), max_index = i;
					min_y = min(min_y, *Y(i, j, k)); max_y = max(max_y, *Y(i, j, k));
					min_z = min(min_z, *Z(i, j, k)); max_z = max(max_z, *Z(i, j, k));
				}
			}
		}
		numOfPoints = ni * nj * nk;
		//numOfCells = 5 * (ni - 1)*(nj - 1)*(nk - 1);
		numOfCells = 6 * (ni - 1)*(nj - 1)*(nk - 1);
		//numOfCells = 12 * (ni - 1)*(nj - 1)*(nk - 1);
		return true;
	}
	void print() {

	}
	bool operator<(const block&b)const {
		if (abs(min_x - b.min_x) <= eps) return max_x < b.max_x;
		return min_x < b.min_x;
	}
}block;
typedef struct point {
	double x, y, z;
	int i, j, k;
	int blockId, sectId;
	bool operator<(const point &b)const {

		ll a1 = x * epsBase, b1 = y * epsBase, c1 = z * epsBase;
		ll a2 = b.x*epsBase, b2 = b.y*epsBase, c2 = b.z*epsBase;
		if (a1 != a2) return a1 < a2;
		else if (b1 != b2) return b1 < b2;
		return c1 < c2;
	}
	void copy(point &b) {
		init(b.x, b.y, b.z, b.i, b.j, b.k, b.blockId, b.sectId);
		sectId = b.sectId;
	}
	void init(double x, double y, double z, int i, int j, int k, int blockId, int sectId) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->i = i;
		this->j = j;
		this->k = k;
		this->blockId = blockId;
		this->sectId = sectId;
	}
	void set_blockId(int x) {
		blockId = x;
	}
	void set_sectId(int x) {
		sectId = x;
	}
	void print() {
		printf("(%lf,%lf,%lf),blockId=%d,(%d,%d,%d)\n", x, y, z, blockId, i, j, k);
	}
}point;
typedef struct xyzPoint { //x,y,z of point
	double x, y, z;
	bool operator<(const xyzPoint &b)const {
		double a1 = x, b1 = y, c1 = z;
		double a2 = b.x, b2 = b.y, c2 = b.z;
		if (a1 != a2) return a1 < a2;
		if (b1 != b2) return b1 < b2;
		return c1 < c2;
		/*if (abs(a1 - a2) > eps) return a1 < a2;
		else if (abs(b1 - b2) > eps) return b1 < b2;
		return c1 < c2;*/

		/*ll a1 = x * epsBase, b1 = y * epsBase, c1 = z * epsBase;
		ll a2 = b.x*epsBase, b2 = b.y*epsBase, c2 = b.z*epsBase;
		if (a1 != a2) return a1 < a2;
		else if (b1 != b2) return b1 < b2;
		return c1 < c2;*/
	}
	bool operator==(const xyzPoint &b)const {
		if (abs(x - b.x) <= eps && abs(y - b.y) <= eps && abs(z - b.z) <= eps) return true;
		return false;
	}
	void init(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void print() {
		/*cout << "xyzPoint:" << endl;
		cout << "x=" << x << " y=" << y << " z=" << z << endl;*/
	}
}xyzPoint;
typedef struct triFace {
	int id1, id2, id3;// id1 < id2 < id3
	int group1, group2;// group of cell including this face
	void init(int x1, int x2, int x3) {
		group1 = -1;
		group2 = -1;
		if (x1 == min(x1, min(x2, x3))) {
			id1 = x1;
			id2 = min(x2, x3);
			id3 = max(x2, x3);
		}
		else if (x2 == min(x1, min(x2, x3))) {
			id1 = x2;
			id2 = min(x1, x3);
			id3 = max(x1, x3);
		}
		else if (x3 == min(x1, min(x2, x3))) {
			id1 = x3;
			id2 = min(x1, x2);
			id3 = max(x1, x2);
		}
	}
	bool set_group(int group) {
		if (group1 == -1) {
			group1 = group;
			return true;
		}
		if (group2 == -1) {
			group2 = group;
			return true;
		}
		return false;
	}
	bool operator<(const triFace &b)const {
		if (id1 == b.id1) {
			if (id2 == b.id2) {
				return id3 < b.id3;
			}
			return id2 < b.id2;
		}
		return id1 < b.id1;
	}
}triFace;
typedef struct cell {
	int type_id = 0;//cell type int table
	int numPoints = 0;//numbers of points in this  cell
	int *idPoint = NULL;
	int index = 0;
	int group = -1;	// id of cell
	void init(int id) {//triangle_id==5  vetra_id==10
		type_id = id;
		this->group = -1;
		index = 0;
		if (id == 5) {
			numPoints = 3;
			idPoint = (int*)malloc(numPoints * sizeof(int));
			memset(idPoint, 0, sizeof(idPoint));
		}
		if (id == 10) {
			numPoints = 4;
			idPoint = (int*)malloc(numPoints * sizeof(int));
			memset(idPoint, 0, sizeof(idPoint));
		}
	}
	bool set_group(int group) {
		if (this->group == -1) {
			this->group = group;
			return true;
		}
		return false;
	}
	bool addPoint(int x) {// add point id to this cell
		if (index == numPoints) return false;
		idPoint[index++] = x;
		return true;
	}
	void clear() {
		if (idPoint != NULL) {
			free(idPoint);
			idPoint = NULL;
		}
	}
	void print() {

	}
	void print_type() {

	}
}cell;
typedef struct UNSTRUCTURED_GRID {
	string header;
	string title;
	string dataType;
	string datasetType;
	string POINTS;
	string pointDataType;
	int n_points = 0;//num of points

	point *pointData;
	int index_points = 0;//index of pointData
	string CELLS;
	int n_cells = 0;//num of cells
	int size_cells = 0;//total num of points in cells + n_cells
	int index_cells = 0;//index of cells
	cell *cellData;
	string CELL_TYPES;
	void init() {
		//header = "# vtk DataFile Version 2.0";
		//title = "unstructured_grid";
		//dataType = "ASCII";
		//datasetType = "DATASET UNSTRUCTURED_GRID";
		//POINTS = "POINTS";
		//pointDataType = "double";
		//CELLS = "CELLS";
		//CELL_TYPES = "CELL_TYPES";
		n_points = 0;
		pointData = NULL;
		index_points = 0;
		index_cells = 0;
		n_cells = 0;
		size_cells = 0;
		cellData = NULL;
	}

	// set_nPoints and malloc pointData
	void set_nPoints(int n_points) {
		this->n_points = n_points;
		if (pointData != NULL) {
			free(pointData);
			pointData = NULL;
		}
		pointData = (point*)malloc(sizeof(point)*n_points);
		memset(pointData, 0, sizeof(pointData));
		index_points = 0;
	}

	//add point
	bool addPoint(double x, double y, double z, int i, int j, int k, int blockId, int sectId) {
		if (index_points == n_points) return false;
		pointData[index_points++].init(x, y, z, i, j, k, blockId, sectId);
	}

	//set_ncells and malloc cells
	void set_nCells(int n_cells) {
		if (cellData != NULL) {
			for (int i = 0; i < n_cells; i++) {
				cellData[i].clear();
			}

			free(cellData);
			cellData = NULL;
		}
		this->n_cells = n_cells;
		cellData = (cell*)malloc(sizeof(cell)*n_cells);
		memset(cellData, 0, sizeof cellData);
	}

	// add vetra cell 
	void addVetraCell(int id1, int id2, int id3, int id4) {
		cellData[index_cells].init(10);
		cellData[index_cells].set_group(index_cells);
		cellData[index_cells].addPoint(id1);
		cellData[index_cells].addPoint(id2);
		cellData[index_cells].addPoint(id3);
		cellData[index_cells].addPoint(id4);
		index_cells++;
	}
	void addVetraCell(int id1, int id2, int id3) {
		cellData[index_cells].init(5);
		cellData[index_cells].set_group(index_cells);
		cellData[index_cells].addPoint(id1);
		cellData[index_cells].addPoint(id2);
		cellData[index_cells].addPoint(id3);
		index_cells++;
	}
	// count size_cells
	bool count_size_cells() {
		if (n_cells == 0) {
			size_cells = 0;
			return true;
		}
		int res = n_cells;
		if (cellData == NULL) return false;
		for (int i = 0; i < n_cells; i++) {
			if (cellData[i].idPoint == NULL) return false;
			res += cellData[i].numPoints;
		}
		size_cells = res;
		return true;
	}
	void check() {
		int overlapNum = 0;
		map<point, int> mp;
		for (int i = 0; i < n_points; i++) {
			auto it = mp.find(pointData[i]);

			if (it == mp.end()) mp.insert(make_pair(pointData[i], i));
			else {
				overlapNum++;
			}
		}
	}
	void print() {
	}
	void clear() {
		if (pointData != NULL) {
			free(pointData);
			pointData = NULL;
		}
		if (cellData != NULL) {
			for (int i = 0; i < n_cells; i++) {
				cellData[i].clear();
			}

			free(cellData);
			cellData = NULL;
		}
	};

}UNSTRUCTURED_GRID;
typedef struct sectPool {
	int sectNum;
	int* pointNum;
	int* index;//to add point
	int** idInfo;
	int *colorFlag;//color or not
	double *min_x, *max_x, *min_y, *max_y, *max_z, *min_z;

	//sectIdMap[i]=j   i         Ӧsect е j    
	int *sectIdMap;
	int n_points;
	int *cellIndex;//to add cell
	//    sect е cell    
	int *cellNum;
	int **cellId1, **cellId2, **cellId3, **cellId4;
	void init() {
		sectNum = 0;
		pointNum = NULL;
		idInfo = NULL;
		index = NULL;

		colorFlag = NULL;
		min_x = max_x = min_y = max_y = min_z = max_z = NULL;

		sectIdMap = NULL;
		cellIndex = NULL;
		cellNum = NULL;
		cellId1 = cellId2 = cellId3 = cellId4 = NULL;
		n_points = 0;
	}
	//set sectNum and malloc
	void set_sectNum(int sectNum) {
		this->sectNum = sectNum;
		pointNum = (int*)malloc(sizeof(int)*sectNum);
		memset(pointNum, 0, sizeof(int)*sectNum);
		index = (int*)malloc(sizeof(int)*sectNum);
		memset(index, 0, sizeof(int)*sectNum);
		idInfo = (int**)malloc(sizeof(int*)*sectNum);
		memset(idInfo, 0, sizeof(int*)*sectNum);

		cellNum = new int[sectNum] {0};
		cellIndex = new int[sectNum] {0};
		cellId1 = new int*[sectNum] {0};
		cellId2 = new int*[sectNum] {0};
		cellId3 = new int*[sectNum] {0};
		cellId4 = new int*[sectNum] {0};


		colorFlag = (int*)malloc(sizeof(int)*sectNum);
		memset(colorFlag, 0, sizeof(int)*sectNum);
		min_x = (double*)malloc(sizeof(double)*sectNum);
		memset(min_x, 0, sizeof(double)*sectNum);
		max_x = (double*)malloc(sizeof(double)*sectNum);
		memset(max_x, 0, sizeof(double)*sectNum);
		min_y = (double*)malloc(sizeof(double)*sectNum);
		memset(min_y, 0, sizeof(double)*sectNum);
		max_y = (double*)malloc(sizeof(double)*sectNum);
		memset(max_y, 0, sizeof(double)*sectNum);
		min_z = (double*)malloc(sizeof(double)*sectNum);
		memset(min_z, 0, sizeof(double)*sectNum);
		max_z = (double*)malloc(sizeof(double)*sectNum);
		memset(max_z, 0, sizeof(double)*sectNum);
	}
	//set pointNum of sectId and malloc
	void setPointNum(int sectId, int pointCount) {
		pointNum[sectId] = pointCount;
		idInfo[sectId] = (int*)malloc(sizeof(int)*pointCount);
		memset(idInfo[sectId], 0, sizeof(idInfo[sectId]));
	}
	void setCellNum(int sectId, int cellCount) {
		cellNum[sectId] = cellCount;
		cellId1[sectId] = new int[cellCount] {0};
		cellId2[sectId] = new int[cellCount] {0};
		cellId3[sectId] = new int[cellCount] {0};
		cellId4[sectId] = new int[cellCount] {0};
	}
	void setNumofPoints(int pointNum) {
		n_points = pointNum;
		sectIdMap = new int[pointNum] {0};
	}
	void addPoint(int sectId, int pointId) {
		sectIdMap[pointId] = index[sectId];
		idInfo[sectId][index[sectId]++] = pointId;
	}
	void addCell(int sectId, int id1, int id2, int id3, int id4) {
		int Index = cellIndex[sectId];
		cellId1[sectId][Index] = id1;
		cellId2[sectId][Index] = id2;
		cellId3[sectId][Index] = id3;
		cellId4[sectId][Index] = id4;

		cellIndex[sectId]++;
	}
	void addCell(int sectId, int id1, int id2, int id3) {
		int Index = cellIndex[sectId];
		cellId1[sectId][Index] = id1;
		cellId2[sectId][Index] = id2;
		cellId3[sectId][Index] = id3;
		cellIndex[sectId]++;
	}
	void check() {

	}
	void clear() {
		for (int i = 0; i < sectNum; i++) {
			free(idInfo[i]);
			idInfo[i] = NULL;

			delete[]cellId1;
			cellId1 = NULL;
			delete[]cellId2;
			cellId2 = NULL;
			delete[]cellId3;
			cellId3 = NULL;
			delete[]cellId4;
			cellId4 = NULL;
		}
		if (cellNum != NULL) {
			delete[]cellNum;
			cellNum = NULL;
		}
		if (cellId1 != NULL) {
			delete[]cellId1;
			cellId1 = NULL;
		}
		if (cellId2 != NULL) {
			delete[]cellId2;
			cellId2 = NULL;
		}
		if (cellId3 != NULL) {
			delete[]cellId3;
			cellId3 = NULL;
		}
		if (cellId4 != NULL) {
			delete[]cellId4;
			cellId4 = NULL;
		}
		if (cellIndex != NULL) {
			delete[]cellIndex;
			cellIndex = NULL;
		}
		if (sectIdMap != NULL) {
			delete[]sectIdMap;
			sectIdMap = NULL;
		}

		free(idInfo);
		idInfo = NULL;
		free(pointNum);
		free(index);
		free(colorFlag);
		colorFlag = NULL;
		free(min_x);
		min_x = NULL;
		free(max_x);
		max_x = NULL;
		free(min_y);
		min_y = NULL;
		free(max_y);
		max_y = NULL;
		free(min_z);
		min_z = NULL;
		free(max_z);
		max_z = NULL;
		n_points = 0;
	}


}sectPool;
typedef struct decimatedInfo {
	int n_points;
	int n_cells;
	int(*direMark)[6];
	int *pointMark;
	//pointIdMap[i]=j  ԭ    i     ھ     id  j       -1  ˵       Ѹõ ȥ    
	int *pointIdMap;
	int index;
	//pointList[i]=j       i     ڶ Ӧ    ǰ  j  
	int *pointList;
	int cellIndex;
	//int(*cellList)[4];
	vector<vector<int>> cellList;
	//sectInfo
	int sectNum;
	int *pointNum;
	int** idInfo;
	void init() {
		n_points = 0;
		n_cells = 0;
		direMark = NULL;
		pointMark = NULL;
		pointIdMap = NULL;
		index = 0;
		pointList = NULL;
		cellIndex = 0;

		cellList.resize(0);
		//cellList = NULL;

		sectNum = 0;
		pointNum = NULL;
		idInfo = NULL;
	}
	void set_sectNum(int num) {
		sectNum = num;
		pointNum = new int[num] {0};
		idInfo = new int*[num] {0};
	}
	void setPointNum(int sectId, int pointCount) {
		pointNum[sectId] = pointCount;
		idInfo[sectId] = new int[pointCount] {0};
	}
	void mark(int id, int dire) {
		if (id >= n_points || dire >= 6 || direMark == NULL) {
			return;
		}
		direMark[id][dire] = 1;
	}
	void setNumOfPoints(int num) {
		n_points = num;
		direMark = new int[num][6]{ {0,0,0,0,0,0} };
		pointMark = new int[num] {0};
		pointIdMap = new int[num];
		memset(pointIdMap, -1, sizeof(int)*num);
		pointList = new int[num] {0};

	}
	void setNumOfCells(int num) {
		n_cells = 0;
		
		//cellList = new int[num][4]{ {0, 0, 0, 0} };
	}
	void addPoint(int id) {
		pointList[index] = id;
		pointIdMap[id] = index;
		index++;
	}
	void addCell(int id1, int id2, int id3, int id4) {
		cellList.push_back({ id1,id2,id3,id4 });
		/*cellList[cellIndex][0] = id1;
		cellList[cellIndex][1] = id2;
		cellList[cellIndex][2] = id3;
		cellList[cellIndex][3] = id4;*/
		cellIndex++;
	}
	void addCell(int id1, int id2, int id3) {
		cellList.push_back({ id1,id2,id3 });
		/*cellList[cellIndex][0] = id1;
		cellList[cellIndex][1] = id2;
		cellList[cellIndex][2] = id3;*/
		cellIndex++;
	}
	void clear() {
		if (direMark != NULL) delete[]direMark;
		if (pointMark != NULL) delete[]pointMark;
		if (pointIdMap != NULL) delete[]pointIdMap;
		if (pointList != NULL) delete[]pointList;
		cellList.resize(0);
		//if (cellList != NULL) delete[]cellList;
		if (pointNum != NULL) delete[]pointNum;
		if (idInfo != NULL) {
			for (int i = 0; i < sectNum; i++) {
				delete[]idInfo[i];
			}
			delete[]idInfo;
		}

		n_points = 0;
		n_cells = 0;
		direMark = NULL;
		pointMark = NULL;
		pointIdMap = NULL;
		index = 0;
		pointList = NULL;
		cellIndex = 0;
		cellList.resize(0);
		//cellList = NULL;

		sectNum = 0;
		pointNum = NULL;
		idInfo = NULL;
	}
}decimatedInfo;
typedef struct lineInfo {
	int lineNum;
	int *pointNum;
	int *sectId;

	int *index;
	int **pointList;

	void init() {
		lineNum = 0;
		pointNum = NULL;
		pointList = NULL;
		sectId = NULL;
		index = NULL;
	}
	void set_lineNum(int num) {
		lineNum = num;
		pointNum = new int[num] {0};
		pointList = new int*[num] {0};
		sectId = new int[num] {0};
		index = new int[num] {0};
	}
	void setPointNum(int lineId, int num) {
		//if (!num) return;
		//qDebug() << "lineId=" << lineId << " num=" << num;
		pointNum[lineId] = num;
		pointList[lineId] = new int[num] {0};
	}
	void addPoint(int lineId, int pointId) {
		pointList[lineId][index[lineId]++] = pointId;
	}
	void clear() {
		if (pointNum != NULL) {
			delete[]pointNum;
			pointNum = NULL;
		}
		if (pointList != NULL) {
			
			for (int i = 0; i < lineNum; i++) {
				//qDebug() << "i=" << i << " pointList[i]=" << pointList[i];
				if(pointList[i]!=NULL)delete[]pointList[i];
				pointList[i] = NULL;
			}
			delete[]pointList;
			pointList = NULL;
		}
		if (sectId != NULL) {
			delete[]sectId;
			sectId = NULL;
		}
		if (index != NULL) {
			delete[]index;
			index = NULL;
		}
		lineNum = 0;

	}
}lineInfo;

typedef struct bcInfo {
	//source_region
	int minI, maxI, minJ, maxJ, minK, maxK;
	int bc_num;
	//target_region
	int minI2, maxI2, minJ2, maxJ2, minK2, maxK2;
	int targetId;
	void setLine1(int minI, int maxI, int minJ, int maxJ, int minK, int maxK, int bc_num) {
		this->minI = minI;
		this->maxI = maxI;
		this->minJ = minJ;
		this->maxJ = maxJ;
		this->minK = minK;
		this->maxK = maxK;
		this->bc_num = bc_num;
	}
	void setLine2(int minI2, int maxI2, int minJ2, int maxJ2, int minK2, int maxK2, int targetId) {
		this->minI2 = minI2;
		this->maxI2 = maxI2;
		this->minJ2 = minJ2;
		this->maxJ2 = maxJ2;
		this->minK2 = minK2;
		this->maxK2 = maxK2;
		this->targetId = targetId;
	}
}bcInfo;