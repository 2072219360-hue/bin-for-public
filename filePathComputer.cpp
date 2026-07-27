#pragma once
#include "filePathComputer.h"  // 注意头文件名称同步修改

filePathComputer::filePathComputer() {

}
filePathComputer::~filePathComputer() {

}
void filePathComputer::solvePath(std::string filePath, std::string grdPath, std::string inpPath, std::string& resGrdPath, std::string& resInpPath)
{
	
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	std::replace(grdPath.begin(), grdPath.end(), '\\', '/');
	std::replace(inpPath.begin(), inpPath.end(), '\\', '/');

	QStringList list1 = QString::fromStdString(filePath).split("/");
	QStringList list2 = QString::fromStdString(grdPath).split("/");
	QStringList list3 = QString::fromStdString(inpPath).split("/");

	int len1 = list1.size();
	int len2 = list2.size();
	int len3 = list3.size();
	int mi = std::min(len1, std::min(len2, len3));

	int pos = -1;  // 找到filePath和grdPath里第一个不同的位置
	for (int i = 0; i < mi; ++i)
	{
		if (list1.at(i) != list2.at(i))
		{
			pos = i;
			break;
		}
	}

	int num = len1 - pos - 1;  // 需要有几个../

	qDebug() << "num = " << num << endl;
	qDebug() << "pos = " << pos << endl;
	resGrdPath = list2.at(pos).toLocal8Bit().toStdString();
	resInpPath = list3.at(pos).toLocal8Bit().toStdString();

	std::string tmp = "";
	for (int i = 0; i < num; ++i) tmp += "../";

	resGrdPath = tmp + resGrdPath;
	resInpPath = tmp + resInpPath;
	qDebug() << "resGrdPath = " << QString::fromStdString(resGrdPath) << endl;
	qDebug() << "resInpPath = " << QString::fromStdString(resInpPath) << endl;

}


void filePathComputer::changeInputPath(QString filePATH, QString grdPATH, QString inpPATH)
{
	if (grdPATH == "" || inpPATH == "") return;
	qDebug() << "filePath=" << filePATH << " grdPath=" << grdPATH << " inpPath=" << inpPATH;
	// 确定应该改成什么    
	std::string filePath = filePATH.toLocal8Bit().toStdString();
	std::string grdPath = grdPATH.toLocal8Bit().toStdString();
	std::string inpPath = inpPATH.toLocal8Bit().toStdString();
	std::string grdS, inpS;
	solvePath(filePath, grdPath, inpPath, grdS, inpS);

	// 读input.dat
	std::ifstream inputLoad;
	inputLoad.open(filePath);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inputLoad, line)) lines.push_back(line);
	inputLoad.close();
	qDebug() << "lines" << lines.size();
	//修改指定行数内容
	QStringList list;
	int posGrd = 30; // 确定行号
	int posInp = 31; // 确定行号
	for (int i = 0; i < lines.size(); ++i)
	{
		list = QString::fromStdString(lines[i]).split("=");
		if (list.at(0) == "GridFileName") posGrd = i;
		if (list.at(0) == "bdFileName") posInp = i;
	}
	lines[posGrd] = "GridFileName=" + grdS;
	lines[posInp] = "bdFileName=" + inpS;

	//写input.dat
	std::ofstream inputSave;
	inputSave.open(filePath);
	for (const auto& lin : lines) inputSave << lin << std::endl;
	inputSave.close();
}
void filePathComputer::changeHeadconfigPath(QString filePATH, QString grdPATH, QString inpPATH)
{
	if (grdPATH == "" || inpPATH == "") return;
	// 确定应该改成什么    
	std::string filePath = filePATH.toLocal8Bit().toStdString();
	std::string grdPath = grdPATH.toLocal8Bit().toStdString();
	std::string inpPath = inpPATH.toLocal8Bit().toStdString();

	std::string grdS, inpS;
	solvePath(filePath, grdPath, inpPath, grdS, inpS);

	// 读headconfig.dat
	std::ifstream headconfigLoad;
	headconfigLoad.open(filePath);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(headconfigLoad, line)) lines.push_back(line);
	headconfigLoad.close();

	//修改指定行数内容
	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
	// qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines.size(); ++i)
	{
		if (lines[i].size() > 5 && lines[i].size() < 35 && lines[i][0] == '!' && lines[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}

	lines[pos - 3] = grdS;
	lines[pos - 1] = inpS;

	//写headconfig.dat
	std::ofstream headconfigSave;
	headconfigSave.open(filePath);
	for (const auto& lin : lines) headconfigSave << lin << std::endl;
	headconfigSave.close();
}