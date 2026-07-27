#include "inp_process.h"
namespace platforms {
	namespace inp {
		void inp_process::work(const std::string& inputPath, const std::string& outputPath,int num) {
			std::ifstream infile(inputPath);
			std::string line;
			while (std::getline(infile, line)) fileContent.push_back(line);
			infile.close();
			bodyinp.reserve(fileContent.size() * 7); //预留大小
			for (int i = 0; i < fileContent.size(); ++i) {
				std::stringstream ss(fileContent[i]);
				std::string strdb;
				while (ss >> strdb) {
					if (isalpha(strdb[0]) == 1)   blkname.push_back(strdb);
					else bodyinp.push_back(std::stoi(strdb));
				}
			}

			std::ofstream outfile(outputPath);

			// 初始化inp
			struct INP inp;
			int pos = 0;
			inp.a = bodyinp[pos++];
			inp.blknum = bodyinp[pos++];
			inp.blk.resize(inp.blknum);

			//将每块数据存入
			for (int c = 0; c < inp.blknum; ++c) {
				//int c = 0;
				inp.blk[c].size.push_back(bodyinp[pos++]);
				inp.blk[c].size.push_back(bodyinp[pos++]);
				inp.blk[c].size.push_back(bodyinp[pos++]);
				inp.blk[c].name = blkname[c];
				inp.blk[c].fnum = bodyinp[pos++];
				inp.blk[c].rownum = 0;
				inp.blk[c].mergenum = 0;

				inp.blk[c].row.resize(2 * inp.blk[c].fnum, vector<int>(7));
				inp.blk[c].bc.push_back(0);
				int cnt = 0;
				int pos_bc = 1;
				int pos_row = 0;
				while (cnt != inp.blk[c].fnum) {
					//cout << cnt << "\t";
					for (int a = 0; a < 7; ++a) {
						//cout << pos_bc-1 << "\t";
						inp.blk[c].row[pos_bc - 1][a] = bodyinp[pos];
						if (a == 6) {
							inp.blk[c].bc.push_back(bodyinp[pos]);
							inp.blk[c].rownum++;
							if (inp.blk[c].bc[pos_bc] == num && inp.blk[c].bc[pos_bc - 1] != -1) {
								inp.blk[c].mergenum++;
								inp.blk[c].mergerow.push_back(inp.blk[c].rownum - 1);
							}
							if (inp.blk[c].bc[pos_bc] > 0) {
								cnt++;
							}
							pos_bc++;
							//cout << cnt << "\t";
							//cout << pos_bc;
						}
						pos++;
					}
				}
				inp.blk[c].rownum = pos_bc - 1;
				//cout << inp.blk[c].rownum;
				//for(auto x : inp.blk[c].row[0]) cout << x << "\t";
				// cout << "\n" << inp.blk[c].rownum;
				// cout << "\n" << inp.blk[c].mergenum;
				// cout << "\n";
				// for(int acx = 0; acx < inp.blk[c].mergenum; ++acx){
				//     cout << inp.blk[c].mergerow[acx] << "  ";
				// }
				// cout << "\n";
				// for(int i = 0; i < inp.blk[c].rownum; ++i){
				//         for(int j = 0; j < 7; ++j){
				//             // if(inp.blk[c].bc[i+1] > 0 && inp.blk[c].bc[i+1] != -1){
				//             //     res.blk[c].row[i][j] = abs(inp.blk[c].row[i][j]);
				//             // }
				//             // else res.blk[c].row[i][j] = inp.blk[c].row[i][j];
				//             cout << inp.blk[c].row[i][j] << "\t";
				//             //cout << res.blk[c].row[i][j] << "\t";
				//         }
				//         cout << "\n";
				//     }

				// for(auto x : inp.blk[c].mergerow) cout << x << "\t";
				// cout << "\n";
			}

			struct INP res;
			res.a = inp.a;
			res.blknum = inp.blknum;
			res.blk.resize(res.blknum);
			for (int c = 0; c < res.blknum; ++c) {
				//int c = 0;
				int i1, i2, j1, j2, k1, k2;
				res.blk[c].size.push_back(inp.blk[c].size[0]);
				res.blk[c].size.push_back(inp.blk[c].size[1]);
				res.blk[c].size.push_back(inp.blk[c].size[2]);
				res.blk[c].name = inp.blk[c].name;
				int p = inp.blk[c].mergenum - 1;     //要合并的次数
				//cout << p << "\n";
				if (p >= 1) {
					res.blk[c].fnum = inp.blk[c].fnum - inp.blk[c].mergenum + 1;
					res.blk[c].rownum = inp.blk[c].rownum - inp.blk[c].mergenum + 1;

					for (int mrow = 0; mrow < inp.blk[c].mergenum; ++mrow) {
						res.blk[c].I.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][0]));
						res.blk[c].I.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][1]));
						res.blk[c].J.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][2]));
						res.blk[c].J.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][3]));
						res.blk[c].K.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][4]));
						res.blk[c].K.insert(abs(inp.blk[c].row[inp.blk[c].mergerow[mrow]][5]));
					}
					i1 = *res.blk[c].I.rbegin();
					i2 = *res.blk[c].I.begin();
					j1 = *res.blk[c].J.rbegin();
					j2 = *res.blk[c].J.begin();
					k1 = *res.blk[c].K.rbegin();
					k2 = *res.blk[c].K.begin();
					// cout << *res.blk[c].I.begin() << "  " << *res.blk[c].I.rbegin() << "\n";
					// cout << *res.blk[c].J.begin() << "  " << *res.blk[c].J.rbegin() << "\n";
					// cout << *res.blk[c].K.begin() << "  " << *res.blk[c].K.rbegin() << "\n";
				}
				else {
					res.blk[c].fnum = inp.blk[c].fnum;
					res.blk[c].rownum = inp.blk[c].rownum;
				}
				res.blk[c].row.resize(res.blk[c].rownum, vector<int>(7));
				//cout << res.blk[c].rownum << "\t";
				//cout << p << "\t";

				// if (inp.blk[c].mergenum > 1){
				//     cout << inp.blk[c].name << "  ";
				//     for(auto x : inp.blk[c].mergerow) cout << x << "\t";
				// }


				//写入
				// int b = res.blk[c].rownum;
				// cout << b << "\n";
				int i_inp = 0;
				for (int i = 0; i < res.blk[c].rownum; ++i) {
					if (inp.blk[c].mergenum > 1) {
						//存入合并行
						//cout << inp.blk[c].mergerow[0] << "\n";
						if (i == inp.blk[c].mergerow[0]) {
							res.blk[c].row[i][0] = i1;
							res.blk[c].row[i][1] = i2;
							res.blk[c].row[i][2] = j1;
							res.blk[c].row[i][3] = j2;
							res.blk[c].row[i][4] = k1;
							res.blk[c].row[i][5] = k2;
							res.blk[c].row[i][6] = num;
							//cout << k1 << "  " << k2 << "\n";
							i++;
							i_inp++;
						}
						// 跳过原来的被合并行
						for (int z = 1; z < inp.blk[c].mergenum; ++z) {
							//cout << inp.blk[c].mergerow[z] << "\t";
							if (i_inp == inp.blk[c].mergerow[z]) {
								i_inp++;
								//cout << i_inp-1 << "\n";
							}
						}

					}
					// 更新合并行数据,完成后进入下一行处理
					for (int j = 0; j < 7; ++j) {
						if (inp.blk[c].bc[i_inp + 1] > 0 && inp.blk[c].bc[i_inp] != -1) {
							res.blk[c].row[i][j] = abs(inp.blk[c].row[i_inp][j]);
							//cout << i << "  " << j << "  " << res.blk[c].row[i][j] << "\n";
						}
						else res.blk[c].row[i][j] = inp.blk[c].row[i_inp][j];
						//cout << inp.blk[c].row[i][j] << "\t";
						//cout << res.blk[c].row[i][j] << "\t";
					}
					//cout << "\n";
					i_inp++;
				}

				// for(int i = 0; i < res.blk[c].rownum; ++i){
				//     for(int j = 0; j < 7; ++j){
				//         cout << res.blk[c].row[i][j] << "\t";
				//     }
				//     cout << "\n";
				// }
				// cout << "\n";
			}
			// int c = 0;
			// for(int i = 0; i < res.blk[c].rownum; ++i){
			//     for(int j = 0; j < 7; ++j){
			//         cout << res.blk[c].row[i][j] << "\t";
			//     }
			//     cout << "\n";
			// }
			outfile << "     " << res.a << "\n";
			outfile << "    " << res.blknum << "\n";
			for (int c = 0; c < res.blknum; ++c) {
				outfile << "    " << res.blk[c].size[0] << "     " << res.blk[c].size[1] << "     " << res.blk[c].size[2] << "\n";
				outfile << res.blk[c].name << "\n";
				outfile << "     " << res.blk[c].fnum << "\n";
				for (int i = 0; i < res.blk[c].rownum; ++i) {
					for (int j = 0; j < 7; ++j) {
						outfile << "        " << res.blk[c].row[i][j];
					}
					outfile << "\n";
				}
				//outfile << "\n";
			}
			outfile.close();
		}
	}
}
