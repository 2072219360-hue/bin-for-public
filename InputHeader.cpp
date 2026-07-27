#include "InputHeader.h"

using platforms::sspns::reveal::OutPutLogger;
extern QString appComDir;
extern OutPutLogger* outPutLogger;
extern statusSubject* myStatus;
namespace  platforms {
	namespace sspns {
		namespace reveal {
			InputHeader::InputHeader()
			{
				initUi();
			}

			InputHeader::~InputHeader()
			{
			}
			void InputHeader::addObserver() {
				
			}
			void InputHeader::initUi()
			{
				
				/*QPushButton*btnExe = new QPushButton(u8"运行");
				btnExe->setStyleSheet("QPushButton{font-size:25px;background:lightblue;color:\
			#ffffff;border:none;border-radius:4px;\
		}QPushButton:hover, QPushButton:pressed{\
			background:rgb(7, 189, 253);\
		}");*/

				initHeadPart1();
				initHeadPart2();
				QVBoxLayout* layout_main = new QVBoxLayout;
				layout_main->addLayout(m_main1Layout);
				layout_main->addLayout(m_main2Layout);
				QWidget*mainWidget = new QWidget;
				mainWidget->setLayout(layout_main);
				QScrollArea*scroll = new QScrollArea;
				scroll->setWidget(mainWidget);
				scroll->setWidgetResizable(true);
				

				m_mainLayout = new QHBoxLayout;
				m_mainLayout->addWidget(scroll);
				this->setLayout(m_mainLayout);
				this->loadStyleSheet("InputSet2");


				connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(loadHeaderParameter(QString)));
				connect(SignalInstance::getInstance(), SIGNAL(signalLoadDefaultParameter()), this, SLOT(loadDefaultParameter()));
				connect(SignalInstance::getInstance(), SIGNAL(signalRedictHeadParameter()), this, SLOT(redictHeadParameter()));
				connect(m_twConditionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTwConditionChanged(int)));
				connect(SignalInstance::getInstance(), SIGNAL(signalSetRestart(bool)), this, SLOT(setRestartFlag(bool)));

			}


