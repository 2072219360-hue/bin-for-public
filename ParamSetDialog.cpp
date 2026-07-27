// .cpp
#include <QGridLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QDebug>
#include <QCoreApplication>
#include <QFrame>
#include "ParamSetDialog.h"
#include "huangmainwindow.h"
#include "SignalInstance.h"
#include "OutPutLogger.h"
#include "bodyPathChecker.h"


extern QString appComDir;
extern OutPutLogger* outPutLogger;
extern TrTableShow* tableShow;
//int current_page;
//int djk;

ParamSetDialog::ParamSetDialog(int _nsFlag, QWidget*parent) :QWidget(parent)
{
	nsFlag = _nsFlag;
	
	setMinimumSize(700, 500);
	initUI();
	initMap();
	initLayout();
	initConnect();
	m_workspace = SignalInstance::getInstance()->getWorkSpace();
	QDir workspaceDir(m_workspace + "/head");
	//connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, &ParamSetDialog::updateDialog);
	connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, [=](QString new_workspace) {
		FileParamChecker checker;
		QString dirPath = new_workspace;
		QString headConfigPath = dirPath + "/head/headconfig.dat";
		checker.checkRestart(dirPath + "/head/restart.dat", headConfigPath);
		updateDialog();
		bool flag = returnWallForcestatus();
		emit SignalInstance::getInstance()->signalChangeAirParam(flag);
	});
}

ParamSetDialog::~ParamSetDialog() {
	
}

void ParamSetDialog::initUI()
{
	loadTrTable();
	initNSUI();
	initPNSUI();
	initEdgeUI();
	initCalUI();
	initAirUI();
	initAirCateUI();
	initPostUI();
	this->loadStyleSheet("InputSet2");
	initStyleSheet();
}

void ParamSetDialog::initPNSUI()
{
	// 原本头部
	btnNS = new QLabel(u8"PNS求解器");
	btnNS->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnNS->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnNS->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnNS->setFixedHeight(40);

	P = new QLabel(u8"来流压力");
	QDoubleValidator* doubleValidatorP = new QDoubleValidator(this);
	doubleValidatorP->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorP->setNotation(QDoubleValidator::StandardNotation);
	m_pLineEdit = new QLineEdit;
	m_pLineEdit->setValidator(doubleValidatorP);
	//headUi.append({ P, m_pLineEdit });
	EdgeUi.append({ P,m_pLineEdit });//边界条件

	// 只改这个控件的信息，其他控件不用改
	nout = new QLabel(u8"信息输出步数");    // = 100
	noutLineEdit = new QLineEdit;
	noutLineEdit->setText("100");
	QIntValidator* intValidatorNout = new QIntValidator(1, std::numeric_limits<int>::max(), this);
	noutLineEdit->setValidator(intValidatorNout);
	noutLineEdit->setObjectName("noutLineEdit");
	postUi.append({ nout, noutLineEdit });

	AreaRef = new QLabel(u8"参考面积");
	m_AreaRefLineEdit = new QLineEdit;
	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
	m_AreaRefLineEdit->setValidator(doubleValidatorGe);
	//headUi.append({ AreaRef, m_AreaRefLineEdit });

	Lref = new QLabel(u8"参考长度");
	m_LrefRefLineEdit = new QLineEdit;
	m_LrefRefLineEdit->setValidator(doubleValidatorGe);
	//headUi.append({ Lref, m_LrefRefLineEdit });

	// xmc、ymc、zmc的限制器
	QDoubleValidator* doubleValidatorMc = new QDoubleValidator(this);
	doubleValidatorMc->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), 8);
	doubleValidatorMc->setNotation(QDoubleValidator::StandardNotation);

	xc = new QLabel(u8"质心坐标xc");
	m_xcLineEdit = new QLineEdit;
	m_xcLineEdit->setValidator(doubleValidatorMc);
	headUi.append({ xc, m_xcLineEdit });

	yc = new QLabel(u8"质心坐标yc");
	m_ycLineEdit = new QLineEdit;
	m_ycLineEdit->setValidator(doubleValidatorMc);
	headUi.append({ yc, m_ycLineEdit });

	zc = new QLabel(u8"质心坐标zc");
	m_zcLineEdit = new QLineEdit;
	m_zcLineEdit->setValidator(doubleValidatorMc);
	headUi.append({ zc, m_zcLineEdit });

	m_pLineEdit->setObjectName("m_pLineEdit");
	m_AreaRefLineEdit->setObjectName("m_AreaRefLineEdit");
	m_LrefRefLineEdit->setObjectName("m_LrefRefLineEdit");
	m_xcLineEdit->setObjectName("m_xcLineEdit");
	m_ycLineEdit->setObjectName("m_ycLineEdit");
	m_zcLineEdit->setObjectName("m_zcLineEdit");

	// 原本身部
	initBoundaryConditionUI();
	initMeshUI();
	initSolverControl1UI();
	initFarfieldUI();
	initSolverControlUI();
	initGeometryUI();
	initAdvancedUI();
	initGasUI();
}

// initCalUI 网格缩放比例 这一坨改一下，添加限制器
void ParamSetDialog::initCalUI()
{
	Callabel = new QLabel(u8"计算网格参数设置");
	Callabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	Callabel->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	Callabel->setFixedHeight(40);

	Xscale = new QLabel(u8"网格缩放比例");   // 0.001
	m_xscaleLineEdit = new QLineEdit;
	QDoubleValidator* doubleValidatorXscale = new QDoubleValidator(this);
	doubleValidatorXscale->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorXscale->setNotation(QDoubleValidator::StandardNotation);
	m_xscaleLineEdit->setValidator(doubleValidatorXscale);

	m_xscaleLineEdit->setObjectName("m_xscaleLineEdit");
}

//边界
void ParamSetDialog::initEdgeUI()
{
	btnEdge = new QLabel(u8"边界条件参数设置");
	btnEdge->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnEdge->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnEdge->setFixedHeight(40);

	T = new QLabel(u8"温度");
	QDoubleValidator* doubleValidatorT = new QDoubleValidator(this);
	doubleValidatorT->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorT->setNotation(QDoubleValidator::StandardNotation);
	m_tLineEdit = new QLineEdit;
	m_tLineEdit->setValidator(doubleValidatorT);
	EdgeUi.append({ T, m_tLineEdit });

	Ma = new QLabel(u8"马赫数");
	QDoubleValidator* doubleValidatorMa = new QDoubleValidator(this);
	doubleValidatorMa->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorMa->setNotation(QDoubleValidator::StandardNotation);
	m_maLineEdit = new QLineEdit;
	m_maLineEdit->setValidator(doubleValidatorMa);
	EdgeUi.append({ Ma, m_maLineEdit });

	// 只改这两个控件的信息，其他控件不用改
	alpha = new QLabel(u8"攻角");
	alphaLineEdit = new QLineEdit;
	QDoubleValidator* doubleValidatoralpha = new QDoubleValidator(this);
	doubleValidatoralpha->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), 8);
	doubleValidatoralpha->setNotation(QDoubleValidator::StandardNotation);
	alphaLineEdit->setValidator(doubleValidatoralpha);
	EdgeUi.append({ alpha, alphaLineEdit });

	beta = new QLabel(u8"侧滑角");
	m_betaLineEdit = new QLineEdit;
	m_betaLineEdit->setValidator(doubleValidatoralpha);
	EdgeUi.append({ beta, m_betaLineEdit });

	TwCondition = new QLabel(u8"壁面温度条件");
	m_twConditionComboBox = new QComboBox;
	m_twConditionComboBox->addItem(u8"常壁温");
	m_twConditionComboBox->addItem(u8"绝热壁");
	EdgeUi.append({ TwCondition, m_twConditionComboBox });

	Tw = new QLabel(u8"壁温");
	m_TwLineEdit = new QLineEdit;
	QDoubleValidator* doubleValidatorTw = new QDoubleValidator(this);
	doubleValidatorTw->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorTw->setNotation(QDoubleValidator::StandardNotation);
	m_TwLineEdit->setPlaceholderText(u8"请输入壁温");
	m_TwLineEdit->setValidator(doubleValidatorTw);
	EdgeUi.append({ Tw, m_TwLineEdit });

	m_tLineEdit->setObjectName("m_tLineEdit");
	m_maLineEdit->setObjectName("m_maLineEdit");
	alphaLineEdit->setObjectName("alphaLineEdit");
	m_betaLineEdit->setObjectName("m_betaLineEdit");
	m_TwLineEdit->setObjectName("m_TwLineEdit");
}

void ParamSetDialog::initNSUI()
{
	btnPNS = new QLabel(u8"NS求解器设置参数");
	btnPNS->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnPNS->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnPNS->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnPNS->setFixedHeight(40);

	cInit = new QLabel(u8"流场初始化");
	cInitComboBox = new QComboBox;
	cInitComboBox->addItem(u8"自由来流");
	cInitComboBox->addItem(u8"续算");
	cInitComboBox->setCurrentIndex(0);
	pnsUi.append({ cInit, cInitComboBox });

	NStop = new QLabel(u8"计算步数");
	QIntValidator*intValidatorNStop = new QIntValidator(1, 100000, this);
	NStopLineEdit = new QLineEdit;
	NStopLineEdit->setValidator(intValidatorNStop);
	pnsUi.append({ NStop, NStopLineEdit });

	cfl = new QLabel("cfl");
	cflLineEdit = new QLineEdit;
	// 加这一行就行了
	cflLineEdit->setValidator(intValidatorNStop);
	pnsUi.append({ cfl, cflLineEdit });

	cInitComboBox->setObjectName("cInitComboBox");
	NStopLineEdit->setObjectName("NStopLineEdit");
	cflLineEdit->setObjectName("cflLineEdit");
}

