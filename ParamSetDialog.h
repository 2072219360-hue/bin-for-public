// .h
#pragma once
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <regex>
#include <QGridLayout>
#include <fstream>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QScrollArea>
#include<QStackedWidget>
#include<fileParamChecker.h>
#include "CusButton.h"
#include "SignalInstance.h"


class ParamSetDialog :
	public QWidget
{
	Q_OBJECT
public:
	ParamSetDialog(int _nsFlag = 0, QWidget*parent = nullptr);
	~ParamSetDialog();
	void initUI();
	void initLayout();
	void initConnect();
	void loadTrTable();
	void initNSUI();
	void initPNSUI();
	void initEdgeUI();
	void initCalUI();
	void initAirUI();
	void initPostUI();
	void initAirCateUI();

	void initBoundaryConditionUI();
	void initMeshUI();
	void initSolverControl1UI();
	void initFarfieldUI();
	void initSolverControlUI();
	void initGeometryUI();
	void initAdvancedUI();
	void initGasUI();

	void initMap();

	void loadStyleSheet(QString sheetName);
	void initStyleSheet();

	void loadHeadConfig(QString path);
	void loadBodyConfig(QString path);
	void saveBodyCalScaleConfig();
	void saveBodyEdgeConfig();
	void saveBodyPNSConfig();
	void saveBodyOutConfig();
	void saveBodyAirConfig();
	void saveBodyAirCateConfig();

	void saveHeadCalScaleConfig();
	void saveHeadEdgeConfig();
	void saveHeadNSConfig();
	void saveHeadAirConfig();
	void saveHeadAirCateConfig();
	int nsFlag;
	bool ParamSetDialog::getRestartFlag();
	bool returnWallForcestatus();
	bool checkParam();
	void syncBatchFiles(const QString& relativeFilePath, std::function<void(const QString&)> modifyFunc);
	QString GetMa();
	QString GetAOA();
	QString GetAOB();
	
public slots:
	void saveTrTable();
	void updateDialog();
	void redictHeadParameter();
	void onTwConditionChanged(int index);

	void onCheckLineEdit();
	void redictBodyParameter();

	void WidgetExpandOrCollapase();

	void slotSaveParam();
	void slotUpdateSect();
	void changeView(int index);

private:
	////表格
	//TrTableShow* m_tableShow;
	//QWidget* m_trTableWidget;
	
	std::ofstream outfile;
	QString m_workspace;
	QMap<QLineEdit*, QLineEdit*> syncMap; // 管理成对变量的映射

	QStackedWidget* stackedWidget;

	// 布局
	QVBoxLayout* m_mainLayout;

	// NS 布局
	QVector<QVector<QWidget*>> headUi;
	QVBoxLayout* headQVLayout;
	QWidget* m_headWidget;
	QScrollArea* scrollAreaHead;
	QVector<QVector<QWidget*>> bodyUi[8];
	QVector<CusButton*> CusButtonVt;
	QMap<CusButton*, int> mp;
	QWidget* CusWidget[8];
	QWidget* wallForceWidget;

	//计算网格布局
	QHBoxLayout* CalQHLayout;

	//Edge布局
	QVector<QVector<QWidget*>> EdgeUi;
	QVBoxLayout* EdgeQVLayout;


	// PNS 布局
	QVector<QVector<QWidget*>> pnsUi;
	QVBoxLayout* pnsQVLayout;

	// Air 布局
	QVBoxLayout* airQVLayout;
	QVector<QVector<QWidget*>> airUi;

	//Air类型布局
	QVBoxLayout* aircateQVLayout;

	// Post 布局
	QVBoxLayout* postQVLayout;
	QWidget* m_postWidget;
	QScrollArea* scrollAreaPost;
	QVector<QVector<QWidget*>> postUi;

	// 隐藏布局
	QWidget* m_hideWidget;

	// 头部ui控件
	QString titleSheet;
	QLabel* btnNS;
	QLabel* btnPNS;
	QLabel* btnAir;
	QLabel* btnAirCate;
	QLabel* btnPost;
	QLabel* btnEdge;
	QLabel* Callabel;

	QLabel* cInit;
	QComboBox* cInitComboBox;

	QLabel* NStop;
	QLineEdit* NStopLineEdit;

	QLabel* cfl;
	QLineEdit* cflLineEdit;

	QLabel* P;
	QLineEdit*m_pLineEdit;

	QLabel* T;
	QLineEdit*m_tLineEdit;

	QLabel* Ma;
	QLineEdit*m_maLineEdit;

	QLabel* alpha;
	QLineEdit* alphaLineEdit;

	QLabel* beta;
	QLineEdit*m_betaLineEdit;

	QLabel* TwCondition;
	QComboBox* m_twConditionComboBox;

	QLabel* Tw;
	QLineEdit* m_TwLineEdit;

	QLabel* Xscale;
	QLineEdit* m_xscaleLineEdit;

	QLabel* AreaRef;
	QLineEdit* m_AreaRefLineEdit;

	QLabel* Lref;
	QLineEdit* m_LrefRefLineEdit;

	QLabel* xc;
	QLineEdit* m_xcLineEdit;

	QLabel* yc;
	QLineEdit* m_ycLineEdit;

	QLabel* zc;
	QLineEdit* m_zcLineEdit;

	QLabel* gasConstant;
	QLineEdit* m_gasLineEdit;

	QLabel* radioHeat;
	QLineEdit* m_radioLineEdit;


	// 身部ui控件
	QLabel* btnBody;

	CusButton* m_btnBoundaryCondition;
	QLabel* TwBody;
	QLineEdit* twLineEdit;

	CusButton* m_btnMesh;
	QLabel* Mesh;
	QLineEdit* meshLineEdit;

	CusButton* m_btnSolverControl1;
	QLabel* linearEq;
	QLineEdit* linearEqLineEdit;
	QLabel* gridGeneration;
	QLineEdit* gridGenerationLineEdit;
	QLabel* massCheck;
	QButtonGroup* btnGroupMassCheck;
	QRadioButton* massCheckBtnON;
	QRadioButton* massCheckBtnOFF;
	QLabel* gridCheck;
	QButtonGroup* btnGroupGridCheck;
	QRadioButton* gridCheckBtnON;
	QRadioButton* gridCheckBtnOFF;
	QLabel* checkDummyGrid;
	QButtonGroup* btnGroupcheckDummyGrid;
	QRadioButton* checkDummyGridBtnON;
	QRadioButton* checkDummyGridBtnOFF;
	QLabel* stepCtrl;
	QLineEdit* stepCtrlLineEdit;

	CusButton* m_btnFarfield;
	QLabel* IncomingMa;
	QLineEdit* MaLineEdit;
	QLabel* IncomingP;
	QLineEdit* PLineEdit;
	QLabel* IncomingT;
	QLineEdit* TLineEdit;
	QLabel* AoA;
	QLineEdit* AoALineEdit;
	QLabel* AoB;
	QLineEdit* AoBLineEdit;

	CusButton* m_btnSolverControl;
	QLabel* scheme;
	QComboBox* schemeComboBox;
	QLabel* limiter;
	QComboBox* limiterComboBox;

	CusButton* m_btnGeometry;
	QLabel* Rlen;
	QLineEdit* rlenLineEdit;
	QLabel* XScale;
	QLineEdit* xscaleLineEdit;
	QLabel* wallForce;
	QRadioButton* wallForceRadioBtnON;
	QRadioButton* wallForceRadioBtnOFF;
	QButtonGroup* btnGroupWall;
	QLabel* bodyAreaRef;
	QLineEdit* arearefLineEdit;
	QLabel* xmc;
	QLineEdit* xmcLineEdit;
	QLabel* ymc;
	QLineEdit* ymcLineEdit;
	QLabel* zmc;
	QLineEdit* zmcLineEdit;

	CusButton* m_btnAdvanced;
	QLabel* pressFactor;
	QComboBox* pressFactorCombBox;
	QLabel* temperal;
	QComboBox* temperalComboBox;
	QLabel* visGradient;
	QComboBox* visGradientComboBox;
	QLabel* implicitVis;
	QComboBox* implicitVisComboBox;

	CusButton* m_btnOutput;
	QLabel* nout;
	QLineEdit* noutLineEdit;
	QLabel* cVolFieldOutput;
	QButtonGroup* btnGroupcVolField;
	QRadioButton* cVolFieldOutputForceRadioBtnON;
	QRadioButton* cVolFieldOutputForceRadioBtnOFF;
	QLabel* cbdFieldOutput;
	QButtonGroup* btnGroupcbdField;
	QRadioButton* cbdFieldRadioBtnON;
	QRadioButton* cbdFieldRadioBtnOFF;

	CusButton* m_btnGas;
	QLabel* nspeciesLabel;
	QLabel* R0;
	QLabel* Gamma;
	QLineEdit* nsLineEdit;
	QLineEdit* r0LineEdit;
	QLineEdit* GammaLineEdit;

	QMap<QString, int>limiterMap;			///< 限制器可选值
	QMap<QString, int>visGradientMap;		///< 梯度格式可选值
	QMap<QString, int>schemeMap;			///< Scheme 参数可选值
	QMap<QString, int>implicitVisMap;		///< 粘性项隐式处理可选值
	QMap<QString, int>pressFactorMap;		///< 压力梯度分裂方法可选值

	QMap<QString, int>temperalMap;			///< 流向推进方法可选值
	QMap<QString, QString> objectNameMap;	///修改过变量名的变量，前后名字映射关系

	int current_page;

};
