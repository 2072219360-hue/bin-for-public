#include "unionFindCT.h"

namespace platforms {
	namespace echv {
		unionFindCT::unionFindCT(vector<pair<int, double>>& dataList, string type) {
			size = 0;
			father=nullptr;
			value = nullptr;
			
			
			treeType = type;
			if (treeType != "JT" && treeType != "ST")throw std::invalid_argument("treeType must be JT or ST");
			size = dataList.size();
			if(size<=0) throw std::invalid_argument("size must be at least 1");
			/*bool checkFlag = true;
			for (auto& it : dataList) {
				if (it.first < 0) checkFlag = false;
				size = max(size, it.first);
			}
			if (checkFlag == false) throw std::invalid_argument("unionFindCT id cannot be negative");
			size++;*/

			father = new int[size]();
			value = new double[size]();

			for (int i = 0; i < size; i++) father[i] = i;
			for (auto& it : dataList) {
				value[it.first] = it.second;
			}


		}
		int unionFindCT::find(int x) {
			if (father[x] == x) return x;
			else return father[x] = find(father[x]);
		}
		void unionFindCT::Union(int x, int y) {
			if (x < 0 || x >= size) return;
			if (y < 0 || y >= size) return;
			int fa_x = find(x), fa_y = find(y);
			if (fa_x != fa_y) {
				if (treeType == "JT") {
					if (value[fa_x] >= value[fa_y]) {
						father[fa_y] = fa_x;
					}else {
						father[fa_x] = fa_y;
					}
				}else {
					if (value[fa_x] < value[fa_y]) {
						father[fa_y] = fa_x;
					}else {
						father[fa_x] = fa_y;
					}
				}
			}
		}
		unionFindCT::~unionFindCT() {
			if (father != nullptr) delete[] father;
			if (value != nullptr) delete[] value;
			
			size = 0;
			father = nullptr;
			value = nullptr;
			
		}


	}
}
