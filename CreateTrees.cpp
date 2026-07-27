#include "CreateTrees.h"

namespace platforms {
	namespace echv {
		void CreateTrees::AlgorithmInterface() {
			/*test();
			return;*/
			
			vtkSmartPointer<vtkStructuredGrid> grid = data->getGrid();
			if (grid == nullptr) {
				qDebug() << "StructuredGrid is null.";
				return;
			}
			string scalar_name = "lambda2-vtk";
			vtkDataArray* scalarArray = grid->GetPointData()->GetArray(scalar_name.c_str());
			if (scalarArray == nullptr) {
				qDebug() << "scalarArray is null.";
				return;
			}

			vector<pair<int, double>> points_magnitude = getPointsMagnitude(scalarArray);
			vector<double> points_value;
			for (auto& it : points_magnitude) {
				points_value.push_back(it.second);
			}
			/*for (int i = 0; i < points_value.size(); i++) {
				qDebug() << "i=" << i << " points_value=" << points_value[i];
			}*/
			qDebug() << "finish points_magnitude";
			vector <vector<pair<int, int>>> edges=getEdges(points_magnitude,grid);
			qDebug() << "finish edges!   edges.size="<<edges.size();
			//points_magnitude按键值降序排列
			sort(points_magnitude.begin(), points_magnitude.end(), [](const pair<int, double>&a, const pair<int, double>&b) {
				return a.second > b.second;
			});
			/*for (int i = 0; i < points_magnitude.size(); i++) {
				qDebug() << "i=" << i << " id=" << points_magnitude[i].first<<" value="<< points_magnitude[i].second;
			}*/
			for (int i = 0; i < points_magnitude.size(); i++) {
				int index = points_magnitude[i].first;
				double value = points_magnitude[i].second;
				/*if (index == 36 || index == 38 || index == 122)
					qDebug() << "i=" << i << " id=" << index << " value=" << value;*/
			}
			vector<vector<int>> JT = creatJST(points_magnitude, edges,"JT");
			vector<vector<int>> ST = creatJST(points_magnitude, edges, "ST");
			qDebug() << "JT.size=" << JT.size() << " ST.size=" << ST.size();
			/*for (int i = 0; i < JT.size(); i++) {
				if (JT[i].size() == 1) continue;
				qDebug() <<"JT[i]size="<<JT[i].size()<< " i=" << i;
				for (auto it : JT[i]) {
					qDebug() << "JT"<<it;
				}
			}
			for (int i = 0; i < ST.size(); i++) {
				if (ST[i].size() == 1) continue;
				qDebug() << "ST[i]size=" << ST[i].size() << " i=" << i;
				for (auto it : ST[i]) {
					qDebug() << "ST" << it;
				}
			}*/
			vector<vector<int>>* new_JT = new vector<vector<int>>;
			*new_JT = creatBranch(points_value, JT, "JT");
			vector<vector<int>>* new_ST = new vector<vector<int>>;
			*new_ST = creatBranch(points_value, ST, "ST");
			vector <vector<int>> JT_Branch = *new_JT;
			vector<vector<int>> ST_Branch = *new_ST;
			//vector<vector<int>> JT_Branch = creatBranch(points_value,JT,"JT");
			//vector<vector<int>> ST_Branch = creatBranch(points_value, ST, "ST");
			qDebug() << "JT_Branch.size=" << JT_Branch.size() << " ST_Branch.size=" << ST_Branch.size();
			string fileName1, fileName2,fileName3;
			fileName1 = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_JT.vtk";
			fileName2 = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_ST.vtk";
			fileName3 = "D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/output_CT.vtk";
			write_vtk(grid, JT_Branch,points_value,fileName1, scalar_name);
			qDebug() << "write JT finish";
			write_vtk(grid, ST_Branch, points_value, fileName2, scalar_name);
			qDebug() << "write ST finish";



			vector<int> last_JT_Branch = JT_Branch[JT_Branch.size() - 1];
			vector<int> last_ST_Branch = ST_Branch[ST_Branch.size() - 1];
			qDebug() << "lastJTbranch.size="<< last_JT_Branch.size()<<" lastSTbranch.size="<< last_ST_Branch.size();
			/*reverse(last_JT_Branch.begin(), last_JT_Branch.end());
			if (last_JT_Branch != last_ST_Branch) qDebug() << "not equal";
			qDebug() << "JT_Branch_begin=" << last_JT_Branch.front() << " JT_Branch_end=" << last_JT_Branch.back();
			qDebug() << "ST_Branch_begin=" << last_ST_Branch.front() << " ST_Branch_end=" << last_ST_Branch.back();*/
			

			vector<vector<int>>* new_CT = new vector<vector<int>>;
			*new_CT = createCT(JT_Branch, ST_Branch);
			vector<vector<int>>& CT = *new_CT;
			//vector<vector<int>> CT= createCT(JT_Branch, ST_Branch);
			qDebug() << "CT size="<<CT.size();

			data->setJT(new_JT);
			data->setST(new_ST);
			data->setCT(new_CT);
			write_vtk(grid, CT, points_value, fileName3, scalar_name);
			qDebug() << "write CT finish";

		}
		void CreateTrees::test() {
			qDebug() << "begin test" ;
			vector<pair<int, int>> graph;
			/*int n = 10;
			graph.push_back(make_pair(9, 5));
			graph.push_back(make_pair(8, 5));
			graph.push_back(make_pair(6, 4));
			graph.push_back(make_pair(7, 4));
			graph.push_back(make_pair(5, 3));
			graph.push_back(make_pair(4, 3));
			graph.push_back(make_pair(3, 2));
			graph.push_back(make_pair(2, 1));
			graph.push_back(make_pair(2, 0));*/

			int n = 25;
			graph.push_back(make_pair(24, 20));
			graph.push_back(make_pair(24, 23));
			graph.push_back(make_pair(24, 18));
			graph.push_back(make_pair(20, 14));
			graph.push_back(make_pair(20, 12));
			graph.push_back(make_pair(20, 18));
			graph.push_back(make_pair(14, 6));
			graph.push_back(make_pair(14, 7));
			graph.push_back(make_pair(14, 12));
			graph.push_back(make_pair(7, 6));
			graph.push_back(make_pair(6, 3));
			graph.push_back(make_pair(6, 1));
			graph.push_back(make_pair(3, 1));
			graph.push_back(make_pair(23, 18));
			graph.push_back(make_pair(23, 17));
			graph.push_back(make_pair(23, 9));
			graph.push_back(make_pair(18, 12));
			graph.push_back(make_pair(18, 15));
			graph.push_back(make_pair(18, 9));
			graph.push_back(make_pair(12, 7));
			graph.push_back(make_pair(12, 10));
			graph.push_back(make_pair(15, 12));
			graph.push_back(make_pair(10, 7));
			graph.push_back(make_pair(13, 7));
			graph.push_back(make_pair(7, 3));
			graph.push_back(make_pair(13, 3));
			graph.push_back(make_pair(17, 9));
			graph.push_back(make_pair(17, 4));
			graph.push_back(make_pair(17, 5));
			graph.push_back(make_pair(15, 9));
			graph.push_back(make_pair(11, 9));
			graph.push_back(make_pair(9, 5));
			graph.push_back(make_pair(15, 10));
			graph.push_back(make_pair(15, 11));
			graph.push_back(make_pair(22, 15));
			graph.push_back(make_pair(22, 10));
			graph.push_back(make_pair(13, 10));
			graph.push_back(make_pair(10, 8));
			graph.push_back(make_pair(13, 8));
			graph.push_back(make_pair(5, 4));
			graph.push_back(make_pair(4, 0));
			graph.push_back(make_pair(4, 2));
			graph.push_back(make_pair(11, 5));
			graph.push_back(make_pair(16, 5));
			graph.push_back(make_pair(5, 2));
			graph.push_back(make_pair(19, 11));
			graph.push_back(make_pair(22, 11));
			graph.push_back(make_pair(16, 11));
			graph.push_back(make_pair(22, 19));
			graph.push_back(make_pair(22, 21));
			graph.push_back(make_pair(22, 8));
			graph.push_back(make_pair(21, 8));
			graph.push_back(make_pair(2, 0));
			graph.push_back(make_pair(16, 2));
			graph.push_back(make_pair(19, 16));
			graph.push_back(make_pair(21, 19));

			vector<pair<int, double>> points_magnitude;
			for (int i = 0; i < n; i++) {
				points_magnitude.push_back(make_pair(i, i));
			}
			qDebug() << "finish points_magnitude";
			vector<double> points_value;
			for (auto& it : points_magnitude) {
				points_value.push_back(it.second);
			}
			sort(points_magnitude.begin(), points_magnitude.end(), [](const pair<int, double>&a, const pair<int, double>&b) {
				return a.second > b.second;
			});
			qDebug() << "finish points_value";
			
			

			

			qDebug() << "finish graph";
			vector <vector<pair<int, int>>> edges(n);
			for (auto edge : graph) {
				int u = edge.first, v = edge.second;
				edges[u].push_back(edge);
				edges[v].push_back(edge);
			}
			qDebug() << "finish edges";
			vector<vector<int>> JT = creatJST(points_magnitude, edges, "JT");
			vector<vector<int>> ST = creatJST(points_magnitude, edges, "ST");
			qDebug() << "JT.size=" << JT.size() << " ST.size=" << ST.size();
			/*for (int i = 0; i < JT.size(); i++) {
				qDebug() <<"JT[i]size="<<JT[i].size()<< " i=" << i;
				for (auto it : JT[i]) {
					qDebug() << "JT"<<it;
				}
			}
			for (int i = 0; i < ST.size(); i++) {
				qDebug() << "ST[i]size=" << ST[i].size() << " i=" << i;
				for (auto it : ST[i]) {
					qDebug() << "ST" << it;
				}
			}*/
			vector<vector<int>> JT_Branch = creatBranch(points_value, JT, "JT");
			vector<vector<int>> ST_Branch = creatBranch(points_value, ST, "ST");
			qDebug() << "JT_Branch.size=" << JT_Branch.size() << " ST_Branch.size=" << ST_Branch.size();
			/*for (int i = 0; i < JT_Branch.size(); i++) {
				qDebug() << "JT_branchSize=" << JT_Branch[i].size() << " i=" << i;
				for(auto it: JT_Branch[i]){
					qDebug() << "JT " << it;
				}
			}
			for (int i = 0; i < ST_Branch.size(); i++) {
				qDebug() << "ST_branchSize=" << ST_Branch[i].size() << " i=" << i;
				for (auto it : ST_Branch[i]) {
					qDebug() << "ST " << it;
				}
			}*/
			vector<int> last_JT_Branch = JT_Branch[JT_Branch.size() - 1];
			vector<int> last_ST_Branch = ST_Branch[ST_Branch.size() - 1];
			qDebug() << "lastJTbranch.size=" << last_JT_Branch.size() << " lastSTbranch.size=" << last_ST_Branch.size();
			reverse(last_JT_Branch.begin(), last_JT_Branch.end());
			if (last_JT_Branch != last_ST_Branch) qDebug() << "not equal";
			else qDebug() << " equal";
			vector<vector<int>> CT = createCT(JT_Branch, ST_Branch);
			qDebug() << "create CT finish";
			qDebug() << "CT size=" << CT.size();
			for (int i = 0; i < CT.size(); i++) {
				qDebug() << "CT[i]size=" << CT[i].size() << " i=" << i;
				for (auto it : CT[i]) {
					qDebug() << "CT " << it;
				}
			}
		}
		