void ParamSetDialog::initAirUI()
{
	btnAir = new QLabel(u8"气动力计算设置");
	btnAir->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnAir->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnAir->setFixedHeight(40);

	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);

	bodyAreaRef = new QLabel(u8"参考面积");         // 0.0074355
	arearefLineEdit = new QLineEdit;
	arearefLineEdit->setValidator(doubleValidatorGe);
	arearefLineEdit->setObjectName("arearefLineEdit");
	airUi.append({ bodyAreaRef, arearefLineEdit });

	Rlen = new QLabel(u8"参考长度");          // = 0.264
	rlenLineEdit = new QLineEdit;
	rlenLineEdit->setValidator(doubleValidatorGe);
	rlenLineEdit->setObjectName("rlenLineEdit");
	rlenLineEdit->setText("1.0");
	airUi.append({ Rlen, rlenLineEdit });
}

void ParamSetDialog::initAirCateUI()
{
	btnAirCate = new QLabel(u8"气体属性设置");
	btnAirCate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnAirCate->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnAirCate->setFixedHeight(40);

	radioHeat = new QLabel(u8"比热比");
	m_radioLineEdit = new QLineEdit; // 确保 m_radioLineEdit 初始化
	QDoubleValidator* doubleValidatorRadioHeat = new QDoubleValidator(this);
	doubleValidatorRadioHeat->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorRadioHeat->setNotation(QDoubleValidator::StandardNotation);
	m_radioLineEdit->setValidator(doubleValidatorRadioHeat);
	//headUi.append({ radioHeat, m_radioLineEdit });

	gasConstant = new QLabel(u8"气体常数");
	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(1e-10, std::numeric_limits<double>::max(), 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
	m_gasLineEdit = new QLineEdit;
	m_gasLineEdit->setValidator(doubleValidatorGe);
	//headUi.append({ gasConstant, m_gasLineEdit });

	m_gasLineEdit->setObjectName("m_gasLineEdit");
	m_radioLineEdit->setObjectName("m_radioLineEdit");

}

void ParamSetDialog::initPostUI()
{
	btnPost = new QLabel(u8"输出控制参数设置");
	btnPost->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnPost->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	btnPost->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
	btnPost->setFixedHeight(40);

	cVolFieldOutput = new QLabel(u8"是否输出空间流场");   // = ON
	btnGroupcVolField = new QButtonGroup;
	cVolFieldOutputForceRadioBtnON = new QRadioButton("ON");
	cVolFieldOutputForceRadioBtnON->setObjectName("cVolFieldOutputForceRadioBtnON");
	cVolFieldOutputForceRadioBtnON->setChecked(true);
	cVolFieldOutputForceRadioBtnOFF = new QRadioButton("OFF");
	cVolFieldOutputForceRadioBtnOFF->setObjectName("cVolFieldOutputForceRadioBtnOFF");
	btnGroupcVolField->addButton(cVolFieldOutputForceRadioBtnON);
	btnGroupcVolField->addButton(cVolFieldOutputForceRadioBtnOFF);
	postUi.append({ cVolFieldOutput, cVolFieldOutputForceRadioBtnON, cVolFieldOutputForceRadioBtnOFF });

	cbdFieldOutput = new QLabel(u8"是否输出边界流场");  // = ON
	btnGroupcbdField = new QButtonGroup;
	cbdFieldRadioBtnON = new QRadioButton("ON");
	cbdFieldRadioBtnON->setObjectName("cbdFieldRadioBtnON");
	cbdFieldRadioBtnON->setChecked(true);
	cbdFieldRadioBtnOFF = new QRadioButton("OFF");
	cbdFieldRadioBtnOFF->setObjectName("cbdFieldRadioBtnOFF");
	btnGroupcbdField->addButton(cbdFieldRadioBtnON);
	btnGroupcbdField->addButton(cbdFieldRadioBtnOFF);
	postUi.append({ cbdFieldOutput, cbdFieldRadioBtnON, cbdFieldRadioBtnOFF });

	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);

	wallForce = new QLabel(u8"是否计算气动力");		 // = ON
	btnGroupWall = new QButtonGroup;
	wallForceRadioBtnON = new QRadioButton("ON");
	wallForceRadioBtnON->setObjectName("wallForceRadioBtnON");
	wallForceRadioBtnOFF = new QRadioButton("OFF");
	wallForceRadioBtnOFF->setObjectName("wallForceRadioBtnOFF");
	btnGroupWall->addButton(wallForceRadioBtnON);
	btnGroupWall->addButton(wallForceRadioBtnOFF);
	wallForceRadioBtnOFF->setChecked(true);
	postUi.append({ wallForce, wallForceRadioBtnON, wallForceRadioBtnOFF });

	//下面几项不是必须的，但在计算力矩系数及升力、阻力系数时必须给定
	xmc = new QLabel(u8"力矩参考点xmc");        // 0.156
	xmcLineEdit = new QLineEdit;
	xmcLineEdit->setObjectName("xmcLineEdit");
	airUi.append({ xmc, m_xcLineEdit });

	ymc = new QLabel(u8"力矩参考点ymc");        // 0
	ymcLineEdit = new QLineEdit;
	ymcLineEdit->setObjectName("ymcLineEdit");
	airUi.append({ ymc, m_ycLineEdit });

	zmc = new QLabel(u8"力矩参考点zmc");        // 0
	zmcLineEdit = new QLineEdit;
	zmcLineEdit->setObjectName("zmcLineEdit");
	zmcLineEdit->setText("0");
	airUi.append({ zmc, m_zcLineEdit });

	wallForceWidget = new QWidget;
	QGridLayout* wallWidgetLayout = new QGridLayout;
	wallForceWidget->setLayout(wallWidgetLayout);
}

void ParamSetDialog::initBoundaryConditionUI()
{
	// BoundaryCondition属性设置 0
	m_btnBoundaryCondition = new CusButton;
	m_btnBoundaryCondition->SetTextLabel(u8"BoundaryCondition属性设置");
	m_btnBoundaryCondition->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	m_btnBoundaryCondition->setFixedWidth(600);
	CusButtonVt.append({ m_btnBoundaryCondition });

	TwBody = new QLabel(u8"壁温");
	twLineEdit = new QLineEdit;
	twLineEdit->setObjectName("twLineEdit");
	bodyUi[0].append({ TwBody, twLineEdit });
}

void ParamSetDialog::initMeshUI()
{
	// Mesh属性设置 1
	m_btnMesh = new CusButton;
	m_btnMesh->SetTextLabel(u8"Mesh属性设置");
	m_btnMesh->setFixedWidth(600);
	m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	CusButtonVt.append({ m_btnMesh });

	Mesh = new QLabel("Mesh");
	meshLineEdit = new QLineEdit;
	meshLineEdit->setObjectName("meshLineEdit");
	bodyUi[1].append({ Mesh, meshLineEdit });
}

void ParamSetDialog::initSolverControl1UI()
{
	// SolverControl属性设置 2
	m_btnSolverControl1 = new CusButton;
	m_btnSolverControl1->SetTextLabel(u8"SolverControl属性设置");
	m_btnSolverControl1->setFixedWidth(600);
	m_btnSolverControl1->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	CusButtonVt.append({ m_btnSolverControl1 });

	linearEq = new QLabel("linearEq");
	linearEqLineEdit = new QLineEdit;
	linearEqLineEdit->setObjectName("linearEqLineEdit");
	bodyUi[2].append({ linearEq, linearEqLineEdit });

	gridGeneration = new QLabel("Grid");
	gridGenerationLineEdit = new QLineEdit;
	gridGenerationLineEdit->setObjectName("gridGenerationLineEdit");
	bodyUi[2].append({ gridGeneration, gridGenerationLineEdit });

	stepCtrl = new QLabel("stepCtrl");
	stepCtrlLineEdit = new QLineEdit;
	stepCtrlLineEdit->setObjectName("stepCtrlLineEdit");
	bodyUi[2].append({ stepCtrl, stepCtrlLineEdit });
}

void ParamSetDialog::initFarfieldUI()
{
	// Farfield属性设置 与头部相同，不显示
	m_btnFarfield = new CusButton;
	m_btnFarfield->SetTextLabel(u8"Farfield属性设置");
	m_btnFarfield->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	m_btnFarfield->setFixedWidth(600);
	//CusButtonVt.append({ m_btnFarfield });

	IncomingMa = new QLabel(u8"来流马赫数");	// = 7.84
	IncomingP = new QLabel(u8"来流压力");		//273.72
	IncomingT = new QLabel(u8"来流温度");       //46.2
	AoA = new QLabel(u8"攻角");                 // 0
	AoB = new QLabel(u8"侧滑角");               // 0

	MaLineEdit = new QLineEdit;
	MaLineEdit->setObjectName("MaLineEdit");
	PLineEdit = new QLineEdit;
	PLineEdit->setObjectName("PLineEdit");
	TLineEdit = new QLineEdit;
	TLineEdit->setObjectName("TLineEdit");
	AoALineEdit = new QLineEdit;
	AoALineEdit->setObjectName("AoALineEdit");
	AoBLineEdit = new QLineEdit;
	AoBLineEdit->setObjectName("AoBLineEdit");
}

