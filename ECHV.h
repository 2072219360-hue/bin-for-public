#pragma once
#include <qwidget.h>
#include<QPushButton>
#include<string>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include<QMessageBox>
#include"GraphWindow.h"
#include"ArgWindow.h"
#include"echvVTKWin.h"
#include"runData.h"
#include"argData.h"
#include"dataServer.h"
#include "ECHVSignalInstance.h"
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
namespace platforms {
	namespace echv {
		//发夹涡提取功能主窗口
		class ECHV:
			public QWidget
		{
			Q_OBJECT
		public:
			ECHV(QWidget *parent = nullptr);
			~ECHV();
			void connectAll();
		public slots:

		private:
			GraphWindow *graphWin1, *graphWin2, *graphWin3;
			ArgWindow *argWin;
			echvVTKWin *vtkWin;
			dataServer *myServer;
		};
	}
}


