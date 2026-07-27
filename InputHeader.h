#pragma once
#include <qwidget.h>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <fstream>
#include <QComboBox>
#include <QFile>
#include <QPushButton>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include"btnObserver.h"
#include "SignalInstance.h"
#include "OutPutLogger.h"
namespace platforms
{
	namespace sspns {
		
		namespace reveal {
			class InputHeader :
				public QWidget
			{
				Q_OBJECT
			public:
				InputHeader();
				~InputHeader();
				void initUi();
				void initHeadPart1();
				void initHeadPart2();
				void loadStyleSheet(QString sheetName);
				void loadHeadConfig(QString path, bool flag);
				bool getRestartFlag();
				std::ofstream outfile;
				void addObserver();
			public slots:
				void exeHeader();
				//加载头部参数
				void loadHeaderParameter(QString workPath);
				void loadDefaultParameter();
				void redictHeadParameter();
				void onTwConditionChanged(int index);
				void setRestartFlag(bool flag);
			private:
				QString m_workspace;
				QGridLayout*m_main1Layout;
				QGridLayout*m_main2Layout;
				QHBoxLayout*m_mainLayout;
				QLabel* cInit;
				QLabel* NStop;
				QLabel* cfl;
				//QDoubleSpinBox* cInitSpinBox;
				QComboBox* cInitComboBox;
				QSpinBox* NStopSpinBox;
				QLineEdit* NStopLineEdit;
				QDoubleSpinBox* cflSpinBox;
				QLabel* P;
				QLineEdit*m_pLineEdit;
				QLabel* T;
				QLineEdit*m_tLineEdit;
				QLabel* Ma;
				QLineEdit*m_maLineEdit;
				QLabel* alpha;
				QSpinBox* alphaSpinBox;
				QLabel* beta;
				QLineEdit*m_betaLineEdit;
				QLabel* TwCondition;
				QComboBox* m_twConditionComboBox;
				QLabel* Tw;
				QLineEdit*m_TwLineEdit;
				QLabel*Xscale;
				//QLineEdit*m_xscaleLineEdit;
				QDoubleSpinBox*m_xscaleSpinBox;
				QLabel*AreaRef;
				QLineEdit*m_AreaRefLineEdit;
				QLabel*Lref;
				QLineEdit*m_LrefRefLineEdit;
				QLabel*xc;
				QLineEdit*m_xcLineEdit;
				QLabel*yc;
				QLineEdit*m_ycLineEdit;
				QLabel*zc;
				QLineEdit*m_zcLineEdit;

				QLabel*gasConstant;
				QLineEdit*m_gasLineEdit;
				QLabel*radioHeat;
				QLineEdit*m_radioLineEdit;

			};
		}/// reveal
	}/// sspns
}/// platforms






//#pragma once
//#include <qwidget.h>
//#include <QGridLayout>
//#include <QLabel>
//#include <QDoubleSpinBox>
//#include <QSpinBox>
//#include <fstream>
//#include <QComboBox>
//
//namespace platforms
//{
//	namespace sspns {
//		namespace reveal {
//			class InputHeader :
//				public QWidget
//			{
//				Q_OBJECT
//			public:
//				InputHeader();
//				~InputHeader();
//				void initUi();
//				void initHeadPart2();
//				void loadStyleSheet(QString sheetName);
//				void loadHeadConfig(QString path, bool flag);
//				std::ofstream outfile;
//
//			public slots:
//				void exeHeader();
//				//加载头部参数
//				void loadHeaderParameter(QString workPath);
//				void loadDefaultParameter();
//				void redictHeadParameter();
//				void onTwConditionChanged(int index);
//			private:
//				QString m_workspace;
//				QGridLayout*m_main1Layout;
//				QGridLayout*m_main2Layout;
//				QHBoxLayout*m_mainLayout;
//				QLabel* cInit;
//				QLabel* NStop;
//				QLabel* cfl;
//				//QDoubleSpinBox* cInitSpinBox;
//				QComboBox* cInitComboBox;
//				QSpinBox* NStopSpinBox;
//				QLineEdit* NStopLineEdit;
//				QDoubleSpinBox* cflSpinBox;
//				QLabel* P;
//				QLineEdit*m_pLineEdit;
//				QLabel* T;
//				QLineEdit*m_tLineEdit;
//				QLabel* Ma;
//				QLineEdit*m_maLineEdit;
//				QLabel* alpha;
//				QSpinBox* alphaSpinBox;
//				QLabel* beta;
//				QLineEdit*m_betaLineEdit;
//				QLabel* TwCondition;
//				QComboBox* m_twConditionComboBox;
//				QLabel* Tw;
//				QLineEdit*m_TwLineEdit;
//				QLabel*Xscale;
//				//QLineEdit*m_xscaleLineEdit;
//				QDoubleSpinBox*m_xscaleSpinBox;
//				QLabel*AreaRef;
//				QLineEdit*m_AreaRefLineEdit;
//				QLabel*Lref;
//				QLineEdit*m_LrefRefLineEdit;
//				QLabel*xc;
//				QLineEdit*m_xcLineEdit;
//				QLabel*yc;
//				QLineEdit*m_ycLineEdit;
//				QLabel*zc;
//				QLineEdit*m_zcLineEdit;
//
//				QLabel*gasConstant;
//				QLineEdit*m_gasLineEdit;
//				QLabel*radioHeat;
//				QLineEdit*m_radioLineEdit;
//
//			};
//		}/// reveal
//	}/// sspns
//}/// platforms
//