void ParamSetDialog::initSolverControlUI()
{
	// SolverControl属性设置 3
	m_btnSolverControl = new CusButton;
	m_btnSolverControl->setFixedWidth(600);
	m_btnSolverControl->SetTextLabel(u8"SolverControl属性设置");
	m_btnSolverControl->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	CusButtonVt.append({ m_btnSolverControl });

	scheme = new QLabel(u8"数值格式");
	schemeComboBox = new QComboBox;
	schemeComboBox->setObjectName("schemeComboBox");
	schemeComboBox->addItem(tr("AUSMPW")); // 添加选项
	schemeComboBox->addItem(tr("AUSMPW+"));
	schemeComboBox->addItem(tr("AUSM+"));
	schemeComboBox->addItem(tr("AUSM++"));
	schemeComboBox->addItem(tr("HLLC"));
	schemeComboBox->setCurrentIndex(0);
	bodyUi[3].append({ scheme, schemeComboBox });

	limiter = new QLabel(u8"限制器");
	limiterComboBox = new QComboBox;
	limiterComboBox->setObjectName("limiterComboBox");
	limiterComboBox->addItem(tr("minmod"));
	limiterComboBox->addItem(tr("van Albada"));
	limiterComboBox->addItem(tr("superbee"));
	limiterComboBox->addItem(tr("van Leer"));
	limiterComboBox->addItem(tr("Modified van Albada"));
	limiterComboBox->setCurrentIndex(4);
	bodyUi[3].append({ limiter, limiterComboBox });
}

void ParamSetDialog::initGeometryUI()
{
	// Geometry属性设置 与头部相同，不显示
	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
	m_btnGeometry = new CusButton;
	m_btnGeometry->SetTextLabel(u8"Geometry属性设置");
	m_btnGeometry->setFixedWidth(600);
	m_btnGeometry->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	//CusButtonVt.append({ m_btnGeometry });

	QLabel* XScale = new QLabel(u8"网格缩放比例");   // 0.001
	xscaleLineEdit = new QLineEdit;
	xscaleLineEdit->setValidator(doubleValidatorGe);
	xscaleLineEdit->setObjectName("xscaleLineEdit");
	xscaleLineEdit->setText("0");
	//bodyUi[5].append({ XScale, xscaleLineEdit });
}

void ParamSetDialog::initAdvancedUI()
{
	// 高级参数 4
	m_btnAdvanced = new CusButton;
	m_btnAdvanced->SetTextLabel(u8"高级参数");
	m_btnAdvanced->setFixedWidth(600);
	m_btnAdvanced->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	CusButtonVt.append({ m_btnAdvanced });

	pressFactor = new QLabel(u8"压力梯度分裂方法");
	pressFactorCombBox = new QComboBox;
	pressFactorCombBox->setObjectName("pressFactorCombBox");
	pressFactorCombBox->addItem(tr("THIVET 方法"));
	pressFactorCombBox->addItem(tr("Vigneron 方法"));
	pressFactorCombBox->setCurrentIndex(0);
	bodyUi[4].append({ pressFactor, pressFactorCombBox });

	temperal = new QLabel(u8"流向推进方法");
	temperalComboBox = new QComboBox;
	temperalComboBox->setObjectName("temperalComboBox");
	temperalComboBox->addItem(tr("LU-SGS"));
	temperalComboBox->addItem(tr("ADI"));
	temperalComboBox->setCurrentIndex(0);
	bodyUi[4].append({ temperal, temperalComboBox });

	visGradient = new QLabel(u8"梯度格式");
	visGradientComboBox = new QComboBox;
	visGradientComboBox->setObjectName("visGradientComboBox");
	visGradientComboBox->addItem(tr("高斯法"));
	visGradientComboBox->addItem(tr("有限差分法"));
	visGradientComboBox->setCurrentIndex(0);
	bodyUi[4].append({ visGradient, visGradientComboBox });

	implicitVis = new QLabel(u8"粘性项隐式处理方法");
	implicitVisComboBox = new QComboBox;
	implicitVisComboBox->setObjectName("implicitVisComboBox");
	implicitVisComboBox->addItem(tr("守恒变量导数"));
	implicitVisComboBox->addItem(tr("最大谱半径线化方法 1"));
	implicitVisComboBox->addItem(tr("最大谱半径线化方法 2"));
	implicitVisComboBox->setCurrentIndex(0);
	bodyUi[4].append({ implicitVis, implicitVisComboBox });
}

void ParamSetDialog::initGasUI()
{
	// Gas属性设置 5
	m_btnGas = new CusButton;
	m_btnGas->setFixedWidth(600);
	m_btnGas->SetTextLabel(u8"Gas属性设置");
	m_btnGas->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
	CusButtonVt.append({ m_btnGas });

	nspeciesLabel = new QLabel("Nspecies");
	nsLineEdit = new QLineEdit;
	nsLineEdit->setObjectName("nsLineEdit");
	bodyUi[5].append({ nspeciesLabel, nsLineEdit });

	R0 = new QLabel(u8"气体常数");
	r0LineEdit = new QLineEdit;
	QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
	doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
	doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
	r0LineEdit->setValidator(doubleValidatorGe);
	r0LineEdit->setObjectName("r0LineEdit");
	r0LineEdit->setText("287.06");
	//bodyUi[8].append({ R0, r0LineEdit });

	Gamma = new QLabel(u8"比热比");
	GammaLineEdit = new QLineEdit;
	GammaLineEdit->setObjectName("GammaLineEdit");
	GammaLineEdit->setText("1.4");
	//bodyUi[8].append({ Gamma, GammaLineEdit });
}

void ParamSetDialog::initMap()
{
	/*limiterMap["minmod"] = 0;
	limiterMap["van Albada"] = 1;
	limiterMap["superbee"] = 2;
	limiterMap["van Leer"] = 3;
	limiterMap["Modified van Albada"] = 4;*/
	limiterMap["MINMOD"] = 0;
	limiterMap["VANALBADA"] = 1;
	limiterMap["SUPERBEE"] = 2;
	limiterMap["VANLEER"] = 3;
	limiterMap["MODIFIED VANALBADA"] = 4;


	schemeMap["AUSMPW"] = 0;
	schemeMap["AUSMPW+"] = 1;
	schemeMap["AUSM+"] = 2;
	schemeMap["AUSM++"] = 3;
	schemeMap["HLLC"] = 4;

	pressFactorMap["THIVET"] = 0;
	pressFactorMap["VIGNERON"] = 1;
	/*pressFactorMap["THIVET 方法"] = 0;
	pressFactorMap["Vigneron 方法"] = 1;*/

	/*temperalMap["LU-SGS"] = 0;
	temperalMap["ADI"] = 1;*/
	temperalMap["LUSGS_NS_VOL"] = 0;
	temperalMap["ADI_NS"] = 1;

	/*visGradientMap["高斯法"] = 0;
	visGradientMap["有限差分法"] = 1;*/
	visGradientMap["VIS_VOL"] = 0;
	visGradientMap["VIS_AREA"] = 1;


	/*implicitVisMap["守恒变量导数"] = 0;
	implicitVisMap["最大谱半径线化方法 1"] = 1;
	implicitVisMap["最大谱半径线化方法 2"] = 2;*/
	implicitVisMap["VISCOUS JACOBIAN"] = 0;
	implicitVisMap["TYSINGER"] = 1;
	implicitVisMap["CHENBING"] = 2;


	objectNameMap["spinBoxNout"] = "noutLineEdit";
	objectNameMap["maSpinBox"] = "MaLineEdit";
	objectNameMap["pSpinBox"] = "PLineEdit";
	objectNameMap["tSpinBox"] = "TLineEdit";
	objectNameMap["aoaSpinBox"] = "AoALineEdit";
	objectNameMap["aobSpinBox"] = "AoBLineEdit";
}

