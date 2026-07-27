#include "relevance.h"

namespace platforms {
	namespace echv {
		void relevance::AlgorithmInterface() {
			
			vector<vector<int>> *new_JT = data->getJT();
			vector<vector<int>>& JT = *new_JT;
			vector<vector<int>> *new_ST = data->getST();
			vector<vector<int>>& ST = *new_ST;
			vector<vector<int>> *new_CT = data->getCT();
			vector<vector<int>>& CT = *new_CT;
			qDebug() << "JT.size=" << JT.size() << " ST.size=" << ST.size() << " CT.size=" << CT.size();
			string scalar_name = "lambda2-vtk";
			vtkSmartPointer<vtkStructuredGrid> grid = data->getGrid();

			double interval = arg->getInterval();
			//vtkSmartPointer<vtkPoints> points = grid->GetPoints();
			//vtkPointData* attributes_data = grid->GetPointData();
			//vtkDataArray* scalars_data = attributes_data->GetArray(scalar_name.c_str());

			qDebug() << "begin get points ";
			vector<vector<double>> nodes = getPoints(grid);
			qDebug() << "begin get scalars ";
			vector<double> pvalue_list = getScalars(grid, scalar_name);
			if (pvalue_list.size()) qDebug() << "scalar_name is found";
			else qDebug() << "scalar_name is not found";

			vector<vector<int>>& lines = ST;
			
			
			qDebug() << "begin saddle_points ";
			//计算鞍点
			set<int> saddle_points = count_elements(lines);
			//获得极值点的分支（极值点到鞍点）
			vector<vector<int>> extreme_point_branch;
			vector<vector<double>> bound_list;
			vector<double> volum_list;
			vector<pair<double, double>> q_ranges;
			qDebug() << "begin extreme_point_branch";
			//这部分最耗时
			for (const auto& line : lines) {
				for (int i = 0; i < line.size(); i++) {
					if (saddle_points.find(line[i])!=saddle_points.end() && (i + 1) > 2 && (i+1)<=line.size() ) {
						vector<int> branch(line.begin(), line.begin() + i + 1);
						vector<double> re_bounds= get_Bounding_Box(nodes, branch);
						double vol = caculate_volume(re_bounds);
						if (vol != 0) {
							double min_q=0, max_q=0;
							if (!branch.empty()) min_q = max_q = pvalue_list[branch[0]];
							for (const auto& index : branch) {
								min_q = min(min_q,pvalue_list[index]);
								max_q = max(max_q, pvalue_list[index]);
							}
							//qDebug() << "min_q=" << min_q << " max_q=" << max_q;
							bool flag = judge_qRanges(scalar_name, min_q, max_q);
							if (flag) {
								q_ranges.push_back(make_pair(min_q, max_q));
								bound_list.push_back(re_bounds);
								volum_list.push_back(vol);
								extreme_point_branch.push_back(branch);
							}

						}
					}
				}
			}
			qDebug() << " extreme_point_branch.size=" << extreme_point_branch.size();
			sort(q_ranges.begin(), q_ranges.end());
			string extrem_outline_name = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_extrem_outline.vtk";
			qDebug() << "begin write";
			write_extreme_outline(extreme_point_branch,bound_list, extrem_outline_name);

			string region_name = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_extrem_region.vtk";
			vector<int> seed_point_ids;
			for (const auto&it : extreme_point_branch) {
				seed_point_ids.push_back(it[0]);
			}
			//区域增长
			vtkSmartPointer<vtkThreshold> regions = region_growing(region_name, scalar_name, grid, pvalue_list, seed_point_ids, 0, 1);
			//vtkSmartPointer<vtkThreshold>过滤后的数据类型是非结构化网格
			vtkSmartPointer<vtkUnstructuredGrid> regionsData = regions->GetOutput();
			//对增长后的区域进行聚类
			cluster_cell(regionsData,interval);

			string regionIdName = "RegionId";
			regionIdName = "ClusterId";
			vtkSmartPointer<vtkIdTypeArray> regionIdArrayVTK = vtkIdTypeArray::SafeDownCast(regionsData->GetCellData()->GetArray(regionIdName.c_str()));
			vector<int> regionIdArray;
			for (vtkIdType i = 0; i < regionIdArrayVTK->GetNumberOfTuples(); ++i)
			{
				
				regionIdArray.push_back(regionIdArrayVTK->GetValue(i));
			}
			// 计算值的分布
			std::map<int, int> regionCount;
			for (int id : regionIdArray)
			{
				regionCount[id]++;
			}
			vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
			for (const auto& pair : regionCount)
			{
				int unid = pair.first;
				qDebug() << "unid=" << unid<<" size="<< pair.second<<" point.size="<< regionIdArrayVTK->GetNumberOfTuples();
				vtkSmartPointer<vtkThreshold> threshold = vtkSmartPointer<vtkThreshold>::New();
				threshold->SetInputData(regionsData);
				threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, regionIdName.c_str());
				threshold->ThresholdBetween(unid, unid);
				threshold->Update();

				vtkSmartPointer<vtkPolyData> faceFilter = extractSurface(threshold->GetOutput());
				appendFilter->AddInputData(faceFilter);
			}
			appendFilter->Update();

