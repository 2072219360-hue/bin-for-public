#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <set>
using namespace std;
namespace platforms {
	namespace inp {
		class inp_process
		{
		private:
			struct block {
				vector<int> size;
				string name;
				int fnum;
				int rownum;                //行数
				vector<int> bc;           //第一个元素写0
				vector<vector <int>> row;   //存各个元素
				int mergenum;               //存符合条件的8的数量
				vector<int> mergerow;
				set<int> I;      //存储要合并的行的I
				set<int> J;
				set<int> K;
			};

			struct INP {
				int a;
				int blknum;
				vector<struct block> blk;
			};

			std::vector<string> fileContent;

			std::vector<int> bodyinp;
			std::vector<int> index_bodyinp;
			std::vector<string> blkname;
			std::vector<int> facenum;
		public:
			void work(const std::string& inputPath, const std::string& outputPath,int num);
		};
	}
}