void ParamSetDialog::initLayout()
{
	int row, col;
	stackedWidget = new QStackedWidget;
	QVector<QWidget*> pages;
	// NS求解器
	headQVLayout = new QVBoxLayout;

	//row = headUi.size();
	//for (int i = 0; i < row; ++i)
	//{
	//	QHBoxLayout* headQHLayout = new QHBoxLayout;
	//	col = headUi[i].size();
	//	headQHLayout->addWidget(headUi[i][0], 1);
	//	headQHLayout->addWidget(headUi[i][1], 3);
	//	headQVLayout->addLayout(headQHLayout);
	//}

	for (int i = 3; i < 5; ++i)
	{
		QVBoxLayout* tmpQVLayout = new QVBoxLayout;
		for (int j = 0; j < bodyUi[i].size(); ++j)
		{
			QHBoxLayout* bodyQHLayout = new QHBoxLayout;
			col = bodyUi[i][j].size();
			if (col == 2)
			{
				bodyQHLayout->addWidget(bodyUi[i][j][0], 1);
				bodyQHLayout->addWidget(bodyUi[i][j][1], 2);
			}
			else
			{
				bodyQHLayout->addWidget(bodyUi[i][j][0], 1);
				bodyQHLayout->addWidget(bodyUi[i][j][1], 1);
				bodyQHLayout->addWidget(bodyUi[i][j][2], 1);
			}
			tmpQVLayout->addLayout(bodyQHLayout);
		}
		CusWidget[i] = new QWidget;
		CusWidget[i]->setLayout(tmpQVLayout);

	}

	for (int i = 3; i < 5; ++i)
	{
		headQVLayout->addWidget(CusButtonVt[i]);
		headQVLayout->addWidget(CusWidget[i]);
		mp[CusButtonVt[i]] = i;
	}
	// 网格计算
	CalQHLayout = new QHBoxLayout;
	CalQHLayout->addWidget(Xscale, 1);
	CalQHLayout->addWidget(m_xscaleLineEdit, 2);

	//Edge
	EdgeQVLayout = new QVBoxLayout;
	for (int i = 0; i < 7; ++i)
	{
		QHBoxLayout* tmpQHLayout = new QHBoxLayout;
		tmpQHLayout->addWidget(EdgeUi[i][0], 1);
		tmpQHLayout->addWidget(EdgeUi[i][1], 3);
		EdgeQVLayout->addLayout(tmpQHLayout);
		EdgeQVLayout->setSpacing(0);
	}
	//pages.push_back(new QWidget());
	//pages.back()->setLayout(EdgeQVLayout);

	// PNS
	pnsQVLayout = new QVBoxLayout;
	for (int i = 0; i < 3; ++i)
	{
		QHBoxLayout* tmpQHLayout = new QHBoxLayout;
		tmpQHLayout->addWidget(pnsUi[i][0], 1);
		tmpQHLayout->addWidget(pnsUi[i][1], 3);
		pnsQVLayout->addLayout(tmpQHLayout);
	}

	//AirCategory
	aircateQVLayout = new QVBoxLayout;
	QHBoxLayout* tmpQHLayout_1 = new QHBoxLayout;
	tmpQHLayout_1->addWidget(radioHeat, 1);
	tmpQHLayout_1->addWidget(m_radioLineEdit, 3);
	aircateQVLayout->addLayout(tmpQHLayout_1);

	QHBoxLayout* tmpQHLayout_2 = new QHBoxLayout;
	tmpQHLayout_2->addWidget(gasConstant, 1);
	tmpQHLayout_2->addWidget(m_gasLineEdit, 3);
	aircateQVLayout->addLayout(tmpQHLayout_2);


	// Air 
	airQVLayout = new QVBoxLayout;
	for (int i = 0; i < 5; ++i)
	{
		QHBoxLayout* tmpQHLayout = new QHBoxLayout;
		tmpQHLayout->addWidget(airUi[i][0], 1);
		tmpQHLayout->addWidget(airUi[i][1], 2);
		airQVLayout->addLayout(tmpQHLayout);
	}
	//pages.push_back(new QWidget());
	//pages.back()->setLayout(airQVLayout);



	// Post
	postQVLayout = new QVBoxLayout;
	for (int i = 0; i < 4; ++i)
	{
		QHBoxLayout* tmpQHLayout = new QHBoxLayout;
		if (i == 0) {
			tmpQHLayout->addWidget(postUi[i][0], 1);
			tmpQHLayout->addWidget(postUi[i][1], 2);
		}
		else {
			tmpQHLayout->addWidget(postUi[i][0], 1);
			tmpQHLayout->addWidget(postUi[i][1], 1);
			tmpQHLayout->addWidget(postUi[i][2], 1);

		}
		postQVLayout->addLayout(tmpQHLayout);
	}

	// 添加滚动区域
	m_headWidget = new QWidget;
	m_headWidget->setLayout(headQVLayout);
	scrollAreaHead = new QScrollArea;
	scrollAreaHead->setWidgetResizable(true);
	scrollAreaHead->setWidget(m_headWidget);

	m_postWidget = new QWidget;
	//m_postWidget->setLayout(postQVLayout);
	scrollAreaPost = new QScrollArea;
	scrollAreaPost->setWidgetResizable(true);
	scrollAreaPost->setWidget(m_postWidget);


	// 隐藏布局
	QHBoxLayout* tmp = new QHBoxLayout;
	m_hideWidget = new QWidget;
	tmp->addWidget(m_AreaRefLineEdit);
	tmp->addWidget(m_LrefRefLineEdit);
	m_hideWidget->setLayout(tmp);
	m_hideWidget->setVisible(false);


	// 主布局
	m_mainLayout = new QVBoxLayout;
	QVector<QVBoxLayout*> layouts;

	layouts.push_back(new QVBoxLayout);
	layouts.back()->setSpacing(0);
	layouts.back()->addWidget(Callabel);
	layouts.back()->addLayout(CalQHLayout);
	layouts.back()->addStretch();

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnEdge);
	layouts.back()->addLayout(EdgeQVLayout);
	layouts.back()->addStretch();

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnPNS);
	layouts.back()->addLayout(pnsQVLayout);
	//layouts.back()->addWidget(m_trTableWidget);
	layouts.back()->addStretch();
	/*m_mainLayout->addWidget(btnPNS);
	m_mainLayout->addLayout(pnsQHLayout);*/

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnNS);
	layouts.back()->addWidget(scrollAreaHead);
	/*m_mainLayout->addWidget(btnNS);
	m_mainLayout->addWidget(scrollAreaHead);*/

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnPost);
	layouts.back()->addLayout(postQVLayout);
	layouts.back()->addStretch();
	//layouts.back()->addWidget(scrollAreaPost);
	/*m_mainLayout->addWidget(btnPost);
	m_mainLayout->addWidget(scrollAreaPost);*/

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnAir);
	layouts.back()->addLayout(airQVLayout);
	layouts.back()->addWidget(wallForceWidget);
	/*m_mainLayout->addWidget(btnAir);
	m_mainLayout->addLayout(airQVLayout);*/

	layouts.push_back(new QVBoxLayout);
	layouts.back()->addWidget(btnAirCate);
	layouts.back()->addLayout(aircateQVLayout);
	layouts.back()->addStretch();

	pages.resize(layouts.size());
	for (int i = 0; i < pages.size(); i++) {
		pages[i] = new QWidget;
		pages[i]->setLayout(layouts[i]);
		stackedWidget->addWidget(pages[i]);
	}
	//stackedWidget->addWidget(wallForceWidget);

	m_mainLayout->addWidget(stackedWidget);

	m_mainLayout->addWidget(m_hideWidget);

	this->setLayout(m_mainLayout);
}

void ParamSetDialog::loadTrTable()
{

}

void ParamSetDialog::changeView(int index)
{
	index = min(index, stackedWidget->count() - 1);
	index = max(0, index);
	stackedWidget->setCurrentIndex(index);
	current_page = index;

	//if(!checkParam()) 
	//	QMessageBox::information(nullptr, u8"提示", u8"导入文件成功", QMessageBox::Ok);

	qDebug() << current_page << endl;
}

void ParamSetDialog::updateDialog() {
	m_workspace = SignalInstance::getInstance()->getWorkSpace();
	QDir workspaceDir_head(m_workspace + "/head");
	QDir workspaceDir_body(m_workspace + "/body");
	if (!workspaceDir_head.exists()) { 
		//仅加载身部
		loadBodyConfig(m_workspace); 
	}else
	{
		//加载头部和身部
		loadBodyConfig(m_workspace);
		loadHeadConfig(m_workspace);
	}
}