		//根据指标生成point的优先级
		vector<pair<int, double>> CreateTrees::getPointsMagnitude(vtkDataArray* scalarArray) {
			
			int compNum = scalarArray->GetNumberOfComponents();
			vector<double> scalarArrayValue;
			if (compNum == 1) {
				for (int i = 0; i < scalarArray->GetNumberOfTuples(); i++) {
					scalarArrayValue.push_back(scalarArray->GetComponent(i, 0));
				}
			}
			else if (compNum == 3) {// scalar is vector
				for (int i = 0; i < scalarArray->GetNumberOfTuples(); i++) {
					double x = 0, y = 0, z = 0;
					x = scalarArray->GetComponent(i, 0);
					y = scalarArray->GetComponent(i, 1);
					z = scalarArray->GetComponent(i, 2);
					scalarArrayValue.push_back(sqrt(x*x + y * y + z * z));
				}
			}
			vector<pair<int, double>> points_magnitude;
			int index = 0;
			for (auto& it : scalarArrayValue) {
				points_magnitude.push_back(make_pair(index++, it));
			}
			/*for (int i = 0; i < points_magnitude.size(); i++) {
				qDebug() << "before i=" << i << " value=" << points_magnitude[i].second;
			}*/
			//确保数据的值都不同
			double eps = 1e-8;
			bool invalidFlag = false;
			double max_value=0, min_value=0;
			for (auto &it : points_magnitude) {
				double val = it.second;
				if (isinf(val) || isnan(val)) invalidFlag = true;
				if (val > max_value) max_value = val;
				if (val < min_value) min_value = val;
			}
			eps = (max_value - min_value)*0.1 / points_magnitude.size();
			eps = 0.0001;
			if (invalidFlag) qDebug() << "exist invalid double";
			else qDebug() << "no exist invalid double";
			/*auto compare = [](double a, double b) {
				return std::fabs(a - b) > 1e-9 && a < b;
			};
			set<double, decltype(compare)> s(compare);*/
			set<double> s;
			
			for (int i = 0; i < points_magnitude.size(); i++) {
				double x = points_magnitude[i].second;
				auto it = s.find(x);
				bool flag = false;
				while(it != s.end()) {
					x += eps;
					it = s.find(x);
					flag = true;
				}
				//if (flag) qDebug() << "---------before_value=" << points_magnitude[i].second << "  afther_value=" << x<<" i="<<i;
				points_magnitude[i].second=x;
				s.insert(x);
			}
			/*for (int i = 0; i < points_magnitude.size(); i++) {
				qDebug() << "afther i=" << i << " value=" << points_magnitude[i].second;
			}*/
			return points_magnitude;
		}

