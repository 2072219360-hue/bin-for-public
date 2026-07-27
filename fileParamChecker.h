#pragma once
#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <fstream>
#include <vector>
#include <string>
#include<cctype>
#include<iostream>
#include"bodyPathChecker.h"
class FileParamChecker {
public:
	// 构造函数：初始化参数映射表（与原ParamSetDialog保持一致）
	FileParamChecker();
	// 检查指定目录下的配置文件
	bool checkFileParam(const QString& dirPath, int fullFlag,int checkRestart=0);
	// 检查body/input.dat文件
	bool checkInputDat(const QString& bodyPath);
	// 检查head/headconfig.dat文件
	bool checkHeadConfigDat(const QString& headPath);
	bool checkRestart(const QString& file1, const QString& file2);
private:
	std::string trim(const std::string& s);
	// 解析input.dat文件数据到键值对
	bool parseInputDat(const QString& filePath, QMap<QString, QString>& params);
	// 解析headconfig.dat文件数据到键值对
	bool parseHeadConfigDat(const QString& filePath, QMap<QString, QString>& params);
	// 验证input.dat中的参数，包括文件路径验证
	bool validateInputParams(const QMap<QString, QString>& bodyParams, const QString& configFileAbsPath);
	// 验证headconfig.dat中的参数，包括文件路径验证
	bool validateHeadParams(const QMap<QString, QString>& headParams, const QString& configFileAbsPath);
	// 验证文件路径是否有效
	bool validateFilePath(const QString& filePath, const QString& configFileAbsPath);
	// 移除字符串首尾的引号
	QString removeQuotes(const QString& value);
	
private:
	// 各种参数映射表，用于验证参数的标准值
	QMap<QString, int> limiterMap;
	QMap<QString, int> visGradientMap;
	QMap<QString, int> schemeMap;
	QMap<QString, int> implicitVisMap;
	QMap<QString, int> pressFactorMap;
	QMap<QString, int> temperalMap;
};