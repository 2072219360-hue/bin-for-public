#pragma once
#include <qwidget.h>
#include <QGridLayout>
#include <QStackedWidget>
#include <QColorDialog>
#include<QScreen>
#include <QStackedWidget>
#include<QPushButton>
#include "scenewidget.h"
#include "log4qt\logger.h"
#include "inputSet.h"
#include "inputHeader.h"
#include "TrTableShow.h"
#include "WorkSpaceOp.h"
#include "RWThread.h"
#include "BatchConfigWidget.h"
#include "OutPutLogger.h"
#include "SignalInstance.h"
#include"btnObserver.h"
#include"statusSubject.h"
#include"ProgressDialog.h"
#include"progressTable.h"
#include"ParamSetDialog.h"
#include"batchProcess.h"
#include"workFlow.h"
#include"workContext.h"
#include"readFileStrategy.h"
#include"updateStrategy.h"
#include"pnsStrategy.h"
#include"sectStrategy.h"
#include"nsStrategy.h"
#include"checkProgramStrategy.h"
#include"fileParamChecker.h"
using namespace platforms::sspns::reveal;
extern SceneWidget* main_sceneWidget;
extern RWThread* rwTh;
//extern RWThread* rwTh2;
extern QMutex* rwMutex;
extern statusSubject* myStatus;
namespace platforms {
	namespace sspns {
		class SSPNS :
			public QWidget
		{
			Q_OBJECT
				LOG4QT_DECLARE_QCLASS_LOGGER
		public:
			//OutPutLogger* outPutLogger;
			unordered_map<int, workFlow*> nodeAdr;
			SSPNS(int nsFlag=0);
			~SSPNS();
			void initUi();
			//加载工作区
			void loadWorkSpace();
			void loadSceneWidget();
			void loadOutputLoger();
			void loadInputSet1();
			void loadInputSet2();
			void loadTrTable();
			void loadAndRun();
			bool checkWorkSpace();
			void initConnect();
			void loadStyleSheet(QString sheetName);
			void addObserver();
			void reshow();
			SceneWidget* getSceneWidget();
			void setMainSceneWidget();
			void updateWindow();
			void initData();
			void setSingalMode();
			void loadRunButton();
			void loadProgressWidget();
			void closeAllWindow();
			void loadArgButton();
			void changeToArgWindow(int i);
			void init_computeFlow();
			void closeAllProcess();
		public slots:
			void changeWallForcestatus(bool flag);
			void loadDefaultParameter();
			void runHeaderPrograme();
			void runBodyPrograme();
			void runSectPrograme();
			void runAllPrograme();
			void showRWMessage(QString messageInfo);
			void onShowBatchWidget();
			bool checkHeadRestart();
			void createProcessPool();
			void deleteProcessPool();
			void init_workFlow();
			void initContext();
		private:

			//workFlow
			workFlow* m_workFlow;
			workContext* m_workContext;


			QMap<QString, int> argButtonIndex;
			SceneWidget* m_sceneWidget;
			InputHeader* m_inputPage1;
			InputSet* m_inputPage2;
			TrTableShow* m_tableShow;
			QPushButton* btnRunCompute;
			QPushButton* btnRunProfiling;
			QPushButton* airButton;
			//按钮窗口
			QWidget  *m_runWidget;
			//参数按钮窗口
			QWidget  *m_argRunWidget;
			QWidget *m_rightChangeWidget;
			//切换窗口
			QStackedWidget *stackedWidget;
			QVector<QPushButton*> argButton;
			QPushButton *btnRunBatch, *btnArgBatch;
			//进程控制按钮
			progressTable*m_processTable;
			batchProcess* m_processPool;
			QWidget  *m_progressWidget;
			// 批量窗口
			detail::BatchConfigWidget* batchConfig ;

			ParamSetDialog* m_paramDialog;
			//QMutex color_mutex;
			QString titleSheet;
			QGridLayout* m_mainLayout;
			QWidget* m_workSpace;
			QWidget* m_outputWidget;
			QWidget* m_inputSet1Widget;
			QWidget* m_inputSet2Widget;
			QWidget* m_loadAndRunData;
			QWidget* m_trTableWidget;
			QPushButton* btnLoadDefault;
			QPushButton* btnRunHead;
			QPushButton* btnRunBody;
			QPushButton* btnRunSect;
			QPushButton* btnRunAll;
			QPushButton* btnBatchCalConfig;
			QPushButton* btnBatchCal;
			QPushButton* btnShowBatch;
			ProgressDialog* m_dialog;
			int m_nsFlag;
			//layout 部件
			//QWidget* fatherWidget;
			/*QSplitter*splitterVisDos;
			QWidget* widgetHead;
			QHBoxLayout* layoutHead;
			QWidget* widgetRight;
			QVBoxLayout* layoutRight;
			QWidget* widgetBody ;
			QVBoxLayout* layoutBody ;
			QSplitter*splitterBodyHead;
			QSplitter*splitterLeftRight;
			QHBoxLayout* layoutMain;*/
		};/// namespace reveal
	}/// namespace sspns
}/// namespace platforms