void ParamSetDialog::loadBodyConfig(QString path)
{
	qDebug() << "loadBodyConfig()" << ' ' << "pos2" << '\n';
	path += "/body/input.dat";
	BodyPathChecker tempChecker;
	path = tempChecker.checkAndGetPath(path);
	qDebug() << "path=" << path;
	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString line;
		int indexLine = 0;
		while (!in.atEnd())
		{
			line = in.readLine();
			//解析OutPut, 输出控制
			if (line == "[Output]")
			{
				//连续读取四段更新
				QStringList list;

				// 信息输出步数
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Nout") break;
				}
				noutLineEdit->setText(list.at(1));

				// 是否计算气动力
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "WallForce") break;
				}
				if (list.at(1) == "ON") {
					wallForceRadioBtnON->setChecked(true);
					wallForceWidget->setVisible(true);
				}
				else
				{
					wallForceRadioBtnOFF->setChecked(true);
				}

				// 是否输出空间流场
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "cVolFieldOutput") break;
				}
				if (list.at(1) == "ON") cVolFieldOutputForceRadioBtnON->setChecked(true);
				else cVolFieldOutputForceRadioBtnOFF->setChecked(true);

				// 是否输出边界流场
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "cbdFieldOutput") break;
				}
				if (list.at(1) == "ON") cbdFieldRadioBtnON->setChecked(true);
				else cbdFieldRadioBtnOFF->setChecked(true);
			}
			else if (line == "[Farfield]")
			{
				QStringList list = line.split("=");

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Ma") break;
				}
				m_maLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "P") break;
				}
				m_pLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "T") break;
				}
				m_tLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "AoA") break;
				}
				alphaLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "AoB") break;
				}
				m_betaLineEdit->setText(list.at(1));
			}
			else if (line == "[BoundaryCondition]")
			{
				QStringList list;
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Tw") break;
				}
				if (list.at(1) == "adiabatic") m_twConditionComboBox->setCurrentIndex(1);
				else m_TwLineEdit->setText(list.at(1));
				qDebug() << "m_TwLineEdit=" << m_TwLineEdit->text();
			}
			else if (line == "[Geometry]")
			{
				QStringList list = line.split("=");

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Rlen") break;
				}
				rlenLineEdit->setText(list.at(1));
				qDebug() << "rlenLineEdit=" << rlenLineEdit->text();

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "XScale") break;
				}
				m_xscaleLineEdit->setText(list.at(1));
				qDebug() << "m_xscaleLineEdit=" << m_xscaleLineEdit->text();

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "AreaRef") break;
				}
				arearefLineEdit->setText(list.at(1));
				qDebug() << "arearefLineEdit=" << arearefLineEdit->text();

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "xmc") break;
				}
				m_xcLineEdit->setText(list.at(1));
				qDebug() << "m_xcLineEdit=" << m_xcLineEdit->text();

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "ymc") break;
				}
				m_ycLineEdit->setText(list.at(1));
				qDebug() << "m_ycLineEdit=" << m_ycLineEdit->text();

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "zmc") break;
				}
				m_zcLineEdit->setText(list.at(1));
				qDebug() << "m_zcLineEdit=" << m_zcLineEdit->text();
			}
			else if (line == "[Mesh]")
			{
				line = in.readLine();
				QStringList list = line.split("=");
				meshLineEdit->setText(list.at(1));
			}
			else if (line == "[Gas]")
			{
				line = in.readLine();
				QStringList list = line.split("=");
				nsLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "R0") break;
				}
				m_gasLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Gamma") break;
				}
				m_radioLineEdit->setText(list.at(1));
			}
			else if (line == "[SolverControl]")
			{
				QStringList list;
				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Scheme") break;
				}
				schemeComboBox->setCurrentIndex(schemeMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Limiter") break;
				}
				limiterComboBox->setCurrentIndex(limiterMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "PressFactor") break;
				}
				pressFactorCombBox->setCurrentIndex(pressFactorMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "Temperal") break;
				}
				temperalComboBox->setCurrentIndex(temperalMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "VisGradient") break;
				}
				visGradientComboBox->setCurrentIndex(visGradientMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "ImplicitVis") break;
				}
				implicitVisComboBox->setCurrentIndex(implicitVisMap[list.at(1)]);

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "StepCtrl") break;
				}
				stepCtrlLineEdit->setText(list.at(1));


				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "LinearEq") break;
				}
				linearEqLineEdit->setText(list.at(1));

				while (true)
				{
					line = in.readLine();
					list = line.split("=");
					if (list.at(0) == "GridGeneration") break;
				}
				gridGenerationLineEdit->setText(list.at(1));
			}
			indexLine++;
		}
	}
	//// loadDefaultBody();
}

void ParamSetDialog::loadHeadConfig(QString path)
{
	qDebug() << "loadHeadConfig()" << ' ' << "pos1" << "\n";
	path += "/head/headconfig.dat";

	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString line;
		while (!in.atEnd()) line = in.readLine();

		QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

		cInitComboBox->setCurrentIndex(parts[0].toInt() - 1);
		NStopLineEdit->setText(parts[1]);
		cflLineEdit->setText(parts[2]);
	}
}