		//获取网格的边<a,b>,其中a的优先级要大于b,以邻接边表的方式存储
		vector <vector<pair<int, int>>>  CreateTrees::getEdges(vector<pair<int, double>>& points_magnitude, vtkSmartPointer<vtkStructuredGrid> grid) {
			int n = points_magnitude.size();
			vector <vector<pair<int, int>>> edges(n);
			//qDebug() << " n=" << n;
			int dims[3];
			grid->GetDimensions(dims);
			int xDim = dims[0];
			int yDim = dims[1];
			int zDim = dims[2];
			//qDebug() << "xDim="<<xDim<<" yDim="<<yDim<<" zDim="<<zDim;
			for (int i = 0; i < xDim; i++) {
				for (int j = 0; j < yDim; j++) {
					for (int k = 0; k < zDim; k++) {
						int id = i + j * xDim + k * xDim*yDim;
						int id2=-1;
						//qDebug() << "vector size=" << points_magnitude.size() << " id=" << id << " id2=" << id2;
						//向右的边
						if (i < xDim - 1) {
							id2 = (i + 1) + j * xDim + k * xDim*yDim;
							/*if (id2 >= points_magnitude.size()||id2==-1) 
								qDebug() << "i=" << i << " j=" << j << " k=" << k<<" id="<<id<<" id2="<<id2;*/
							pair<int, int> pii;
							if (points_magnitude[id].second > points_magnitude[id2].second) pii.first=id,pii.second=id2;
							else pii.first = id2, pii.second = id;
							edges[id].push_back(pii);
							edges[id2].push_back(pii);
						}
						//向前的边
						if (j < yDim - 1) {
							id2 = i  + (j + 1) * xDim + k * xDim*yDim;
							/*if (id2 >= points_magnitude.size() || id2 == -1)
								qDebug() << "i=" << i << " j=" << j << " k=" << k << " id=" << id << " id2=" << id2; */
							pair<int, int> pii;
							if (points_magnitude[id].second > points_magnitude[id2].second) pii.first = id, pii.second = id2;
							else pii.first = id2, pii.second = id;
							edges[id].push_back(pii);
							edges[id2].push_back(pii);
						}
						//向上的边
						if (k < zDim - 1) {
							id2 = i + j  * xDim + (k + 1) * xDim*yDim;
							/*if (id2 >= points_magnitude.size() || id2 == -1)
								qDebug() << "i=" << i << " j=" << j << " k=" << k << " id=" << id << " id2=" << id2;*/
							pair<int, int> pii;
							if (points_magnitude[id].second > points_magnitude[id2].second) pii.first = id, pii.second = id2;
							else pii.first = id2, pii.second = id;
							edges[id].push_back(pii);
							edges[id2].push_back(pii);
						}

					}
				}
			}


			return edges;
		}
		vector<vector<int>> CreateTrees::creatJST(vector<pair<int, double>>& points_magnitude, vector <vector<pair<int, int>>>& edges,string treeType) {
			unionFindCT UF(points_magnitude, treeType);
			int n= points_magnitude.size();
			vector<vector<int>> JST(n);
			for (int i = 0; i < n; i++) {
				JST[i].push_back(i);
			}
			//qDebug() << "treeType=" << treeType.c_str();
			if (treeType == "JT") {
				for (auto it = points_magnitude.begin(); it != points_magnitude.end(); ++it) {
					int v = it->first;
					//qDebug() << "JT v=" << v;
					//for (auto edge : edges[v]) qDebug() << "edge=<" << edge.first<<","<<edge.second<<">";
					for (auto edge : edges[v]) {
						if (v == edge.second) {
							int u = edge.first;
							int fv = UF.find(v), fu = UF.find(u);
							if (fv != fu) {
								//qDebug() << "v=" << v<<" u="<<u<< "fv="<<fv<<" fu="<<fu;
								JST[fu].push_back(v);
								UF.Union(v, u);
							}
						}
					}
				}

			}else {
				for (auto it = points_magnitude.rbegin(); it != points_magnitude.rend(); ++it) {
					int v = it->first;
					//qDebug() << "ST v=" << v;
					//for (auto edge : edges[v]) qDebug() << "edge=<" << edge.first << "," << edge.second << ">";
					for (auto edge : edges[v]) {
						if (v == edge.first) {
							int u = edge.second;
							int fv = UF.find(v), fu = UF.find(u);
							if (fv != fu) {
								//qDebug() << "v=" << v << " u=" << u << "fv=" << fv << " fu=" << fu;
								JST[fu].push_back(v);
								UF.Union(v, u);
							}
						}
					}
				}

			}



			return JST;
		}
		//生成分支
		vector<vector<int>> CreateTrees::creatBranch(vector<double>& points_value, vector<vector<int>>& JST,string treeType) {
			vector<vector<int>> branch;
			for (auto it : JST) {
				if (it.size() > 1) branch.push_back(it);
			}
			if (treeType == "JT") {
				sort(branch.begin(), branch.end(), [&points_value](const vector<int>&a, const vector<int>&b) {
					return points_value[a[0]] < points_value[b[0]];
				});
			}else{
				sort(branch.begin(), branch.end(), [&points_value](const vector<int>&a, const vector<int>&b) {
					return points_value[a[0]] > points_value[b[0]];
				});
			}

			return branch;
		}
		//将分支存储到vtk文件中
		void CreateTrees::write_vtk(vtkSmartPointer<vtkStructuredGrid> grid,const vector<vector<int>>& edges,
			const vector<double>& points_value,string fileName,string scalarName) {
			ofstream file(fileName);
			if (!file.is_open()) {
				std::cerr << "Failed to open file: " << fileName << std::endl;
				return;
			}
			
			vtkSmartPointer<vtkPoints>  points=grid->GetPoints();
			file << "# vtk DataFile Version 4.2\n";
			file << "vtk output\n";
			file << "ASCII\n";
			file << "DATASET POLYDATA\n";
			file << "POINTS " << points->GetNumberOfPoints() << " float\n";
			for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
				double point[3];
				points->GetPoint(i,point);
				file << point[0] << " " << point[1] << " " << point[2] << "\n";
			}
			file << "\n";
			int line_count = 0;
			for (auto &it : edges) {
				line_count += it.size();
			}
			//file << "LINES " << edges.size() << " " << line_count << "\n";
			file << "LINES " << edges.size() << " " << line_count + edges.size() << "\n";
			for (auto &edge : edges) {
				file << edge.size();
				for (auto& index : edge) {
					file << " " << index;
				}
				file << "\n";
			}
			file << "POINT_DATA " << points_value.size() << "\n";
			file << "SCALARS " << scalarName << " double\n";
			file << "LOOKUP_TABLE default\n";
			for (int i = 0; i < points_value.size(); i += 6) {
				for (int j = 0; j < 6; ++j) {
					if (i + j < points_value.size()) {
						file << points_value[i + j];
						if (j == 5 || i + j == points_value.size() - 1) {
							file << "\n";
						}
						else {
							file << " ";
						}
					}
				}
			}