			void InputHeader::loadStyleSheet(QString sheetName)
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
			void InputHeader::setRestartFlag(bool flag) {
				cInitComboBox->setCurrentIndex(flag);
			}
			void InputHeader::loadHeadConfig(QString path, bool flag)
			{
				QString gridFile;
				QString bdFile;
				//flag为0加载config文件
				if (!flag)
				{
					std::ofstream outHeadLoad;
					QFile file(path);
					////outfile.open(QString(m_workspace + "/head/initialConfig.dat").toStdString());
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
								//outfile << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
								m_tLineEdit->setText(list.at(1));

								//outfile << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
								m_maLineEdit->setText(list.at(2));
								//outfile << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
								outHeadLoad << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
								alphaSpinBox->setValue(list.at(3).toInt());
								//outfile << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
								outHeadLoad << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
								m_betaLineEdit->setText(list.at(4));
								//outfile << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
								outHeadLoad << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
								m_TwLineEdit->setText(list.at(5));
								//outfile << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
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
								//outfile << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
								m_AreaRefLineEdit->setText(list.at(1));
								//outfile << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
								m_LrefRefLineEdit->setText(list.at(2));
								//outfile << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
								outHeadLoad << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
								m_xcLineEdit->setText(list.at(3));
								//outfile << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
								outHeadLoad << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
								m_ycLineEdit->setText(list.at(4));
								//outfile << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
								outHeadLoad << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
								m_zcLineEdit->setText(list.at(5));
								//outfile << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
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
								//outfile << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "m_pLineEdit" << " " << list.at(0).toStdString() << std::endl;
								m_tLineEdit->setText(list.at(1));

								//outfile << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "m_tLineEdit" << " " << list.at(1).toStdString() << std::endl;
								m_maLineEdit->setText(list.at(2));
								//outfile << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
								outHeadLoad << "m_maLineEdit" << " " << list.at(2).toStdString() << std::endl;
								alphaSpinBox->setValue(list.at(3).toInt());
								//outfile << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
								outHeadLoad << "alphaSpinBox" << " " << list.at(3).toStdString() << std::endl;
								m_betaLineEdit->setText(list.at(4));
								//outfile << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
								outHeadLoad << "m_betaLineEdit" << " " << list.at(4).toStdString() << std::endl;
								m_TwLineEdit->setText(list.at(5));
								if (list.at(5) == "adiabatic") m_twConditionComboBox->setCurrentIndex(1);
								else
								{
									m_twConditionComboBox->setCurrentIndex(0);
								}
								//outfile << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
								outHeadLoad << "m_TwLineEdit" << " " << list.at(5).toStdString() << std::endl;
								/*m_gasLineEdit->setText(list.at(0));
								//outfile << "m_gasLineEdit" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "m_gasLineEdit" << " " << list.at(0).toStdString() << std::endl;
								m_radioLineEdit->setText(list.at(1));
								//outfile << "m_radioLineEdit" << " " << list.at(1).toStdString() << std::endl;
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
								//outfile << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "m_xscaleLineEdit" << " " << list.at(0).toStdString() << std::endl;
								m_AreaRefLineEdit->setText(list.at(1));
								//outfile << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "m_AreaRefLineEdit" << " " << list.at(1).toStdString() << std::endl;
								m_LrefRefLineEdit->setText(list.at(2));
								//outfile << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
								outHeadLoad << "m_LrefRefLineEdit" << " " << list.at(2).toStdString() << std::endl;
								m_xcLineEdit->setText(list.at(3));
								//outfile << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
								outHeadLoad << "m_xcLineEdit" << " " << list.at(3).toStdString() << std::endl;
								m_ycLineEdit->setText(list.at(4));
								//outfile << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
								outHeadLoad << "m_ycLineEdit" << " " << list.at(4).toStdString() << std::endl;
								m_zcLineEdit->setText(list.at(5));
								//outfile << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
								outHeadLoad << "m_zcLineEdit" << " " << list.at(5).toStdString() << std::endl;
								/*cInitComboBox->setCurrentText(list.at(0));
								//outfile << "cInitSpinBox" << " " << list.at(0).toStdString() << std::endl;
								outHeadLoad << "cInitSpinBox" << " " << list.at(0).toStdString() << std::endl;
								NStopLineEdit->setText(list.at(1));
								//outfile << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
								cflSpinBox->setValue(list.at(2).toInt());
								//outfile << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
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
								//outfile << "m_gasLineEdit" << " " << gasValue.toStdString() << std::endl;
								outHeadLoad << "m_gasLineEdit" << " " << gasValue.toStdString() << std::endl;
								m_radioLineEdit->setText(list.at(3));
								//outfile << "m_radioLineEdit" << " " << list.at(3).toStdString() << std::endl;
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
								//outfile << "cInitComboBox" << " " << std::to_string(cInitIndex) << std::endl;
								outHeadLoad << "cInitComboBox" << " " << std::to_string(cInitIndex) << std::endl;
								NStopLineEdit->setText(list.at(1));
								//outfile << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
								outHeadLoad << "NStopLineEdit" << " " << list.at(1).toStdString() << std::endl;
								cflSpinBox->setValue(list.at(2).toInt());
								//outfile << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
								outHeadLoad << "cflSpinBox" << " " << list.at(2).toStdString() << std::endl;
							}
							indexLine++;
						}
					}
					//outfile.close();
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

			void InputHeader::loadHeaderParameter(QString workPath)
			{
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
					emit SignalInstance::getInstance()->signalSetRestart(true);
				}

			}

			void InputHeader::loadDefaultParameter()
			{
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
					}
				}
			}

			void InputHeader::exeHeader()
			{
				//qDebug() << u8"exeHeader！！";
				QString headerPath = "/SSPNS/ui/ui/head/";
				//加载头部区域参数
				loadHeaderParameter(headerPath);

				outPutLogger->exeProgramHeader(headerPath);
			}
			bool InputHeader::getRestartFlag() {
				return cInitComboBox->currentIndex();
			}
			void InputHeader::initHeadPart1() {
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

			}
			void InputHeader::initHeadPart2()
			{
				QDoubleValidator* doubleValidatorGe = new QDoubleValidator(this);
				doubleValidatorGe->setRange(0.000000001, 1000.000, 8);
				doubleValidatorGe->setNotation(QDoubleValidator::StandardNotation);
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
				m_AreaRefLineEdit->setValidator(doubleValidatorGe);
				Lref = new QLabel(u8"参考长度");
				m_LrefRefLineEdit = new QLineEdit;
				m_LrefRefLineEdit->setValidator(doubleValidatorGe);
				xc = new QLabel(u8"质心坐标xc");
				m_xcLineEdit = new QLineEdit;
				m_xcLineEdit->setValidator(doubleValidatorGe);
				yc = new QLabel(u8"质心坐标yc");
				m_ycLineEdit = new QLineEdit;
				zc = new QLabel(u8"质心坐标zc");
				m_zcLineEdit = new QLineEdit;
				m_zcLineEdit->setValidator(doubleValidatorGe);

				m_gasLineEdit->setValidator(doubleValidatorGe);
				m_betaLineEdit->setValidator(doubleValidatorGe);
				m_ycLineEdit->setValidator(doubleValidatorGe);

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
			}
			void InputHeader::redictHeadParameter()
			{
				std::ofstream outHeadLoad;
				outHeadLoad.open(QString(m_workspace + "/head/headload.dat").toLocal8Bit().toStdString());
				outHeadLoad << "m_pLineEdit" << " " << m_pLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_tLineEdit" << " " << m_tLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_maLineEdit" << " " << m_maLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "alphaSpinBox" << " " << alphaSpinBox->value() << std::endl;
				outHeadLoad << "m_betaLineEdit" << " " << m_betaLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_TwLineEdit" << " " << m_TwLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_xscaleLineEdit" << " " << m_xscaleSpinBox->value() << std::endl;
				outHeadLoad << "m_AreaRefLineEdit" << " " << m_AreaRefLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_LrefRefLineEdit" << " " << m_LrefRefLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_xcLineEdit" << " " << m_xcLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_ycLineEdit" << " " << m_ycLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_zcLineEdit" << " " << m_zcLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_gasLineEdit" << " " << m_gasLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "m_radioLineEdit" << " " << m_radioLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "cInitComboBox" << " " << cInitComboBox->currentIndex() << std::endl;
				outHeadLoad << "NStopLineEdit" << " " << NStopLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "cflSpinBox" << " " << cflSpinBox->value() << std::endl;
				outHeadLoad.close();


				//写headConfig.dat
				outHeadLoad.open(QString(m_workspace + "/head/headconfig.dat").toLocal8Bit().toStdString());
				/*
				! P T Ma alpha beta: 来流压力、温度、马赫数、攻角、侧滑角
				! Tw : 壁温，如果为正值则为常壁温，负值为绝热壁
				! Xscale : 1网格单位代表的长度（以m为单位）
				! AreaRef : 参考面积， Lref : 参考长度, xc, yc, zc : 质心坐标，单位m
				! cInit : 1, 自由来流， 2, 续算
				! NStop : 计算步数
				!P    T   Ma alpha beta Tw
				*/
				outHeadLoad << "! Gridfile, bdfile: 网格和边界条件文件名" << std::endl;
				outHeadLoad << "! P T Ma alpha beta: 来流压力、温度、马赫数、攻角、侧滑角" << std::endl;
				outHeadLoad << "! Tw : 壁温，如果为正值则为常壁温，负值为绝热壁" << std::endl;
				outHeadLoad << "! Xscale : 1网格单位代表的长度（以m为单位）" << std::endl;
				//outHeadLoad << "! Tw : 壁温，如果为正值则为常壁温，负值为绝热壁" << std::endl;
				outHeadLoad << "! AreaRef : 参考面积， Lref : 参考长度, xc, yc, zc : 质心坐标，单位m" << std::endl;
				outHeadLoad << "! cInit : 1, 自由来流， 2, 续算" << std::endl;
				outHeadLoad << "! NStop : 计算步数" << std::endl;
				outHeadLoad << "! GridFile" << std::endl;
				outHeadLoad << "../head_grid.grd" << std::endl;
				outHeadLoad << "! bdfile" << std::endl;
				outHeadLoad << "../head_bd.inp" << std::endl;
				outHeadLoad << "!P    T   Ma alpha beta Tw" << std::endl;

				outHeadLoad << m_pLineEdit->text().toStdString() << " " << m_tLineEdit->text().toStdString() << " " << m_maLineEdit->text().toStdString() << " " << alphaSpinBox->value() << " "
					<< m_betaLineEdit->text().toStdString() << " " << m_TwLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "! Xscale    AreaRef Lref    xc  yc  zc" << std::endl;
				outHeadLoad << m_xscaleSpinBox->value() << " " << m_AreaRefLineEdit->text().toStdString() << " " << m_LrefRefLineEdit->text().toStdString() << " "
					<< m_xcLineEdit->text().toStdString() << " " << m_ycLineEdit->text().toStdString() << " " << m_zcLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "! 气体常数(J/kg/K) 比热比" << std::endl;
				outHeadLoad << m_gasLineEdit->text().toStdString() << " " << m_radioLineEdit->text().toStdString() << std::endl;
				outHeadLoad << "! cInit NStop  cfl" << std::endl;
				outHeadLoad << cInitComboBox->currentIndex() + 1 << " " << NStopLineEdit->text().toStdString() << " " << cflSpinBox->value() << std::endl;
				outPutLogger->exeProgramHeader(SignalInstance::getInstance()->m_workspace);
			}
			void InputHeader::onTwConditionChanged(int index)
			{
				/*Tw->setVisible(!index);
				m_TwLineEdit->setVisible(!index);*/
				if (index == 0)
				{
					m_TwLineEdit->setPlaceholderText(u8"请输入壁温");
					m_TwLineEdit->setEnabled(true);
				}
				else
				{
					m_TwLineEdit->setPlaceholderText(u8"adiabatic ");
					m_TwLineEdit->setEnabled(false);
				}
			}
		}
	}
}