void ParamSetDialog::saveHeadCalScaleConfig()
{
	//读headConfig.dat
	std::ifstream inHeadLoad2;
	inHeadLoad2.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	//修改指定行数内容
	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
	// qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines2.size(); ++i)
	{
		if (lines2[i].size() > 5 && lines2[i].size() < 35 && lines2[i][0] == '!' && lines2[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}

	qDebug() << "pos" << pos;
	size_t spacePos = lines2[pos + 3].find(' ');
	qDebug() << "spacePos" << spacePos;
	lines2[pos + 3].replace(0, spacePos, m_xscaleLineEdit->text().toLocal8Bit().toStdString());

	//写headConfig.dat
	std::ofstream outHeadLoad;
	outHeadLoad.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();
	qDebug() << "xscale" << m_xscaleLineEdit->text();

	// 同步Batch目录下的headconfig.dat
	syncBatchFiles("head/headconfig.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		size_t batchSpacePos = batchLines[pos + 3].find(' ');
		batchLines[pos + 3].replace(0, batchSpacePos, m_xscaleLineEdit->text().toLocal8Bit().toStdString());

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveBodyCalScaleConfig()
{
	//读input.dat
	std::ifstream inHeadLoad2;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad2.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	//修改指定行数内容
	std::stringstream ss2;
	string xscale = m_xscaleLineEdit->text().toLocal8Bit().toStdString();
	ss2 << "XScale=" << xscale;
	QStringList list;
	int pos = 21; // 确定行号
	for (int i = 0; i < lines2.size(); ++i)
	{
		list = QString::fromStdString(lines2[i]).split("=");
		if (list.at(0) == "XScale")
		{
			pos = i;
			break;
		}
	}
	lines2[pos] = ss2.str();

	//写input.dat
	std::ofstream outHeadLoad;
	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));

	outHeadLoad.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		std::stringstream batchSs2;
		batchSs2 << "XScale=" << xscale;
		batchLines[pos] = batchSs2.str();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

// paramSetDialog.cpp 只需要改这一个函数
void ParamSetDialog::saveHeadEdgeConfig()
{
	// 读headConfig.dat并修改当前文件（保留单文件修改逻辑）
	std::ifstream inHeadLoad2;
	inHeadLoad2.open(QString(m_workspace +
		"/head/headconfig.dat").toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();
	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
   // qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines2.size(); ++i)
	{
		if (lines2[i].size() > 5 && lines2[i].size() < 35 && lines2[i][0] == '!'
			&& lines2[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}
	lines2[pos + 1] = m_pLineEdit->text().toStdString() + " " + m_tLineEdit-> text().toStdString() + " " + m_maLineEdit->text().toStdString() + " " +
		alphaLineEdit->text().toStdString() + " " + m_betaLineEdit->text().toStdString()
		+ " ";
	if (m_TwLineEdit->text().toStdString() != "请输入壁温") {
		std::string tw = m_TwLineEdit->text().toStdString();
		if (tw == u8"adiabatic") tw = "-1";
		lines2[pos + 1] += tw;
	}
		
	std::ofstream outHeadLoad;
	outHeadLoad.open(QString(m_workspace +
		"/head/headconfig.dat").toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();
	// 同步Batch目录下的headconfig.dat（跳过5个参数的批量修改）
	syncBatchFiles("head/headconfig.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine))
			batchLines.push_back(batchLine);
		batchIn.close();
		if (m_TwLineEdit->text().toStdString() != "请输入壁温")
		{
			std::string tw = m_TwLineEdit->text().toStdString();
			if (tw == u8"adiabatic") tw = "-1";
			std::vector<std::string> nums;
			std::stringstream ssLine(batchLines[12]);
			std::string num;
			while (ssLine >> num) nums.push_back(num);
			if (nums.size() == 6) nums[5] = tw;
			else nums.push_back(tw);

			std::string newLine;
			for (size_t i = 0; i < nums.size(); ++i) {
				if (i > 0) newLine += " ";  // 数字间加空格
				newLine += nums[i];
			}
			batchLines[pos + 1] = newLine;
		}

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}
QString ParamSetDialog::GetMa() {
	return m_maLineEdit->text();
}
QString ParamSetDialog::GetAOA() {
	return alphaLineEdit->text();
}
QString ParamSetDialog::GetAOB() {
	return m_betaLineEdit->text();
}
void ParamSetDialog::saveBodyEdgeConfig()
{
	string ma = m_maLineEdit->text().toStdString();
	string P = m_pLineEdit->text().toStdString();
	string temp = m_tLineEdit->text().toStdString();
	string aoa = alphaLineEdit->text().toStdString();
	string aob = m_betaLineEdit->text().toStdString();
	string tw = m_TwLineEdit->text().toStdString();

	if (tw == "请输入壁温") tw = "";

	// 读input.dat并修改当前文件（保留单文件修改逻辑）
	std::ifstream inHeadLoad2;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad2.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	std::stringstream sss, sss1, sss2, sss3, sss4, sss5;
	sss << "Ma=" << ma;
	sss1 << "P=" << P;
	sss2 << "T=" << temp;
	sss3 << "AoA=" << aoa;
	sss4 << "AoB=" << aob;
	sss5 << "Tw=" << tw;

	QStringList list;
	int posMa = 9; // 确定行号
	int posP = 10; // 确定行号
	int posT = 11; // 确定行号
	int posAoA = 12; // 确定行号
	int posAoB = 13; // 确定行号
	int posTw = 17; // 确定行号
	for (int i = 0; i < lines2.size(); ++i)
	{
		list = QString::fromStdString(lines2[i]).split("=");
		if (list.at(0) == "Ma") posMa = i;
		if (list.at(0) == "P") posP = i;
		if (list.at(0) == "T") posT = i;
		if (list.at(0) == "AoA") posAoA = i;
		if (list.at(0) == "AoB") posAoB = i;
		if (list.at(0) == "Tw") posTw = i;
	}

	lines2[posMa] = sss.str();
	lines2[posP] = sss1.str();
	lines2[posT] = sss2.str();
	lines2[posAoA] = sss3.str();
	lines2[posAoB] = sss4.str();
	lines2[posTw] = sss5.str();

	std::ofstream outHeadLoad;
	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	outHeadLoad.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录（跳过ma、p、t、AOA、AOB的修改，仅保留Tw）
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		// 仅保留Tw的批量修改，注释掉其他5个参数
		// batchLines[9] = sss.str();    // 跳过Ma
		// batchLines[10] = sss1.str();  // 跳过P
		// batchLines[11] = sss2.str();  // 跳过T
		// batchLines[12] = sss3.str();  // 跳过AoA
		// batchLines[13] = sss4.str();  // 跳过AoB
		batchLines[posTw] = sss5.str();   // 保留Tw

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveHeadNSConfig()
{
	std::ifstream inHeadLoad2;
	inHeadLoad2.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
	// qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines2.size(); ++i)
	{
		if (lines2[i].size() > 5 && lines2[i].size() < 35 && lines2[i][0] == '!' && lines2[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}

	//修改指定行数内容
	size_t spacePos = lines2[14].find(' ');
	qDebug() << "spacePos" << spacePos;
	lines2[pos + 7] = std::to_string(cInitComboBox->currentIndex() + 1) + " " + NStopLineEdit->text().toStdString() + " " + cflLineEdit->text().toStdString();

	//写headConfig.dat
	std::ofstream outHeadLoad;
	outHeadLoad.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的headconfig.dat
	syncBatchFiles("head/headconfig.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();
		// 不修改批量的计算标记（保持第一个变量不变）
		if (pos + 7 < batchLines.size()) {  // 安全检查：避免越界访问
			std::string originalLine = batchLines[pos + 7];
			std::istringstream iss(originalLine);
			std::string firstParam;  // 存储原有第一个变量
			iss >> firstParam;       // 提取第一个字段（保持不变）

			// 拼接新行：原有第一个变量 + 新的两个参数
			batchLines[pos + 7] = firstParam + " " +
				NStopLineEdit->text().toStdString() + " " +
				cflLineEdit->text().toStdString();
		}
		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveBodyPNSConfig()
{
	//读入参数保存
	std::ifstream inHeadLoad;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inHeadLoad, line)) lines.push_back(line);
	inHeadLoad.close();

	// 修改input.dat
	std::stringstream sss[9];
	QString str[9];

	str[0] = "Scheme cant find";
	for (auto it = schemeMap.begin(); it != schemeMap.end(); it++) {

		if (it.value() == schemeComboBox->currentIndex()) {
			str[0] = it.key();
			break;
		}
	}
	sss[0] << "Scheme=" << str[0].toStdString();

	str[1] = "limiter cant find";
	for (auto it = limiterMap.begin(); it != limiterMap.end(); it++) {
		if (it.value() == limiterComboBox->currentIndex()) {
			str[1] = it.key();
			break;
		}
	}
	sss[1] << "Limiter=" << str[1].toStdString();

	str[2] = "PressFactor cant find";
	for (auto it = pressFactorMap.begin(); it != pressFactorMap.end(); it++) {
		if (it.value() == pressFactorCombBox->currentIndex()) {
			str[2] = it.key();
			break;
		}
	}
	sss[2] << "PressFactor=" << str[2].toStdString();

	str[3] = "Temperal cant find";
	for (auto it = temperalMap.begin(); it != temperalMap.end(); it++) {
		if (it.value() == temperalComboBox->currentIndex()) {
			str[3] = it.key();
			break;
		}
	}
	sss[3] << "Temperal=" << str[3].toStdString();

	str[4] = "VisGradient cant find";

	for (auto it = visGradientMap.begin(); it != visGradientMap.end(); it++) {
		if (it.value() == visGradientComboBox->currentIndex()) {
			str[4] = it.key();
			break;
		}
	}
	sss[4] << "VisGradient=" << str[4].toStdString();

	str[5] = "ImplicitVis cant find";
	for (auto it = implicitVisMap.begin(); it != implicitVisMap.end(); it++) {
		if (it.value() == implicitVisComboBox->currentIndex()) {
			str[5] = it.key();
			break;
		}
	}
	sss[5] << "ImplicitVis=" << str[5].toStdString();


	QStringList list;
	int posScheme = 41; // 确定行号
	for (int i = 0; i < lines.size(); ++i)
	{
		list = QString::fromStdString(lines[i]).split("=");
		if (list.at(0) == "Scheme")
		{
			posScheme = i;
			break;
		}
	}
	for (int i = 0; i < 6; ++i) lines[posScheme + i] = sss[i].str();

	std::ofstream outBodyInput;

	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	outBodyInput.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l : lines) outBodyInput << l << std::endl;
	outBodyInput.close();

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		for (int i = 0; i < 6; ++i) batchLines[posScheme + i] = sss[i].str();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveBodyOutConfig()
{
	//读input.dat
	std::ifstream inHeadLoad2;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad2.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	//修改指定行数内容
	QStringList list;
	int posNout = 3; // 确定行号
	int posWallForce = 4; // 确定行号
	int poscVolFieldOutput = 5; // 确定行号
	int poscbdFieldOutput = 6; // 确定行号
	for (int i = 0; i < lines2.size(); ++i)
	{
		list = QString::fromStdString(lines2[i]).split("=");
		if (list.at(0) == "Nout") posNout = i;
		if (list.at(0) == "WallForce") posWallForce = i;
		if (list.at(0) == "cVolFieldOutput") poscVolFieldOutput = i;
		if (list.at(0) == "cbdFieldOutput") poscbdFieldOutput = i;
	}

	lines2[posNout] = "Nout=" + noutLineEdit->text().toStdString();

	if (wallForceRadioBtnON->isChecked()) lines2[posWallForce] = "WallForce=ON";
	else lines2[posWallForce] = "WallForce=OFF";

	if (cVolFieldOutputForceRadioBtnON->isChecked()) lines2[poscVolFieldOutput] = "cVolFieldOutput=ON";
	else lines2[poscVolFieldOutput] = "cVolFieldOutput=OFF";

	if (cbdFieldRadioBtnON->isChecked()) lines2[poscbdFieldOutput] = "cbdFieldOutput=ON";
	else lines2[poscbdFieldOutput] = "cbdFieldOutput=OFF";

	//写input.dat
	std::ofstream outHeadLoad;

	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	outHeadLoad.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		batchLines[posNout] = "Nout=" + noutLineEdit->text().toStdString();
		batchLines[posWallForce] = wallForceRadioBtnON->isChecked() ? "WallForce=ON" : "WallForce=OFF";
		batchLines[poscVolFieldOutput] = cVolFieldOutputForceRadioBtnON->isChecked() ? "cVolFieldOutput=ON" : "cVolFieldOutput=OFF";
		batchLines[poscbdFieldOutput] = cbdFieldRadioBtnON->isChecked() ? "cbdFieldOutput=ON" : "cbdFieldOutput=OFF";

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});

}

void ParamSetDialog::saveHeadAirConfig()
{
	//读headConfig.dat
	std::ifstream inHeadLoad2;
	inHeadLoad2.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
	// qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines2.size(); ++i)
	{
		if (lines2[i].size() > 5 && lines2[i].size() < 35 && lines2[i][0] == '!' && lines2[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}

	//修改指定行数内容
	size_t spacePos = lines2[pos + 3].find(' ');
	qDebug() << "spacePos" << spacePos;
	lines2[pos + 3] = lines2[pos + 3].substr(0, spacePos + 1) + " " + arearefLineEdit->text().toStdString() + " " + rlenLineEdit->text().toStdString() + " " + m_xcLineEdit->text().toStdString() + " " + m_ycLineEdit->text().toStdString() + " " + m_zcLineEdit->text().toStdString();

	//写headConfig.dat
	std::ofstream outHeadLoad;
	outHeadLoad.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的headconfig.dat
	syncBatchFiles("head/headconfig.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		size_t batchSpacePos = batchLines[pos + 3].find(' ');
		batchLines[pos + 3] = batchLines[pos + 3].substr(0, batchSpacePos + 1) + " " + arearefLineEdit->text().toStdString() + " " + rlenLineEdit->text().toStdString() + " " + m_xcLineEdit->text().toStdString() + " " + m_ycLineEdit->text().toStdString() + " " + m_zcLineEdit->text().toStdString();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveBodyAirConfig()
{
	string area = arearefLineEdit->text().toStdString();
	string len = rlenLineEdit->text().toStdString();
	string x = m_xcLineEdit->text().toStdString();
	string y = m_ycLineEdit->text().toStdString();
	string z = m_zcLineEdit->text().toStdString();

	//读input.dat
	std::ifstream inHeadLoad2;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad2.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	//修改指定行数内容
	std::stringstream sss1;
	sss1 << "Rlen=" << len;
	std::stringstream sss2;
	sss2 << "AreaRef=" << area;
	std::stringstream sss3;
	sss3 << "xmc=" << x;
	std::stringstream sss4;
	sss4 << "ymc=" << y;
	std::stringstream sss5;
	sss5 << "zmc=" << z;

	QStringList list;
	int posRlen = 20; // 确定行号
	int posAreaRef = 23; // 确定行号
	int posxmc = 24; // 确定行号
	int posymc = 25; // 确定行号
	int poszmc = 25; // 确定行号
	for (int i = 0; i < lines2.size(); ++i)
	{
		list = QString::fromStdString(lines2[i]).split("=");
		if (list.at(0) == "Rlen") posRlen = i;
		if (list.at(0) == "AreaRef") posAreaRef = i;
		if (list.at(0) == "xmc") posxmc = i;
		if (list.at(0) == "ymc") posymc = i;
		if (list.at(0) == "zmc") poszmc = i;
	}

	lines2[posRlen] = sss1.str();
	lines2[posAreaRef] = sss2.str();
	lines2[posxmc] = sss3.str();
	lines2[posymc] = sss4.str();
	lines2[poszmc] = sss5.str();

	//写input.dat
	std::ofstream outHeadLoad;

	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	outHeadLoad.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		std::stringstream batchSss1;
		batchSss1 << "Rlen=" << len;
		std::stringstream batchSss2;
		batchSss2 << "AreaRef=" << area;
		std::stringstream batchSss3;
		batchSss3 << "xmc=" << x;
		std::stringstream batchSss4;
		batchSss4 << "ymc=" << y;
		std::stringstream batchSss5;
		batchSss5 << "zmc=" << z;

		batchLines[posRlen] = batchSss1.str();
		batchLines[posAreaRef] = batchSss2.str();
		batchLines[posxmc] = batchSss3.str();
		batchLines[posymc] = batchSss4.str();
		batchLines[poszmc] = batchSss5.str();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveHeadAirCateConfig()
{
	//读headConfig.dat
	std::ifstream inHeadLoad2;
	inHeadLoad2.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	int pos = 11; //行号 "!P"这一行对应的行号，以此为标准
	// qDebug() << QString::fromStdString(lines2[13]) << '\n';
	for (int i = 0; i < lines2.size(); ++i)
	{
		if (lines2[i].size() > 5 && lines2[i].size() < 35 && lines2[i][0] == '!' && lines2[i][1] == 'P')
		{
			pos = i;
			break;
		}
	}

	//修改指定行数内容
	lines2[pos + 5] = m_gasLineEdit->text().toStdString() + " " + m_radioLineEdit->text().toStdString();

	//写headConfig.dat
	std::ofstream outHeadLoad;
	outHeadLoad.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的headconfig.dat
	syncBatchFiles("head/headconfig.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		batchLines[pos + 5] = m_gasLineEdit->text().toStdString() + " " + m_radioLineEdit->text().toStdString();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::saveBodyAirCateConfig()
{
	string radio = m_radioLineEdit->text().toStdString();
	string gas = m_gasLineEdit->text().toStdString();

	//读input.dat
	std::ifstream inHeadLoad2;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad2.open(truePath.toLocal8Bit().toStdString());
	std::vector<std::string> lines2;
	std::string line2;
	while (std::getline(inHeadLoad2, line2)) lines2.push_back(line2);
	inHeadLoad2.close();

	//修改指定行数内容
	std::stringstream sss1;
	sss1 << "R0=" << gas;
	std::stringstream sss2;
	sss2 << "Gamma=" << radio;
	QStringList list;
	int posR0 = 37; // 确定行号
	int posGamma = 38; // 确定行号
	for (int i = 0; i < lines2.size(); ++i)
	{
		list = QString::fromStdString(lines2[i]).split("=");
		if (list.at(0) == "R0") posR0 = i;
		if (list.at(0) == "Gamma") posGamma = i;
	}
	lines2[posR0] = sss1.str();
	lines2[posGamma] = sss2.str();

	//写input.dat
	std::ofstream outHeadLoad;

	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	outHeadLoad.open(truePath.toLocal8Bit().toStdString());
	for (const auto& l2 : lines2) outHeadLoad << l2 << std::endl;
	outHeadLoad.close();

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		std::stringstream batchSss1;
		batchSss1 << "R0=" << gas;
		std::stringstream batchSss2;
		batchSss2 << "Gamma=" << radio;
		batchLines[posR0] = batchSss1.str();
		batchLines[posGamma] = batchSss2.str();

		std::ofstream batchOut(targetPath.toLocal8Bit().toStdString());
		for (const auto& l : batchLines) batchOut << l << std::endl;
		batchOut.close();
	});
}

void ParamSetDialog::redictHeadParameter()
{
	//saveHeadConfig();

	qDebug() << "redictHeadParameter()" << '\n';
	outPutLogger->exeProgramHeader(SignalInstance::getInstance()->m_workspace);
}

// 设置成空字符串就行了
void ParamSetDialog::onTwConditionChanged(int index)
{
	if (index == 0)
	{
		m_TwLineEdit->setPlaceholderText(u8"请输入壁温");
		// 改下面这一行
		m_TwLineEdit->setText(u8"");
		m_TwLineEdit->setEnabled(true);
	}
	else
	{
		m_TwLineEdit->setPlaceholderText(u8"adiabatic ");
		m_TwLineEdit->setText(u8"adiabatic");
		m_TwLineEdit->setEnabled(false);
	}
}

void ParamSetDialog::onCheckLineEdit()
{
	if (!arearefLineEdit->text().trimmed().isEmpty())
	{
		QString styleSheet = arearefLineEdit->styleSheet();
		styleSheet += "border:1px solid black;";
		arearefLineEdit->setStyleSheet(styleSheet);
	}
}

void ParamSetDialog::saveTrTable() {
	//读入参数保存
	std::ifstream inHeadLoad;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	inHeadLoad.open(truePath.toLocal8Bit().toStdString());
	if (!inHeadLoad.is_open()) return;
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inHeadLoad, line)) lines.push_back(line);
	inHeadLoad.close();

	vector<string> trInfos;
	ifstream trLoad;
	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/trData.dat"));
	trLoad.open(truePath.toLocal8Bit().toStdString());
	while (std::getline(trLoad, line)) {
		if (line == "#Section Xstep0 xstepmin min1  max kstep  ncstep") {
			do {
				if (line == "") break;
				trInfos.push_back(line);
			} while (std::getline(trLoad, line));

		}

	}
	trLoad.close();


	truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
	FILE* output = fopen(truePath.toLocal8Bit().toStdString().c_str(), "w");
	if (output == NULL) return;
	int p = 0;
	while (p < lines.size()) {
		string str = lines[p];
		if (str == "[StepCtrl]") {
			fprintf(output, "%s\n", str.c_str());
			p++;

			int count = trInfos.size();
			for (auto it : trInfos) {
				fprintf(output, "%s\n", it.c_str());
			}
			while (p < lines.size() && lines[p] != "") p++;
		}
		else {
			fprintf(output, "%s\n", str.c_str());
			p++;
		}

	}
	fclose(output);

	// 同步Batch目录下的body/input.dat
	syncBatchFiles("body/input.dat", [&](const QString& targetPath) {
		std::ifstream batchIn(targetPath.toLocal8Bit().toStdString());
		std::vector<std::string> batchLines;
		std::string batchLine;
		while (std::getline(batchIn, batchLine)) batchLines.push_back(batchLine);
		batchIn.close();

		FILE* batchOutput = fopen(targetPath.toLocal8Bit().toStdString().c_str(), "w");
		if (batchOutput == NULL) return;
		int p = 0;
		while (p < batchLines.size()) {
			string str = batchLines[p];
			if (str == "[StepCtrl]") {
				fprintf(batchOutput, "%s\n", str.c_str());
				p++;

				int count = trInfos.size();
				for (auto it : trInfos) {
					fprintf(batchOutput, "%s\n", it.c_str());
				}
				while (p < batchLines.size() && batchLines[p] != "") p++;
			}
			else {
				fprintf(batchOutput, "%s\n", str.c_str());
				p++;
			}
		}
		fclose(batchOutput);
	});
}

void ParamSetDialog::slotUpdateSect() {

}

void ParamSetDialog::redictBodyParameter()
{
	qDebug() << "redictBodyParameter()" << '\n';
	emit SignalInstance::getInstance()->signalColorLine();
	outPutLogger->exeProgramBody(SignalInstance::getInstance()->m_workspace);

}

void ParamSetDialog::WidgetExpandOrCollapase()
{
	CusButton* button = qobject_cast<CusButton*>(sender());
	int idx = mp[button];
	if (!CusWidget[idx]->isVisible())
	{
		button->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
		CusWidget[idx]->setVisible(true);
	}
	else
	{
		button->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
		CusWidget[idx]->setVisible(false);
	}
	update();
}

void ParamSetDialog::slotSaveParam()
{
	QDir workspaceDir(m_workspace + "/head");
	QFile file1(workspaceDir.path() + "/headconfig.dat");

	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(m_workspace + "/body/input.dat");
	QFile file2(truePath);

	if (file1.exists())
	{
		qDebug() << "ns+pns 保存" << endl;
		switch (current_page) {
		case 0:
			saveHeadCalScaleConfig();
			saveBodyCalScaleConfig();
			break;
		case 1:
			qDebug() << "case:" << 1 << '\n';
			saveHeadEdgeConfig();
			saveBodyEdgeConfig();
			break;
		case 2:
			saveHeadNSConfig();
			break;
		case 3:
			//saveHeadPNSConfig();
qDebug() << "case:" << 3 << '\n';
saveBodyPNSConfig();
//saveBodyConfig();
break;
		case 4:
			qDebug() << "case:" << 4 << '\n';
			saveBodyOutConfig();
			break;
		case 5:
			saveHeadAirConfig();
			saveBodyAirConfig();
			break;
		case 6:
			saveHeadAirCateConfig();
			saveBodyAirCateConfig();
			break;
		}


	}
	else if (file2.exists())
	{
	qDebug() << "pns 保存" << endl;
	switch (current_page) {
	case 0:
		saveBodyCalScaleConfig();
		break;
	case 1:
		qDebug() << "case:" << 1 << '\n';
		saveBodyEdgeConfig();
		break;
	case 2:
		break;
	case 3:
		//saveHeadPNSConfig();
		qDebug() << "case:" << 3 << '\n';
		saveBodyPNSConfig();
		//saveBodyConfig();
		break;
	case 4:
		qDebug() << "case:" << 4 << '\n';
		saveBodyOutConfig();
		break;
	case 5:
		saveBodyAirConfig();
		break;
	case 6:
		saveBodyAirCateConfig();
		break;
	}
	}

	qDebug() << "current page is:" << current_page;
}

bool ParamSetDialog::getRestartFlag()
{
	return cInitComboBox->currentIndex();
}

void ParamSetDialog::initConnect()
{
	// 工作区
	connect(SignalInstance::getInstance(), SIGNAL(signalRedictHeadParameter()), this, SLOT(redictHeadParameter()));
	connect(m_twConditionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTwConditionChanged(int)));
	connect(SignalInstance::getInstance(), SIGNAL(saveTrTable()), this, SLOT(saveTrTable()));
	// 身部参数展开
	for (int i = 0; i < 6; ++i) connect(CusButtonVt[i], SIGNAL(clicked()), this, SLOT(WidgetExpandOrCollapase()));

	// 工作区
	connect(arearefLineEdit, SIGNAL(returnPressed()), this, SLOT(onCheckLineEdit()));
	connect(SignalInstance::getInstance(), SIGNAL(signalRedictBodyParameter()), this, SLOT(redictBodyParameter()));

	//保存
	connect(m_xscaleLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);

	connect(m_pLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_tLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_maLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(alphaLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_betaLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_twConditionComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_TwLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);

	connect(cInitComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(NStopLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(cflLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);

	connect(schemeComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(limiterComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(pressFactorCombBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(temperalComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(visGradientComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);
	connect(implicitVisComboBox, &QComboBox::currentTextChanged, this, &ParamSetDialog::slotSaveParam);

	connect(noutLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(cVolFieldOutputForceRadioBtnON, &QRadioButton::clicked, this, &ParamSetDialog::slotSaveParam);
	connect(cVolFieldOutputForceRadioBtnOFF, &QRadioButton::clicked, this, &ParamSetDialog::slotSaveParam);
	connect(cbdFieldRadioBtnON, &QRadioButton::clicked, this, &ParamSetDialog::slotSaveParam);
	connect(cbdFieldRadioBtnOFF, &QRadioButton::clicked, this, &ParamSetDialog::slotSaveParam);
	connect(wallForceRadioBtnON, &QRadioButton::clicked, this, [=](){

		slotSaveParam();
		emit SignalInstance::getInstance()->signalChangeAirParam(true);
	});
	connect(wallForceRadioBtnOFF, &QRadioButton::clicked, this, [=]() {

		slotSaveParam();
		emit SignalInstance::getInstance()->signalChangeAirParam(false);
	});

	connect(arearefLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(rlenLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_xcLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_ycLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_zcLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);

	connect(m_radioLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
	connect(m_gasLineEdit, &QLineEdit::textChanged, this, &ParamSetDialog::slotSaveParam);
}

void ParamSetDialog::loadStyleSheet(QString sheetName)
{
	QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//以只读的方式读取文件
	if (file.isOpen()) {
		setStyleSheet("");
		//将样式表全部读取，保存到字符串中
		QString qsstyleSheet = QLatin1String(file.readAll());

		setStyleSheet(qsstyleSheet);
		//qDebug() << "qsstyleSheet:" << qsstyleSheet;
	}
	file.close();
}

void ParamSetDialog::initStyleSheet()
{
	QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton1.css";
	QString sheet = FIlEINSTANCE->readFile(fileName);
	//m_btnOutput->setStyleSheet(sheet);
	m_btnFarfield->setStyleSheet(sheet);
	m_btnBoundaryCondition->setStyleSheet(sheet);
	m_btnGeometry->setStyleSheet(sheet);
	m_btnMesh->setStyleSheet(sheet);
	m_btnAdvanced->setStyleSheet(sheet);
	m_btnGeometry->setStyleSheet(sheet);
	m_btnSolverControl->setStyleSheet(sheet);
	m_btnSolverControl1->setStyleSheet(sheet);
	m_btnGas->setStyleSheet(sheet);
}

bool ParamSetDialog::returnWallForcestatus()
{
	return wallForceRadioBtnON->isChecked();
}

bool ParamSetDialog::checkParam()
{
	QString text = noutLineEdit->text();
	bool ok;
	int intValue = text.toInt(&ok);
	if (!ok) return ok;

	if (!wallForceRadioBtnON->isChecked() && !wallForceRadioBtnOFF->isChecked()) return false;
	else if (wallForceRadioBtnON->isChecked() && wallForceRadioBtnOFF->isChecked()) return false;

	if (!cVolFieldOutputForceRadioBtnON->isChecked() && !cVolFieldOutputForceRadioBtnOFF->isChecked()) return false;
	else if (cVolFieldOutputForceRadioBtnON->isChecked() && cVolFieldOutputForceRadioBtnOFF->isChecked()) return false;

	if (!cbdFieldRadioBtnON->isChecked() && !cbdFieldRadioBtnOFF->isChecked()) return false;
	else if (cbdFieldRadioBtnON->isChecked() && cbdFieldRadioBtnOFF->isChecked()) return false;

	text = m_maLineEdit->text();
	double doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	text = m_pLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	text = m_tLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	text = alphaLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;

	text = m_betaLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;

	if (m_TwLineEdit->isEnabled())
	{
		text = m_TwLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;
	}

	text = m_xscaleLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	if (wallForceRadioBtnON->isChecked())
	{
		text = rlenLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;

		text = arearefLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;

		text = m_xcLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;

		text = m_ycLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;

		text = m_zcLineEdit->text();
		doubleValue = text.toDouble(&ok);
		if (!ok) return ok;
		if (doubleValue <= 0.0) return false;
	}

	text = m_gasLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	text = m_radioLineEdit->text();
	doubleValue = text.toDouble(&ok);
	if (!ok) return ok;
	if (doubleValue <= 0.0) return false;

	if (schemeComboBox->currentIndex() == -1) return false;
	if (limiterComboBox->currentIndex() == -1) return false;
	if (pressFactorCombBox->currentIndex() == -1) return false;
	if (temperalComboBox->currentIndex() == -1) return false;
	if (visGradientComboBox->currentIndex() == -1) return false;
	if (implicitVisComboBox->currentIndex() == -1) return false;

	if (cInitComboBox->currentIndex() == -1) return false;

	text = NStopLineEdit->text();
	intValue = text.toInt(&ok);
	if (!ok) return ok;
	if (intValue < 0) return false;

	text = cflLineEdit->text();
	intValue = text.toInt(&ok);
	if (!ok) return ok;
	if (intValue < 0) return false;

	return true;
}

// 辅助函数：同步修改Batch目录下的对应文件
void ParamSetDialog::syncBatchFiles(const QString& relativeFilePath, std::function<void(const QString&)> modifyFunc) {
	// 获取工作区和Batch目录路径
	QString workspacePath = SignalInstance::getInstance()->getWorkSpace();
	QString batchDirPath = workspacePath + "/Batch";
	
	QDir batchDir(batchDirPath);

	// 若Batch目录不存在，直接返回
	if (!batchDir.exists()) {
		return;
	}

	// 遍历Batch目录下的所有子目录（每个子目录对应一个算例）
	foreach(const QString& subDirName, batchDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		QString subDirPath = batchDirPath + "/" + subDirName;

		// 拼接子目录中对应文件的完整路径（如body/input.dat）
		QString targetFilePath = subDirPath + "/" + relativeFilePath;

		BodyPathChecker tempChecker;
		targetFilePath = tempChecker.checkAndGetPath(targetFilePath);
		// 若目标文件存在，则执行修改函数
		if (QFile::exists(targetFilePath)) {
			modifyFunc(targetFilePath);
		}
	}
}
