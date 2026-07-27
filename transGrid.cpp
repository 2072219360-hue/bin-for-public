#include "transGrid.h"
namespace platforms {
	namespace echv {
		void transGrid::AlgorithmInterface() {
			string filename = arg->getFilePath();
			int xSize = arg->getXsize(), ySize = arg->getYsize(), zSize = arg->getZsize() ;
			double interval = arg->getInterval();
			//qDebug() << " xSize=" << xSize << " ySize=" << ySize << " zSize=" << zSize;
			vtkNew<vtkDataSetReader> reader;
			reader->SetFileName(filename.c_str());
			reader->Update();
			// 获取读取的输出数据
			vtkSmartPointer<vtkDataSet> dataSet = reader->GetOutput();
			// 确定网格类型
			vtkSmartPointer<vtkUnstructuredGrid>  unstructuredGrid = vtkUnstructuredGrid::SafeDownCast(dataSet);
			vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkStructuredGrid::SafeDownCast(dataSet);
			if (structuredGrid)
			{
				this->data->setGird(structuredGrid);
				//qDebug() << "The file contains a structured grid.";
				//emit ECHVINSTANCE->signalPrint("The file contains a structured grid.");
			}
			else if (unstructuredGrid)
			{
				//qDebug() << "The file contains an unstructured grid." ;
				this->data->setUnGird(unstructuredGrid);
				structuredGrid=UnstrToStr(unstructuredGrid,xSize,ySize,zSize,interval);
				//qDebug() << "afther2 trans pointData num=" << structuredGrid->GetPointData()->GetNumberOfArrays();
				this->data->setGird(structuredGrid);
				
				vtkSmartPointer<vtkStructuredGridWriter> writer = vtkSmartPointer<vtkStructuredGridWriter>::New();
				writer->SetFileName("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output.vtk");
				writer->SetInputData(structuredGrid);
				writer->Write();

				//emit ECHVINSTANCE->signalPrint("The file contains an unstructured grid.We need transform it to structured grid first.");
			}
			else
			{
				//qDebug() << "The file does not contain a recognized grid type." ;
				//emit ECHVINSTANCE->signalPrint("The file does not contain a recognized grid type.");
			}
		}
		vtkSmartPointer<vtkStructuredGrid> transGrid::UnstrToStr(vtkSmartPointer<vtkUnstructuredGrid> ungrid,int xSize,int ySize,int zSize,double interval) {
			
			//点坐标
			vtkSmartPointer<vtkPoints> points = ungrid->GetPoints();
			//点上数据
			vtkSmartPointer<vtkPointData> pointData = ungrid->GetPointData();
			vtkSmartPointer<vtkCellArray> cells = ungrid->GetCells();
			//vector<string> scalarName;
			
			//获得结构化网格的点的坐标
			double xMin= points->GetPoint(0)[0], xMax= points->GetPoint(0)[0],
				yMin= points->GetPoint(0)[1], yMax= points->GetPoint(0)[1],
				zMin= points->GetPoint(0)[2], zMax= points->GetPoint(0)[2];
			for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
				double *p=points->GetPoint(i);
				double x = p[0], y = p[1], z = p[2];
				xMin = min(x, xMin);
				xMax = max(x, xMax);
				yMin = min(y, yMin);
				yMax = max(y, yMax);
				zMin = min(z, zMin);
				zMax = max(z, zMax);
			}
			vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
			//根据xyzSize计算网格大小
			//xSize = 81, ySize = 151, zSize = 138;
			//double xGap = (xMax-xMin)/(xSize-1), yGap = (yMax - yMin) / (ySize - 1), zGap = (zMax - zMin) / (zSize - 1);
			//qDebug() << "xGap=" << xGap << " yGap=" << yGap << " zGap=" << zGap;
			//for (int x = 0; x < xSize; x++) {
			//	for (int y = 0; y < ySize; y++) {
			//		for (int z = 0; z < zSize; z++) {
			//			newPoints->InsertNextPoint(xMin+xGap*x,yMin+yGap*y,zMin+zGap*z);
			//			//qDebug() << "x=" << xMin + xGap * x << " y=" << yMin + yGap * y << " z=" << zMin + zGap * z;
			//		}
			//	}
			//}
			//根据interval计算网格大小
			double xGap = interval, yGap = interval, zGap = interval;
			xSize= (xMax - xMin) / xGap, ySize = (yMax - yMin) / yGap, zSize = (zMax - zMin) / zGap;
			xSize++; ySize++; zSize++;

