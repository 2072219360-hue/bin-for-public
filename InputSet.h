#pragma once
#include <qwidget.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "CusButton.h"
#include "Common.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <fstream>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMap>
#include<QScrollArea>
#include"btnObserver.h"
#include"bodyPathChecker.h"
namespace platforms
{
	namespace sspns {

		namespace reveal {
			class InputSet :
				public QWidget
			{
				Q_OBJECT
			public:
				InputSet(QWidget*parent = 0);
				~InputSet();
				void initUi();
				void initOutput();
				void initFarfieldUi();
				void initBndryConditionUi();
				void initGeometryUi();
				void initMeshUi();
				void initStepCtrl();
				void initAdvancedSet();
				void initMap();	///< 用于初始化input中名称和可选值的的映射关系
				void initStyleSheet();
				void loadBodyConfig(QString path, bool flag);
				void loadStyleSheet(QString sheetName);

			signals:
				void wallStateChanged(bool state);
			public slots:
				void loadBodyParameter(QString workPath);
				void loadDefaultParameter();
				void onWallStateChanged();
				void onCheckLineEdit();
				void redictBodyParameter();
			private:
				::ofstream outfile;
				QHBoxLayout*m_mainLayout;
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
				QLineEdit*arearefSpinBox;
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
				QLabel*	AreaRef;
				QLabel*xmc;
				QLabel*ymc;
				QLabel*zmc;
				//保存工作路径
				QString m_workspace;
				QMap<QString, int>limiterMap;		///< 限制器可选值
				QMap<QString, int>visGradientMap;	///< 梯度格式可选值
				QMap<QString, int>schemeMap;		///< Scheme 参数可选值
				QMap<QString, int>implicitVisMap;	///< 粘性项隐式处理可选值
				QMap<QString, int>pressFactorMap;	///< 压力梯度分裂方法可选值
				QMap<QString, int>temperalMap;		///< 流向推进方法可选值
			};
		}
	}
}