			string face_name = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_face.vtk";
			// 写入文件
			vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
			writer->SetFileName(face_name.c_str());
			writer->SetInputData(appendFilter->GetOutput());
			writer->Write();

		}
		vector<vector<double>> relevance::getPoints(vtkSmartPointer<vtkStructuredGrid> grid) {
			vtkSmartPointer<vtkPoints> points = grid->GetPoints();
			int n = points->GetNumberOfPoints();
			vector<vector<double>> nodes(n);
			for (int i = 0; i < n; i++) {
				
				double point[3];
				points->GetPoint(i, point);
				for (int j = 0; j < 3; j++) nodes[i].push_back(point[j]);
			}
			return nodes;
		}
		vector<double> relevance::getScalars(vtkSmartPointer<vtkStructuredGrid> grid,string scalar_name) {
			vector<double> pvalue_list;
			vtkDataArray* scalars_data = grid->GetPointData()->GetArray(scalar_name.c_str());
			if (scalars_data!=nullptr) {
				for (int i = 0; i < scalars_data->GetNumberOfTuples(); i++) {
					pvalue_list.push_back(scalars_data->GetComponent(i, 0));
				}
			}
			return pvalue_list;
		}
		//统计lines中出现次数超过一次的节点id
		set<int> relevance::count_elements(vector<vector<int>>& lines) {
			set<int> result;
			/*map<int, int> mp;
			for (auto& line : lines) {
				for (auto& id : line) {
					auto it = mp.find(id);
					if (it == mp.end()) {
						mp[id] = 1;
					}else it->second++;
				}
			}
			for (auto& it : mp) {
				if (it.second > 1) result.push_back(it.first);
			}*/
			for (auto& line : lines) {
				for (auto& id : line) {
					result.insert(id);
				}
			}
			return result;
		}
		//获取包围盒x_min,x_max,y_min,y_max,z_min,z_max
		vector<double> relevance::get_Bounding_Box(vector<vector<double>>& nodes,vector<int> &id_list) {
			vector<double> BoundingBox;
			if (nodes.size() == 0) {
				BoundingBox.resize(6, 0.0);
				return BoundingBox;
			}
			BoundingBox.push_back(nodes[id_list[0]][0]); BoundingBox.push_back(nodes[id_list[0]][0]);
			BoundingBox.push_back(nodes[id_list[0]][1]); BoundingBox.push_back(nodes[id_list[0]][1]);
			BoundingBox.push_back(nodes[id_list[0]][2]); BoundingBox.push_back(nodes[id_list[0]][2]);
			for (auto id : id_list) {
				double x = nodes[id][0], y = nodes[id][1], z = nodes[id][2];
				BoundingBox[0] = min(x, BoundingBox[0]);
				BoundingBox[1] = max(x, BoundingBox[1]);
				BoundingBox[2] = min(y, BoundingBox[2]);
				BoundingBox[3] = max(y, BoundingBox[3]);
				BoundingBox[4] = min(z, BoundingBox[4]);
				BoundingBox[5] = max(z, BoundingBox[5]);
			}
			return BoundingBox;
		}
		//计算包围盒体积
		double relevance::caculate_volume(vector<double> bound) {
			return (bound[1] - bound[0]) * (bound[3] - bound[2]) * (bound[5] - bound[4]);
		}
		bool relevance::judge_qRanges(string scalar_name,double min_q,double max_q) {
			if (scalar_name == "lambda2-vtk") {
				if (max_q < 0) return true;
			}
			if (scalar_name == "q-vtk") {
				if (min_q > 0) return true;
			}

			return false;
		}
		//将outline轮廓写到vtk文件中
		void relevance::write_extreme_outline(const vector<vector<int>>& extreme_point_branch, const vector<vector<double>>& bound_list,string file_name) {
			vtkSmartPointer<vtkAppendPolyData> appendFilter1 = vtkSmartPointer<vtkAppendPolyData>::New();
			for (int i = 0; i < extreme_point_branch.size(); i++) {
				vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
				vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
				double bounds[6];
				for (int j = 0; j < 6; j++) bounds[j] = bound_list[i][j];
				cube->SetBounds(bounds);
				outline->SetInputConnection(cube->GetOutputPort());
				outline->Update();
				appendFilter1->AddInputData(outline->GetOutput());
			}
			appendFilter1->Update();

			vtkSmartPointer<vtkPolyDataWriter> writer1 = vtkSmartPointer<vtkPolyDataWriter>::New();
			writer1->SetFileName(file_name.c_str());
			writer1->SetInputData(appendFilter1->GetOutput());
			writer1->Write();
		}

