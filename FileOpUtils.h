#pragma once
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <QMessageBox>
#include <string>
class FileOpUtils
{
public:
	static FileOpUtils*instance;
	static FileOpUtils*getInstance();

	QString pathToFileName(QString path);

	bool copyFile(const QString &toDir, const QString&fromFile, const QString&toFile, bool converFileIfExit);

	// 判断文件是否存在
	bool fileExist(const QString filePath);

	// 文件是否存在Dir中
	bool FileOpUtils::isExistInDir(const QString& Dir, const QString& File);

	// 删除文件夹下的所有东西
	bool removeAllFileOnDir(const QString path);

	// 删除某个文件
	bool removeFile(const QString path);

	QStringList getFileContent(const QString path);

	QString fileToSuffix(QString path);

	QString fileToPrefix(QString path);
private:
	FileOpUtils();
	~FileOpUtils();
};