			/*if (xMin + xGap * (xSize - 1) < xMax) xSize++;
			if (yMin + yGap * (ySize - 1) < yMax) ySize++;
			if (zMin + zGap * (zSize - 1) < zMax) zSize++;*/
			qDebug() << "min_x=" << xMin << " max_x=" << xMax << " min_y=" << yMin << " max_y=" << yMax << " min_z=" << zMin << " max_z=" << zMax;
			qDebug() << " new_maxX=" << xMin + xGap * (xSize - 1) << " new_maxY=" << yMin + yGap * (ySize - 1) << " new_maxZ=" << zMin + zGap * (zSize - 1);
			qDebug() << "interval=" << interval << " xSize=" << xSize << " ySize=" << ySize << " zSize=" << zSize;
			//for (int x = 0; x < xSize; x++)
			for (int z = 0; z < zSize; z++) {
				//qDebug() << "x=" << xMin + xGap * x;
				for (int y = 0; y < ySize; y++) {
					//for (int z = 0; z < zSize; z++) 
						for (int x = 0; x < xSize; x++) {
						//if(xMin + xGap * x<=xMax && yMin + yGap * y<=yMax && zMin + zGap * z<=zMax)
						double now_x = xMin + xGap * x, now_y = yMin + yGap * y, now_z = zMin + zGap * z;
						if (now_x > xMax) now_x = xMax;
						if (now_y > yMax) now_y = yMax;
						if (now_z > zMax) now_z = zMax;
						newPoints->InsertNextPoint(now_x, now_y, now_z);
					}
				}
			}

			//qDebug() << "num of points="<<newPoints->GetNumberOfPoints();
			//vtkSmartPointer<vtkPointData> newScalars = vtkSmartPointer<vtkPointData>::New();
			vtkIdType newSize = xSize * ySize*zSize;
			vector< vtkSmartPointer<vtkDataArray> > v;
			for (int i = 0; i < pointData->GetNumberOfArrays(); i++) {
				//string name=pointData->GetArray(i)->GetName();
				//scalarName.push_back(name);
				int dataType=pointData->GetArray(i)->GetDataType();
				switch (dataType)
				{
				case VTK_DOUBLE: {
					
					v.push_back(vtkSmartPointer<vtkDoubleArray>::New() );
;					break;
				}
				case VTK_INT: {
					v.push_back(vtkSmartPointer<vtkIntArray>::New());
					break;
				}
				case VTK_FLOAT: {
					v.push_back(vtkSmartPointer<vtkFloatArray>::New());
					break;
				}
				default: {
					v.push_back(vtkSmartPointer<vtkDoubleArray>::New());
					break;
				}
					
				}
				//vtkSmartPointer<vtkDataArray> temp = vtkSmartPointer<vtkDataArray>::New();
				//v.push_back(temp);
				v[i]->SetName(pointData->GetArray(i)->GetName());
				v[i]->SetNumberOfComponents(pointData->GetArray(i)->GetNumberOfComponents());
				v[i]->SetNumberOfTuples(newSize);
				
			}
			//qDebug() << "befor trans pointData num="<<pointData->GetNumberOfArrays();
			//对标量进行插值
			/*vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
			cellLocator->SetDataSet(ungrid);
			cellLocator->BuildLocator();*/