		//进行区域增长
		vtkSmartPointer<vtkThreshold> relevance::region_growing(string file_name,string scalar_name, vtkSmartPointer<vtkStructuredGrid> grid,
			const vector<double>& scalar_data,const vector<int>& seed_point_ids,double min_value,double max_value) {
			
			vtkSmartPointer<vtkIdTypeArray> region_ids = vtkSmartPointer<vtkIdTypeArray>::New();
			region_ids->SetName("RegionId");
			region_ids->SetNumberOfTuples(grid->GetNumberOfCells());
			for (vtkIdType i = 0; i < grid->GetNumberOfCells(); ++i)
			{
				// 设置这个单元格的RegionId为1
				region_ids->SetValue(i, 0);
			}
			// 创建一个集合来存储已经访问过的点
			set<vtkIdType> visited;
			// 创建一个新的点数据数组来保存结果
			vtkSmartPointer<vtkPoints> new_points = vtkSmartPointer<vtkPoints>::New();
			// 创建一个新的标量数据数组来保存结果
			vtkSmartPointer<vtkDoubleArray> new_scalars = vtkSmartPointer<vtkDoubleArray>::New();
			// 设置数组的名称
			new_scalars->SetName("q");

			for (const auto& re_id : seed_point_ids) {
				queue<vtkIdType> q;
				set<vtkIdType> q_set;
				q.push(re_id);
				q_set.insert(re_id);
				while (!q.empty()) {
					// 从队列中取出一个点
					vtkIdType point_id = q.front();
					q.pop();
					// 获取这个点的标量值
					// double value = scalar_data->GetValue(point_id);
					// 检查这个点所在的所有单元格
					vtkSmartPointer<vtkIdList> cell_ids = vtkSmartPointer<vtkIdList>::New();
					grid->GetPointCells(point_id, cell_ids);
					for (vtkIdType i = 0; i < cell_ids->GetNumberOfIds(); ++i) {
						vtkIdType cell_id = cell_ids->GetId(i);
						vtkCell* cell = grid->GetCell(cell_id);
						vtkSmartPointer<vtkIdList> point_ids = cell->GetPointIds();
						bool flag = judge_region_growing(scalar_name,min_value,max_value,scalar_data,point_ids);
						if (flag) {
							region_ids->SetValue(cell_id, 1);
							for (vtkIdType j = 0; j < point_ids->GetNumberOfIds(); ++j)
							{
								vtkIdType p = point_ids->GetId(j);
								if (q_set.find(p) == q_set.end() && visited.find(p) != visited.end() )
								{
									q.push(p);
									q_set.insert(p);
								}
							}
						}
					}
					visited.insert(point_id);
				}
			}



			for (vtkIdType i = 0; i < grid->GetNumberOfCells(); ++i)
			{
				// 检查 RegionId 是否为 1
				if (region_ids->GetValue(i) == 1)
				{
					// 计算邻居数量
					int num_neighbors = compute_neighbors_count(grid, i, region_ids);

					// 检查邻居数量是否小于 6
					if (num_neighbors < 6)
					{
						region_ids->SetValue(i, 0);
					}
				}
			}
			grid->GetCellData()->AddArray(region_ids);
			// 创建一个 vtkThreshold 实例
			vtkSmartPointer<vtkThreshold> threshold = vtkSmartPointer<vtkThreshold>::New();
			// 设置输入连接
			threshold->SetInputData(grid);
			// 设置处理的输入数组
			threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, "RegionId");
			// 设置阈值
			threshold->ThresholdByUpper(1);
			// 更新过滤器
			threshold->Update();

			vtkSmartPointer<vtkStructuredGridWriter > writer1 = vtkSmartPointer<vtkStructuredGridWriter>::New();
			qDebug() << "write fileName=" << file_name.c_str();
			writer1->SetFileName(file_name.c_str());
			writer1->SetInputData(threshold->GetOutput());
			writer1->Write();

