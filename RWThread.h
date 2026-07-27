#pragma once
#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include<qmutex.h>
#include <QObject>
#include <QMessageBox>
#include<QDebug>
#include"scenewidget.h"
class RWThread :
	public QThread
{
	Q_OBJECT
protected:
	QString arg1;
	QString arg2;
	int funFlag;
	//线程运行结束以后是否改变状态
	int statusFlag;
	void run();

public:
	RWThread(QString arg1="",QString arg2="",int funFlag=0);
	void setFunFlag(int funFlag);
	void setArg(QString arg1, QString arg2);
	void setStatusFlag(int statusFlag);
signals:
	void signalRWFinsh(QString messInfo);
};

