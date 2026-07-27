#include "BatchConfigWidget.h"
#include <QDir>
extern QString appComDir;
namespace platforms {

	namespace sspns {

		namespace detail {
			BatchConfigWidget::BatchConfigWidget(int ns_flag)
			{
				this->ns_flag = ns_flag;
				init();

			}
			BatchConfigWidget::~BatchConfigWidget() {
				
			}
			void BatchConfigWidget::init()
			{
				//保留的小数位数
				decimal = 1;
				updateArg = new updateBatchArgs();
				m_width = 550;
				m_height = 1000;
				m_maxNum = 30;
				m_paramSetDialog = nullptr;
				this->setWindowTitle(u8"批量计算配置");
				this->setFixedSize(m_width, m_height);
				initControl();
				initConnection();
			}
			void BatchConfigWidget::setParamSetDialog(ParamSetDialog* _dialog) {
				this->m_paramSetDialog = _dialog;
			}
			void BatchConfigWidget::initControl()
			{
				m_btnExampleNum = new QLabel(u8"算例个数");
				m_inputExampleNum = new QLineEdit;
				m_inputExOk = new QPushButton(u8"确认");
				QHBoxLayout* exampleNumLayout = new QHBoxLayout;
				exampleNumLayout->addWidget(m_btnExampleNum);
				exampleNumLayout->addWidget(m_inputExampleNum);
				exampleNumLayout->addWidget(m_inputExOk);
				exampleNumLayout->addStretch();
				//--------------------------------------------
				m_batchTabel = new QTableWidget;
				// 初始化小数验证委托
				m_oneDecimalDelegate = new OneDecimalDelegate(this);
				// 为表格设置委托（所有单元格都应用）
				m_batchTabel->setItemDelegate(m_oneDecimalDelegate);

				m_tableHeader << "Ma" << "H" << "AOA" << "AOB";
				initTableWidget(m_batchTabel, m_tableHeader);
				/*m_batchTabel->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				m_batchTabel->verticalHeader()->setMaximumSectionSize(100);*/


				//--------------------------------------------
				QHBoxLayout* btnLayout = new QHBoxLayout;
				m_btnLoadData = new QPushButton(u8"载入数据");
				m_btnLoadEx = new QPushButton(u8"导入数据");
				m_btnUpdateData = new QPushButton(u8"更新数据");
				m_btnGenerateEx = new QPushButton(u8"生成实例");
				btnLayout->addStretch();
				//btnLayout->addWidget(m_btnLoadData);
				//btnLayout->addWidget(m_btnUpdateData);

				btnLayout->addWidget(m_btnLoadEx);
				btnLayout->addWidget(m_btnGenerateEx);
				//--------------------------------------------
				QVBoxLayout* tabelLayout = new QVBoxLayout;
				tabelLayout->addWidget(m_batchTabel);
				tabelLayout->addStretch();
				/*m_batchTabelWidget = new QWidget;
				m_batchTabelWidget->setLayout(tabelLayout);*/

				m_mainLayout = new QVBoxLayout;
				m_mainLayout->addLayout(exampleNumLayout);
				m_mainLayout->addLayout(tabelLayout);
				m_mainLayout->addLayout(btnLayout);
				this->setLayout(m_mainLayout);
				//m_inputExampleNum->setText("4");
				//onUpdateTable();
			}
			void BatchConfigWidget::initConnection()
			{
				connect(m_inputExampleNum, SIGNAL(returnPressed()), this, SLOT(onUpdateTable()));
				connect(m_inputExOk, SIGNAL(clicked(bool)), this, SLOT(onUpdateTable()));
				connect(m_btnLoadData, SIGNAL(clicked(bool)), this, SLOT(onLoadData()));
				//connect(m_btnUpdateData, SIGNAL(clicked(bool)), this, SLOT(onUpdateData()));
				connect(m_btnGenerateEx, SIGNAL(clicked(bool)), this, SLOT(onGenerateEx()));
				connect(m_btnLoadEx, SIGNAL(clicked(bool)), this, SLOT(onLoadEx()));
			}
			void BatchConfigWidget::onLoadEx() {
				QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All Files (*.*)"));
				QFile file(fileName);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					
					return;
				}
				QTextStream in(&file);
				int num = 0;
				QVector<QVector<double>> info;
				// 逐行读取文件内容
				while (!in.atEnd()) {
					QString line = in.readLine();
					QStringList stringList = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					QVector<double> lineInfo;
					
					QVector<int> indexs = {4,0,5,6};
					double height = stringList[0].toDouble();
					double p = stringList[1].toDouble();
					double t = stringList[2].toDouble();
					hp[height] = p;
					ht[height] = t;
					for (int i = 0; i < indexs.size(); i++) {
						int index = indexs[i];
						if (i >= stringList.size())continue;
						bool ok;
						QString str = stringList[index];
						double number = str.toDouble(&ok);
						if (ok) {
							lineInfo.push_back(number);
						}
					}
					if (lineInfo.size() == m_tableHeader.size()) info.push_back(lineInfo);
					qDebug() << "lineInfo.size=" << lineInfo.size()<<" m_tableHeader.size()="<< m_tableHeader.size();
					qDebug() << line; // 处理每一行，例如打印到控制台
				}
				file.close(); // 关闭文件
				if (info.size() == 0) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"文件中无有效数据");
					return;
				}
				m_inputExampleNum->setText(QString::number(info.size()));
				onUpdateTable();
				for (int i = 0; i < info.size(); i++) {
					for (int j = 0; j < m_tableHeader.size(); j++) {
						m_batchTabel->item(i, j)->setText(QString::number(info[i][j]));
					}
				}
				QMessageBox::information(nullptr, u8"提示", u8"导入文件成功", QMessageBox::Ok);
				//emit SignalInstance::getInstance()->signalPrintInfo(u8"导入文件成功");
			}
			void BatchConfigWidget::initTableWidget(QTableWidget *tableWidget, QStringList headerList)
			{
				tableWidget->setColumnCount(headerList.size());
				tableWidget->setRowCount(0);

				tableWidget->setHorizontalHeaderLabels(headerList);
				tableWidget->horizontalHeader()->setStretchLastSection(false);
				tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				tableWidget->verticalHeader()->setStretchLastSection(false);
				tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
				tableWidget->setFixedSize(500, 800);

				//tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

				tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
				tableWidget->verticalHeader()->setDefaultSectionSize(50);
				//tableWidget->verticalHeader()->setMaximumSectionSize(50);

			}
			/*************************
			* @brief 生成Json配置文件
			* @note 目录格式👇
			* case_number:实例个数
			* case_running_current:当前运行到的实例
			* case_x:实例x(0-case_number)
			*************************/

			bool BatchConfigWidget::generateBatchFile()
			{
				QString batch_config_file = SignalInstance::getInstance()->getWorkSpace() + "/Batch/batchConfig.json";
				Json::Value root;
				int case_num = m_inputExampleNum->text().toInt();
				root["case_number"] = case_num;
				root["case_running_current"] = 0;

				std::vector<Json::Value> _case(case_num);
				// 先收集所有参数行并检查重复
				for (int i = 0; i < case_num; i++)
				{
					// 检查是否有空白单元格
					for (int j = 0; j < m_tableHeader.size(); j++)
					{
						if (m_batchTabel->item(i, j) == nullptr || m_batchTabel->item(i, j)->text().isEmpty())
						{
							QMessageBox::information(this, u8"提示", u8"请将所有参数填写完成");
							return false;
						}
						std::string key = m_tableHeader[j].toStdString();
						float value = m_batchTabel->item(i, j)->text().toFloat();
						_case[i][key] = value;
					}

					// 检查与之前的参数是否完全相同
					for (int j = 0; j < i; j++)
					{
						bool isSame = true;
						for (const auto& key : m_tableHeader)
						{
							std::string k = key.toStdString();
							if (_case[i][k] != _case[j][k])
							{
								isSame = false;
								break;
							}
						}
						if (isSame)
						{
							QMessageBox::information(this, u8"提示", QString(u8"第%1行与第%2行参数完全相同，请修改").arg(i + 1).arg(j + 1));
							return false;
						}
					}

					// 设置其他固定字段
					_case[i]["last_run_time"] = "2000-00-00";
					_case[i]["status"] = processStatus::IS_FREE;
					_case[i]["finish_head"] = false;
					_case[i]["finish_body"] = false;
					std::string caseI = "case_" + std::to_string(i);
					root[caseI] = _case[i];
				}

				QDir batchDir(SignalInstance::getInstance()->getWorkSpace() + "/Batch");
				QFile batchConfigDir(batch_config_file);
				qDebug() << "batchConfigDir=" << batch_config_file;
				if (!batchConfigDir.exists()) {
					if (!batchDir.exists())
						batchDir.mkpath(SignalInstance::getInstance()->getWorkSpace() + "/Batch");
				}
				else
				{
					QMessageBox::information(this, u8"提示", u8"批量配置文件已存在");
					return false;
				}

				Json::StyledWriter sw;
				std::ofstream os;
				os.open(batch_config_file.toLocal8Bit().toStdString());
				os << sw.write(root);
				os.close();

				return true;
			}

			/**********************
			* @brief 生成对应的参数文件夹并对资源进行复制
			* @note 文件夹名称Ma%d_H%d_A_%d_B%d
			* @note 所需资源head文件夹+body文件夹(复制单个运行的)
			* @note 将对应Ma、H、AOA、AOB进行替换
			**********************/
			void BatchConfigWidget::generateBatchDirs()
			{
				QDir dir(SignalInstance::getInstance()->getWorkSpace() + "/Batch");

				QString fromDir = SignalInstance::getInstance()->getWorkSpace();
				int headerCount = m_tableHeader.size();

				// 构建格式化字符串模板，例如："Header1%1_Header2%2_Header3%3"
				QString dirNameTemplate;
				for (int j = 0; j < headerCount; j++) {
					dirNameTemplate += m_tableHeader[j] + "%" + QString::number(j + 1);
					if (j != headerCount - 1) {
						dirNameTemplate += "_";
					}
				}

				for (int i = 0; i < m_inputExampleNum->text().toInt(); i++) {
					// 使用带格式化参数的arg()链式调用
					QString dirName = dirNameTemplate;
					for (int j = 0; j < headerCount; j++) {
						// 将表格内容转换为数值，并按格式填充：宽度0，浮点数格式，保留5位小数
						double value = m_batchTabel->item(i, j)->text().toDouble();
						dirName = dirName.arg(value, 0, 'f', decimal);
					}

					dir.mkdir(dirName);

					QFile file(dir.path() + "/" + dirName + "/log.dat");
					if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
						qDebug() << "Failed to open the file for writing.";
						qDebug() << dir.path() + "/" + dirName + "/log.dat";
						return;
					}
					file.close();

					copyBatchRes(fromDir, dir.path() + "/" + dirName);///< 拷贝计算资源
				}
			}

			void BatchConfigWidget::copyBatchFiles(QString toPath) {

			}
			//传入workspace地址以及批量计算参数
			void BatchConfigWidget::copyBatchRes(const QString& fromDir, const QString& toPath)
			{
				QString fromHeadDir = fromDir + "/head";
				QString fromBodyDir = fromDir + "/body";
				QDir dir;
				if (nsFlag == 0) {
					//只有完整网格再区分
					dir.mkpath(toPath + "/head");
					dir.mkpath(toPath + "/body");
				}
				QVector<QString> fileHeadNames = { "/headconfig.dat" };

				if (nsFlag == 0) {
					for (auto fileName : fileHeadNames) {
						QFile::copy(fromHeadDir + fileName, toPath + "/head" + fileName);
					}
					QVector<QString> fileBodyNames = { "/input.dat" };

					for (auto fileName : fileBodyNames) {
						QString bodyPath = fromBodyDir + fileName;
						BodyPathChecker tempChecker;
						QString truePath = tempChecker.checkAndGetPath(bodyPath);
						if (truePath == bodyPath) {
							QFile::copy(fromBodyDir + fileName, toPath + "/body" + fileName);
						}
						else {
							QFile::copy(truePath, toPath + fileName);
						}

					}
				}
				else {
					//分离网格，直接复制input.dat
					QVector<QString> fileBodyNames = { "/input.dat" };

					for (auto fileName : fileBodyNames) {
						QString bodyPath = fromDir + fileName;
						BodyPathChecker tempChecker;
						QString truePath = tempChecker.checkAndGetPath(bodyPath);
						if (truePath == bodyPath) {
							QFile::copy(fromDir + fileName, toPath + fileName);
						}
						else {
							QFile::copy(truePath, toPath + fileName);
						}

					}
				}
				
				


			}
			void BatchConfigWidget::onUpdateTable()
			{
				int inputNum = m_inputExampleNum->text().toInt();
				if (m_inputExampleNum->text() == "") return;
				if ((inputNum > m_maxNum) || (inputNum == 0))
				{
					QMessageBox::information(this, u8"提示", QString(u8"批量处理数量支持(0-%1),请重新输入!").arg(m_maxNum));
					return;
				}
				m_batchTabel->clearContents();
				m_batchTabel->setRowCount(inputNum);

				// 从ParamSetDialog获取参数，处理空值情况
				QString maStr = "0";       // 默认值
				QString aoaStr = "0";      // 默认值
				QString aobStr = "0";      // 默认值
				if (m_paramSetDialog != nullptr) {
					// 若返回空字符串，则使用默认值；否则使用获取到的值
					QString tempMa = m_paramSetDialog->GetMa();
					if (!tempMa.isEmpty()) {
						maStr = tempMa;
					}

					QString tempAOA = m_paramSetDialog->GetAOA();
					if (!tempAOA.isEmpty()) {
						aoaStr = tempAOA;
					}

					QString tempAOB = m_paramSetDialog->GetAOB();
					if (!tempAOB.isEmpty()) {
						aobStr = tempAOB;
					}
				}
				// H列固定默认值为0（字符串形式，与表格委托兼容）
				QString hStr = "0";

				// 填充表格
				for (int i = 0; i < inputNum; i++) {
					m_batchTabel->setItem(i, 0, new QTableWidgetItem(maStr));   // Ma列
					m_batchTabel->setItem(i, 1, new QTableWidgetItem(hStr));    // H列
					m_batchTabel->setItem(i, 2, new QTableWidgetItem(aoaStr));  // AOA列
					m_batchTabel->setItem(i, 3, new QTableWidgetItem(aobStr));  // AOB列
				}
				qDebug() << m_tableHeader.size() << '\n';
				m_batchTabel->update();
			}
			/*void BatchConfigWidget::onUpdateTable()
			{
				int inputNum = m_inputExampleNum->text().toInt();
				if (m_inputExampleNum->text() == "")return;
				if ((inputNum > m_maxNum) || (inputNum == 0))
				{
					QMessageBox::information(this, u8"提示", QString(u8"批量处理数量支持(0-%1),请重新输入!").arg(m_maxNum));
					return;
				}
				m_batchTabel->clearContents();
				m_batchTabel->setRowCount(inputNum);
				for (int i = 0; i < inputNum; i++) {
					for (int j = 0; j < m_tableHeader.size() - 2; j++) {
						m_batchTabel->setItem(i, j, new QTableWidgetItem(QString::number(i + 1)));
					}
					m_batchTabel->setItem(i, 2, new QTableWidgetItem(QString::number(0)));
					m_batchTabel->setItem(i, 3, new QTableWidgetItem(QString::number(0)));
				}
				qDebug() << m_tableHeader.size() << '\n';
				m_batchTabel->update();
			}*/
			//加载数据
			void BatchConfigWidget::onLoadData()
			{
				QString batch_config_file = SignalInstance::getInstance()->getWorkSpace() + "/Batch/batchArgConfig.json";
				//const char* batch_config_file = "batchConfig.json";
				Json::Reader json_reader;
				Json::Value root;
				std::ifstream infile(batch_config_file.toLocal8Bit().toStdString());
				if (!infile.is_open())
				{
					QMessageBox::information(this, u8"提示", u8"文件打开失败，请检查文件是否存在!");
					return;
				}
				if (json_reader.parse(infile, root))
				{
					int case_num = root["case_number"].asInt();
					m_inputExampleNum->setText(QString::number(case_num));
					m_batchTabel->clearContents();
					m_batchTabel->setRowCount(case_num);
					for (int i = 0; i < case_num; i++)
					{
						std::string caseName = "case_" + std::to_string(i);
						Json::Value _case = root[caseName];
						for (int j = 0; j < m_tableHeader.size(); j++) {
							std::string key = m_tableHeader[j].toStdString();
							float value = _case[key].asFloat();
							m_batchTabel->setItem(i, j, new QTableWidgetItem(QString::number(value)));
						}

					}
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"成功加载" + QString::number(case_num) + u8"个实例");
					//QMessageBox::information(this, u8"提示", u8"成功加载" + QString::number(case_num) + u8"个实例");
				}

				infile.close();
			}
			
			bool BatchConfigWidget::updateInput() {
				// 移除原有的updateHeadInput和updateBodyInput调用，改为使用updateBatchArgs
				QDir batchDir(SignalInstance::getInstance()->getWorkSpace() + "/Batch");
				int caseNum = m_inputExampleNum->text().toInt();

				for (int i = 0; i < caseNum; ++i) {
					// 构建与ParamEditDialog一致的目录名称格式
					double ma = m_batchTabel->item(i, 0)->text().toDouble();
					double h = m_batchTabel->item(i, 1)->text().toDouble();
					double aoa = m_batchTabel->item(i, 2)->text().toDouble();
					double aob = m_batchTabel->item(i, 3)->text().toDouble();

					QString dirName = QString("Ma%1_H%2_AOA%3_AOB%4")
						.arg(ma, 0, 'f', decimal)
						.arg(h, 0, 'f', decimal)
						.arg(aoa, 0, 'f', decimal)
						.arg(aob, 0, 'f', decimal);
					QString targetDirPath = batchDir.path() + "/" + dirName;

					// 调用updateBatchArgs中的方法更新文件
					QString inpPath = SignalInstance::getWorkSpace()+"/"+SignalInstance::getInpName();
					QString grdPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getGridName();
					//通过判断目录来看是否是fullFlag
					if (ns_flag == 0) {
						//如果是完整网格
						updateArg->setFullFlag(1);
					}
					else {
						updateArg->setFullFlag(0);
					}
					
					updateArg->setInpAndGrd(inpPath, grdPath);
					updateArg->updateInputDat(targetDirPath, h, ma, aoa, aob);
				}

				return true;
			}
			//生成算例
			void BatchConfigWidget::onGenerateEx()
			{
				//生成批量计算信息
				if (!generateBatchFile()) {
					//QMessageBox::information(this, u8"提示", u8"请检查是否有参数空白或重复,或者当前已进行批量设置");
					return;
				}
				//生成批量计算各个文件夹
				generateBatchDirs();
				//update Args
				updateInput();
				emit SignalInstance::getInstance()->signalBatchDone();
				//QMessageBox::information(this, u8"提示", u8"生成实例成功!");

				emit SignalInstance::getInstance()->signalUpdateBatchTable();

				this->close();


			}
		}
	}
}
