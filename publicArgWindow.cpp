#include "publicArgWindow.h"
extern QString appComDir;
namespace platforms {
	namespace sspns {
		publicArgWindow::publicArgWindow() {
			initUi();
		}
		void publicArgWindow::initUi() {
			
			initHeadUi();
			initBodyUi();
			initMap();
			initLayout();
			initConnection();
		}
		void publicArgWindow::initConnection() {
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
			connect(wallForceRadioBtnON, SIGNAL(clicked(bool)), this, SLOT(onWallStateChanged()));
			connect(wallForceRadioBtnOFF, SIGNAL(clicked(bool)), this, SLOT(onWallStateChanged()));
			connect(AreaRefBodySpinBox, SIGNAL(returnPressed()), this, SLOT(onCheckLineEdit()));
			connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(loadParameter(QString)));
		}
		void publicArgWindow::loadHeadParameter(QString workPath) {
			m_workspace = workPath;
			//检查head下是否存在键值对文件
			QDir workspaceDir(m_workspace + "/head");
			if (!workspaceDir.exists())
			{
				QMessageBox::information(this, u8"提示", u8"工作区选择错误", QMessageBox::Ok);
				return;
			}

			QFile file(workspaceDir.path() + "/headload.dat");
			if (file.exists())
			{
				//表示读取json文件
				loadHeadConfig(workspaceDir.path() + "/headload.dat", 1);
			}
			else
			{
				//读取headconfig文件，读取缺省值
				loadHeadConfig(workspaceDir.path() + "/headconfig.dat", 0);
			}
			QFile fileRestart(workspaceDir.path() + "/restart.dat");
			if (fileRestart.exists() && fileRestart.size()) {
				setRestartFlag(true);
			}
		}
		void publicArgWindow::loadBodyParameter(QString workPath) {
			m_workspace = workPath;
			QDir workspaceDir(m_workspace + "/body");
			if (!workspaceDir.exists())
			{
				QMessageBox::information(this, u8"提示", u8"工作区选择错误", QMessageBox::Ok);
				return;
			}
			//set body config
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
		void publicArgWindow::loadParameter(QString workPath){
			loadHeadParameter(workPath);
			loadBodyParameter(workPath);
		}
		void publicArgWindow :: setRestartFlag(bool flag) {
			cInitComboBox->setCurrentIndex(flag);
		}
		void publicArgWindow::loadBodyConfig(QString path, bool flag)
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
					outfile.open(QString(m_workspace + "/body/initialInput.dat").toLocal8Bit().toStdString());
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
							AreaRefBodySpinBox->setText(list.at(1));
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
		void publicArgWindow::initMap()
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
		void publicArgWindow::initLayout() {
			
			QLabel* argTitle = new QLabel(u8"公共参数");
			argTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			argTitle->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
			argTitle->setFixedHeight(40);
			QVBoxLayout* layout_main = new QVBoxLayout;
			layout_main->addWidget(argTitle);
			layout_main->addWidget(headWidget);
			layout_main->addWidget(bodyWidget);

			QWidget*mainWidget = new QWidget;
			mainWidget->setLayout(layout_main);
			mainWidget->setEnabled(false);
			QScrollArea*scroll = new QScrollArea;
			scroll->setWidget(mainWidget);
			scroll->setWidgetResizable(true);

			m_mainLayout = new QHBoxLayout;
			m_mainLayout->addWidget(scroll);
			this->setLayout(m_mainLayout);
			this->loadStyleSheet("InputSet2");
		}
		void publicArgWindow::loadStyleSheet(QString sheetName)
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


		void publicArgWindow::initBodyUi() {
			//init UI1
			m_mainLayoutBody = new QVBoxLayout();
			m_btnOutput = new CusButton;
			m_btnOutput->setFixedWidth(350);
			m_btnOutput->SetTextLabel(u8"Output属性设置");
			m_btnOutput->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
			QFrame* hframe5 = new QFrame;
			hframe5->setFrameShape(QFrame::VLine);      // 设置水平方向
			hframe5->setStyleSheet("QFrame{background:lightblue;min-height:0.1px}");
			m_mainLayoutBody->addWidget(hframe5);
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

			QLabel* cVolFieldOutput = new QLabel(u8"是否输出空间流场");// = ON
			QLabel* cbdFieldOutput = new QLabel(u8"是否输出边界流场");// = ON
			outputLayout->addWidget(nout, 0, 0, 1, 1);
			outputLayout->addWidget(spinBoxNout, 0, 1, 1, 2);
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

			QGridLayout* gasLayout = new QGridLayout;
			QLabel*nspeciesLabel = new QLabel("Nspecies");
			gasWidget = new QWidget;
			QLabel*R0 = new QLabel(u8"气体常数");
			QLabel*Gamma = new QLabel(u8"比热比");
			nsSpinBox = new QSpinBox;
			nsSpinBox->setObjectName("nsSpinBox");
			QDoubleValidator* doubleValidatorGe2 = new QDoubleValidator(this);
			doubleValidatorGe2->setRange(0.000000001, 1000.000, 8);
			doubleValidatorGe2->setNotation(QDoubleValidator::StandardNotation);
			r0LineEdit = new QLineEdit;
			r0LineEdit->setValidator(doubleValidatorGe2);
			r0LineEdit->setObjectName("r0LineEdit");
			r0LineEdit->setText("287.06");
			GammaDoubleSpinBox = new QDoubleSpinBox;
			GammaDoubleSpinBox->setSingleStep(0.01);
			GammaDoubleSpinBox->setObjectName("GammaDoubleSpinBox");
			GammaDoubleSpinBox->setValue(1.4);

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
			
			//init UI2
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
			m_btnSolverControl = new CusButton;
			m_btnSolverControl->setFixedWidth(400);
			m_btnSolverControl->SetTextLabel(u8"SolverControl属性设置");
			m_btnSolverControl->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
			QGridLayout*solverControlLaout = new QGridLayout;
			solverControlWidget = new QWidget;
			scheme = new QLabel(u8"数值格式");
			schemeComboBox = new QComboBox;
			schemeComboBox->setObjectName("schemeComboBox");
			schemeComboBox->addItem(tr("AUSMPW")); // 添加选项
			schemeComboBox->addItem(tr("AUSMPW+"));
			schemeComboBox->addItem(tr("AUSM+"));
			schemeComboBox->addItem(tr("AUSM++"));
			schemeComboBox->addItem(tr("HLLC"));
			schemeComboBox->setCurrentIndex(0);

			solverControlLaout->addWidget(scheme, 0, 0, 1, 1);
			solverControlLaout->addWidget(schemeComboBox, 0, 1, 1, 2);

			limiter = new QLabel(u8"限制器");

			limiterComboBox = new QComboBox;
			limiterComboBox->setObjectName("limiterComboBox");
			limiterComboBox->addItem(tr("minmod"));
			limiterComboBox->addItem(tr("van Albada"));
			limiterComboBox->addItem(tr("superbee"));
			limiterComboBox->addItem(tr("van Leer"));
			limiterComboBox->addItem(tr("Modified van Albada"));
			limiterComboBox->setCurrentIndex(4);

			solverControlLaout->addWidget(limiter, 1, 0, 1, 1);
			solverControlLaout->addWidget(limiterComboBox, 1, 1, 1, 2);
			solverControlWidget->setLayout(solverControlLaout);
			farfield_main->addWidget(m_btnSolverControl);
			farfield_main->addWidget(solverControlWidget);
			farfield_main->addStretch();
			//init UI3
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
			QGridLayout*boundaryConditionLayout = new QGridLayout;
			boundaryConditionLayout->addWidget(Tw, 0, 0, 1, 1);
			boundaryConditionLayout->addWidget(twSpinBox, 0, 1, 1, 2);
			boundaryConditionWidget->setLayout(boundaryConditionLayout);
			QVBoxLayout*boundaryConditionLayout_main = new QVBoxLayout;

			boundaryConditionLayout_main->addWidget(m_btnBoundaryCondition);
			boundaryConditionLayout_main->addWidget(boundaryConditionWidget);
			m_btnMesh = new CusButton;
			m_btnMesh->SetTextLabel(u8"Mesh属性设置");
			m_btnMesh->setFixedWidth(400);
			m_btnMesh->SetImageLabel(QPixmap(appComDir + "/images/SSPNS/expand.png"));
			meshLineEdit = new QLineEdit;
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
			boundaryConditionLayout_main->addLayout(mesh_main);

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
			boundaryConditionLayout_main->addStretch();

			QWidget*boundaryCondition = new QWidget;
			boundaryCondition->setLayout(boundaryConditionLayout_main);
			m_mainLayoutBody->addWidget(boundaryCondition);
			//init UI4
			QDoubleValidator* doubleValidatorGe3 = new QDoubleValidator(this);
			doubleValidatorGe3->setRange(0.000000001, 1000.000, 8);
			doubleValidatorGe3->setNotation(QDoubleValidator::StandardNotation);
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
			AreaRefBody = new QLabel(u8"参考面积");// 0.0074355
			xmc = new QLabel(u8"力矩参考点xmc");// 0.156
			ymc = new QLabel(u8"力矩参考点ymc");// 0
			zmc = new QLabel(u8"力矩参考点zmc");// 0
			rlenSpinBox = new QLineEdit;
			rlenSpinBox->setValidator(doubleValidatorGe3);
			rlenSpinBox->setObjectName("rlenSpinBox");
			rlenSpinBox->setText("1.0");
			//rlenSpinBox->setText("0.264");
			//rlenSpinBox->setSingleStep(0.001);
			xscaleSpinBox = new QLineEdit;
			xscaleSpinBox->setValidator(doubleValidatorGe3);
			xscaleSpinBox->setObjectName("xscaleSpinBox");
			xscaleSpinBox->setText("0");
			AreaRefBodySpinBox = new QLineEdit;
			AreaRefBodySpinBox->setValidator(doubleValidatorGe3);
			AreaRefBodySpinBox->setObjectName("AreaRefBodySpinBox");
			//AreaRefBodySpinBox->setSingleStep(0.00001);
			//AreaRefBodySpinBox->setText("0.0074355");
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
			wallWidgetLayout->addWidget(AreaRefBody, 0, 0, 1, 1);
			wallWidgetLayout->addWidget(AreaRefBodySpinBox, 0, 1, 1, 2);
			wallWidgetLayout->addWidget(xmc, 1, 0, 1, 1);
			wallWidgetLayout->addWidget(xmcSpinBox, 1, 1, 1, 2);
			wallWidgetLayout->addWidget(ymc, 2, 0, 1, 1);
			wallWidgetLayout->addWidget(ymcSpinBox, 2, 1, 1, 2);
			wallWidgetLayout->addWidget(zmc, 3, 0, 1, 1);
			wallWidgetLayout->addWidget(zmcSpinBox, 3, 1, 1, 2);
			wallForceWidget->setLayout(wallWidgetLayout);
			wallWidgetLayout->setHorizontalSpacing(geometryLayout->horizontalSpacing());
			wallForceWidget->setVisible(false);
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

			QWidget*geometryWiget = new QWidget;
			geometryWiget->setLayout(geometry_main);
			m_mainLayoutBody->addWidget(geometryWiget);



			QLabel* bodyitle = new QLabel(u8"身部参数");
			bodyitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			bodyitle->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
			QVBoxLayout* layout_body = new QVBoxLayout;
			layout_body->addWidget(bodyitle);
			layout_body->addLayout(m_mainLayoutBody);
			bodyWidget = new QWidget;
			bodyWidget->setLayout(layout_body);
		}
		void publicArgWindow::initAdvancedSet()
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
			pressFactorCombBox->addItem(tr(u8"THIVET 方法"));
			pressFactorCombBox->addItem(tr(u8"Vigneron 方法"));
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
			visGradientComboBox->addItem(tr(u8"高斯法"));
			visGradientComboBox->addItem(tr(u8"有限差分法"));
			visGradientComboBox->setCurrentIndex(0);
			advancedSetLayout->addWidget(visGradient, 2, 0, 1, 1);
			advancedSetLayout->addWidget(visGradientComboBox, 2, 1, 1, 2);

			implicitVis = new QLabel(u8"粘性项隐式处理方法");
			implicitVisComboBox = new QComboBox;
			implicitVisComboBox->setObjectName("implicitVisComboBox");
			implicitVisComboBox->addItem(tr(u8"守恒变量导数"));
			implicitVisComboBox->addItem(tr(u8"最大谱半径线化方法 1"));
			implicitVisComboBox->addItem(tr(u8"最大谱半径线化方法 2"));
			implicitVisComboBox->setCurrentIndex(0);
			advancedSetLayout->addWidget(implicitVis, 3, 0, 1, 1);
			advancedSetLayout->addWidget(implicitVisComboBox, 3, 1, 1, 2);
			advancedWidget->setLayout(advancedSetLayout);
			advancedWidget->setVisible(true);
		}
		void publicArgWindow::initHeadUi() {
			m_main1Layout = new QGridLayout;
			cInit = new QLabel(u8"操作选项cInit");
			NStop = new QLabel(u8"计算步数");
			cfl = new QLabel("cfl");
			//cInitSpinBox = new QDoubleSpinBox;
			//cInitSpinBox->setSingleStep(0.1);
			cInitComboBox = new QComboBox;
			cInitComboBox->addItem(u8"自由来流");
			cInitComboBox->addItem(u8"续算");
			cInitComboBox->setCurrentIndex(0);
			cInitComboBox->setObjectName("cInitComboBox");
			QIntValidator*intValidatorNStop = new QIntValidator(1, 100000, this);
			NStopLineEdit = new QLineEdit;
			NStopLineEdit->setValidator(intValidatorNStop);
			//NStopLineEdit->setText("20002");
			cflSpinBox = new QDoubleSpinBox;
			cflSpinBox->setSingleStep(0.1);
			//cflSpinBox->setValue(5);
			m_main1Layout->addWidget(cInit, 0, 0, 1, 1);
			m_main1Layout->addWidget(cInitComboBox, 0, 1, 1, 2);
			m_main1Layout->addWidget(NStop, 1, 0, 1, 1);
			m_main1Layout->addWidget(NStopLineEdit, 1, 1, 1, 2);
			m_main1Layout->addWidget(cfl, 2, 0, 1, 1);
			m_main1Layout->addWidget(cflSpinBox, 2, 1, 1, 2);


			QDoubleValidator* doubleValidatorP = new QDoubleValidator(this);
			doubleValidatorP->setRange(0.0001, 1000.0000, 4);
			doubleValidatorP->setNotation(QDoubleValidator::StandardNotation);
			P = new QLabel(u8"来流压力");
			m_pLineEdit = new QLineEdit;
			m_pLineEdit->setValidator(doubleValidatorP);
			QDoubleValidator* doubleValidatorT = new QDoubleValidator(this);
			doubleValidatorT->setRange(0.001, 100.000, 3);
			doubleValidatorT->setNotation(QDoubleValidator::StandardNotation);

			T = new QLabel(u8"温度");
			m_tLineEdit = new QLineEdit;
			m_tLineEdit->setValidator(doubleValidatorT);
			Ma = new QLabel(u8"马赫数");
			QDoubleValidator* doubleValidatorMa = new QDoubleValidator(this);
			doubleValidatorMa->setRange(0.001, 100.000, 3);
			doubleValidatorMa->setNotation(QDoubleValidator::StandardNotation);
			m_maLineEdit = new QLineEdit;
			m_maLineEdit->setValidator(doubleValidatorMa);
			alpha = new QLabel(u8"攻角");
			alphaSpinBox = new QSpinBox;
			beta = new QLabel(u8"侧滑角");
			m_betaLineEdit = new QLineEdit;
			Tw = new QLabel(u8"壁温");
			m_TwLineEdit = new QLineEdit;
			m_TwLineEdit->setPlaceholderText(u8"请输入壁温");

			TwCondition = new QLabel(u8"壁面温度条件");
			m_twConditionComboBox = new QComboBox;
			m_twConditionComboBox->addItem(u8"常壁温");
			m_twConditionComboBox->addItem(u8"绝热壁");

			QDoubleValidator* doubleValidatorTw = new QDoubleValidator(this);
			doubleValidatorTw->setRange(0.001, 1000.000, 3);
			doubleValidatorTw->setNotation(QDoubleValidator::StandardNotation);
			m_TwLineEdit->setValidator(doubleValidatorTw);
			radioHeat = new QLabel(u8"比热比");
			m_radioLineEdit = new QLineEdit;
			gasConstant = new QLabel(u8"气体常数");
			m_gasLineEdit = new QLineEdit;
			QDoubleValidator* doubleValidatorRadioHeat = new QDoubleValidator(this);
			doubleValidatorRadioHeat->setRange(0.001, 100.000, 2);
			doubleValidatorRadioHeat->setNotation(QDoubleValidator::StandardNotation);
			m_radioLineEdit->setValidator(doubleValidatorRadioHeat);
			m_main1Layout->addWidget(P, 3, 0, 1, 1);
			m_main1Layout->addWidget(m_pLineEdit, 3, 1, 1, 2);
			m_main1Layout->addWidget(T, 4, 0, 1, 1);
			m_main1Layout->addWidget(m_tLineEdit, 4, 1, 1, 2);
			m_main1Layout->addWidget(Ma, 5, 0, 1, 1);
			m_main1Layout->addWidget(m_maLineEdit, 5, 1, 1, 2);
			m_main1Layout->addWidget(alpha, 6, 0, 1, 1);
			m_main1Layout->addWidget(alphaSpinBox, 6, 1, 1, 2);
			m_main1Layout->addWidget(radioHeat, 7, 0, 1, 1);
			m_main1Layout->addWidget(m_radioLineEdit, 7, 1, 1, 2);
			m_main1Layout->addWidget(gasConstant, 8, 0, 1, 1);
			m_main1Layout->addWidget(m_gasLineEdit, 8, 1, 1, 2);

			QDoubleValidator* doubleValidatorGe1 = new QDoubleValidator(this);
			doubleValidatorGe1->setRange(0.000000001, 1000.000, 8);
			doubleValidatorGe1->setNotation(QDoubleValidator::StandardNotation);
			m_main2Layout = new QGridLayout;
			Xscale = new QLabel(u8"网格缩放比例");
			//m_xscaleLineEdit = new QLineEdit;
			m_xscaleSpinBox = new QDoubleSpinBox;
			m_xscaleSpinBox->setDecimals(5);
			m_xscaleSpinBox->setRange(0, 1000);
			m_xscaleSpinBox->setSingleStep(0.0001);
			m_xscaleSpinBox->setValue(0.001);
			//m_xscaleLineEdit->setValidator(doubleValidatorGe);
			AreaRef = new QLabel(u8"参考面积");
			m_AreaRefLineEdit = new QLineEdit;
			m_AreaRefLineEdit->setValidator(doubleValidatorGe1);
			Lref = new QLabel(u8"参考长度");
			m_LrefRefLineEdit = new QLineEdit;
			m_LrefRefLineEdit->setValidator(doubleValidatorGe1);
			xc = new QLabel(u8"质心坐标xc");
			m_xcLineEdit = new QLineEdit;
			m_xcLineEdit->setValidator(doubleValidatorGe1);
			yc = new QLabel(u8"质心坐标yc");
			m_ycLineEdit = new QLineEdit;
			zc = new QLabel(u8"质心坐标zc");
			m_zcLineEdit = new QLineEdit;
			m_zcLineEdit->setValidator(doubleValidatorGe1);

			m_gasLineEdit->setValidator(doubleValidatorGe1);
			m_betaLineEdit->setValidator(doubleValidatorGe1);
			m_ycLineEdit->setValidator(doubleValidatorGe1);

			m_main2Layout->addWidget(Xscale, 0, 0, 1, 1);
			m_main2Layout->addWidget(m_xscaleSpinBox, 0, 1, 1, 2);

			m_main2Layout->addWidget(AreaRef, 1, 0, 1, 1);
			m_main2Layout->addWidget(m_AreaRefLineEdit, 1, 1, 1, 2);

			m_main2Layout->addWidget(Lref, 2, 0, 1, 1);
			m_main2Layout->addWidget(m_LrefRefLineEdit, 2, 1, 1, 2);

			m_main2Layout->addWidget(xc, 3, 0, 1, 1);
			m_main2Layout->addWidget(m_xcLineEdit, 3, 1, 1, 2);

			m_main2Layout->addWidget(yc, 4, 0, 1, 1);
			m_main2Layout->addWidget(m_ycLineEdit, 4, 1, 1, 2);

			m_main2Layout->addWidget(zc, 5, 0, 1, 1);
			m_main2Layout->addWidget(m_zcLineEdit, 5, 1, 1, 2);
			m_main2Layout->addWidget(beta, 6, 0, 1, 1);
			m_main2Layout->addWidget(m_betaLineEdit, 6, 1, 1, 2);

			//m_main2Layout->addWidget(gasConstant, 7, 0, 1, 1);
			//m_main2Layout->addWidget(m_gasLineEdit, 7, 1, 1, 2);
			m_main2Layout->addWidget(TwCondition, 7, 0, 1, 1);
			m_main2Layout->addWidget(m_twConditionComboBox, 7, 1, 1, 2);
			m_main2Layout->addWidget(Tw, 8, 0, 1, 1);
			m_main2Layout->addWidget(m_TwLineEdit, 8, 1, 1, 2);
			/*m_mainLayout->addLayout(m_main2Layout);
			m_mainLayout->addLayout(m_main1Layout);*/
			m_pLineEdit->setObjectName("m_pLineEdit");
			m_tLineEdit->setObjectName("m_tLineEdit");
			m_maLineEdit->setObjectName("m_maLineEdit");
			alphaSpinBox->setObjectName("alphaSpinBox");
			m_betaLineEdit->setObjectName("m_betaLineEdit");
			m_TwLineEdit->setObjectName("m_TwLineEdit");
			m_xscaleSpinBox->setObjectName("m_xscaleLineEdit");
			m_AreaRefLineEdit->setObjectName("m_AreaRefLineEdit");
			m_LrefRefLineEdit->setObjectName("m_LrefRefLineEdit");
			m_xcLineEdit->setObjectName("m_xcLineEdit");
			m_ycLineEdit->setObjectName("m_ycLineEdit");
			m_zcLineEdit->setObjectName("m_zcLineEdit");
			m_gasLineEdit->setObjectName("m_gasLineEdit");
			m_radioLineEdit->setObjectName("m_radioLineEdit");
			cInitComboBox->setObjectName("cInitComboBox");
			NStopLineEdit->setObjectName("NStopLineEdit");
			cflSpinBox->setObjectName("cflSpinBox");

			QLabel*btnHead = new QLabel(u8"头部参数");
			btnHead->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			btnHead->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
			btnHead->setFixedHeight(40);
			QVBoxLayout* layout_Head = new QVBoxLayout;
			layout_Head->addWidget(btnHead);
			layout_Head->addLayout(m_main1Layout);
			layout_Head->addLayout(m_main2Layout);
			headWidget = new QWidget;
			headWidget->setLayout(layout_Head);

		}