			transCell(ungrid);

			vtkSmartPointer<vtkCellTreeLocator> cellLocator = vtkSmartPointer<vtkCellTreeLocator>::New();
			cellLocator->SetDataSet(ungrid);
			cellLocator->BuildLocator();
			
			//vector<vtkIdType> inside;
			for (vtkIdType i = 0; i < newSize; i++) {
				double *point = newPoints->GetPoint(i);
				//qDebug() << "point x=" << point[0] << " y=" << point[1] << " z=" << point[2];
				
				vtkIdType cellId = cellLocator->FindCell(point);
				//inside.push_back(cellId);
				//目前只处理四面体
				
				if (cellId != -1) {
					vtkSmartPointer<vtkCell> cell = ungrid->GetCell(cellId);
					vtkSmartPointer<vtkIdList> point_ids=cell->GetPointIds();
					vtkIdType npts = cell->GetNumberOfPoints();
					vector<vtkIdType> point_index;
					//vector<vector<double>> point_array;
					double point_array[10][3];
					for (vtkIdType j = 0; j < npts; j++) {
						point_index.push_back(point_ids->GetId(j));

						point_array[j][0] = cell->GetPoints()->GetPoint(j)[0];
						point_array[j][1] = cell->GetPoints()->GetPoint(j)[1];
						point_array[j][2] = cell->GetPoints()->GetPoint(j)[2];
					}
					//qDebug() << " point_array size=" << point_array.size();
					/*for (auto it : point_array) {
						qDebug() << " x=" << it[0] << " y=" << it[1] << " z=" << it[2];
					}*/
					/*for (vtkIdType j = 0; j < npts; j++) {
						qDebug() << "i=" << i << " j=" << j<<" point_index="<<point_index[j];
						qDebug()<< " x=" << point_array[j][0] << " y=" << point_array[j][1] << " z=" << point_array[j][2];
					}*/
					double vol, vol0, vol1, vol2, vol3;
					vol = volume3D(point_array[0], point_array[1], point_array[2], point_array[3]);
					vol0 = volume3D(point, point_array[1], point_array[2], point_array[3]);
					vol1 = volume3D(point_array[0], point, point_array[2], point_array[3]);
					vol2 = volume3D(point_array[0], point_array[1], point, point_array[3]);
					vol3 = volume3D(point_array[0], point_array[1], point_array[2], point);
					/*qDebug() << "i=" << i << " vol=" << vol << " vol0=" << vol0 << " vol1=" << vol1
						<<" vol2="<< vol2<<" vol3="<< vol3;*/
					for (vtkIdType j = 0; j < pointData->GetNumberOfArrays(); j++) {
						for (int k = 0; k < v[j]->GetNumberOfComponents(); k++) {
							double val = 0;
							double per0, per1, per2, per3;
							per0 = safeValue(vol0 / vol);
							per1 = safeValue(vol1 / vol);
							per2 = safeValue(vol2 / vol);
							per3 = safeValue(vol3 / vol);
							//qDebug() << "j=" << j << " k=" << k << " numOfTuples=" << v[j]->GetNumberOfTuples() << " numOfCom=" << v[j]->GetNumberOfComponents();
							val += pointData->GetArray(j)->GetComponent(point_index[0], k)* per0;
							val += pointData->GetArray(j)->GetComponent(point_index[1], k)* per1;
							val += pointData->GetArray(j)->GetComponent(point_index[2], k)* per2;
							val += pointData->GetArray(j)->GetComponent(point_index[3], k)* per3;
							/*if (isinf(val) || isnan(val)) {
								qDebug() << "val=" << val;
								qDebug() << "i=" << i << " vol=" << vol << " vol0=" << vol0 << " vol1=" << vol1
						<<" vol2="<< vol2<<" vol3="<< vol3;
							}*/
								
							v[j]->SetComponent(i, k, val);
						}

						

					}
					
				}
			}
			//设置结构化网格
			vtkSmartPointer<vtkStructuredGrid>  grid= vtkSmartPointer<vtkStructuredGrid>::New();
			grid->SetDimensions(xSize, ySize, zSize);
			grid->SetPoints(newPoints);
			for (int i = 0; i < pointData->GetNumberOfArrays(); i++) {
				grid->GetPointData()->AddArray(v[i]);
			}
			//qDebug() << "afther trans pointData num=" << grid->GetPointData()->GetNumberOfArrays();
			//处理double
			//processDouble(grid);
			return grid;
		}
		double transGrid::safeValue(double x) {
			double inf = 1e50;//可以改变
			if (isnan(x)) x = 0;
			if (isinf(x)) {
				if (x > 0) {
					x = inf;
				}else {
					x = -inf;
				}
			}
			return x;
		}
		double transGrid::volume3D(double *a, double *b, double *c, double *d) {
			/*qDebug() << "a=" << a[0] << " " << a[1] << " " << a[2];
			qDebug() << "b=" << b[0] << " " << b[1] << " " << b[2];
			qDebug() << "c=" << c[0] << " " << c[1] << " " << c[2];
			qDebug() << "d=" << d[0] << " " << d[1] << " " << d[2];*/
			double v1[3], v2[3], v3[3],v4[3],res=0;
			sub3D(a, d, v1);
			sub3D(b, d, v2);
			sub3D(c, d, v3);
			/*qDebug() << "v2=" << v2[0] << " " << v2[1] << " " << v2[2];
			qDebug() << "v3=" << v3[0] << " " << v3[1] << " " << v3[2];
			cross3D(v2,v3,v4);
			qDebug() << "v1=" << v1[0] << " " << v1[1] << " "<<v1[2];
			qDebug() << "v4=" << v4[0] << " " << v4[1] << " " << v4[2];*/
			cross3D(v2, v3, v4);
			res = dot3D(v1,v4);
			if (res < 0) res = -res;
			res /= 6;
			return res;
		}
		void transGrid::sub3D(double *v1, double *v2, double *res) {
			res[0] = v1[0] - v2[0];
			res[1] = v1[1] - v2[1];
			res[2] = v1[2] - v2[2];
		}
		void transGrid::cross3D(double *v1, double *v2, double *res) {
			res[0] = v1[1] * v2[2] - v1[2] * v2[1];
			res[1] = v1[2] * v2[0] - v1[0] * v2[2];
			res[2] = v1[0] * v2[1] - v1[1] * v2[0];
		}
		double transGrid::dot3D(double *v1, double *v2) {
			return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
		}
		//将ungrid中的cell由六面体转变成四面体
		void transGrid::transCell(vtkSmartPointer<vtkUnstructuredGrid> ungrid) {
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

			for (vtkIdType i = 0; i < ungrid->GetNumberOfCells(); i++) {
				vtkCell* cell = ungrid->GetCell(i);
				int cellType=cell->GetCellType();
				switch (cellType)
				{
				//四面体
				case VTK_TETRA: {
					/*vtkSmartPointer<vtkTetra> newCell = vtkSmartPointer<vtkTetra>::New();
					newCell->DeepCopy(cell);*/
					vtkSmartPointer<vtkCell> newCell = ungrid->GetCell(i);
					cells->InsertNextCell(newCell);
					break;
				}
				//六面体
				case VTK_HEXAHEDRON: {
					//vtkSmartPointer<vtkTetra> newCell = vtkSmartPointer<vtkTetra>::New();
					vtkIdList *ids=cell->GetPointIds();
					vtkIdType id[8];
					for (int id_index = 0; id_index < 8; id_index++) {
						id[id_index] = ids->GetId(id_index);
					}
					vtkSmartPointer<vtkIdList>cell1 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell2 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell3 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell4 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell5 = vtkSmartPointer<vtkIdList>::New();
					cell1->InsertNextId(id[0]); cell1->InsertNextId(id[1]); cell1->InsertNextId(id[2]); cell1->InsertNextId(id[5]);
					cell2->InsertNextId(id[0]); cell2->InsertNextId(id[2]); cell2->InsertNextId(id[3]); cell2->InsertNextId(id[7]);
					cell3->InsertNextId(id[0]); cell3->InsertNextId(id[4]); cell3->InsertNextId(id[7]); cell3->InsertNextId(id[5]);
					cell4->InsertNextId(id[2]); cell4->InsertNextId(id[7]); cell4->InsertNextId(id[6]); cell4->InsertNextId(id[5]);
					cell5->InsertNextId(id[0]); cell5->InsertNextId(id[5]); cell5->InsertNextId(id[7]); cell5->InsertNextId(id[2]);

					cells->InsertNextCell(cell1); 
					cells->InsertNextCell(cell2); 
					cells->InsertNextCell(cell3); 
					cells->InsertNextCell(cell4); 
					cells->InsertNextCell(cell5);
					
					
					
					break;
				}
				 //六面体
				case VTK_VOXEL: {
					vtkIdList *ids = cell->GetPointIds();
					vtkIdType id[8];
					for (int id_index = 0; id_index < 8; id_index++) {
						id[id_index] = ids->GetId(id_index);
					}
					vtkSmartPointer<vtkIdList>cell1 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell2 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell3 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell4 = vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList>cell5 = vtkSmartPointer<vtkIdList>::New();
					cell1->InsertNextId(id[0]); cell1->InsertNextId(id[1]); cell1->InsertNextId(id[3]); cell1->InsertNextId(id[5]);
					cell2->InsertNextId(id[0]); cell2->InsertNextId(id[3]); cell2->InsertNextId(id[5]); cell2->InsertNextId(id[6]);
					cell3->InsertNextId(id[0]); cell3->InsertNextId(id[2]); cell3->InsertNextId(id[3]); cell3->InsertNextId(id[6]);
					cell4->InsertNextId(id[0]); cell4->InsertNextId(id[4]); cell4->InsertNextId(id[5]); cell4->InsertNextId(id[6]);
					cell5->InsertNextId(id[3]); cell5->InsertNextId(id[5]); cell5->InsertNextId(id[6]); cell5->InsertNextId(id[7]);

					cells->InsertNextCell(cell1);
					cells->InsertNextCell(cell2);
					cells->InsertNextCell(cell3);
					cells->InsertNextCell(cell4);
					cells->InsertNextCell(cell5);


					break;
				}
				default: break;
					
				}
			}

			ungrid->SetCells(VTK_TETRA,cells);
		}

		void transGrid::processDouble(vtkSmartPointer<vtkStructuredGrid> grid) {
			vtkSmartPointer<vtkPointData> pointData = grid->GetPointData();
			for (int i = 0; i < pointData->GetNumberOfArrays(); i++) {
				//string name=pointData->GetArray(i)->GetName();
				//scalarName.push_back(name);
				int dataType = pointData->GetArray(i)->GetDataType();
				if (dataType != VTK_FLOAT) continue;
				qDebug() << "name=" << pointData->GetArray(i)->GetName()<<" dim="<< pointData->GetArray(i)->GetNumberOfComponents();
				for (int j = 0; j < pointData->GetArray(i)->GetNumberOfTuples(); j++) {
					for (int k = 0; k < pointData->GetArray(i)->GetNumberOfComponents(); k++) {
						float x = pointData->GetArray(i)->GetComponent(j, k);
						if (std::isinf(x)) {
							if (x > 0) {
								x=numeric_limits<float>::max();
							}
							else {
								x=-std::numeric_limits<float>::max();
							}
						}
						else if (isnan(x)) x = 0;
						pointData->GetArray(i)->SetComponent(j, k, x);
					}
				}
				

			}
		}
	}
}




