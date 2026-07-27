#include "createBoundata.h"
#include<QFile>
void createBoundata::setBatch(string batchWorkSpace) {
	batchFlag = true;
	m_batchWorkSpace = batchWorkSpace;
}

void createBoundata::work() {
	double Xscale, lref;
	//读取缩放比
	string path= m_workSpace;
	if (batchFlag) path = m_batchWorkSpace;
	
	std::ifstream infile0(path+"/head/headconfig.dat");
	if (!infile0.is_open()) {
		//qDebug()<< "Failed to open the file.";
		return;
	}
	std::string line0;
	while (std::getline(infile0, line0)) fileContent0.push_back(line0);
	infile0.close();
	std::stringstream ss0(fileContent0[14]);
	std::string num;
	ss0 >> num;
	Xscale = std::stod(num);
	ss0 >> num;
	ss0 >> num;
	lref = std::stod(num);
	double ratio = lref / Xscale;

	//1、读取toPns.dat
	std::ifstream infile(path + "/head/"+"toPns.dat");
	if (!infile.is_open()) {
		//qDebug() << "Failed to open the file.";
		return;
	}
	std:string line;
	while (std::getline(infile, line)) fileContent.push_back(line);
	infile.close();
	toPns.reserve(fileContent.size() * 5); //预留大小
	for (int i = 4; i < fileContent.size(); ++i) {
		std::stringstream ss(fileContent[i]);
		std::string strdb;
		while (ss >> strdb) {
			toPns.push_back(isValidScientificNotation(strdb) ? std::stod(strdb) : 0.0);
		}
	}

	//for (auto x : toPns) cout << x << "\t";  *******通过

//2、插值计算出toPns顶点的属性值 u,v,w,p,t

	int nPatch = toPns[0];   //toPns的patch数量
	int blkid, faceid, patchid, isize, jsize;
	double patch_rbegin[3], patch_rend[3], coordinates_of_patch_center[3];
	int pos_topns = 1;  //toPns指针
	int pos_topns_process = 0; //toPns_process指针
	int cnt1 = nPatch;  // 计数器

	I.reserve(nPatch);
	J.reserve(nPatch);

	//std::ofstream outfile("toPns_process.dat");
	//std::ofstream outfile("body_process.dat");
	std::ofstream outfile(path + "/body/" + "bounddata.dat");
	if (!outfile.is_open()) {
		//qDebug() << "Failed to open the file.";
		return;
	}
	while (cnt1 != 0) {
		//存入toPns各个patch的头部信息
		blkid = toPns[pos_topns++];
		faceid = toPns[pos_topns++];
		patchid = toPns[pos_topns++];
		patch_rbegin[0] = toPns[pos_topns++];
		patch_rbegin[1] = toPns[pos_topns++];
		patch_rbegin[2] = toPns[pos_topns++];
		patch_rend[0] = toPns[pos_topns++];
		patch_rend[1] = toPns[pos_topns++];
		patch_rend[2] = toPns[pos_topns++];
		isize = toPns[pos_topns++];
		jsize = toPns[pos_topns++];
		coordinates_of_patch_center[0] = toPns[pos_topns++];
		coordinates_of_patch_center[1] = toPns[pos_topns++];
		coordinates_of_patch_center[2] = toPns[pos_topns++];

		index_toPns.push_back(pos_topns);
		index_toPns_process.push_back(pos_topns_process);
		I.push_back(isize);
		J.push_back(jsize);

		//cout << isize << " " << jsize << " " << pos_topns_process << "\n";

		//计算toPns顶点的属性值
		if (isize != 1 && jsize != 1) {
			double u1, u2, u3, u4, v1, v2, v3, v4, w1, w2, w3, w4, p1, p2, p3, p4, t1, t2, t3, t4;
			int index_phy_topns = index_toPns[nPatch - cnt1] + isize * jsize * 3; //toPns各个patch物理量的开头索引

			int cellnum = 4;
			for (int j = 0; j < jsize; ++j) {
				for (int i = 0; i < isize; ++i) {
					// if (i == 0 && j == 0) continue;   //最左下顶点单独处理
					// if (i == isize && j == jsize) continue; //最右上顶点单独处理
					cellnum = 4;

					u1 = toPns[index_phy_topns + i + j * (isize - 1)];
					u2 = toPns[index_phy_topns + i - 1 + j * (isize - 1)];
					u3 = toPns[index_phy_topns + i + (j - 1)*(isize - 1)];
					u4 = toPns[index_phy_topns + i - 1 + (j - 1)*(isize - 1)];

					v1 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) + i + j * (isize - 1)];
					v2 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) + i - 1 + j * (isize - 1)];
					v3 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) + i + (j - 1)*(isize - 1)];
					v4 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) + i - 1 + (j - 1)*(isize - 1)];

					w1 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 2 + i + j * (isize - 1)];
					w2 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 2 + i - 1 + j * (isize - 1)];
					w3 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 2 + i + (j - 1)*(isize - 1)];
					w4 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 2 + i - 1 + (j - 1)*(isize - 1)];

					p1 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 3 + i + j * (isize - 1)];
					p2 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 3 + i - 1 + j * (isize - 1)];
					p3 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 3 + i + (j - 1)*(isize - 1)];
					p4 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 3 + i - 1 + (j - 1)*(isize - 1)];

					//最后一个patch的 最右上顶点的t属性会出现toPns越界
					if (index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i + j * (isize - 1) < toPns.size()) {
						t1 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i + j * (isize - 1)];
					}
					else t1 = 0;

					if (index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i - 1 + j * (isize - 1) < toPns.size()) {
						t2 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i - 1 + j * (isize - 1)];
					}
					else t2 = 0;

					if (index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i + (j - 1)*(isize - 1) < toPns.size()) {
						t3 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i + (j - 1)*(isize - 1)];
					}
					else t3 = 0;

					t4 = toPns[index_phy_topns + (isize - 1)*(jsize - 1) * 4 + i - 1 + (j - 1)*(isize - 1)];

					if (i == 0 && j == 0) {
						cellnum = 1;
						u2 = 0; u3 = 0; u4 = 0;
						v2 = 0; v3 = 0; v4 = 0;
						w2 = 0; w3 = 0; w4 = 0;
						p2 = 0; p3 = 0; p4 = 0;
						t2 = 0; t3 = 0; t4 = 0;
					}

					if (i == 0 && j == jsize - 1) {
						cellnum = 1;
						u1 = 0; u2 = 0; u4 = 0;
						v1 = 0; v2 = 0; v4 = 0;
						w1 = 0; w2 = 0; w4 = 0;
						p1 = 0; p2 = 0; p4 = 0;
						t1 = 0; t2 = 0; t4 = 0;
					}

					if (i == isize - 1 && j == 0) {
						cellnum = 1;
						u1 = 0; u3 = 0; u4 = 0;
						v1 = 0; v3 = 0; v4 = 0;
						w1 = 0; w3 = 0; w4 = 0;
						p1 = 0; p3 = 0; p4 = 0;
						t1 = 0; t3 = 0; t4 = 0;
					}

					if (i == isize - 1 && j == jsize - 1) {
						cellnum = 1;
						u1 = 0; u2 = 0; u3 = 0;
						v1 = 0; v2 = 0; v3 = 0;
						w1 = 0; w2 = 0; w3 = 0;
						p1 = 0; p2 = 0; p3 = 0;
						t1 = 0; t2 = 0; t3 = 0;
					}

					if (i == 0 && j != 0 && j != jsize - 1) {
						cellnum = 2;
						u2 = 0; u4 = 0;
						v2 = 0; v4 = 0;
						w2 = 0; w4 = 0;
						p2 = 0; p4 = 0;
						t2 = 0; t4 = 0;
					}

					if (i != 0 && j == 0 && i != isize - 1) {
						cellnum = 2;
						u3 = 0; u4 = 0;
						v3 = 0; v4 = 0;
						w3 = 0; w4 = 0;
						p3 = 0; p4 = 0;
						t3 = 0; t4 = 0;
					}

					if (i != 0 && j == jsize - 1 && i != isize - 1) {
						cellnum = 2;
						u2 = 0; u1 = 0;
						v2 = 0; v1 = 0;
						w2 = 0; w1 = 0;
						p2 = 0; p1 = 0;
						t2 = 0; t1 = 0;
					}

					if (i == isize - 1 && j != 0 && j != jsize - 1) {
						cellnum = 2;
						u3 = 0; u1 = 0;
						v3 = 0; v1 = 0;
						w3 = 0; w1 = 0;
						p3 = 0; p1 = 0;
						t3 = 0; t1 = 0;
					}

					toPns_process.push_back((u1 + u2 + u3 + u4) / cellnum);   //写入u
					toPns_process.push_back((v1 + v2 + v3 + v4) / cellnum);   //写入v
					toPns_process.push_back((w1 + w2 + w3 + w4) / cellnum);   //写入w
					toPns_process.push_back((p1 + p2 + p3 + p4) / cellnum);   //写入p
					toPns_process.push_back((t1 + t2 + t3 + t4) / cellnum);   //写入t
				}
			}
		}

		if (isize != 1 && jsize != 1) {
			pos_topns_process += isize * jsize * 5; //将索引指向下一个patch   (只有网格数据isize/jsize都不为1 topns_process才有数据产生)    
		}
		pos_topns += isize * jsize * 3 + (isize - 1)*(jsize - 1) * 5; //将索引指向下一个patch
		cnt1--;

		//for(auto x : toPns_process) cout << x << "\t";

		// int a = 0;
		// outfile << "new\n";
		// for (int i = 0; i < isize*jsize; ++i){
		//     for (int j = 0; j < 5; ++j){
		//         outfile << std::scientific << std::uppercase << std::setprecision(6);
		//         outfile << toPns_process[index_toPns_process[nPatch-cnt1]+a] << "  ";
		//         a++;
		//     }
		//     outfile << "\n";
		// }

		//cout << index_toPns[nPatch-cnt1] << "  ";
		//cout << index_toPns_process[nPatch-cnt1] << "  ";

	}
	/*********************测试通过***************/
