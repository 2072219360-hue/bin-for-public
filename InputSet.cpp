#include "InputSet.h"
#include "huangmainwindow.h"
#include "OutPutLogger.h"
#include "SignalInstance.h"
#include <QGridLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QDebug>
#include <QCoreApplication>
#include <QFrame>
#include "TrTableShow.h"
using platforms::sspns::reveal::TrTableShow;
using platforms::sspns::reveal::OutPutLogger;
extern OutPutLogger* outPutLogger;
extern QString appComDir;
extern TrTableShow* tableShow;
extern statusSubject* myStatus;
namespace platforms
{
	namespace sspns {

		namespace reveal {
			InputSet::InputSet(QWidget * parent)
			{
				initUi();
				initMap();
			}

			InputSet::~InputSet()
			{
			}

			void InputSet::initUi()
			{
				//创建横线
				//QFrame* hframe = new QFrame(this);
				//hframe->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				//QFrame* hframe1 = new QFrame(this);
				//hframe1->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe1->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				//QFrame* hframe2 = new QFrame(this);
				//hframe2->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe2->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				//QFrame* hframe3 = new QFrame(this);
				//hframe3->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe3->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				//QFrame* hframe4 = new QFrame();
				//hframe4->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe4->setStyleSheet("QFram e{background:lightblue;min-height:0.1px}");
				//QFrame* hframe5 = new QFrame;
				//hframe5->setFrameShape(QFrame::VLine);      // 设置水平方向
				//hframe5->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				
				initOutput();
				//m_mainLayout->addWidget(hframe);

				initFarfieldUi();
				//m_mainLayout->addWidget(hframe1);
				//initMeshUi();
				initBndryConditionUi();
				//m_mainLayout->addWidget(hframe2);

				initGeometryUi();
				//m_mainLayout->addWidget(hframe3);

				this->setLayout(m_mainLayout);
				this->loadStyleSheet("InputSet2");
				initStyleSheet();


				connect(m_btnOutput, &CusButton::clicked, [this](bool) {
					if (!outputWidget->isVisible())
					{
						m_btnOutput->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						outputWidget->setVisible(true);
					}
					else
					{
						m_btnOutput->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						outputWidget->setVisible(false);
					}
					update();
				});
				connect(m_btnFarfield, &CusButton::clicked, [this](bool) {
					if (!farfieldWidget->isVisible())
					{
						m_btnFarfield->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						farfieldWidget->setVisible(true);
					}
					else
					{
						m_btnFarfield->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						farfieldWidget->setVisible(false);
					}
				});
				connect(m_btnBoundaryCondition, &CusButton::clicked, [this](bool) {

					if (!boundaryConditionWidget->isVisible())
					{
						m_btnBoundaryCondition->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						boundaryConditionWidget->setVisible(true);
					}
					else
					{
						m_btnBoundaryCondition->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						boundaryConditionWidget->setVisible(false);
					}
				});
				connect(m_btnGeometry, &CusButton::clicked, [this](bool) {
					if (!geometryWidget->isVisible())
					{
						m_btnGeometry->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						geometryWidget->setVisible(true);
					}
					else
					{
						m_btnGeometry->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						geometryWidget->setVisible(false);
					}
				});
				connect(m_btnMesh, &CusButton::clicked, [this](bool) {
					if (!meshWidget->isVisible())
					{
						m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						meshWidget->setVisible(true);
					}
					else
					{
						m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						meshWidget->setVisible(false);
					}
				});
				connect(m_btnGas, &CusButton::clicked, [this](bool) {
					if (!gasWidget->isVisible())
					{
						m_btnGas->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						gasWidget->setVisible(true);
					}
					else
					{
						m_btnGas->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						gasWidget->setVisible(false);
					}
					update();
				});
				connect(m_btnSolverControl, &CusButton::clicked, [this](bool) {
					if (!solverControlWidget->isVisible())
					{
						m_btnSolverControl->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						solverControlWidget->setVisible(true);
						update();
					}
					else
					{
						m_btnSolverControl->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						solverControlWidget->setVisible(false);
						update();
					}
				});
				connect(m_btnSolverControl1, &CusButton::clicked, [this](bool) {
					if (!solverControlWidget1->isVisible())
					{
						m_btnSolverControl1->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						solverControlWidget1->setVisible(true);
						update();
					}
					else
					{
						m_btnSolverControl1->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						solverControlWidget1->setVisible(false);
						update();
					}
				});
				connect(m_btnAdvanced, &CusButton::clicked, [this](bool) {
					if (!advancedWidget->isVisible())
					{
						m_btnAdvanced->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
						advancedWidget->setVisible(true);
						update();
					}
					else
					{
						m_btnAdvanced->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));
						advancedWidget->setVisible(false);
					}
				});


