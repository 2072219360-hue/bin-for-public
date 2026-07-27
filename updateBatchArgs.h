#ifndef UPDATEBATCHARGS_H
#define UPDATEBATCHARGS_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include"bodyPathChecker.h"
#include"inputFileEditor.h"
#include"USStandardAtmosphere.h"
#include"SignalInstance.h"
#include<PTGeneration.h>
class updateBatchArgs
{
public:
	updateBatchArgs();
	~updateBatchArgs() = default;

	// 更新input.dat文件
	void updateInputDat(const QString& workspacePath, double H, double Ma, double AOA, double AOB);
	void setInpAndGrd(QString inp, QString grd);
	void setFullFlag(int fullFlag);

private:
	// 辅助函数：更新headconfig.dat文件
	void updateHeadConfigDat(const QString& headConfigPath, double
		P, double T, double Ma, double AOA, double AOB, string tw);
	
	USStandardAtmosphere* ptComputer;
	QString inpPath;
	QString grdPath;
	int fullFlag;
	inputFileEditor fileEditor;
};

#endif // UPDATEBATCHARGS_H