			file.close();
		}

		
		void CreateTrees::addBranchToPQ(vector<vector<int>>& tree, vector<vector<int>>& otree,set<vector<int>>& PQ) {
			for (auto branch : tree) {
				auto it = PQ.find(branch);
				//不在集合中
				if (it==PQ.end()) {
					if (branch.size() == 2) PQ.insert(branch);
					else if (canSimplify(branch, tree, otree)) PQ.insert(branch);
				}
			}

		}
		//判断分支是否可以被取消
		bool CreateTrees::canSimplify(vector<int>& branch, vector<vector<int>>& tree, vector<vector<int>>& otree) {
			for (int i = 0; i < branch.size(); i++) {
				for (auto& other_branch : tree) {
					if (other_branch != branch) {
						bool inside = false;
						for (auto &it : other_branch) {
							if (branch[i] == it) inside = true;
						}
						if (i != branch.size() - 1 && inside) return false;
					}
				}
			}
			if (otree.size() > 1) {
				for (int i = 0; i < branch.size() - 1; i++) {
					int p = branch[i];
					for (auto& b : otree) {
						if (!b.empty() && p == b.back()) return false;
					}
				}
			}
			return true;
		}

		/*1.找出分支所在树，进行删除
		2.找到分支鞍点之外的点，在另一棵树上删除并合并分支
		3.如果处理到最后，JT和ST的最后一个分支一样*/
		string CreateTrees::mergeBranch(vector<int>& branch, vector<vector<int>>& JT, vector<vector<int>>& ST) {
			bool flag = false;
			for (auto &b : JT) {
				if (branch == b) {
					flag = true;
					break;
				}
			}
			if (flag) {
				//qDebug() << "begin erase JT";
				//"移除-擦除"惯用法(Erase-Remove Idiom)
				JT.erase(std::remove(JT.begin(), JT.end(), branch), JT.end());
				//qDebug() << "end erase JT";
				for (int i = 0; i < branch.size() - 1; i++) {
					int point = branch[i];
					for (auto &b : ST) {
						if (std::find(b.begin(), b.end() - 1, point) != b.end() - 1) {
							//qDebug() << "begin erase b";
							b.erase(std::remove(b.begin(), b.end(), point), b.end());
							//qDebug() << "end erase b";
							break;
						}
					}
				}
				return "ST";
			}else {
				//qDebug() << "begin erase ST";
				ST.erase(std::remove(ST.begin(), ST.end(), branch), ST.end());
				//qDebug() << "end erase ST";
				for (int i = 0; i < branch.size() - 1; i++) {
					int point = branch[i];
					for (auto &b : JT) {
						if (std::find(b.begin(), b.end() - 1, point) != b.end() - 1) {
							//qDebug() << "begin erase b";
							b.erase(std::remove(b.begin(), b.end(), point), b.end());
							//qDebug() << "end erase b";
							break;
						}
					}
				}
				return "JT";
			}
		}

