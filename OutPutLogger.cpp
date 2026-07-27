#include "OutPutLogger.h"
#include "SignalInstance.h"
#include "scenewidget.h"
#include <Windows.h>
#include <QMessageBox>
#include<QThread>
#include<QTextBlock>
#include<QDir>
using platforms::sspns::reveal::SceneWidget;
using namespace platforms::sspns::reveal;
extern QString appComDir;
extern SceneWidget *gridshowviewer;
extern statusSubject* myStatus;

OutPutLogger* OutPutLogger::instance = nullptr;

LPCWSTR stringToLPCWSTR(const std::string& orig) {
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}
namespace platforms {
	namespace sspns {
		namespace reveal {
			OutPutLogger*  OutPutLogger::getInstance()
			{
				return instance;
			}

			OutPutLogger::OutPutLogger(int ns_Flag)
			{
				nsFlag = ns_Flag;
				thr = nullptr;
				init();
				this->setLayout(m_mainLayout);
			}

			OutPutLogger::~OutPutLogger()
			{
			}
			bool OutPutLogger::checkRunning() {
				if (m_processBody->state() == QProcess::Running) return true;
				if (m_processSect->state() == QProcess::Running) return true;
				if (m_process->state() == QProcess::Running) return true;
				return false;
			}
			//运行头部程序
			void OutPutLogger::exeProgramHeader(QString path)
			{

				m_workSpace = path;
				QString headerPath = path + "/head/";
				//auto setPath = stringToLPCWSTR(headerPath.toLocal8Bit().toStdString());
				//SetCurrentDirectory(setPath);
				QDir::setCurrent(headerPath);
				//m_headDir = appComDir + path;
				qDebug() << "headerPath=" << headerPath;
				QString newPath = appComDir + "/help/" ;
				/*if (m_processBody->isOpen() || m_processSect->isOpen()) {*/
				if ((m_processBody->state() == QProcess::Running) || (m_processSect->state() == QProcess::Running)) {
					bool flag1, flag2, flag3;
					flag1 = flag2 = flag3 = false;
					if (m_process->state() == QProcess::Running) flag1 = true;
					if (m_processBody->state() == QProcess::Running) flag2 = true;
					if (m_processSect->state() == QProcess::Running) flag3 = true;
					qDebug() << "m_process=" << flag1 << "m_processBody=" << flag2 << "m_processSect=" << flag3;
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前有其他程序正在运行，请先停止当前运行程序！");
					return;
				}
				/*if (!m_process->isOpen())*/
				if(!(m_process->state() == QProcess::Running))
					m_process->start(newPath + "ns.exe"); //MA1_
				else
				{
					m_process->terminate();
					m_process->close();
					m_process->start(newPath + "ns.exe");
				}
			}



			void OutPutLogger::exeProgramBody(QString path)
			{
				m_workSpace = path;
				QString bodyPath = path + "/body/";

				QFileInfo fileInfo(bodyPath);
				QString directoryPath = fileInfo.path();

				// 检查目录是否存在
				if (!QDir(directoryPath).exists()) {
					bodyPath = path + "/";
				}
				//auto setPath = stringToLPCWSTR(bodyPath.toLocal8Bit().toStdString());
				//SetCurrentDirectory(setPath);
				QDir::setCurrent(bodyPath);
				m_bodyDir = QString(appComDir + path);
				QString newPath = appComDir + "/help/";
				/*if (m_process->isOpen() || m_processSect->isOpen()) {*/
				if ((m_process->state() == QProcess::Running) || (m_processSect->state() == QProcess::Running)) {
					bool flag1, flag2, flag3;
					flag1 = flag2 = flag3 = false;
					if (m_process->state() == QProcess::Running) flag1 = true;
					if (m_processBody->state() == QProcess::Running) flag2 = true;
					if (m_processSect->state() == QProcess::Running) flag3 = true;
					qDebug() << "m_process=" << flag1 << "m_processBody=" << flag2 << "m_processSect=" << flag3;
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前有其他程序正在运行，请先停止当前运行程序！");
					return;
				}
				//if (!m_processBody->isOpen()) {
				if (!(m_processBody->state() == QProcess::Running)) {
					m_processBody->start(newPath + "pns3d.exe");
				}
				else
				{
					m_processBody->terminate();
					m_processBody->close();
					/// 设置工作目录为bodyPath
					m_processBody->start(newPath + "pns3d.exe");
				}
			}