		void publicArgWindow::onWallStateChanged()
		{
			QRadioButton* radioBtn = qobject_cast<QRadioButton*>(sender());
			if (radioBtn->objectName() == "wallForceRadioBtnON")
			{
				wallForceWidget->setVisible(true);
				if (!AreaRefBodySpinBox->text().isEmpty())
				{
					return;
				}
				QString styleSheet = AreaRefBodySpinBox->styleSheet();
				styleSheet += "border:1px solid red;";
				AreaRefBodySpinBox->setStyleSheet(styleSheet);
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
		void publicArgWindow::onCheckLineEdit()
		{
			if (!AreaRefBodySpinBox->text().trimmed().isEmpty())
			{
				QString styleSheet = AreaRefBodySpinBox->styleSheet();
				styleSheet += "border:1px solid black;";
				AreaRefBodySpinBox->setStyleSheet(styleSheet);
			}
		}
		void publicArgWindow::redictBodyParameter(){}

		void publicArgWindow::loadHeadConfig(QString path, bool flag)
		{
			QString gridFile;
			QString bdFile;
			//flag为0加载config文件
			if (!flag)
			{
				std::ofstream outHeadLoad;
				QFile file(path);
				outfile.open(QString(m_workspace + "/head/initialInput.dat").toLocal8Bit().toStdString());
				outHeadLoad.open(QString(m_workspace + "/head/headload.dat").toLocal8Bit().toStdString());
				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in(&file);
					QString line;
					int indexLine = 0;
					while (!in.atEnd())
					{
						line = in.readLine();
						//保存GridFile
						if (indexLine == 7)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size(); )
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							gridFile = line;
							/*m_pLineEdit->setText(list.at(0));
							outfile << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
							m_tLineEdit->setText(list.at(1));

							outfile << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
							m_maLineEdit->setText(list.at(2));
							outfile << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
							alphaSpinBox->setValue(list.at(3).toInt());
							outfile << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
							outHeadLoad << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
							m_betaLineEdit->setText(list.at(4));
							outfile << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
							outHeadLoad << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
							m_TwLineEdit->setText(list.at(5));
							outfile << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
							outHeadLoad << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;*/
						}
						else if (indexLine == 9)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size();)
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							bdFile = line;
							/*m_xscaleSpinBox->setValue(list.at(0).toDouble());
							outfile << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
							m_AreaRefLineEdit->setText(list.at(1));
							outfile << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
							m_LrefRefLineEdit->setText(list.at(2));
							outfile << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
							m_xcLineEdit->setText(list.at(3));
							outfile << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
							outHeadLoad << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
							m_ycLineEdit->setText(list.at(4));
							outfile << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
							outHeadLoad << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
							m_zcLineEdit->setText(list.at(5));
							outfile << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
							outHeadLoad << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;*/

						}
						/// P T Ma alpha beta Tw
						else if (indexLine == 11)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size(); )
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							m_pLineEdit->setText(list.at(0));
							outfile << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
							m_tLineEdit->setText(list.at(1));