//#pragma once
//#include <qwidget.h>
//#include <QVBoxLayout>
//#include <QLabel>
//#include <QPushButton>
//#include <QLineEdit>
//#include "CusButton.h"
//#include "Common.h"
//#include <QButtonGroup>
//#include <QRadioButton>
//#include <fstream>
//#include <QSpinBox>
//#include <QDoubleSpinBox>
//#include <QMap>
//
//namespace platforms
//{
//	namespace sspns {
//
//		namespace reveal {
//			class InputSet :
//				public QWidget
//			{
//				Q_OBJECT
//			public:
//				InputSet(QWidget*parent = 0);
//				~InputSet();
//				void initUi();
//				void initOutput();
//				void initFarfieldUi();
//				void initBndryConditionUi();
//				void initGeometryUi();
//				void initMeshUi();
//				void initStepCtrl();
//				void initAdvancedSet();
//				void initMap();	///< 用于初始化input中名称和可选值的的映射关系
//				void initStyleSheet();
//				void loadBodyConfig(QString path, bool flag);
//				void loadStyleSheet(QString sheetName);
//
//			signals:
//				void wallStateChanged(bool state);
//			public slots:
//				void loadBodyParameter(QString workPath);
//				void loadDefaultParameter();
//				void onWallStateChanged();
//				void onCheckLineEdit();
//				void redictBodyParameter();
//			private:
//				::ofstream outfile;
//				QHBoxLayout*m_mainLayout;
//				CusButton*m_btnOutput;
//				QLabel* nout;
//				QLabel* wallForce;
//				QRadioButton* wallForceRadioBtnON;
//				QRadioButton* wallForceRadioBtnOFF;
//				QButtonGroup* btnGroupWall;
//				QWidget*wallForceWidget;
//				QSpinBox*spinBoxNout;
//
//				QRadioButton* cVolFieldOutputForceRadioBtnON;
//				QRadioButton* cVolFieldOutputForceRadioBtnOFF;
//				QButtonGroup* btnGroupcVolField;
//
//				QRadioButton* cbdFieldRadioBtnON;
//				QRadioButton* cbdFieldRadioBtnOFF;
//				QButtonGroup* btnGroupcbdField;
//
//				QWidget* outputWidget;
//				CusButton* m_btnFarfield;
//				QWidget* farfieldWidget;
//				CusButton* m_btnBoundaryCondition;
//				QWidget* boundaryConditionWidget;
//				CusButton* m_btnGeometry;
//				QWidget* geometryWidget;
//				CusButton* m_btnMesh;
//				QWidget* meshWidget;
//
//				CusButton* m_btnAdvanced;
//				QWidget* advancedWidget;
//
//				QDoubleSpinBox* maSpinBox;
//				QDoubleSpinBox* pSpinBox;
//				QDoubleSpinBox* tSpinBox;
//				QDoubleSpinBox* aoaSpinBox;
//				QDoubleSpinBox* aobSpinBox;
//				QDoubleSpinBox* twSpinBox;
//				QLineEdit*rlenSpinBox;
//				QLineEdit*xscaleSpinBox;
//				QLineEdit*arearefSpinBox;
//				QDoubleSpinBox*xmcSpinBox;
//				QDoubleSpinBox*ymcSpinBox;
//				QDoubleSpinBox*zmcSpinBox;
//				QLineEdit*meshLineEdit;
//				QSpinBox* nsSpinBox;
//				QLineEdit* r0LineEdit;
//				QDoubleSpinBox* GammaDoubleSpinBox;
//
//				QGridLayout*farfieldLayout = new QGridLayout;
//				CusButton* m_btnGas;
//				QWidget*gasWidget;
//
//				CusButton* m_btnSolverControl;
//				CusButton* m_btnSolverControl1;
//				QWidget*solverControlWidget1;
//				QWidget*solverControlWidget;
//				QLabel*scheme;
//				//QLineEdit*schemeLineEdit;
//				QComboBox*schemeComboBox;
//				QLabel*limiter;
//				//QLineEdit*limiterLineEdit;
//				QComboBox*limiterComboBox;
//				QLabel*pressFactor;
//				//QLineEdit*pressFactorLineEdit;
//				QComboBox*pressFactorCombBox;
//				QLabel*temperal;
//				//QLineEdit*temperalLineEdit;
//				QComboBox* temperalComboBox;
//				QLabel*visGradient;
//				//QLineEdit*visGradientLineEdit;
//				QComboBox*visGradientComboBox;
//				QLabel*implicitVis;
//				//QLineEdit*implicitVisLineEdit;
//				QComboBox*implicitVisComboBox;
//				QLabel*stepCtrl;
//				QLineEdit*stepCtrlLineEdit;
//				QLabel*linearEq;
//				QLineEdit*linearEqLineEdit;
//				QLabel*gridGeneration;
//				QLineEdit*gridGenerationLineEdit;
//
//				QLabel*massCheck;
//				QRadioButton* massCheckBtnON;
//				QRadioButton* massCheckBtnOFF;
//				QButtonGroup* btnGroupMassCheck;
//
//				QLabel*gridCheck;
//				QRadioButton* gridCheckBtnON;
//				QRadioButton* gridCheckBtnOFF;
//				QButtonGroup* btnGroupGridCheck;
//
//				QLabel*checkDummyGrid;
//				QRadioButton* checkDummyGridBtnON;
//				QRadioButton* checkDummyGridBtnOFF;
//				QButtonGroup* btnGroupcheckDummyGrid;
//				QLabel*	AreaRef;
//				QLabel*xmc;
//				QLabel*ymc;
//				QLabel*zmc;
//				//保存工作路径
//				QString m_workspace;
//				QMap<QString, int>limiterMap;		///< 限制器可选值
//				QMap<QString, int>visGradientMap;	///< 梯度格式可选值
//				QMap<QString, int>schemeMap;		///< Scheme 参数可选值
//				QMap<QString, int>implicitVisMap;	///< 粘性项隐式处理可选值
//				QMap<QString, int>pressFactorMap;	///< 压力梯度分裂方法可选值
//				QMap<QString, int>temperalMap;		///< 流向推进方法可选值
//			};
//		}
//	}
//}