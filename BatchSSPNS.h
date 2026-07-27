#pragma once
//#include <qwidget.h>
//#include "scenewidget.h"
//#include "log4qt\logger.h"
//#include <QGridLayout>
#include <qwidget.h>
#include <QGridLayout>
#include <QStackedWidget>
#include <QColorDialog>
#include<QSplitter>
#include<thread>
#include<QScreen>
#include "scenewidget.h"
#include "log4qt\logger.h"
#include"ProgressDialog.h"
#include"OutPutLogger.h"
#include"WorkSpaceOp.h"
#include"progressTable.h"
#include"publicArgWindow.h"
#include"BatchConfigWidget.h"
#include"batchProcess.h"
//#include"RWThread.h"
using namespace platforms::sspns::reveal;
extern SceneWidget*main_sceneWidget2;
namespace platforms {
	namespace sspns {
		class BatchSSPNS :
			public QWidget
		{
			Q_OBJECT
				LOG4QT_DECLARE_QCLASS_LOGGER
		public:
			BatchSSPNS();
			~BatchSSPNS();
			void loadStyleSheet(QString sheetName);
			void initUI();
			void loadSceneWidget();
			void initConnection();
			void initLayout();
			void loadOutputLoger();
			void loadWorkSpace();
			void loadRunButton();
			void loadProgressWidget();
			void loadArgWidget();
			void setMainSceneWidget();
			void showRWMessage(QString messageInfo);
			void updateWindow();
			void setBatchMode();
			void initData();
		public slots:
			bool checkWorkSpace();
			void onShowBatchWidget();
			void createProcessPool();
			void deleteProcessPool();
		private:
			ProgressDialog* m_dialog;
			SceneWidget* m_sceneWidget;
			OutPutLogger* outPutLogger;
			QString titleSheet;
			QWidget *m_workSpace,*m_runWidget,*m_progressWidget;
			QWidget*m_argWidget;
			publicArgWindow*m_argWidget2;
			WorkSpaceOp*m_workSpaceOp;
			progressTable*m_tableShow;
			QPushButton *btnRunBatch,*btnArgBatch;
			QWidget* widget;
			TrTableShow* trWidget;
			batchProcess* m_processPool;
			
		};/// namespace reveal
	}/// namespace sspns
}/// namespace platforms

