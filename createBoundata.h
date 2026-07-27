#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <regex>

using namespace std;

class createBoundata
{
public:
	createBoundata(string workSpace) :m_workSpace(workSpace),batchFlag(false) {}
	void work();
	bool isValidScientificNotation(const std::string& str);
	void setBatch(string batchWorkSpace);
private:
	string m_workSpace;
	bool batchFlag;
	string m_batchWorkSpace;
	/*
1、读取toPns.dat
2、插值计算出toPns顶点的属性值 u,v,w,p,t
3、读取body_xyz_ijk.dat
4、搜索body中每个点距离toPns顶点最近的点，将该点的属性值赋给body顶点
5、插值计算出body中所有网格的格心点物理量u,v,w,p,t
6、输出bounddata文件
*/
	std::vector<string> fileContent0;  //读取headconfig.dat
	std::vector<string> fileContent;  //读取toPns.dat
	std::vector<string> fileContent1; //读取body_xyk_ijk.dat
	std::vector<double> toPns; //存储toPns.dat的所有数据
	std::vector<int> index_toPns; //存储头部截面每个patch具体数据(x,y,z,u,v,w,p,t)开始的索引
	std::vector<double> body; //存储body的所有数据
	std::vector<int> index_body; //存储body每个patch具体数据的索引
	std::vector<double> toPns_process; //存储头部截面顶点物理量(插值计算)
	std::vector<int> index_toPns_process;
	std::vector<double> body_process; //存储身部截面顶点物理量(搜索最近身部顶点)
	std::vector<int> index_body_process;
	std::vector<double> bounddata; //存储身部格心点物理量(使用身部顶点插值计算)
	std::vector<int> index_bounddata;

	std::vector<int> I; //存储topns每个patch的Isize
	std::vector<int> J; //存储topns每个patch的Jsize
	std::vector<int> I_body; //存储body每个patch的Isize
	std::vector<int> J_body; //存储body每个patch的Jsize

	std::vector<int> blk;
};

