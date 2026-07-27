#pragma once
#include <qwidget.h>
#include <QTextEdit>
#include <QProcess>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <QTimer>
#include <stdio.h>
#include <QProgressBar>
#include"fileOp.h"
#include"createBoundata.h"
#define FIlEINSTANCE platforms::detail::FileOp::getInstance()
namespace platforms {
	namespace sspns {
		namespace reveal {
			class OutPutLogger :
				public QWidget
			{
				Q_OBJECT
			public:
				QProgressBar* m_dataProgressBar;
				static OutPutLogger* instance;
				static OutPutLogger* getInstance();
				OutPutLogger(int ns_flag=0);
				int nsFlag;
				void exeProgramHeader(QString path);
				QThread* thr ;
				
				void exeRunAll(QString path);
				void loadParameterHeader();
				void loadParameterBody();
				QString m_workspace;
				~OutPutLogger();
				void setBatchMode();
				void init();
				void initConnectionFull();
				void initConnectionPns();
				void initConnection();
			signals:
				void showTableData();
				void finishSect();
				void bodyFinish();
			public slots:
				void exeProgramBody(QString path);
				void exeProgramSect(QString path);
				void onReadData();
				void onReadBodyData();
				void onReadSectData();
				void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
				void processFinishedHead(int exitCode, QProcess::ExitStatus exitStatus);
				void processFinishedBody(int exitCode, QProcess::ExitStatus exitStatus);
				void processFinishedSect(int exitCode, QProcess::ExitStatus exitStatus);
				void onChangeProgressBar(int value);
				void closeProcess();
				void addInformation(QString msg);
				void processFinishedSectFull(int exitCode, QProcess::ExitStatus exitStatus);
				void processFinishedSectPns(int exitCode, QProcess::ExitStatus exitStatus);
				bool checkRunning();
			private:
				QVBoxLayout*m_mainLayout;
				QTextEdit*m_outPutShow;
				//头部/身部/分段程序对应的线程
				QProcess*m_process;
				QProcess*m_processBody;
				QProcess*m_processSect;

				QLabel*m_label;
				QPushButton*m_clearBtn;
				QPushButton*m_closeProcessBtn;
				QPushButton*m_closeBatchProcessBtn;

				QString m_workSpace;
				QString m_bodyDir;
				QString m_headDir;

				QTimer* m_trCheckTimer;
				
				QProcess*m_runAllprocessHead;
				QProcess*m_runAllprocessBody;

				bool isRunAll;
			};
		}
	}
}

