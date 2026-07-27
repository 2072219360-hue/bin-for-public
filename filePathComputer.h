#pragma once
#include <QFile>
#include <QString>
#include <QDebug>
#include<iostream>
#include<fstream>
#include <vector>    // 补充：使用std::vector需包含此头文件
#include <algorithm> // 补充：使用std::min需包含此头文件
using namespace std;
class filePathComputer
{
public:
	filePathComputer();
	~filePathComputer();
	void changeInputPath(QString filePATH, QString grdPath, QString inpPATH);
	void changeHeadconfigPath(QString filePATH, QString grdPath, QString inpPATH);
	void solvePath(string filePath, string grdPath, string inpPath, string& resGrdPath, string& resInpPath);
};