							outfile << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
							m_maLineEdit->setText(list.at(2));
							outfile << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
							alphaSpinBox->setValue(list.at(3).toInt());
							outfile << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
							outHeadLoad << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
							m_betaLineEdit->setText(list.at(4));
							outfile << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
							outHeadLoad << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
							m_TwLineEdit->setText(list.at(5));
							if (list.at(5) == "adiabatic") m_twConditionComboBox->setCurrentIndex(1);
							else
							{
								m_twConditionComboBox->setCurrentIndex(0);
							}
							outfile << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
							outHeadLoad << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
							/*m_gasLineEdit->setText(list.at(0));
							outfile << "m_gasLineEdit" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "m_gasLineEdit" << " " << list.at(0).toStdString() << std::endl;
							m_radioLineEdit->setText(list.at(1));
							outfile << "m_radioLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "m_radioLineEdit" << " " << list.at(1).toStdString() << std::endl;*/

						}
						else if (indexLine == 13)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size(); i++)
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							m_xscaleSpinBox->setValue(list.at(0).toDouble());
							outfile << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
							m_AreaRefLineEdit->setText(list.at(1));
							outfile << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
							m_LrefRefLineEdit->setText(list.at(2));
							outfile << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
							m_xcLineEdit->setText(list.at(3));
							outfile << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
							outHeadLoad << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
							m_ycLineEdit->setText(list.at(4));
							outfile << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
							outHeadLoad << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
							m_zcLineEdit->setText(list.at(5));
							outfile << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
							outHeadLoad << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
							/*cInitComboBox->setCurrentText(list.at(0));
							outfile << "cInitSpinBox" << " " << list.at(0).toStdString() << std::endl;
							outHeadLoad << "cInitSpinBox" << " " << list.at(0).toStdString() << std::endl;
							NStopLineEdit->setText(list.at(1));
							outfile << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
							cflSpinBox->setValue(list.at(2).toInt());
							outfile << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;*/

						}
						/// 航天给的文件存在bug，后续进行修改
						else if (indexLine == 14)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size(); i++)
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							//拿到气体常数的值
							QString getGas = list[2];
							QString gasValue;
							for (int i = 0; i < getGas.size(); i++)
							{
								if (getGas[i] == '.')gasValue.push_back(getGas[i]);
								if (getGas[i] >= '0' && getGas[i] <= '9')
								{
									gasValue.push_back(getGas[i]);
								}
							}
							//gasValue = QString(gasValue.rbegin(),gasValue.rend())

							m_gasLineEdit->setText(gasValue);
							outfile << "m_gasLineEdit" << " " << gasValue.toStdString() << std::endl;
							outHeadLoad << "m_gasLineEdit" << " " << gasValue.toStdString() << std::endl;
							m_radioLineEdit->setText(list.at(3));
							outfile << "m_radioLineEdit" << " " << list.at(3).toStdString() << std::endl;
							outHeadLoad << "m_radioLineEdit" << " " << list.at(3).toStdString() << std::endl;
						}
						else if (indexLine == 16)
						{
							line.replace(QRegExp("[\\s]+"), " ");
							QStringList list = line.split(" ");

							for (int i = 0; i < list.size(); i++)
							{
								if (list.at(i) == " ")
								{
									list.removeAt(i);
									continue;
								}
								i++;
							}
							int cInitIndex = list.at(0).toInt() - 1;
							cInitComboBox->setCurrentIndex(cInitIndex);
							outfile << "cInitComboBox" << " " << std::to_string(cInitIndex) << std::endl;
							outHeadLoad << "cInitComboBox" << " " << std::to_string(cInitIndex) << std::endl;
							NStopLineEdit->setText(list.at(1));
							outfile << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
							outHeadLoad << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
							cflSpinBox->setValue(list.at(2).toInt());
							outfile << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
							outHeadLoad << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
						}
						indexLine++;
					}
				}
				outfile.close();
				outHeadLoad.close();
				file.close();
			}
			else {
				//qDebug() << "headload.dat load" << endl;
				QString path = SignalInstance::getInstance()->getWorkSpace();
				path += "/head/headload.dat";
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
						QString strObjectName = list.at(0);
						QObject* obj = this->findChild<QObject*>(strObjectName);
						if (typeid(*obj) == typeid(QLineEdit)) {
							// 对象是QLineEdit类型
							QLineEdit *lineEdit = qobject_cast<QLineEdit*>(obj);
							lineEdit->setText(list.at(1));
							if (list.size() > 2)
							{
								lineEdit->setText(list.at(1) + list.at(2));
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
				file.close();
			}
		}
	}
}