				connect(SignalInstance::getInstance(), SIGNAL(signalBodyParameter(QString)), this, SLOT(loadBodyParameter(QString)));
				connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(loadBodyParameter(QString)));
				connect(SignalInstance::getInstance(), SIGNAL(signalLoadDefaultParameter()), this, SLOT(loadDefaultParameter()));
				connect(wallForceRadioBtnON, SIGNAL(clicked(bool)), this, SLOT(onWallStateChanged()));
				connect(wallForceRadioBtnOFF, SIGNAL(clicked(bool)), this, SLOT(onWallStateChanged()));
				connect(arearefSpinBox, SIGNAL(returnPressed()), this, SLOT(onCheckLineEdit()));
				connect(SignalInstance::getInstance(), SIGNAL(signalRedictBodyParameter()), this, SLOT(redictBodyParameter()));
			}

			void InputSet::initOutput()
			{
				m_mainLayout = new QHBoxLayout(this);
				m_btnOutput = new CusButton;
				m_btnOutput->setFixedWidth(350);
				m_btnOutput->SetTextLabel(u8"Output属性设置");
				m_btnOutput->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				QFrame* hframe5 = new QFrame;
				hframe5->setFrameShape(QFrame::VLine);      // 设置水平方向
				hframe5->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
				m_mainLayout->addWidget(hframe5);
				//m_btnFarfield = new CusButton;
				outputWidget = new QWidget;
				QWidget*Farfield = new QWidget;
				QGridLayout*outputLayout = new QGridLayout;
				nout = new QLabel(u8"信息输出步数");// = 100
				spinBoxNout = new QSpinBox();
				spinBoxNout->setRange(0, 1000);
				spinBoxNout->setValue(100);
				spinBoxNout->setObjectName("spinBoxNout");



				cVolFieldOutputForceRadioBtnON = new QRadioButton("ON");
				cVolFieldOutputForceRadioBtnON->setObjectName("cVolFieldOutputForceRadioBtnON");

				cVolFieldOutputForceRadioBtnOFF = new QRadioButton("OFF");
				cVolFieldOutputForceRadioBtnOFF->setObjectName("cVolFieldOutputForceRadioBtnOFF");
				btnGroupcVolField = new QButtonGroup;
				btnGroupcVolField->addButton(cVolFieldOutputForceRadioBtnON);
				cVolFieldOutputForceRadioBtnON->setChecked(true);
				btnGroupcVolField->addButton(cVolFieldOutputForceRadioBtnOFF);
				QHBoxLayout*cVolFieldBtnLayout = new QHBoxLayout;
				cVolFieldBtnLayout->addWidget(cVolFieldOutputForceRadioBtnON);
				cVolFieldBtnLayout->addWidget(cVolFieldOutputForceRadioBtnOFF);


				cbdFieldRadioBtnON = new QRadioButton("ON");
				cbdFieldRadioBtnON->setObjectName("cbdFieldRadioBtnON");
				cbdFieldRadioBtnON->setChecked(true);
				cbdFieldRadioBtnOFF = new QRadioButton("OFF");
				cbdFieldRadioBtnOFF->setObjectName("cbdFieldRadioBtnOFF");
				btnGroupcbdField = new QButtonGroup;
				btnGroupcbdField->addButton(cbdFieldRadioBtnON);
				btnGroupcbdField->addButton(cbdFieldRadioBtnOFF);
				QHBoxLayout*cbdFieldBtnLayout = new QHBoxLayout;
				cbdFieldBtnLayout->addWidget(cbdFieldRadioBtnON);
				cbdFieldBtnLayout->addWidget(cbdFieldRadioBtnOFF);


				//QStringList stringList;
				//stringList << "ON" << "OFF";
				//comboBoxWallForce->addItems(stringList);
				QLabel* cVolFieldOutput = new QLabel(u8"是否输出空间流场");// = ON
				//QComboBox*comboBoxCVolFieldOutput = new QComboBox;
				//comboBoxCVolFieldOutput->addItems(stringList);
				//QComboBox*comboBoxCbdFieldOutput = new QComboBox;
				//comboBoxCbdFieldOutput->addItems(stringList);
				QLabel* cbdFieldOutput = new QLabel(u8"是否输出边界流场");// = ON
				outputLayout->addWidget(nout, 0, 0, 1, 1);
				outputLayout->addWidget(spinBoxNout, 0, 1, 1, 2);
				//outputLayout->addWidget(wallForce, 1, 0, 1, 1);

				//outputLayout->addLayout(wallBtnLayout, 1, 1, 1, 2);
				outputLayout->addWidget(cVolFieldOutput, 1, 0, 1, 1);
				outputLayout->addLayout(cVolFieldBtnLayout, 1, 1, 1, 2);
				outputLayout->addWidget(cbdFieldOutput, 2, 0, 1, 1);
				outputLayout->addLayout(cbdFieldBtnLayout, 2, 1, 1, 2);
				outputWidget->setLayout(outputLayout);
				
				output_mainout = new QVBoxLayout;
				output_mainout->addWidget(m_btnOutput);
				output_mainout->addWidget(outputWidget);


				m_btnGas = new CusButton;
				m_btnGas->setFixedWidth(350);
				m_btnGas->SetTextLabel(u8"Gas属性设置");
				m_btnGas->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				/*Nspecies = 1
				#Species = O O2 NO N N2
				#MassFraction = 0 0.21 0 0 0.79
				R0 = 296.95
				Gamma = 1.4*/
				QGridLayout* gasLayout = new QGridLayout;
				QLabel*nspeciesLabel = new QLabel("Nspecies");
				gasWidget = new QWidget;
				QLabel*R0 = new QLabel(u8"气体常数");
				QLabel*Gamma = new QLabel(u8"比热比");
				nsSpinBox = new QSpinBox;
				nsSpinBox->setObjectName("nsSpinBox");
				QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
				doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
				doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
				r0LineEdit = new QLineEdit;
				r0LineEdit->setValidator(doubleValidatorGe);
				r0LineEdit->setObjectName("r0LineEdit");
				r0LineEdit->setText("287.06");
				//r0LineEdit->setText("296.95");
				GammaDoubleSpinBox = new QDoubleSpinBox;
				GammaDoubleSpinBox->setSingleStep(0.01);
				GammaDoubleSpinBox->setObjectName("GammaDoubleSpinBox");
				GammaDoubleSpinBox->setValue(1.4);

				//GammaDoubleSpinBox->setValue(1.4);
				gasLayout->addWidget(nspeciesLabel, 0, 0, 1, 1);
				gasLayout->addWidget(nsSpinBox, 0, 1, 1, 2);
				gasLayout->addWidget(R0, 1, 0, 1, 1);
				gasLayout->addWidget(r0LineEdit, 1, 1, 1, 2);
				gasLayout->addWidget(Gamma, 2, 0, 1, 1);
				gasLayout->addWidget(GammaDoubleSpinBox, 2, 1, 1, 2);
				gasWidget->setLayout(gasLayout);
				output_mainout->addWidget(m_btnGas);
				output_mainout->addWidget(gasWidget);
				output_mainout->setSpacing(5);
				output_mainout->addStretch();

				//m_mainLayout->addLayout(output_mainout);
			}

			void InputSet::initFarfieldUi()
			{
				m_btnFarfield = new CusButton;
				m_btnFarfield->SetTextLabel(u8"Farfield属性设置");
				m_btnFarfield->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				m_btnFarfield->setFixedWidth(400);
				farfieldWidget = new QWidget;
				QLabel* Ma = new QLabel(u8"来流马赫数");// = 7.84
				QLabel* P = new QLabel(u8"来流压力"); //273.72
				QLabel*	T = new QLabel(u8"来流温度"); //46.2
				QLabel*AoA = new QLabel(u8"攻角");// 0
				QLabel*AoB = new QLabel(u8"侧滑角");// 0
				maSpinBox = new QDoubleSpinBox;
				maSpinBox->setSingleStep(0.01);
				maSpinBox->setObjectName("maSpinBox");
				maSpinBox->setRange(0, 10000);
				pSpinBox = new QDoubleSpinBox;
				pSpinBox->setRange(0, 1000);
				pSpinBox->setSingleStep(0.01);
				pSpinBox->setObjectName("pSpinBox");
				pSpinBox->setRange(0, 10000);
				//pSpinBox->setValue(273.72);
				tSpinBox = new QDoubleSpinBox;
				//tSpinBox->setValue(46.2);
				tSpinBox->setSingleStep(0.01);
				tSpinBox->setObjectName("tSpinBox");
				tSpinBox->setRange(0, 10000);

				aoaSpinBox = new QDoubleSpinBox;
				aoaSpinBox->setObjectName("aoaSpinBox");
				aoaSpinBox->setSingleStep(0.01);
				aoaSpinBox->setValue(0);

				aobSpinBox = new QDoubleSpinBox;
				aobSpinBox->setSingleStep(0.01);
				aobSpinBox->setObjectName("aobSpinBox");
				aobSpinBox->setValue(0);



				//aobSpinBox->setValue(0);
				QGridLayout*farfieldLayout = new QGridLayout;
				farfieldLayout->addWidget(Ma, 0, 0, 1, 1);
				farfieldLayout->addWidget(maSpinBox, 0, 1, 1, 2);
				farfieldLayout->addWidget(P, 1, 0, 1, 1);
				farfieldLayout->addWidget(pSpinBox, 1, 1, 1, 2);
				farfieldLayout->addWidget(T, 2, 0, 1, 1);
				farfieldLayout->addWidget(tSpinBox, 2, 1, 1, 2);
				farfieldLayout->addWidget(AoA, 3, 0, 1, 1);
				farfieldLayout->addWidget(aoaSpinBox, 3, 1, 1, 2);
				farfieldLayout->addWidget(AoB, 4, 0, 1, 1);
				farfieldLayout->addWidget(aobSpinBox, 4, 1, 1, 2);
				farfieldWidget->setLayout(farfieldLayout);

				farfield_main = new QVBoxLayout;
				farfield_main->addWidget(m_btnFarfield);
				farfield_main->addWidget(farfieldWidget);


				//--------------------//
				m_btnSolverControl = new CusButton;
				m_btnSolverControl->setFixedWidth(400);
				m_btnSolverControl->SetTextLabel(u8"SolverControl属性设置");
				m_btnSolverControl->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				QGridLayout*solverControlLaout = new QGridLayout;
				solverControlWidget = new QWidget;
				//QHBoxLayout*schemeLayout = new QHBoxLayout;
				scheme = new QLabel(u8"数值格式");
				schemeComboBox = new QComboBox;
				schemeComboBox->setObjectName("schemeComboBox");
				schemeComboBox->addItem(tr("AUSMPW")); // 添加选项
				schemeComboBox->addItem(tr("AUSMPW+"));
				schemeComboBox->addItem(tr("AUSM+"));
				schemeComboBox->addItem(tr("AUSM++"));
				schemeComboBox->addItem(tr("HLLC"));
				schemeComboBox->setCurrentIndex(0);
				//schemeLineEdit = new QLineEdit;
				//schemeLineEdit->setObjectName("schemeLineEdit");
				//schemeLineEdit->setText(u8"AUSMPW");
				//schemeLayout->addWidget(scheme);
				//schemeLayout->addWidget(schemeLineEdit);
				solverControlLaout->addWidget(scheme, 0, 0, 1, 1);
				solverControlLaout->addWidget(schemeComboBox, 0, 1, 1, 2);

				//QHBoxLayout*limiterLayout = new QHBoxLayout;
				limiter = new QLabel(u8"限制器");
				/*limiterLineEdit = new QLineEdit;
				limiterLineEdit->setObjectName("limiterLineEdit");*/
				limiterComboBox = new QComboBox;
				limiterComboBox->setObjectName("limiterComboBox");
				limiterComboBox->addItem(tr("minmod"));
				limiterComboBox->addItem(tr("van Albada"));
				limiterComboBox->addItem(tr("superbee"));
				limiterComboBox->addItem(tr("van Leer"));
				limiterComboBox->addItem(tr("Modified van Albada"));
				limiterComboBox->setCurrentIndex(4);
				//limiterLayout->addWidget(limiter);
				//limiterLayout->addWidget(limiterLineEdit);
				solverControlLaout->addWidget(limiter, 1, 0, 1, 1);
				solverControlLaout->addWidget(limiterComboBox, 1, 1, 1, 2);

				//QHBoxLayout*pressFactorLayout = new QHBoxLayout;
				/*pressFactor = new QLabel(u8"压力梯度分裂方法");
				pressFactorCombBox = new QComboBox;
				pressFactorCombBox->setObjectName("pressFactorCombBox");
				pressFactorCombBox->addItem(tr("THIVET 方法"));
				pressFactorCombBox->addItem(tr("Vigneron 方法"));
				pressFactorCombBox->setCurrentIndex(0);
				solverControlLaout->addWidget(pressFactor, 2, 0, 1, 1);
				solverControlLaout->addWidget(pressFactorCombBox, 2, 1, 1, 2);*/


				//temperal = new QLabel(u8"流向推进方法");
				//temperalComboBox = new QComboBox;
				//temperalComboBox->setObjectName("temperalComboBox");
				//temperalComboBox->addItem(tr("LU-SGS"));
				//temperalComboBox->addItem(tr("ADI"));
				//temperalComboBox->setCurrentIndex(0);
				///*temperalLineEdit = new QLineEdit;
				//temperalLineEdit->setObjectName("temperalLineEdit");*/
				//solverControlLaout->addWidget(temperal, 3, 0, 1, 1);
				//solverControlLaout->addWidget(temperalComboBox, 3, 1, 1, 2);
				//QHBoxLayout*visGradientLayout = new QHBoxLayout;
				//visGradient = new QLabel(u8"梯度格式");
				///*visGradientLineEdit = new QLineEdit;
				//visGradientLineEdit->setObjectName("visGradientLineEdit");*/
				//visGradientComboBox = new QComboBox;
				//visGradientComboBox->setObjectName("visGradientComboBox");
				//visGradientComboBox->addItem(tr("高斯法"));
				//visGradientComboBox->addItem(tr("有限差分法"));
				//visGradientComboBox->setCurrentIndex(0);
				//solverControlLaout->addWidget(visGradient, 4, 0, 1, 1);
				//solverControlLaout->addWidget(visGradientComboBox, 4, 1, 1, 2);



				solverControlWidget->setLayout(solverControlLaout);


				farfield_main->addWidget(m_btnSolverControl);
				farfield_main->addWidget(solverControlWidget);
				farfield_main->addStretch();

				//m_mainLayout->addLayout(farfield_main);

			}

			void InputSet::initBndryConditionUi()
			{
				m_btnBoundaryCondition = new CusButton;
				boundaryConditionWidget = new QWidget;
				m_btnBoundaryCondition->SetTextLabel(u8"BoundaryCondition属性设置");
				m_btnBoundaryCondition->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				m_btnBoundaryCondition->setFixedWidth(400);
				QLabel* Tw = new QLabel(u8"壁温");
				twSpinBox = new QDoubleSpinBox;
				twSpinBox->setObjectName("twSpinBox");
				twSpinBox->setRange(0, 1000);
				twSpinBox->setSingleStep(0.01);
				//twSpinBox->setValue(316.7);
				QGridLayout*boundaryConditionLayout = new QGridLayout;
				boundaryConditionLayout->addWidget(Tw, 0, 0, 1, 1);
				boundaryConditionLayout->addWidget(twSpinBox, 0, 1, 1, 2);
				boundaryConditionWidget->setLayout(boundaryConditionLayout);
				QVBoxLayout*boundaryConditionLayout_main = new QVBoxLayout;

				boundaryConditionLayout_main->addWidget(m_btnBoundaryCondition);
				boundaryConditionLayout_main->addWidget(boundaryConditionWidget);


				/****************************/
				m_btnMesh = new CusButton;
				m_btnMesh->SetTextLabel(u8"Mesh属性设置");
				m_btnMesh->setFixedWidth(400);
				m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				meshLineEdit = new QLineEdit;
				//meshLineEdit->setText("GRIDGEN GENERIC");
				meshLineEdit->setObjectName("meshLineEdit");
				QLabel* Mesh = new QLabel("Mesh");

				QGridLayout*meshLayout = new QGridLayout;
				meshLayout->addWidget(Mesh, 0, 0, 1, 1);
				meshLayout->addWidget(meshLineEdit, 0, 1, 1, 2);
				meshWidget = new QWidget;
				meshWidget->setLayout(meshLayout);
				QVBoxLayout* mesh_main = new QVBoxLayout;

				mesh_main->addWidget(m_btnMesh);
				mesh_main->addWidget(meshWidget);
				/***************************/
				boundaryConditionLayout_main->addLayout(mesh_main);

				/***************************/
				solverControlWidget1 = new QWidget;
				m_btnSolverControl1 = new CusButton;
				m_btnSolverControl1->SetTextLabel(u8"SolverControl属性设置");
				m_btnSolverControl1->setFixedWidth(400);
				m_btnSolverControl1->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				QGridLayout*solverlayout = new QGridLayout;
				linearEq = new QLabel("linearEq");

				linearEqLineEdit = new QLineEdit;
				linearEqLineEdit->setObjectName("linearEqLineEdit");
				solverlayout->addWidget(linearEq, 0, 0, 1, 1);
				solverlayout->addWidget(linearEqLineEdit, 0, 1, 1, 2);
				gridGeneration = new QLabel("Grid");
				gridGenerationLineEdit = new QLineEdit;
				gridGenerationLineEdit->setObjectName("gridGenerationLineEdit");
				solverlayout->addWidget(gridGeneration, 1, 0, 1, 1);
				solverlayout->addWidget(gridGenerationLineEdit, 1, 1, 1, 2);

				massCheck = new QLabel("MassCheck");
				btnGroupMassCheck = new QButtonGroup;
				massCheckBtnON = new QRadioButton("ON");
				massCheckBtnON->setObjectName("massCheckBtnON");
				massCheckBtnOFF = new QRadioButton("OFF");
				massCheckBtnOFF->setObjectName("massCheckBtnOFF");
				btnGroupMassCheck->addButton(massCheckBtnON);
				btnGroupMassCheck->addButton(massCheckBtnOFF);
				solverlayout->addWidget(massCheck, 2, 0, 1, 1);
				solverlayout->addWidget(massCheckBtnON, 2, 1, 1, 1);
				solverlayout->addWidget(massCheckBtnOFF, 2, 2, 1, 1);

				gridCheck = new QLabel("GridCheck");
				btnGroupGridCheck = new QButtonGroup;
				gridCheckBtnON = new QRadioButton("ON");
				gridCheckBtnON->setObjectName("gridCheckBtnON");
				gridCheckBtnOFF = new QRadioButton("OFF");
				gridCheckBtnOFF->setObjectName("gridCheckBtnOFF");
				btnGroupGridCheck->addButton(gridCheckBtnON);
				btnGroupGridCheck->addButton(gridCheckBtnOFF);
				solverlayout->addWidget(gridCheck, 3, 0, 1, 1);
				solverlayout->addWidget(gridCheckBtnON, 3, 1, 1, 1);
				solverlayout->addWidget(gridCheckBtnOFF, 3, 2, 1, 1);

				checkDummyGrid = new QLabel("CheckDummyGrid");
				checkDummyGridBtnON = new QRadioButton("ON");
				checkDummyGridBtnON->setObjectName("checkDummyGridBtnON");
				checkDummyGridBtnOFF = new QRadioButton("OFF");
				checkDummyGridBtnOFF->setObjectName("checkDummyGridBtnOFF");
				btnGroupcheckDummyGrid = new QButtonGroup;
				btnGroupcheckDummyGrid->addButton(checkDummyGridBtnON);
				btnGroupcheckDummyGrid->addButton(checkDummyGridBtnOFF);
				solverlayout->addWidget(checkDummyGrid, 4, 0, 1, 1);
				solverlayout->addWidget(checkDummyGridBtnON, 4, 1, 1, 1);
				solverlayout->addWidget(checkDummyGridBtnOFF, 4, 2, 1, 1);
				stepCtrl = new QLabel("stepCtrl");
				stepCtrlLineEdit = new QLineEdit;
				stepCtrlLineEdit->setObjectName("stepCtrlLineEdit");
				solverlayout->addWidget(stepCtrl, 5, 0, 1, 1);
				solverlayout->addWidget(stepCtrlLineEdit, 5, 1, 1, 2);


				solverControlWidget1->setLayout(solverlayout);

				boundaryConditionLayout_main->addWidget(m_btnSolverControl1);
				boundaryConditionLayout_main->addWidget(solverControlWidget1);
				boundaryConditionLayout_main->addLayout(farfield_main);
				/***************************/
				boundaryConditionLayout_main->addStretch();

				QWidget*boundaryCondition = new QWidget;
				boundaryCondition->setLayout(boundaryConditionLayout_main);
				m_mainLayout->addWidget(boundaryCondition);
				//m_mainLayout->addLayout(boundaryConditionLayout_main);
				QScrollArea*scroll = new QScrollArea;
				scroll->setWidget(boundaryCondition);
				scroll->setWidgetResizable(true);
				m_mainLayout->addWidget(scroll);

			}

			void InputSet::initGeometryUi()
			{
				QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
				doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
				doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
				m_btnGeometry = new CusButton;
				m_btnGeometry->SetTextLabel(u8"Geometry属性设置");
				m_btnGeometry->setFixedWidth(410);
				geometryWidget = new QWidget;

				m_btnGeometry->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				QLabel*Rlen = new QLabel(u8"参考长度");// = 0.264
				QLabel*XScale = new QLabel(u8"网格缩放比例");// 0.001
				wallForce = new QLabel(u8"是否计算气动力");// = ON
				//wallForce->setStyleSheet("border-radius:20px");
				//QComboBox*comboBoxWallForce = new QComboBox;
				wallForceRadioBtnON = new QRadioButton("ON");
				wallForceRadioBtnON->setObjectName("wallForceRadioBtnON");

				wallForceRadioBtnOFF = new QRadioButton("OFF");
				wallForceRadioBtnOFF->setObjectName("wallForceRadioBtnOFF");

				btnGroupWall = new QButtonGroup;
				btnGroupWall->addButton(wallForceRadioBtnON);
				wallForceRadioBtnOFF->setChecked(true);
				btnGroupWall->addButton(wallForceRadioBtnOFF);
				QHBoxLayout*wallBtnLayout = new QHBoxLayout;
				wallBtnLayout->addWidget(wallForceRadioBtnON);
				wallBtnLayout->addWidget(wallForceRadioBtnOFF);
				//下面几项不是必须的，但在计算力矩系数及升力、阻力系数时必须给定
				AreaRef = new QLabel(u8"参考面积");// 0.0074355
				xmc = new QLabel(u8"力矩参考点xmc");// 0.156
				ymc = new QLabel(u8"力矩参考点ymc");// 0
				zmc = new QLabel(u8"力矩参考点zmc");// 0
				rlenSpinBox = new QLineEdit;
				rlenSpinBox->setValidator(doubleValidatorGe);
				rlenSpinBox->setObjectName("rlenSpinBox");
				rlenSpinBox->setText("1.0");
				//rlenSpinBox->setText("0.264");
				//rlenSpinBox->setSingleStep(0.001);
				xscaleSpinBox = new QLineEdit;
				xscaleSpinBox->setValidator(doubleValidatorGe);
				xscaleSpinBox->setObjectName("xscaleSpinBox");
				xscaleSpinBox->setText("0");
				arearefSpinBox = new QLineEdit;
				arearefSpinBox->setValidator(doubleValidatorGe);
				arearefSpinBox->setObjectName("arearefSpinBox");
				//arearefSpinBox->setSingleStep(0.00001);
				//arearefSpinBox->setText("0.0074355");
				xmcSpinBox = new QDoubleSpinBox;
				xmcSpinBox->setSingleStep(0.001);
				//xmcSpinBox->setValidator(doubleValidatorGe);
				xmcSpinBox->setObjectName("xmcSpinBox");
				//xmcSpinBox->setText("0");
				//xmcSpinBox->setText("0.156");
				//xmcSpinBox->setSingleStep(0.001);
				ymcSpinBox = new QDoubleSpinBox;
				ymcSpinBox->setObjectName("ymcSpinBox");
				//ymcSpinBox->setValue(0);
				ymcSpinBox->setSingleStep(0.001);
				zmcSpinBox = new QDoubleSpinBox;
				zmcSpinBox->setSingleStep(0.001);
				zmcSpinBox->setObjectName("zmcSpinBox");
				zmcSpinBox->setValue(0);
				QGridLayout*geometryLayout = new QGridLayout;
				geometryLayout->addWidget(Rlen, 0, 0, 1, 1);
				geometryLayout->addWidget(rlenSpinBox, 0, 1, 1, 2);
				geometryLayout->addWidget(XScale, 1, 0, 1, 1);
				geometryLayout->addWidget(xscaleSpinBox, 1, 1, 1, 2);
				geometryLayout->addWidget(wallForce, 2, 0, 1, 1);
				geometryLayout->addLayout(wallBtnLayout, 2, 1, 1, 2);
				wallForceWidget = new QWidget;
				QGridLayout* wallWidgetLayout = new QGridLayout;
				wallWidgetLayout->addWidget(AreaRef, 0, 0, 1, 1);
				wallWidgetLayout->addWidget(arearefSpinBox, 0, 1, 1, 2);
				wallWidgetLayout->addWidget(xmc, 1, 0, 1, 1);
				wallWidgetLayout->addWidget(xmcSpinBox, 1, 1, 1, 2);
				wallWidgetLayout->addWidget(ymc, 2, 0, 1, 1);
				wallWidgetLayout->addWidget(ymcSpinBox, 2, 1, 1, 2);
				wallWidgetLayout->addWidget(zmc, 3, 0, 1, 1);
				wallWidgetLayout->addWidget(zmcSpinBox, 3, 1, 1, 2);
				wallForceWidget->setLayout(wallWidgetLayout);
				wallWidgetLayout->setHorizontalSpacing(geometryLayout->horizontalSpacing());
				wallForceWidget->setVisible(false);

				/*geometryLayout->addWidget(xmc, 3, 0, 1, 1);
				geometryLayout->addWidget(xmcSpinBox, 3, 1, 1, 2);
				geometryLayout->addWidget(ymc, 4, 0, 1, 1);
				geometryLayout->addWidget(ymcSpinBox, 4, 1, 1, 2);
				geometryLayout->addWidget(zmc, 5, 0, 1, 1);
				geometryLayout->addWidget(zmcSpinBox, 5, 1, 1, 2);*/
				//geometryLayout->addWidget(wallForceWidget, 3, 0, 4, 3);
				geometryWidget->setLayout(geometryLayout);
				QVBoxLayout* geometry_main = new QVBoxLayout;

				geometry_main->addWidget(m_btnGeometry);
				geometry_main->addWidget(geometryWidget);
				geometry_main->addWidget(wallForceWidget);

				initAdvancedSet();
				geometry_main->addWidget(m_btnAdvanced);
				geometry_main->addWidget(advancedWidget);
				geometry_main->addLayout(output_mainout);
				geometry_main->addStretch();
				geometry_main->setSpacing(geometryLayout->spacing());



				//m_mainLayout->addLayout(geometry_main);


				QWidget*geometryWiget = new QWidget;
				geometryWiget->setLayout(geometry_main);
				m_mainLayout->addWidget(geometryWiget);
				QScrollArea*scroll = new QScrollArea;
				scroll->setWidget(geometryWiget);
				scroll->setWidgetResizable(true);
				m_mainLayout->addWidget(scroll);

			}

			void InputSet::initMeshUi()
			{
				m_btnMesh = new CusButton;
				m_btnMesh->SetTextLabel(u8"Mesh属性设置");
				m_btnMesh->setFixedWidth(300);
				m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
				meshLineEdit = new QLineEdit;
				//meshLineEdit->setText("GRIDGEN GENERIC");
				meshLineEdit->setObjectName("meshLineEdit");
				QLabel* Mesh = new QLabel("Mesh");

				QGridLayout*meshLayout = new QGridLayout;
				meshLayout->addWidget(Mesh, 0, 0, 1, 1);
				meshLayout->addWidget(meshLineEdit, 0, 1, 1, 2);
				meshWidget = new QWidget;
				meshWidget->setLayout(meshLayout);
				QVBoxLayout* mesh_main = new QVBoxLayout;

				mesh_main->addWidget(m_btnMesh);
				mesh_main->addWidget(meshWidget);
				mesh_main->addStretch();
				//m_mainLayout->addLayout(mesh_main);

			}

			void InputSet::initStepCtrl()
			{

			}

			void InputSet::initAdvancedSet()
			{
				advancedWidget = new QWidget;
				m_btnAdvanced = new CusButton;
				m_btnAdvanced->SetTextLabel(u8"高级参数");
				m_btnAdvanced->setFixedWidth(410);
				m_btnAdvanced->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/collapase.png"));

				QGridLayout*advancedSetLayout = new QGridLayout;
				pressFactor = new QLabel(u8"压力梯度分裂方法");
				pressFactorCombBox = new QComboBox;
				pressFactorCombBox->setObjectName("pressFactorCombBox");
				pressFactorCombBox->addItem(tr("THIVET 方法"));
				pressFactorCombBox->addItem(tr("Vigneron 方法"));
				pressFactorCombBox->setCurrentIndex(0);
				advancedSetLayout->addWidget(pressFactor, 0, 0, 1, 1);
				advancedSetLayout->addWidget(pressFactorCombBox, 0, 1, 1, 2);

				temperal = new QLabel(u8"流向推进方法");
				temperalComboBox = new QComboBox;
				temperalComboBox->setObjectName("temperalComboBox");
				temperalComboBox->addItem(tr("LU-SGS"));
				temperalComboBox->addItem(tr("ADI"));
				temperalComboBox->setCurrentIndex(0);
				/*temperalLineEdit = new QLineEdit;
				temperalLineEdit->setObjectName("temperalLineEdit");*/
				advancedSetLayout->addWidget(temperal, 1, 0, 1, 1);
				advancedSetLayout->addWidget(temperalComboBox, 1, 1, 1, 2);

				visGradient = new QLabel(u8"梯度格式");
				visGradientComboBox = new QComboBox;
				visGradientComboBox->setObjectName("visGradientComboBox");
				visGradientComboBox->addItem(tr("高斯法"));
				visGradientComboBox->addItem(tr("有限差分法"));
				visGradientComboBox->setCurrentIndex(0);
				advancedSetLayout->addWidget(visGradient, 2, 0, 1, 1);
				advancedSetLayout->addWidget(visGradientComboBox, 2, 1, 1, 2);

				implicitVis = new QLabel(u8"粘性项隐式处理方法");
				implicitVisComboBox = new QComboBox;
				implicitVisComboBox->setObjectName("implicitVisComboBox");
				implicitVisComboBox->addItem(tr("守恒变量导数"));
				implicitVisComboBox->addItem(tr("最大谱半径线化方法 1"));
				implicitVisComboBox->addItem(tr("最大谱半径线化方法 2"));
				implicitVisComboBox->setCurrentIndex(0);
				advancedSetLayout->addWidget(implicitVis, 3, 0, 1, 1);
				advancedSetLayout->addWidget(implicitVisComboBox, 3, 1, 1, 2);

				advancedWidget->setLayout(advancedSetLayout);
				advancedWidget->setVisible(false);
			}

			void InputSet::initMap()
			{
				limiterMap["MINMOD"] = 0;
				limiterMap["VANALBADA"] = 1;
				limiterMap["SUPERBEE"] = 2;
				limiterMap["VANLEER"] = 3;
				limiterMap["MODIFIED VANALBADA"] = 4;

				visGradientMap["VIS_VOL"] = 0;
				visGradientMap["VIS_AREA"] = 1;
				schemeMap["AUSMPW"] = 0;
				schemeMap["AUSMPW+"] = 1;
				implicitVisMap["VISCOUS JACOBIAN"] = 0;
				implicitVisMap["TYSINGER"] = 1;
				implicitVisMap["CHENBING"] = 2;
				pressFactorMap["THIVET"] = 0;
				pressFactorMap["VIGNERON"] = 1;
				temperalMap["LUSGS_NS_VOL"] = 0;
				temperalMap["ADI_NS"] = 1;
			}

			void InputSet::initStyleSheet()
			{
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton1.css";
				QString sheet = FIlEINSTANCE->readFile(fileName);
				m_btnOutput->setStyleSheet(sheet);
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

			void InputSet::loadStyleSheet(QString sheetName)
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

			void InputSet::loadDefaultParameter()
			{
				QString path = SignalInstance::getInstance()->getWorkSpace();
				path += "/body/initialDefaultInput.dat";
				QFile file(path);
				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in(&file);
					QString line;
					while (!in.atEnd())
					{
						line = in.readLine();
						QStringList list = line.split(" ");
						//说明是按钮
						if (list.size() == 1)
						{
							QString strObjectName = list.at(0);
							QRadioButton*radioBtn = this->findChild<QRadioButton*>(strObjectName);
							radioBtn->setChecked(true);
						}
						else
						{
							QString strObjectName = list.at(0);
							QObject* obj = this->findChild<QObject*>(strObjectName);
							if (typeid(*obj) == typeid(QLineEdit)) {
								// 对象是QLineEdit类型
								QLineEdit *lineEdit = qobject_cast<QLineEdit*>(obj);
								lineEdit->setText(list.at(1));
								if (list.size() > 2)
								{
									lineEdit->setText(list.at(1) + " " + list.at(2));
								}

							}
							else if (typeid(*obj) == typeid(QSpinBox)) {
								// 对象是QLineEdit类型
								QSpinBox *spinBox = qobject_cast<QSpinBox*>(obj);
								spinBox->setValue(list.at(1).toInt());

							}
							else if (typeid(*obj) == typeid(QDoubleSpinBox)) {
								// 对象是QLineEdit类型
								QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox*>(obj);
								doubleSpinBox->setValue(list.at(1).toDouble());
							}
							else if (typeid(*obj) == typeid(QComboBox))
							{
								QComboBox *comboBox = qobject_cast<QComboBox*>(obj);
								comboBox->setCurrentIndex(list.at(1).toInt());
							}
						}
					}
				}


			}
			/* @brief Handle wallforce dependencies
			*
			*/
			void InputSet::onWallStateChanged()
			{
				QRadioButton* radioBtn = qobject_cast<QRadioButton*>(sender());
				if (radioBtn->objectName() == "wallForceRadioBtnON")
				{
					wallForceWidget->setVisible(true);
					if (!arearefSpinBox->text().isEmpty())
					{
						return;
					}
					QString styleSheet = arearefSpinBox->styleSheet();
					styleSheet += "border:1px solid red;";
					arearefSpinBox->setStyleSheet(styleSheet);
				}
				else if (radioBtn->objectName() == "wallForceRadioBtnOFF")
				{
					wallForceWidget->setVisible(false);
					//arearefSpinBox->setText("");
					//xmcSpinBox->setValue(0);
					//ymcSpinBox->setValue(0);
					//zmcSpinBox->setValue(0);
				}
			}

			void InputSet::onCheckLineEdit()
			{
				if (!arearefSpinBox->text().trimmed().isEmpty())
				{
					QString styleSheet = arearefSpinBox->styleSheet();
					styleSheet += "border:1px solid black;";
					arearefSpinBox->setStyleSheet(styleSheet);
				}
			}

			void InputSet::loadBodyParameter(QString workPath)
			{
				
				m_workspace = workPath;
				//检查head下是否存在键值对文件
				QDir workspaceDir(m_workspace + "/body");
				if (!workspaceDir.exists())
				{
					QMessageBox::information(this, u8"提示", u8"工作区选择错误", QMessageBox::Ok);
					return;
				}

				QFile file(workspaceDir.path() + "/bodyload.dat");
				if (file.exists())
				{
					//表示读取json文件
					loadBodyConfig(workspaceDir.path() + "/bodyload.dat", 1);
				}
				else
				{
					//读取headconfig文件，读取缺省值
					BodyPathChecker tempChecker;
					QString truePath = tempChecker.checkAndGetPath(workspaceDir.path() + "/input.dat");
					loadBodyConfig(truePath, 0);
				}

			}
			void InputSet::redictBodyParameter() {
				BodyPathChecker tempChecker;
				QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/input.dat"));
				FILE* output = fopen(truePath.toLocal8Bit().toStdString().c_str(), "w");
				if (output == NULL) return;
				fprintf(output, "#注释是被程序忽略的部分，由#开头\n");
				fprintf(output, "#每个设置部分的开始由一个节头指明，其形式是[sectionname]\n");

				fprintf(output, "[Output]\n");
				fprintf(output, "Nout=%d\n", spinBoxNout->value());
				if (wallForceRadioBtnON->isChecked()) fprintf(output, "WallForce=ON\n");
				else fprintf(output, "WallForce=OFF\n");

				if (cVolFieldOutputForceRadioBtnON->isChecked()) fprintf(output, "cVolFieldOutput=ON\n");
				else fprintf(output, "cVolFieldOutput=OFF\n");

				if (cbdFieldRadioBtnON->isChecked()) fprintf(output, "cbdFieldOutput=ON\n");
				else fprintf(output, "cbdFieldOutput=OFF\n");
				fprintf(output, "\n");

				fprintf(output, "[Farfield]\n");
				fprintf(output, "Ma=%lf\n", maSpinBox->value());
				fprintf(output, "P=%lf\n", pSpinBox->value());
				fprintf(output, "T=%lf\n", tSpinBox->value());
				fprintf(output, "AoA=%lf\n", aoaSpinBox->value());
				fprintf(output, "AoB=%lf\n", aobSpinBox->value());
				fprintf(output, "\n");

				fprintf(output, "[BoundaryCondition]\n");
				fprintf(output, "#主要用于设置壁温及入口参数\n");
				fprintf(output, "Tw=%lf\n", twSpinBox->value());
				fprintf(output, "\n");

				fprintf(output, "[Geometry]\n");
				fprintf(output, "Rlen=%lf\n", rlenSpinBox->text().toDouble());
				fprintf(output, "XScale=%lf\n", xscaleSpinBox->text().toDouble());
				fprintf(output, "#下面几项不是必须的，但在计算力矩系数及升力、阻力系数时必须给定\n");
				fprintf(output, "AreaRef=%lf\n", arearefSpinBox->text().toDouble());
				fprintf(output, "xmc=%lf\n", xmcSpinBox->text().toDouble());
				fprintf(output, "ymc=%lf\n", ymcSpinBox->text().toDouble());
				fprintf(output, "zmc=%lf\n", zmcSpinBox->text().toDouble());
				fprintf(output, "\n");

				fprintf(output, "[Mesh]\n");
				fprintf(output, "Medium=%s\n", meshLineEdit->text().toStdString().c_str());
				fprintf(output, "GridFileName=../body_grid.grd\n");
				fprintf(output, "bdFileName=../body_genericbd.inp\n");
				fprintf(output, "\n");

				fprintf(output, "[Gas]\n");
				fprintf(output, "Nspecies=%d\n", nsSpinBox->value());
				fprintf(output, "#Species=O O2 NO N N2\n");
				fprintf(output, "#MassFraction=0 0.21 0 0 0.79\n");
				fprintf(output, "R0=%lf\n", r0LineEdit->text().toDouble());
				fprintf(output, "Gamma=%lf\n", GammaDoubleSpinBox->value());
				fprintf(output, "\n");

				QString str = "Scheme cant find";
				fprintf(output, "[SolverControl]\n");
				for (auto it = schemeMap.begin(); it != schemeMap.end(); it++) {

					if (it.value() == schemeComboBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "Scheme=%s\n", str.toStdString().c_str());
				str = "limiter cant find";
				for (auto it = limiterMap.begin(); it != limiterMap.end(); it++) {
					if (it.value() == limiterComboBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "Limiter=%s\n", str.toStdString().c_str());
				str = "PressFactor cant find";
				for (auto it = pressFactorMap.begin(); it != pressFactorMap.end(); it++) {
					if (it.value() == pressFactorCombBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "PressFactor=%s\n", str.toStdString().c_str());
				str = "Temperal cant find";
				for (auto it = temperalMap.begin(); it != temperalMap.end(); it++) {
					if (it.value() == temperalComboBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "Temperal=%s\n", str.toStdString().c_str());
				str = "VisGradient cant find";
				for (auto it = visGradientMap.begin(); it != visGradientMap.end(); it++) {
					if (it.value() == visGradientComboBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "VisGradient=%s\n", str.toStdString().c_str());
				str = "ImplicitVis cant find";
				for (auto it = implicitVisMap.begin(); it != implicitVisMap.end(); it++) {
					if (it.value() == implicitVisComboBox->currentIndex()) {
						str = it.key();
						break;
					}
				}
				fprintf(output, "ImplicitVis=%s\n", str.toStdString().c_str());
				fprintf(output, "StepCtrl=%s\n", stepCtrlLineEdit->text().toStdString().c_str());
				fprintf(output, "#StepCtrl=CFL\n");
				fprintf(output, "LinearEq=%s\n", linearEqLineEdit->text().toStdString().c_str());
				fprintf(output, "GridGeneration=%s\n", gridGenerationLineEdit->text().toStdString().c_str());
				fprintf(output, "#SourceOrNot=ON\n");
				fprintf(output, "#SourceImplicit=SRCEXPLICIT\n");
				if (massCheckBtnON->isChecked()) fprintf(output, "MassCheck=ON\n");
				else fprintf(output, "MassCheck=OFF\n");
				fprintf(output, "#这个选项打开会检查每一段开始读入的网格\n");
				if (gridCheckBtnON->isChecked()) fprintf(output, "GridCheck=ON\n");
				else fprintf(output, "GridCheck=OFF\n");
				fprintf(output, "#这个选项如果打开每一层生成的网格会被输出\n");
				if (checkDummyGridBtnON->isChecked()) fprintf(output, "CheckDummyGrid=ON\n");
				else fprintf(output, "CheckDummyGrid=OFF\n");
				fprintf(output, "#设定全局的cfl数\n");
				fprintf(output, "#CFL=3\n");
				fprintf(output, "\n");

				fprintf(output, "#这一节或许有或许没有，如果有它设置了特定节的一些参数\n");
				fprintf(output, "#其它没有指明的参数就使用默认的参数\n");
				fprintf(output, "[StepCtrl]\n");
				fprintf(output, "#Section Xstep0 XStepMax0 XStepMinEnd   StepMax   KStep    NCStep\n");
				auto table = tableShow->getTable();
				auto sectCount = tableShow->getSectNames().size();
				for (int i = 0; i < sectCount; i++)
				{
					QString data = "";
					for (int j = 0; j < table->columnCount(); j++)
					{
						data += table->item(i, j)->text();
						data += " ";
					}
					data += "\n";
					fprintf(output, data.toStdString().c_str());
				}
				/*fprintf(output, "1 1.D-6 1.D-4 5.D-5 1.D-4 1.01D0    500  !sect1\n");
				fprintf(output, "2 1.D-6 1.D-3 5.D-5 1.D-3 1.01D0    200  !sect2\n");
				fprintf(output, "3 5.D-7 5.D-7 5.D-6 5.D-5 1.005D0   100  !sect3\n");
				fprintf(output, "4 1.D-4 5.D-5 5.D-4 1.D-3 1.007D0   100  !sect4\n");
				fprintf(output, "5 5.D-4 5.D-5 5.D-4 8.D-4 1.007D0   100  !sect5\n");
				fprintf(output, "6 1.D-3 5.D-5 5.D-4 1.D-3 1.01D0    70  !sect6\n");*/
				fprintf(output, "\n");
				fprintf(output, "#覆盖默认的控制参数，没有指定的参数由全局设置决定\n");

				fclose(output);
				output = nullptr;
				/// @brief if (sectInfo == NULL) return;
				emit SignalInstance::getInstance()->signalColorLine();
				outPutLogger->exeProgramBody(SignalInstance::getInstance()->m_workspace);


			}
			void InputSet::loadBodyConfig(QString path, bool flag)
			{
				//flag为0加载config文件
				if (!flag)
				{
					QFile file(path);
					if (file.open(QIODevice::ReadOnly | QIODevice::Text))
					{
						QTextStream in(&file);
						QString line;
						int indexLine = 0;
						//对默认初始文件进行编写（键值对保存）
						BodyPathChecker tempChecker;
						QString truePath = tempChecker.checkAndGetPath(QString(m_workspace + "/body/initialInput.dat"));
						outfile.open(truePath.toLocal8Bit().toStdString());
						while (!in.atEnd())
						{
							line = in.readLine();
							//解析OutPut
							if (line == "[Output]")
							{
								//连续读取四段更新
								line = in.readLine();
								QStringList list = line.split("=");
								spinBoxNout->setValue(list.at(1).toInt());
								outfile << "spinBoxNout " << list.at(1).toStdString() << std::endl;
								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									wallForceRadioBtnON->setChecked(true);
									//onWallStateChanged();
									wallForceWidget->setVisible(true);
									//QString styleSheet = arearefSpinBox->styleSheet();
									//styleSheet += "border:1px solid red;";
									//arearefSpinBox->setStyleSheet(styleSheet);
									outfile << "wallForceRadioBtnON" << std::endl;
								}
								else
								{
									wallForceRadioBtnOFF->setChecked(true);
									onWallStateChanged();
									outfile << "wallForceRadioBtnOFF" << std::endl;
								}


								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									cVolFieldOutputForceRadioBtnON->setChecked(true);
									outfile << "cVolFieldOutputForceRadioBtnON" << std::endl;
								}
								else
								{
									cVolFieldOutputForceRadioBtnOFF->setChecked(true);
									outfile << "cVolFieldOutputForceRadioBtnOFF" << std::endl;
								}
								//outfile << "cVolFieldOutput " << list.at(1).toStdString() << endl;

								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									cbdFieldRadioBtnON->setChecked(true);

									outfile << "cbdFieldRadioBtnON" << std::endl;
								}
								else
								{
									cbdFieldRadioBtnOFF->setChecked(true);
									outfile << "cbdFieldRadioBtnOFF" << std::endl;
								}

							}
							else if (line == "[Farfield]")
							{
								line = in.readLine();
								QStringList list = line.split("=");
								maSpinBox->setValue(list.at(1).toDouble());
								outfile << "maSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								pSpinBox->setValue(list.at(1).toDouble());
								outfile << "pSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								tSpinBox->setValue(list.at(1).toDouble());
								outfile << "tSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								aoaSpinBox->setValue(list.at(1).toDouble());
								outfile << "aoaSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								aobSpinBox->setValue(list.at(1).toDouble());
								outfile << "aobSpinBox " << list.at(1).toStdString() << std::endl;
							}

							else if (line == "[BoundaryCondition]")
							{
								line = in.readLine();
								line = in.readLine();
								QStringList list = line.split("=");
								twSpinBox->setValue(list.at(1).toDouble());
								outfile << "twSpinBox " << list.at(1).toStdString() << std::endl;

							}
							else if (line == "[Geometry]")
							{
								line = in.readLine();
								QStringList list = line.split("=");
								rlenSpinBox->setText(list.at(1));
								outfile << "rlenSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								xscaleSpinBox->setText(list.at(1));
								outfile << "xscaleSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								line = in.readLine();
								list = line.split("=");
								arearefSpinBox->setText(list.at(1));
								outfile << "arearefSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								xmcSpinBox->setValue(list.at(1).toDouble());
								outfile << "xmcSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								ymcSpinBox->setValue(list.at(1).toDouble());
								outfile << "ymcSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								zmcSpinBox->setValue(list.at(1).toDouble());
								outfile << "zmcSpinBox " << list.at(1).toStdString() << std::endl;


							}
							else if (line == "[Mesh]")
							{
								line = in.readLine();
								QStringList list = line.split("=");
								meshLineEdit->setText(list.at(1));
								outfile << "meshLineEdit " << list.at(1).toStdString() << std::endl;
							}
							else if (line == "[Gas]")
							{
								line = in.readLine();
								QStringList list = line.split("=");
								nsSpinBox->setValue(list.at(1).toInt());
								outfile << "nsSpinBox " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								line = in.readLine();
								line = in.readLine();
								list = line.split("=");
								r0LineEdit->setText(list.at(1));
								outfile << "r0LineEdit " << list.at(1).toStdString() << std::endl;


								line = in.readLine();
								list = line.split("=");
								GammaDoubleSpinBox->setValue(list.at(1).toDouble());
								outfile << "GammaDoubleSpinBox " << list.at(1).toStdString() << std::endl;
							}
							else if (line == "[SolverControl]")
							{
								line = in.readLine();
								QStringList list = line.split("=");
								schemeComboBox->setCurrentIndex(schemeMap[list.at(1)]);
								outfile << "schemeComboBox " << schemeMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								limiterComboBox->setCurrentIndex(limiterMap[list.at(1)]);
								int t = limiterMap[list.at(1)];
								outfile << "limiterComboBox " << limiterMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								pressFactorCombBox->setCurrentIndex(pressFactorMap[list.at(1)]);
								outfile << "pressFactorCombBox " << pressFactorMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								temperalComboBox->setCurrentIndex(temperalMap[list.at(1)]);
								outfile << "temperalComboBox " << temperalMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								visGradientComboBox->setCurrentIndex(visGradientMap[list.at(1)]);
								outfile << "visGradientComboBox " << visGradientMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								implicitVisComboBox->setCurrentIndex(implicitVisMap[list.at(1)]);
								outfile << "implicitVisComboBox " << implicitVisMap[list.at(1)] << std::endl;

								line = in.readLine();
								list = line.split("=");
								stepCtrlLineEdit->setText(list.at(1));
								outfile << "stepCtrlLineEdit " << list.at(1).toStdString() << std::endl;

								//#StepCtrl=CFL
								line = in.readLine();

								line = in.readLine();
								list = line.split("=");
								linearEqLineEdit->setText(list.at(1));
								outfile << "linearEqLineEdit " << list.at(1).toStdString() << std::endl;

								line = in.readLine();
								list = line.split("=");
								gridGenerationLineEdit->setText(list.at(1));
								outfile << "gridGenerationLineEdit " << list.at(1).toStdString() << std::endl;

								//#SourceOrNot=ON
								//#SourceImplicit = SRCEXPLICIT
								line = in.readLine();
								line = in.readLine();


								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									massCheckBtnON->setChecked(true);
									outfile << "massCheckBtnON" << std::endl;
								}
								else {
									massCheckBtnOFF->setChecked(true);
									outfile << "massCheckBtnOFF" << std::endl;
								}
								//#这个选项打开会检查每一段开始读入的网格
								line = in.readLine();

								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									gridCheckBtnON->setChecked(true);
									outfile << "gridCheckBtnON" << std::endl;
								}
								else {
									gridCheckBtnOFF->setChecked(true);
									outfile << "gridCheckBtnOFF" << std::endl;
								}
								//#这个选项如果打开每一层生成的网格会被输出
								line = in.readLine();

								line = in.readLine();
								list = line.split("=");
								if (list.at(1) == "ON") {
									checkDummyGridBtnON->setChecked(true);
									outfile << "checkDummyGridBtnON" << std::endl;
								}
								else
								{
									checkDummyGridBtnOFF->setChecked(true);
									outfile << "checkDummyGridBtnOFF" << std::endl;
								}
							}
							indexLine++;
						}
						outfile.close();
					}
				}
			}
		}
	}
}

