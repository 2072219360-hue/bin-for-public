#pragma once
#include <QWidget>
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
#include <QScrollArea>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QMap>


#include "CusButton.h"
#include "Common.h"
#include "SignalInstance.h"
#include "OutPutLogger.h"
#include"TrTableShow.h"
#include"bodyPathChecker.h"
namespace platforms {
	namespace sspns {
		class publicArgWindow:
			public QWidget
		{
			Q_OBJECT
		public slots:
			void onWallStateChanged();
			void onCheckLineEdit();
			void redictBodyParameter();
			void loadParameter(QString workPath);
		public:
			publicArgWindow();
			void initMap();
			void setRestartFlag(bool flag);
			void loadBodyConfig(QString path, bool flag);
			void loadHeadParameter(QString workPath);
			void loadBodyParameter(QString workPath);
			void initUi();
			void initHeadUi();
			void initBodyUi();
			void initLayout();
			void loadStyleSheet(QString sheetName);
			void initAdvancedSet();
			void initConnection();
			void loadHeadConfig(QString path, bool flag);
			std::ofstream outfile;
		private:
			QWidget *headWidget,*bodyWidget;

			//头部
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
			//身部
			QVBoxLayout*m_mainLayoutBody;
			CusButton*m_btnOutput;
			QLabel* nout;
			QLabel* wallForce;
			QRadioButton* wallForceRadioBtnON;
			QRadioButton* wallForceRadioBtnOFF;
			QButtonGroup* btnGroupWall;
			QWidget*wallForceWidget;
			QSpinBox*spinBoxNout;

			QRadioButton* cVolFieldOutputForceRadioBtnON;
			QRadioButton* cVolFieldOutputForceRadioBtnOFF;
			QButtonGroup* btnGroupcVolField;

			QRadioButton* cbdFieldRadioBtnON;
			QRadioButton* cbdFieldRadioBtnOFF;
			QButtonGroup* btnGroupcbdField;

			QWidget* outputWidget;
			CusButton* m_btnFarfield;
			QWidget* farfieldWidget;
			CusButton* m_btnBoundaryCondition;
			QWidget* boundaryConditionWidget;
			CusButton* m_btnGeometry;
			QWidget* geometryWidget;
			CusButton* m_btnMesh;
			QWidget* meshWidget;

			CusButton* m_btnAdvanced;
			QWidget* advancedWidget;

			QDoubleSpinBox* maSpinBox;
			QDoubleSpinBox* pSpinBox;
			QDoubleSpinBox* tSpinBox;
			QDoubleSpinBox* aoaSpinBox;
			QDoubleSpinBox* aobSpinBox;
			QDoubleSpinBox* twSpinBox;
			QLineEdit*rlenSpinBox;
			QLineEdit*xscaleSpinBox;
			QLineEdit*AreaRefBodySpinBox;
			QDoubleSpinBox*xmcSpinBox;
			QDoubleSpinBox*ymcSpinBox;
			QDoubleSpinBox*zmcSpinBox;
			QLineEdit*meshLineEdit;
			QSpinBox* nsSpinBox;
			QLineEdit* r0LineEdit;
			QDoubleSpinBox* GammaDoubleSpinBox;

			QGridLayout*farfieldLayout = new QGridLayout;
			QVBoxLayout*farfield_main;
			QVBoxLayout*output_mainout;


			CusButton* m_btnGas;
			QWidget*gasWidget;

			CusButton* m_btnSolverControl;
			CusButton* m_btnSolverControl1;
			QWidget*solverControlWidget1;
			QWidget*solverControlWidget;
			QLabel*scheme;
			//QLineEdit*schemeLineEdit;
			QComboBox*schemeComboBox;
			QLabel*limiter;
			//QLineEdit*limiterLineEdit;
			QComboBox*limiterComboBox;
			QLabel*pressFactor;
			//QLineEdit*pressFactorLineEdit;
			QComboBox*pressFactorCombBox;
			QLabel*temperal;
			//QLineEdit*temperalLineEdit;
			QComboBox* temperalComboBox;
			QLabel*visGradient;
			//QLineEdit*visGradientLineEdit;
			QComboBox*visGradientComboBox;
			QLabel*implicitVis;
			//QLineEdit*implicitVisLineEdit;
			QComboBox*implicitVisComboBox;
			QLabel*stepCtrl;
			QLineEdit*stepCtrlLineEdit;
			QLabel*linearEq;
			QLineEdit*linearEqLineEdit;
			QLabel*gridGeneration;
			QLineEdit*gridGenerationLineEdit;

			QLabel*massCheck;
			QRadioButton* massCheckBtnON;
			QRadioButton* massCheckBtnOFF;
			QButtonGroup* btnGroupMassCheck;

			QLabel*gridCheck;
			QRadioButton* gridCheckBtnON;
			QRadioButton* gridCheckBtnOFF;
			QButtonGroup* btnGroupGridCheck;

			QLabel*checkDummyGrid;
			QRadioButton* checkDummyGridBtnON;
			QRadioButton* checkDummyGridBtnOFF;
			QButtonGroup* btnGroupcheckDummyGrid;
			QLabel*	AreaRefBody;
			QLabel*xmc;
			QLabel*ymc;
			QLabel*zmc;
			QMap<QString, int>limiterMap;		///< 限制器可选值
			QMap<QString, int>visGradientMap;	///< 梯度格式可选值
			QMap<QString, int>schemeMap;		///< Scheme 参数可选值
			QMap<QString, int>implicitVisMap;	///< 粘性项隐式处理可选值
			QMap<QString, int>pressFactorMap;	///< 压力梯度分裂方法可选值
			QMap<QString, int>temperalMap;		///< 流向推进方法可选值
		};
	}
}