		//构建轮廓树
		vector<vector<int>> CreateTrees::createCT(vector<vector<int>>& JT_Branch, vector<vector<int>>& ST_Branch) {
			vector<vector<int>> CT;
			set<vector<int>> PQ;
			addBranchToPQ(JT_Branch, ST_Branch, PQ);
			addBranchToPQ(ST_Branch, JT_Branch, PQ);
			qDebug() << "PQ.size=" << PQ.size();
			while (!PQ.empty()) {
				auto it = PQ.begin();
				vector<int> B_top = *it;
				//qDebug() << "begin erase PQ";
				PQ.erase(it);
				//qDebug() << "finish erase PQ";
				if (!B_top.empty()) {
					CT.push_back(B_top);
					//qDebug() << "begin merge";
					string choice= mergeBranch(B_top, JT_Branch, ST_Branch);
					//qDebug() << "end merge";
					if (PQ.empty()) {
						//qDebug() << "JTsize=" << JT_Branch.size() << " STsize=" << ST_Branch.size();
						if (JT_Branch.size() == 1 && ST_Branch.size() == 1) {
							vector<int> reversed_JT_Branch = JT_Branch[0];
							reverse(reversed_JT_Branch.begin(), reversed_JT_Branch.end());
							if (reversed_JT_Branch != ST_Branch[0]) {
								qDebug() << u8"构建失败";
								return CT;
								//exit(EXIT_FAILURE);
							}
							CT.push_back(JT_Branch[0]);
							qDebug() << u8"构建成功";
						}
						else {
							if (JT_Branch.size() > 1) {
								addBranchToPQ(JT_Branch, ST_Branch, PQ);
							}
							if (ST_Branch.size() > 1) {
								addBranchToPQ(ST_Branch, JT_Branch, PQ);
							}
						}
					}
				}
			}
			/*qDebug() << "final   JTsize=" << JT_Branch.size() << " STsize=" << ST_Branch.size();
			for(auto it:JT_Branch){
				qDebug() << "JT_Branch size=" << it.size();
			}
			for (auto it : ST_Branch) {
				qDebug() << "ST_Branch size=" << it.size();
			}*/
			if (JT_Branch.size() != 1 || ST_Branch.size() != 1) qDebug() << u8"构建失败,有剩余节点";
			
			return CT;
		}
	}
}