			return threshold;
		}
		//判断是否进行区域增长
		bool relevance::judge_region_growing(string scalar_name,double min_value,double max_value,
			const vector<double>& scalar_data, vtkSmartPointer<vtkIdList> point_ids) {
			if (scalar_name == "q") {
				for (int i = 0; i < point_ids->GetNumberOfIds(); i++) {
					if (scalar_data[i] < min_value) return false;
				}
				return true;
			}
			if (scalar_name == "lambda2-vtk") {
				for (int i = 0; i < point_ids->GetNumberOfIds(); i++) {
					if (scalar_data[i] > max_value) return false;
				}
				return true;
			}
			return false;
		}
		//计算邻居数量
		int relevance::compute_neighbors_count(vtkSmartPointer<vtkStructuredGrid> grid, vtkIdType cell_id, vtkSmartPointer<vtkIdTypeArray> region_ids) {
			vtkCell* cell = grid->GetCell(cell_id); // 获取单元格
			vtkSmartPointer<vtkIdList> point_ids = cell->GetPointIds(); // 获取单元格的点
			std::set<vtkIdType> neighbors;
			for (vtkIdType point_id_index = 0; point_id_index < point_ids->GetNumberOfIds(); ++point_id_index)
			{
				vtkIdType point_id = point_ids->GetId(point_id_index); // 获取点的索引
				vtkSmartPointer<vtkIdList> cell_ids = vtkSmartPointer<vtkIdList>::New();
				grid->GetPointCells(point_id, cell_ids); // 获取包含该点的所有单元格

				for (vtkIdType j = 0; j < cell_ids->GetNumberOfIds(); ++j)
				{
					vtkIdType neighbor_cell_id = cell_ids->GetId(j);
					if (cell_id != neighbor_cell_id && region_ids->GetValue(neighbor_cell_id) == 1) // 排除自身
					{
						neighbors.insert(neighbor_cell_id);
					}
				}
			}

			return neighbors.size();

		}
		//提取表面
		vtkSmartPointer<vtkPolyData> relevance::extractSurface(vtkSmartPointer<vtkUnstructuredGrid> dataset) {
			if (dataset->GetNumberOfPoints() == 0 || dataset->GetNumberOfCells() == 0)
			{
				return nullptr;
			}
			// 创建vtkGeometryFilter对象
			vtkSmartPointer<vtkGeometryFilter> surfaceFilter = vtkSmartPointer<vtkGeometryFilter>::New();
			surfaceFilter->SetInputData(dataset);
			surfaceFilter->Update();

			if (surfaceFilter->GetOutput()->GetNumberOfPoints() == 0 || surfaceFilter->GetOutput()->GetNumberOfCells() == 0)
			{
				return nullptr;
			}
			// 创建vtkSmoothPolyDataFilter对象
			vtkSmartPointer<vtkSmoothPolyDataFilter> smoothFilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
			smoothFilter->SetInputConnection(surfaceFilter->GetOutputPort());
			smoothFilter->SetNumberOfIterations(300);
			// smoothFilter->SetRelaxationFactor(0.1);
			// smoothFilter->FeatureEdgeSmoothingOff();
			// smoothFilter->BoundarySmoothingOn();
			smoothFilter->Update();

			if (smoothFilter->GetOutput()->GetNumberOfPoints() == 0 || smoothFilter->GetOutput()->GetNumberOfCells() == 0)
			{
				return nullptr;
			}

			return smoothFilter->GetOutput();

		}
		void relevance::cluster_cell(vtkSmartPointer<vtkUnstructuredGrid> regionData,double interval) {
			vector<DBSCAN::Point> points;
			for (int i = 0; i < regionData->GetNumberOfCells(); i++) {
				vtkCell* cell = regionData->GetCell(i);
				vtkIdList* pointIds = cell->GetPointIds();
				double center[3] = { 0,0,0 };
				if (pointIds->GetNumberOfIds() == 0) {
					points.push_back(DBSCAN::Point(center[0], center[1], center[2]));
					continue;
				}
				for (vtkIdType i = 0; i < pointIds->GetNumberOfIds(); ++i)
				{
					double p[3];
					regionData->GetPoint(pointIds->GetId(i), p);
					center[0] += p[0];
					center[1] += p[1];
					center[2] += p[2];
					
				}
				center[0] /= pointIds->GetNumberOfIds();
				center[1] /= pointIds->GetNumberOfIds();
				center[2] /= pointIds->GetNumberOfIds();
				points.push_back(DBSCAN::Point(center[0], center[1], center[2]));
			}
			double eps= 0.24;
			int min_pts= 26;// 6 26

			eps = interval * 1.732 + interval * 0.1;
			min_pts = 26;

			/*eps = interval + interval * 0.1;
			min_pts = 6;*/
			DBSCAN::myDBSCAN(points, eps, min_pts);

			vtkSmartPointer<vtkIdTypeArray> Cluster_ids = vtkSmartPointer<vtkIdTypeArray>::New();
			Cluster_ids->SetName("ClusterId");
			Cluster_ids->SetNumberOfTuples(regionData->GetNumberOfCells());
			for (vtkIdType i = 0; i < points.size(); i++) {
				Cluster_ids->SetValue(i, points[i].cluster_id);
			}
			regionData->GetCellData()->AddArray(Cluster_ids);
			regionData->GetCellData()->SetActiveScalars("ClusterId");
			
		}

	}
}
