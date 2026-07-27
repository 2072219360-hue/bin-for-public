#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include <stdexcept>

using namespace std;
namespace platforms {
	namespace echv {
		//该类用于构建CT时要用到的并查集
		class unionFindCT
		{
		public:
			unionFindCT(vector<pair<int,double>>& dataList,string type);
			~unionFindCT();
			int find(int x);
			void Union(int x, int y);
		private:
			string treeType;
			int size;
			int *father;
			double* value;

		};
	}
}