			void OutPutLogger::closeProcess() {
				QProcess* currentProcess = nullptr;
				if (m_process->isOpen()) currentProcess = m_process;
				if (m_processBody->isOpen()) currentProcess = m_processBody;
				if (m_processSect->isOpen()) currentProcess = m_processSect;
				m_process->terminate();
				m_process->close();
				
				m_processBody->terminate();
				m_processBody->close();

				m_processSect->terminate();
				m_processSect->close();
				addInformation(u8"程序关闭\n");
				//if (currentProcess == nullptr) {
				//	addInformation(u8"当前无运行程序\n");
				//}else {
				//	//currentProcess->close();
				//	/*m_process->close();
				//	m_processBody->close();
				//	m_processSect->close();*/
				//	m_process->terminate();
				//	m_process->close();

				//	m_processBody->terminate();
				//	m_processBody->close();

				//	m_processSect->terminate();
				//	m_processSect->close();
				//	addInformation(u8"程序关闭\n");
				//}
			}

			void OutPutLogger::addInformation(QString msg) {
				//QString msg = "process head program error exit!";
				QString msgShow = m_outPutShow->toPlainText();
				msgShow += msg;
				m_outPutShow->setText(msgShow);
				m_outPutShow->moveCursor(QTextCursor::End);
				update();
			}

			void OutPutLogger::loadParameterHeader()
			{
			}