//3、读取body_xyz_ijk.dat
	std::ifstream infile1(m_workSpace + "/" + "body_xyz_ijk.dat");
	if (!infile1.is_open()) {
		//qDebug() << "Failed to open the file.";
		return;
	}
	std::string line1;
	while (std::getline(infile1, line1)) fileContent1.push_back(line1);
	infile1.close();
	body.reserve(fileContent1.size() * 5); //预留大小
	for (int i = 3; i < fileContent1.size(); ++i) {
		std::stringstream ss1(fileContent1[i]);
		std::string strdb1;
		while (ss1 >> strdb1) {
			body.push_back(isValidScientificNotation(strdb1) ? std::stod(strdb1) : 0.0);
		}
	}

	//for (auto x : body) cout << x << "\t";  
	/*******通过*/

//4、搜索body中每个点距离toPns顶点最近的点，将该点的属性值赋给body顶点
	int bodyPatch = body[0];   //body的patch数量
	int blockID, blockid, isize_body, jsize_body; //全局blkID 局部blkid
	int pos_body = 1;  //body指针
	int pos_body_process = 0; //body_process指针
	int cnt2 = bodyPatch;  // 计数器

	blk.reserve(bodyPatch);

	while (cnt2 != 0) {
		blockID = body[pos_body++];
		blockid = body[pos_body++];
		isize_body = body[pos_body++];
		jsize_body = body[pos_body++];

		index_body.push_back(pos_body);
		index_body_process.push_back(pos_body_process);
		I_body.push_back(isize_body);
		J_body.push_back(jsize_body);
		blk.push_back(blockid);



		double dist, pos_dist, x1, y1, z1, x2, y2, z2, u, v, w, p, t;
		//遍历body该patch中的每一个点
		for (int x = 0; x < isize_body*jsize_body; ++x) {
			dist = 10000;
			x1 = body[index_body[bodyPatch - cnt2] + x * 3] / ratio;
			y1 = body[index_body[bodyPatch - cnt2] + x * 3 + 1] / ratio;
			z1 = body[index_body[bodyPatch - cnt2] + x * 3 + 2] / ratio;
			for (int y = 0; y < nPatch; ++y) {
				//if( I[y] == 1 || J[y] == 1) continue;
				for (int j = 0; j < J[y]; ++j) {
					for (int i = 0; i < I[y]; ++i) {
						x2 = toPns[index_toPns[y] + i + j * I[y]];
						y2 = toPns[index_toPns[y] + i + j * I[y] + I[y] * J[y]];
						z2 = toPns[index_toPns[y] + i + j * I[y] + I[y] * J[y] * 2];
						pos_dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
						if (pos_dist < dist) {
							dist = pos_dist;
							u = toPns_process[index_toPns_process[y] + (i + j * I[y]) * 5];
							v = toPns_process[index_toPns_process[y] + (i + j * I[y]) * 5 + 1];
							w = toPns_process[index_toPns_process[y] + (i + j * I[y]) * 5 + 2];
							p = toPns_process[index_toPns_process[y] + (i + j * I[y]) * 5 + 3];
							t = toPns_process[index_toPns_process[y] + (i + j * I[y]) * 5 + 4];
						}
					}
				}
			}
			body_process.push_back(u);
			body_process.push_back(v);
			body_process.push_back(w);
			body_process.push_back(p);
			body_process.push_back(t);
		}

		//for(auto x : body_process) cout << x << "\t";

		// int a = 0;
		// outfile << "new\n";
		// for (int i = 0; i < isize_body*jsize_body; ++i){
		//     for (int j = 0; j < 5; ++j){
		//         outfile << std::scientific << std::uppercase << std::setprecision(6);
		//         outfile << body_process[index_body_process[bodyPatch-cnt2]+a] << "  ";
		//         a++;
		//     }
		//     outfile << "\n";
		// }
		pos_body += isize_body * jsize_body * 3;
		pos_body_process += isize_body * jsize_body * 5;
		cnt2--;
	}
	/**********************通过******************/

	//5、插值计算出body中所有网格的格心点物理量u,v,w,p,t
	int pos_bounddata = 0;
	cnt2 = bodyPatch;
	while (cnt2 != 0) {
		index_bounddata.push_back(pos_bounddata);

		double u1, u2, u3, u4, v1, v2, v3, v4, w1, w2, w3, w4, p1, p2, p3, p4, t1, t2, t3, t4;
		for (int j = 0; j < J_body[bodyPatch - cnt2] - 1; ++j) {
			for (int i = 0; i < I_body[bodyPatch - cnt2] - 1; ++i) {
				u1 = body_process[index_body_process[bodyPatch - cnt2] + (i + j * I_body[bodyPatch - cnt2]) * 5];
				v1 = body_process[index_body_process[bodyPatch - cnt2] + (i + j * I_body[bodyPatch - cnt2]) * 5 + 1];
				w1 = body_process[index_body_process[bodyPatch - cnt2] + (i + j * I_body[bodyPatch - cnt2]) * 5 + 2];
				p1 = body_process[index_body_process[bodyPatch - cnt2] + (i + j * I_body[bodyPatch - cnt2]) * 5 + 3];
				t1 = body_process[index_body_process[bodyPatch - cnt2] + (i + j * I_body[bodyPatch - cnt2]) * 5 + 4];

				u2 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + j * I_body[bodyPatch - cnt2]) * 5];
				v2 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + j * I_body[bodyPatch - cnt2]) * 5 + 1];
				w2 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + j * I_body[bodyPatch - cnt2]) * 5 + 2];
				p2 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + j * I_body[bodyPatch - cnt2]) * 5 + 3];
				t2 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + j * I_body[bodyPatch - cnt2]) * 5 + 4];

				u3 = body_process[index_body_process[bodyPatch - cnt2] + (i + (j + 1)*I_body[bodyPatch - cnt2]) * 5];
				v3 = body_process[index_body_process[bodyPatch - cnt2] + (i + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 1];
				w3 = body_process[index_body_process[bodyPatch - cnt2] + (i + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 2];
				p3 = body_process[index_body_process[bodyPatch - cnt2] + (i + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 3];
				t3 = body_process[index_body_process[bodyPatch - cnt2] + (i + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 4];

				u4 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + (j + 1)*I_body[bodyPatch - cnt2]) * 5];
				v4 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 1];
				w4 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 2];
				p4 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 3];
				t4 = body_process[index_body_process[bodyPatch - cnt2] + (i + 1 + (j + 1)*I_body[bodyPatch - cnt2]) * 5 + 4];

				bounddata.push_back((u1 + u2 + u3 + u4) / 4);
				bounddata.push_back((v1 + v2 + v3 + v4) / 4);
				bounddata.push_back((w1 + w2 + w3 + w4) / 4);
				bounddata.push_back((p1 + p2 + p3 + p4) / 4);
				bounddata.push_back((t1 + t2 + t3 + t4) / 4);
			}
		}

		//for(auto x : bounddata) cout << x << "\t";

		// int a = 0;
		// outfile << "new\n";
		// for (int i = 0; i < (I_body[bodyPatch-cnt2]-1)*(J_body[bodyPatch-cnt2]-1); ++i){
		//     for (int j = 0; j < 5; ++j){
		//         outfile << std::scientific << std::uppercase << std::setprecision(6);
		//         outfile << bounddata[index_bounddata[bodyPatch-cnt2]+a] << "  ";
		//         a++;
		//     }
		//     outfile << "\n";
		// }

		pos_bounddata += (I_body[bodyPatch - cnt2] - 1)*(J_body[bodyPatch - cnt2] - 1) * 5;
		cnt2--;
	}
	/*********************测试通过***************/

	//6、输出bounddata文件
	for (int x = 0; x < bodyPatch; ++x) {
		outfile << blk[x] << "  " << 1 << "  " << 1 << "  " << 1 << "  " << 1 << "  " << (I_body[x] - 1)*(J_body[x] - 1) << " LOCAL  0\n";
		int n = 0;
		for (int j = 0; j < 5; ++j) {
			for (int i = 0; i < (I_body[x] - 1)*(J_body[x] - 1); ++i) {
				outfile << std::scientific << std::uppercase << std::setprecision(6);
				if (bounddata[index_bounddata[x] + i * 5 + j] < 0) outfile << " ";
				else outfile << "  ";
				outfile << bounddata[index_bounddata[x] + i * 5 + j];
				n++;
				if (n == 5) {
					outfile << "\n";
					n = 0;
				}
			}
		}
	}

	outfile.close();

}



// 正则表达式匹配科学计数法
bool createBoundata::isValidScientificNotation(const std::string& str) {
	// 匹配形式为：[数字]+(?:\.[数字]+)?(?:[eE][+-]?[数字]+)?
	static const std::regex pattern(R"(^[+-]?(\d+\.?\d*|\d*\.\d+)(?:[eE][+-]?\d+)?$)");
	return std::regex_match(str, pattern);
}