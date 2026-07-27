#pragma once
#include <qwidget.h>
#include <QDebug>
#include<QMutex>
#include "SingletonBase.h"
using namespace platforms::detail;

class SignalInstance :
	public SingletonBase
{
	Q_OBJECT
public:
	static SignalInstance*instance;
	static SignalInstance*getInstance();
	static int nsFlag;
	static bool batchFlag;
	static void setGridName(QString path)
	{
		instance->m_gridName = path;
	}
	static void setInpName(QString path)
	{
		instance->m_inpName = path;
	}
	static void setWorkSpace(QString path)
	{
		instance->m_workspace = path;
	}
	//currentWorkSpace
	static QString getWorkSpace()
	{
		return instance->m_workspace;
	}
	static QString getGridName()
	{
		return instance->m_gridName;
	}
	static QString getInpName()
	{
		return instance->m_inpName;
	}
	static void swapWorkSpace() {
		swap(instance->m_workspace, instance->m_workSpace2);
	}
	QString m_workspace,m_workSpace2;
	QString m_gridName;
	QString m_inpName;
	QMutex color_mutex;
signals:
	void signalReClipClear();
	void signalProcessPoolDeleted();
	void signalStartWorkFlow();
	void signalUpdateRow(int row);
	void signalChangeAirParam(bool flag);

	void signalLoadGrid(QString path);
	void signalLoadAndOut(QString path1, QString path2);
	void signalPreRead(QString path1);
	void signalBodyParameter(QString path);
	void signalLoadTrData();
	void signalLoadTrData2();
	void signalWorkSpaceChanged(QString path);
	void signalOpenWorkSpace(QString path);
	void signalLoadDefaultParameter();
	void signalSectProgramOver();
	void signalChooseSect(int sectId);
	void signalColorMutex();
	void signalPreLoad(int value);//进度条
	void signalShowDialog(QString title,QString descrpion);
	void signalCloseDialog();
	void signalUpdateTime(int threadId, QString str);
	void signalCloseProcess();
	void signalCloseAllWindow();
	void signalPrintInfo(QString info);
	void signalChooseColor(int r, int g, int b, int sectId);
	void signalRedictHeadParameter();
	void signalSelectTrData(int id,QStringList list);
	void signalColorLine();
	void signalTrDataLoad(int sectNum,QStringList*list, unsigned char**);
	void signalOnlyTrDataLoad(int sectNum, QStringList*list, unsigned char**);
	void signalSceneWorking();
	void signalSceneWorked(bool isWorking);
	void signalHighlight(int sectId);
	void signalCancelHighlight();
	void signaleSelectCell(int row);

	void signalShowClip();
	void signalCancelShowClip();
	void signalClip();//根据I进行clip
	void signalBothClip();//head和body同时显示
	void signalRedictBodyParameter();
	void signalClearBothClip();
	void signalSectFinish();
	void signalSetRestart(bool flag);
	//改变状态
	//打开工作区解析完成
	void signalLoadFinish();
	//新建工作区解析网格完成
	void signalBuildFinish();
	void signalClipShowFinish();
	void signalClipCancelFinish();
	/// 分段完将表格数据进行添加，预填写信号
	void signalRedictTableData();
	//读取状态
	void signalReadStatus();
	void signalExeSect();
	void signalUpdateBatchTable();
	//
	void signalProcessPoolCreate();
	void signalProcessPoolDelete();
	void signalProcessOp1(int threadId);
	void signalProcessOp2(int threadId);
	void signalProcessOp3(int threadId);
	void signalProcessOp4(int threadId);

	void signalUpdateTable(int threadId, QString str);
	

	void signalBatchRunHead(int threadId);
	void signalBatchRunBody(int threadId);
	void signalBatchRun();
	void signalOutPutLoggerAddInformation(QString msg);
	void signalBatchCloseRun();

	void signalShowLogWidget(int threadId);

	void signalRecover();
	void signalLoadOld(QString _gridPath, QString _inpPath, int _index, int _dire);
	void signalLoadAndClip(QString _gridPath,QString _inpPath,int _index,int _dire);
	void signalExeProgramSect(QString path);

	void signalJudgeRunning();

	void signalZonesVisbleChanged(QVector<int32_t>zones);

	void saveTrTable();
	void finishSect();
	void headFinish(bool success);
	void bodyFinish();
	void finishEventLoop();

	void signalShowProfiling();
	void signalUpdateWorkSpaceView();

	void signalSetJson(int index, const std::string& jsonKey, const std::string& value);
	void signalSetJson(int index, const std::string& jsonKey, double value);
	void signalSetJson(int index, const std::string& jsonKey, bool value);
	void signalSetJson(int index, const std::string& jsonKey, int value);
	void signalSetJsonFinished();
	// 控制批量的按钮
	void signalBatchDone();
	void signalBatchDoneNot();
private:
	SignalInstance();
	~SignalInstance();


};