			void OutPutLogger::loadParameterBody()
			{
			}
			void OutPutLogger::initConnectionFull() {
				initConnection();
				connect(m_processSect, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedSectFull(int, QProcess::ExitStatus)));
				// TODOTEST:2025-4-22
				connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadData()));
				connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedHead(int, QProcess::ExitStatus)));
			}
			void OutPutLogger::initConnectionPns() {
				initConnection();
				connect(m_processSect, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedSectPns(int, QProcess::ExitStatus)));
			}
			void OutPutLogger::initConnection() {
				connect(m_closeBatchProcessBtn, &QPushButton::clicked, this, [=]()
				{
					emit SignalInstance::getInstance()->signalBatchCloseRun();
				});
				connect(m_clearBtn, &QPushButton::clicked, this, [=]()
				{
					m_outPutShow->clear();
				});
				connect(m_closeProcessBtn, &QPushButton::clicked, this, [=]()
				{
					emit SignalInstance::getInstance()->signalCloseProcess();
				});
				connect(SignalInstance::getInstance(), SIGNAL(signalCloseProcess()), this, SLOT(closeProcess()));
				// TODOTEST:2025-4-22
				connect(m_processSect, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadSectData()));
				//connect(m_processSect, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedSect(int, QProcess::ExitStatus)));
				// TODOTEST:2025-4-22
				connect(SignalInstance::getInstance(), SIGNAL(signalPreLoad(int)), this, SLOT(onChangeProgressBar(int)));
				connect(SignalInstance::getInstance(), SIGNAL(signalExeProgramSect(QString)), this, SLOT(exeProgramSect(QString)));

				connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, [=]() {
					m_dataProgressBar->setValue(0);
				});
				connect(SignalInstance::getInstance(), &SignalInstance::signalOutPutLoggerAddInformation, this, &OutPutLogger::addInformation);
				// TODOTEST:2025-4-22
				connect(m_processSect, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadSectData()));
				// TODOTEST:2025-4-22
				connect(m_processBody, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadBodyData()));
				connect(m_processBody, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinishedBody(int, QProcess::ExitStatus)));
			}
			void OutPutLogger::init()
			{

				getInstance();
				m_process = new QProcess;
				m_processBody = new QProcess;
				m_processSect = new QProcess;
				m_runAllprocessHead = new QProcess;
				m_runAllprocessBody = new QProcess;
				m_outPutShow = new QTextEdit;
				m_outPutShow->setReadOnly(true);
				isRunAll = false;
				m_trCheckTimer = new QTimer(this);
				m_trCheckTimer->setInterval(500);


				m_label = new QLabel(u8"运行日志输出窗口");
				m_clearBtn = new QPushButton(u8"清理");
				m_label->setFixedHeight(30);
				m_label->setAlignment(Qt::AlignCenter);
				m_clearBtn->setFixedHeight(35);

				
				m_closeProcessBtn = new QPushButton(u8"终止当前程序");
				m_closeProcessBtn->setFixedHeight(35);

				m_closeBatchProcessBtn = new QPushButton(u8"终止批量程序");
				m_closeBatchProcessBtn->setFixedHeight(35);
				m_closeBatchProcessBtn->setText(u8"终止批量运行");

				m_dataProgressBar = new QProgressBar;
				m_dataProgressBar->setRange(0, 100);
				m_dataProgressBar->setValue(0);
				//m_dataProgressBar->setFixedHeight(30);

				m_mainLayout = new QVBoxLayout;
				m_mainLayout->addWidget(m_label);
				m_mainLayout->addWidget(m_outPutShow);
				QHBoxLayout*layoutProgress = new QHBoxLayout;
				QLabel*labelLoad = new QLabel(u8"加载进度");
				layoutProgress->addWidget(labelLoad);
				layoutProgress->addWidget(m_dataProgressBar);
				layoutProgress->addWidget(m_clearBtn);
				layoutProgress->addWidget(m_closeProcessBtn);
				layoutProgress->addWidget(m_closeBatchProcessBtn);
				layoutProgress->setSpacing(5);
				//m_mainLayout->addWidget(m_clearBtn);
				m_mainLayout->addLayout(layoutProgress);

				if (nsFlag == 0) initConnectionFull();
				else initConnectionPns();
				
				

				

				
				
				m_process->setReadChannel(QProcess::StandardOutput);
				
				
				
				
				

				
				
				
			}
			void OutPutLogger::setBatchMode() {
				m_closeProcessBtn->disconnect();
				connect(m_closeProcessBtn, &QPushButton::clicked, this, [=]()
				{
					emit SignalInstance::getInstance()->signalBatchCloseRun();
				});
				m_closeProcessBtn->setText(u8"终止批量运行");
			}

			//头部运行成功,目前已经弃用。该断点永远不会触发。。
			void OutPutLogger::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
			{
				m_process->close();
				if (exitStatus == QProcess::NormalExit)
				{
					//static QThread* thr = nullptr;
					if (thr != nullptr) return;
					QString msg = "generate bounddata...please wait...\n";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					thr=QThread::create([&]() {
						{
							createBoundata* cB = new createBoundata(SignalInstance::getWorkSpace().toLocal8Bit().toStdString());
							cB->work();
							QDir dir(SignalInstance::getWorkSpace());
							if (!dir.exists()) {
								qDebug() << "Directory does not exist:";

							}
							else {
								//QVector<QString> v = { "body_xyz_ijk.dat","head_xyz_ijk.dat","head_bd_before.inp","body_genericbd_before.inp" };
								QVector<QString> v = { "head_xyz_ijk.dat","head_bd_before.inp","body_genericbd_before.inp" };
								QString filePath;
								for (auto it : v) {
									filePath = dir.filePath(it);
									QFile::remove(filePath);
								}
							}
						}
					});
					connect(thr, &QThread::finished, this, [&]() {
						qDebug() << "process head program finished!";
						if (isRunAll)
						{
							//运行exeBody
							exeProgramBody(m_workSpace);

						}
						QString msg = "process head program finished!";
						QString msgShow = m_outPutShow->toPlainText();
						msgShow += msg;
						m_outPutShow->setText(msgShow);
						m_outPutShow->moveCursor(QTextCursor::End);
						update();

						myStatus->setHeadFinish(true);
					});
					thr->start();

					

				}
				else
				{
					//程序异常结束
					QString msg = "process head program error exit!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
				}
			}
			
			void mergeAero() {
				QString fileName = "aerocoef.dat";
				QString path1 = SignalInstance::getInstance()->getWorkSpace() + "/head/" + fileName;
				QString path2 = SignalInstance::getInstance()->getWorkSpace() + "/body/aeroforce.dat" ;
				QFile file1(path1);
				if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
					qDebug() << "mergeAero无法打开文件:" << file1.errorString();
					return;
				}
				QFile file2(path2);
				if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
					qDebug() << "mergeAero无法打开文件:" << file2.errorString();
					file1.close();
					return;
				}
				QTextStream in(&file2);
				QVector<QString> info;
				QString value1, value2;
				while (!in.atEnd()) {
					value1 = in.readLine();
					if (info.size() < 7) info.push_back(value1);
					//qDebug() << line; // 打印每一行
				}
				QTextStream in2(&file1);
				while (!in2.atEnd()) {
					value2 = in2.readLine();
					//qDebug() << line; // 打印每一行
				}
				file1.close();
				file2.close();
				QStringList v1 = value1.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				QStringList v2 = value2.split(QRegExp("\\s+"), QString::SkipEmptyParts);
				qDebug() << "v1=" << value1;
				qDebug() << "v2=" << value2;
				if (v1.size()+1 != v2.size()) {
					qDebug() << "size not equal!";
					qDebug() << "size1=" << v1.size() << " size2=" << v2.size();
					return;
				}
				
				QString outPath = SignalInstance::getInstance()->getWorkSpace() + "/body/" + "new_aerocoef.dat";
				QFile file3(outPath);
				if (!file3.open(QIODevice::WriteOnly | QIODevice::Text)) {
					qDebug() << "mergeAero无法打开文件:" << file3.errorString();
					return;
				}
				QTextStream out(&file3);
				for (auto& str : info) {
					out << str<<"\n";
				}
				for (int i = 0; i < v1.size(); i++) {
					double value = v1[i].toDouble()+v2[i+1].toDouble();
					out << value << " ";
				}
				file3.close();

			}
			void OutPutLogger::processFinishedHead(int exitCode, QProcess::ExitStatus exitStatus)
			{
				m_process->close();

				if (exitStatus == QProcess::NormalExit)
				{
					mergeAero(); // 原有逻辑：合并气动数据

					// ========== 新增：从 processFinished 移植的线程创建逻辑 ==========
					if (thr != nullptr) {
						thr->quit();
						thr->wait();
						thr = nullptr;
					}
					// 打印生成boundata提示日志，暂时不提示
					QString msg = "generate bounddata...please wait...\n";
					QString msgShow = m_outPutShow->toPlainText();
					/*msgShow += msg;*/
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					// 创建线程执行 createBoundata 逻辑
					thr = QThread::create([&]() {
						createBoundata* cB = new createBoundata(SignalInstance::getWorkSpace().toLocal8Bit().toStdString());
						cB->work();
						QDir dir(SignalInstance::getWorkSpace());
						if (dir.exists()) {
							QVector<QString> v = { "head_xyz_ijk.dat","head_bd_before.inp","body_genericbd_before.inp" };
							/*QString filePath;
							for (auto it : v) {
								filePath = dir.filePath(it);
								QFile::remove(filePath);
							}*/
						}
						delete cB; // 新增：避免内存泄漏
					});
					// 线程结束后的回调（保留原有 processFinished 的逻辑 + processFinishedHead 的结果判断）
					connect(thr, &QThread::finished, this, [&]() {
						qDebug() << "process head program finished!";
						// 原有 processFinished 逻辑：如果是批量执行，触发 body 程序
						if (isRunAll)
						{
							exeProgramBody(m_workSpace);
						}

						// 原有 processFinishedHead 逻辑：解析最后一行日志判断结果
						auto getLastLine = [&]()-> QString {
							QTextCursor cursor = m_outPutShow->textCursor();
							cursor.movePosition(QTextCursor::End);
							QString lastNonEmptyLine = "";
							while (true) {
								cursor.movePosition(QTextCursor::StartOfBlock);
								QString currentLine = cursor.block().text();
								if (!currentLine.trimmed().isEmpty()) {
									lastNonEmptyLine = currentLine;
									break;
								}
								if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
									break;
								}
							}
							return lastNonEmptyLine;
						};
						QString lastLine = getLastLine();
						QString msg = "process head program finished successfully!\n";
						qDebug() << "lastLine=" << lastLine;
						bool head_success_flag = true;
						if (lastLine == " computation complete!           0") {
							msg = "process head program finished successfully!\n";
							head_success_flag = true;
						}
						else {
							msg = "process head program failed!\n";
							head_success_flag = false;
						}
						// 更新日志显示
						QString msgShow = m_outPutShow->toPlainText();
						msgShow += msg;
						m_outPutShow->setText(msgShow);
						m_outPutShow->moveCursor(QTextCursor::End);
						update();

						// 原有状态更新 + 信号触发
						myStatus->setHeadFinish(true);
						emit SignalInstance::getInstance()->headFinish(head_success_flag);

						// 线程执行完毕后释放
						thr->deleteLater();
						thr = nullptr;
					});
					thr->start();
					// ========== 新增逻辑结束 ==========

				}
				else
				{
					//程序异常结束
					QString msg = "process head program error exit!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					emit SignalInstance::getInstance()->headFinish(false);
				}
			}
			//
			//void OutPutLogger::processFinishedHead(int exitCode, QProcess::ExitStatus exitStatus)
			//{
			//	m_process->close();
			//	
			//	if (exitStatus == QProcess::NormalExit)
			//	{
			//		mergeAero();
			//		if (isRunAll) {
			//			isRunAll = false;
			//			QMessageBox::information(this, u8"提示", u8"一键运行成功！");
			//		}

			//		QString msg = "process head program finished!";
			//		QString msgShow = m_outPutShow->toPlainText();
			//		auto getLastLine = [&]()-> QString {
			//			QTextCursor cursor = m_outPutShow->textCursor();
			//			// 移动光标到文本末尾
			//			cursor.movePosition(QTextCursor::End);

			//			// 变量用于存储最后非空行的文本
			//			QString lastNonEmptyLine = "";
			//			while (true) {
			//				// 移动到当前块的开始
			//				cursor.movePosition(QTextCursor::StartOfBlock);
			//				// 获取当前块的文本
			//				QString currentLine = cursor.block().text();

			//				// 如果当前行不是空行，则保存并退出循环
			//				if (!currentLine.trimmed().isEmpty()) {
			//					lastNonEmptyLine = currentLine;
			//					break;
			//				}

			//				// 向上移动到前一个块
			//				if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
			//					// 如果已经到达文本的开头，退出循环
			//					break;
			//				}
			//			}
			//			return lastNonEmptyLine;
			//		};

			//		QString lastLine = getLastLine();
			//		qDebug() << "lastLine=" << lastLine;
			//		bool head_success_flag = true;
			//		if (lastLine == " computation complete!           0") {
			//			msg = "process head program finished successfully!\n";
			//			head_success_flag = true;
			//			//emit SignalInstance::getInstance()->signalPrintInfo(u8"身部程序运行成功");
			//		}
			//		else {
			//			msg = "process head program failed!\n";
			//			head_success_flag = false;
			//			//emit SignalInstance::getInstance()->signalPrintInfo(u8"身部程序结束，运行失败");
			//		}

			//		m_outPutShow->moveCursor(QTextCursor::End);
			//		msgShow += msg;
			//		m_outPutShow->setText(msgShow);
			//		m_outPutShow->moveCursor(QTextCursor::End);
			//		update();

			//		qDebug() << "process flag=" << m_process->state();
			//		myStatus->setHeadFinish(true);
			//		emit SignalInstance::getInstance()->headFinish(head_success_flag);
			//	}
			//	else
			//	{
			//		//程序异常结束
			//		QString msg = "process head program error exit!";
			//		QString msgShow = m_outPutShow->toPlainText();
			//		msgShow += msg;
			//		m_outPutShow->setText(msgShow);
			//		m_outPutShow->moveCursor(QTextCursor::End);
			//		update();
			//		emit SignalInstance::getInstance()->headFinish(false);
			//	}
			//}

			void OutPutLogger::processFinishedBody(int exitCode, QProcess::ExitStatus exitStatus)
			{
				m_processBody->close();
				if (exitStatus == QProcess::NormalExit)
				{
					
					mergeAero();
					if (isRunAll) {
						isRunAll = false;
						QMessageBox::information(this, u8"提示", u8"一键运行成功！");
					}

					QString msg = "process body program finished!";
					QString msgShow = m_outPutShow->toPlainText();
					auto getLastLine = [&]()-> QString{
						QTextCursor cursor = m_outPutShow->textCursor();
						// 移动光标到文本末尾
						cursor.movePosition(QTextCursor::End);

						// 变量用于存储最后非空行的文本
						QString lastNonEmptyLine="";
						while (true) {
							// 移动到当前块的开始
							cursor.movePosition(QTextCursor::StartOfBlock);
							// 获取当前块的文本
							QString currentLine = cursor.block().text();

							// 如果当前行不是空行，则保存并退出循环
							if (!currentLine.trimmed().isEmpty()) {
								lastNonEmptyLine = currentLine;
								break;
							}

							// 向上移动到前一个块
							if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
								// 如果已经到达文本的开头，退出循环
								break;
							}
						}
						return lastNonEmptyLine;
					};
					
					QString lastLine = getLastLine();
					qDebug() << "lastLine=" << lastLine;
					if (lastLine == " All Missions Succeed!") {
						msg = "process body program finished successfully!";
						//emit SignalInstance::getInstance()->signalPrintInfo(u8"身部程序运行成功");
					}else {
						msg = "process body program failed!";
						//emit SignalInstance::getInstance()->signalPrintInfo(u8"身部程序结束，运行失败");
					}

					m_outPutShow->moveCursor(QTextCursor::End);
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();

					
					myStatus->setBodyFinish(true);
					emit SignalInstance::getInstance()->bodyFinish();
				}
				else
				{
					//程序异常结束
					QString msg = "process body program error exit!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					emit SignalInstance::getInstance()->bodyFinish();
				}
			}
			void OutPutLogger::processFinishedSectPns(int exitCode, QProcess::ExitStatus exitStatus) {
				m_processSect->close();
				QString msg = "process sect program finished!";
				QString msgShow = m_outPutShow->toPlainText();
				msgShow += msg;
				m_outPutShow->setText(msgShow);
				m_outPutShow->moveCursor(QTextCursor::End);
				update();
				FIlEINSTANCE->copyFile(m_workspace, m_workspace + "/sect/TR.dat", "TR.dat");
				qDebug() << "m_workSpace=" << m_workspace;
				//auto setPath = stringToLPCWSTR(m_workspace.toLocal8Bit().toStdString());
				//SetCurrentDirectory(setPath);
				QDir::setCurrent(m_workspace);
				FIlEINSTANCE->removeDirectory(m_workspace + "/sect");

				myStatus->setStatus(Status::SEGMENT_FINISH);
				myStatus->setSectFinish(true);
				if (myStatus->getLastStatus() != NEW_WORKSPACE) myStatus->setStatus(myStatus->getLastStatus());
				else myStatus->setStatus(Status::SEGMENT_FINISH);


				emit SignalInstance::getInstance()->finishSect();
				return;
			}
			void OutPutLogger::processFinishedSectFull(int exitCode, QProcess::ExitStatus exitStatus) {
				m_processSect->close();
				if (exitStatus == QProcess::NormalExit)
				{

					QString msg = "process sect program finished!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, SignalInstance::getWorkSpace() + "/sect/TR.dat", "TR.dat");
					//auto setPath = stringToLPCWSTR(SignalInstance::getWorkSpace().toLocal8Bit().toStdString());
					//SetCurrentDirectory(setPath);
					QDir::setCurrent(SignalInstance::getWorkSpace());
					FIlEINSTANCE->removeDirectory(SignalInstance::getWorkSpace() + "/sect");
					emit SignalInstance::getInstance()->signalLoadTrData();
					myStatus->setStatus(Status::SEGMENT_FINISH);
					myStatus->setSectFinish(true);
					if (myStatus->getLastStatus() != Status::NEW_WORKSPACE&& myStatus->getLastStatus() != Status::DEFAULT_STATUS)
						myStatus->setStatus(myStatus->getLastStatus());
					else myStatus->setStatus(Status::SEGMENT_FINISH);
					return;
				}
				else
				{
					QMessageBox::information(this, u8"提示", u8"Sect程序运行失败，请检查grid文件以及inp文件");
					QString msg = "process sect program error exit!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();


					myStatus->setBodyFinish(true);
					return;
				}
			}
			void OutPutLogger::processFinishedSect(int exitCode, QProcess::ExitStatus exitStatus)
			{
				m_processSect->close();
				qDebug() << "nsFlag=" << nsFlag;
				if (nsFlag == 1) {
					QString msg = "process sect program finished!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					FIlEINSTANCE->copyFile(m_workspace, m_workspace + "/sect/TR.dat", "TR.dat");
					qDebug() << "m_workSpace=" << m_workspace;
					//auto setPath = stringToLPCWSTR(m_workspace.toLocal8Bit().toStdString());
					//SetCurrentDirectory(setPath);
					QDir::setCurrent(m_workspace);
					FIlEINSTANCE->removeDirectory(m_workspace + "/sect");
					
					myStatus->setStatus(Status::SEGMENT_FINISH);
					myStatus->setSectFinish(true);
					if (myStatus->getLastStatus() != NEW_WORKSPACE) myStatus->setStatus(myStatus->getLastStatus());
					else myStatus->setStatus(Status::SEGMENT_FINISH);


					emit SignalInstance::getInstance()->finishSect();
					return;
				}
				if (exitStatus == QProcess::NormalExit)
				{

					QString msg = "process sect program finished!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, SignalInstance::getWorkSpace()+"/sect/TR.dat", "TR.dat");
					//auto setPath = stringToLPCWSTR(SignalInstance::getWorkSpace().toLocal8Bit().toStdString());
					//SetCurrentDirectory(setPath);
					QDir::setCurrent(SignalInstance::getWorkSpace());
					FIlEINSTANCE->removeDirectory(SignalInstance::getWorkSpace()+"/sect");
					emit SignalInstance::getInstance()->signalLoadTrData();
					myStatus->setStatus(Status::SEGMENT_FINISH);
					myStatus->setSectFinish(true);
					if (myStatus->getLastStatus() != Status::NEW_WORKSPACE&& myStatus->getLastStatus()!= Status::DEFAULT_STATUS)
						myStatus->setStatus(myStatus->getLastStatus());
					else myStatus->setStatus(Status::SEGMENT_FINISH);
					return;
				}
				else
				{
					QMessageBox::information(this, u8"提示", u8"Sect程序运行失败，请检查grid文件以及inp文件");
					QString msg = "process sect program error exit!";
					QString msgShow = m_outPutShow->toPlainText();
					msgShow += msg;
					m_outPutShow->setText(msgShow);
					m_outPutShow->moveCursor(QTextCursor::End);
					update();

					
					myStatus->setBodyFinish(true);
					return;
				}
			}
			void OutPutLogger::onChangeProgressBar(int value)
			{
				m_dataProgressBar->setValue(value);

			}
			void OutPutLogger::onReadData()
			{
				/*update();
				return;*/
				qDebug() << "Attach onReadData!!!!!";
				if (m_process->state() == QProcess::NotRunning) {
					return; // 进程已终止，忽略后续数据
				}
				QString msg = m_process->readAllStandardOutput().data();
				// TODOTEST:2025-4-22
				QString msgShow = m_outPutShow->toPlainText();
				msgShow += msg;
				m_outPutShow->setText(msgShow);
				m_outPutShow->moveCursor(QTextCursor::End);
				qDebug() << "Attach onReadData over!!!!!";
				update();
			}
			
			void OutPutLogger::onReadBodyData()
			{
				/*update();
				return;*/
				qDebug() << "Attach onReadBodyData!!!!!";
				if (m_processBody->state() == QProcess::NotRunning) {
					return; // 进程已终止，忽略后续数据
				}
				QString msg = m_processBody->readAllStandardOutput().data();
				// TODOTEST:2025-4-22
				QString msgShow = m_outPutShow->toPlainText();
				msgShow += msg;
				m_outPutShow->setText(msgShow);
				m_outPutShow->moveCursor(QTextCursor::End);
				update();
				qDebug() << "Attach onReadBodyData over!!!!!";
			}
			void OutPutLogger::onReadSectData()
			{
				/*update();
				return;*/
				qDebug() << "Attach onReadSectData!!!!!";
				if (m_processSect->state() == QProcess::NotRunning) {
					return; // 进程已终止，忽略后续数据
				}
				QString msg = m_processSect->readAllStandardOutput().data();
				// TODOTEST:2025-4-22
				QString msgShow = m_outPutShow->toPlainText();
				msgShow += msg;
				m_outPutShow->setText(msgShow);
				m_outPutShow->moveCursor(QTextCursor::End);
				update();
				qDebug() << "Attach onReadSectData over!!!!!";
			}

			void OutPutLogger::exeProgramSect(QString path)
			{
				m_workspace = path;
				QString sectPath = path + "/sect/";


				//auto setPath = stringToLPCWSTR(sectPath.toLocal8Bit().toStdString());
				//SetCurrentDirectory(setPath);
				QDir::setCurrent(sectPath);
				QString newPath = appComDir + "/help/";
				if((m_process->state() == QProcess::Running) || (m_processBody->state() == QProcess::Running)) {
					bool flag1, flag2, flag3;
					flag1 = flag2 = flag3 = false;
					if (m_process->state() == QProcess::Running) flag1 = true;
					if (m_processBody->state() == QProcess::Running) flag2 = true;
					if (m_processSect->state() == QProcess::Running) flag3 = true;
					qDebug() << "m_process=" << flag1 << "m_processBody=" << flag2 << "m_processSect=" << flag3;
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前有其他程序正在运行，请先停止当前运行程序！");
					return;
				}
				if (m_processSect->state() == QProcess::Running) m_processSect->close();
				m_processSect->start(newPath + "sect.exe");
				if (m_processSect->isOpen()) {
					qDebug() << "sectBegin";
				}
				else qDebug() << "sect not begin";
				qDebug() << "temp";

			}

			void OutPutLogger::exeRunAll(QString path)
			{
				isRunAll = true;
				exeProgramHeader(path);
			}
		}
	}
}



