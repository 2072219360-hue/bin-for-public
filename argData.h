#pragma once

#include<string>
using namespace std;


namespace platforms {
	namespace echv {
		//该类负责保存各种运行前的参数数据
		class argData
		{
			friend class Strategy;
		public:
			argData(string arg_filePath = "",int arg_xSize=100, int arg_ySize = 100, int arg_zSize = 100,double arg_interval=1);
			~argData();
			void copy(argData *arg);
			//获取vtk文件的目录
			string getFilePath();
			int getXsize() { return this->xSize; }
			int getYsize() { return this->ySize; }
			int getZsize() { return this->zSize; }
			double getInterval() { return this->interval; }
		private:
			string filePath;
			//保存非结构化网格转结构化网格的维度
			int xSize, ySize, zSize;
			double interval;

		};
	}
}