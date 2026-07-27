#include "batchProcess.h"
#include<QDate>
#include<computeLPCWSTR.h>
extern QString appComDir;
namespace platforms {
	namespace sspns {
		/*LPCWSTR static stringToLPCWSTR(std::string orig)
		{
			size_t origsize = orig.length() + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
			mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

			return wcstring;
		}*/
		template <typename valueType>
		valueType batchProcess::getJson(int index, std::string json_key) {
			std::string caseI = "case_" + std::to_string(index);
			return jsonData[caseI][json_key];
		}
		template <>
		int batchProcess::getJson(int index, std::string json_key) {
			std::string caseI = "case_" + std::to_string(index);
			return jsonData[caseI][json_key].asInt();
		}
		template <>
		bool batchProcess::getJson(int index, std::string json_key) {
			std::string caseI = "case_" + std::to_string(index);
			return jsonData[caseI][json_key].asBool();
		}
		template <>
		float batchProcess::getJson(int index, std::string json_key) {
			std::string caseI = "case_" + std::to_string(index);
			return jsonData[caseI][json_key].asFloat();
		}
		batchProcess::batchProcess(int _nsFlag) {
			nsFlag = _nsFlag;
			m_process = new QProcess;
			isBatchRunning = false;

			m_currentSize = 0;
			batchIndex = 0;
			initConnection();

			QString batch_config_file = SignalInstance::getInstance()->getWorkSpace() + "/Batch/batchConfig.json";
			auto lpwstr = stringToLPCWSTR(batch_config_file.toLocal8Bit().toStdString());
			hFile = CreateFileW(
				lpwstr,          // 文件名
				GENERIC_READ | GENERIC_WRITE,            // 访问模式
				FILE_SHARE_READ,                       // 共享模式
				NULL,                    // 安全属性
				OPEN_EXISTING,           // 创建方式
				FILE_ATTRIBUTE_NORMAL,   // 文件属性
				NULL                     // 模板文件句柄
			);
			//qDebug() << lpwstr;
			if (hFile == INVALID_HANDLE_VALUE) {
				DWORD dwError = GetLastError();
				qDebug() << u8"无法打开文件，错误代码：" << dwError;
				std::exit(EXIT_FAILURE);
				//return 1;
			}

			// 创建文件映射对象
			hFileMapping = CreateFileMapping(
				hFile,                   // 文件句柄
				NULL,                    // 安全属性
				PAGE_READWRITE,           // 保护模式
				0,                       // 最大对象大小（高位）
				0,                       // 最大对象大小（低位）
				NULL                     // 映射对象名称
			);

			if (hFileMapping == NULL) {
				qDebug() << u8"无法创建文件映射对象。";
				CloseHandle(hFile);
				std::exit(EXIT_FAILURE);
				//return 1;
			}

			// 将文件映射到进程的地址空间
			pFileView = MapViewOfFile(
				hFileMapping,            // 映射对象句柄
				FILE_MAP_ALL_ACCESS,           // 访问模式
				0,                       // 偏移量高位
				0,                       // 偏移量低位
				0                        // 要映射的字节数
			);
			if (pFileView == NULL) {
				qDebug() << u8"无法映射文件视图。";
				CloseHandle(hFileMapping);
				CloseHandle(hFile);
				std::exit(EXIT_FAILURE);
			}
			char* pData = static_cast<char*>(pFileView);
			/*qDebug() << u8"文件内容：";
			qDebug() << pData ;*/
			fileSize = GetFileSize(hFile, NULL);
			// 解析 JSON 数据
			// 解析 JSON 数据
			Json::CharReaderBuilder readerBuilder;
			//Json::Value jsonData;
			std::string errors;
			std::string jsonString(pData, fileSize);
			std::istringstream jsonStream(jsonString);
			//Json::StreamWriterBuilder writerBuilder;

			if (!Json::parseFromStream(readerBuilder, jsonStream, &jsonData, &errors)) {
				qDebug() << u8"JSON 解析失败: ";
				std::exit(EXIT_FAILURE);
			}

			m_poolSize = 3;
			m_size = jsonData["case_number"].asInt();
			m_threadIndex = QVector<int>(m_size, -1);
			m_logWidget = QVector<logWidget*>(m_size, nullptr);
			for (int i = 0; i < m_logWidget.size(); i++) {
				m_logWidget[i] = new logWidget(SignalInstance::getWorkSpace() + "/Batch/" + getPath(i) + "/log.dat");
				qDebug() << "path=" << SignalInstance::getWorkSpace() + "/Batch/" + getPath(i) + "/log.dat";
			}

		}

		QString batchProcess::getProcessName(int threadId) {
			return getPath(threadId);
			QString str;
			QStringList qlist;
			QVector<float> numList;
			qlist << "Ma" << "H" << "AOA" << "AOB";
			for (int j = 0; j < qlist.size(); j++)
			{
				str += qlist[j] + QString::number(getJson<float>(threadId, qlist[j].toStdString())) + "_";
				numList.push_back(getJson<float>(threadId, qlist[j].toStdString()));
			}
			int decimal = 1;
			QString dirName = QString("Ma%1_H%2_AOA%3_AOB%4")
				.arg(numList[0], 0, 'f', decimal)
				.arg(numList[1], 0, 'f', decimal)
				.arg(numList[2], 0, 'f', decimal)
				.arg(numList[3], 0, 'f', decimal);
			qDebug() << "dirName="<<dirName;
			return dirName;
		}
		QString batchProcess::statusContent(int threadId) {
			QString str;
			std::string caseI = "case_" + std::to_string(threadId);
			Json::Value _case = jsonData[caseI];
			int statusValue = _case["status"].asInt();
			if (statusValue) {

				str = QString(statusName[statusValue]);
			}
			else {

				bool flagHead = _case["finish_head"].asBool(), flagBody = _case["finish_body"].asBool();
				if (nsFlag == 0) {
					if (flagHead&&flagBody) str = QString(u8"全部运行完成");
					else if (!flagHead&&flagBody)str = QString(u8"身部运行完成");
					else if (flagHead && !flagBody) str = QString(u8"头部运行完成");
					else if (!flagHead && !flagBody) str = QString(u8"都未运行");
				}
				else {
					if (flagBody)str = QString(u8"pns运行成功");
					else {
						auto judgeRun = [&]()->bool {
							QString filePath = SignalInstance::getWorkSpace() + "/Batch/" + getPath(threadId) + "/log.dat";
							QFile file(filePath);
							if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
								qWarning() << "Cannot open file for reading:" << file.errorString();
								return false;
							}

							QTextStream in(&file);
							QString lastLine;
							while (!in.atEnd()) {
								lastLine = in.readLine();
							}
							file.close();
							if (lastLine != " All Missions Succeed!") return true;
							return false;
						};
						bool flag = false;
						flag = judgeRun();
						if (flag) {
							str = QString(u8"pns运行失败");
							//setJson(threadId, "status", processStatus::BODY_FAIL);
						}
						else str = QString(u8"未运行");
					}
				}

			}
			return str;
		}
		void batchProcess::initConnection() {
			connect(SignalInstance::getInstance(), &SignalInstance::signalJudgeRunning, this, &batchProcess::judgeRunning);
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessOp1, this, [=](int threadId) {
				int flag = checkHead(threadId);
				if (flag==-1) {
					//通过
					runHead(threadId);
				}
				else {
					QString info = QString(u8"工作区%1头部参数设置不合法，请修改参数后运行!").arg(flag+1);
					emit SignalInstance::getInstance()->signalPrintInfo(info);
				}
				
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessOp2, this, [=](int threadId) {
				int flag = checkBody(threadId);
				if (flag == -1) {
					runBody(threadId);
				}
				else {
					QString info = QString(u8"工作区%1身部参数设置不合法，请修改参数后运行!").arg(flag+1);
					emit SignalInstance::getInstance()->signalPrintInfo(info);
				}
				

			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessOp3, this, [=](int threadId) {

				closeProcess(threadId);
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessOp4, this, [=](int threadId) {
				m_logWidget[threadId]->showWidget();
				//emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalBatchRun, this, [=]() {
				int flag = checkAll();
				if (flag == -1) {
					BatchRun();
				}
				else {
					QString info = QString(u8"工作区%1参数设置不合法，无法批量运行，请修改参数后运行!").arg(flag+1);
					emit SignalInstance::getInstance()->signalPrintInfo(info);
				}
				

			});
			connect(m_process, &QProcess::readyReadStandardOutput, this, [=]() {
				QString msg = m_process->readAllStandardOutput().data();
				emit SignalInstance::getInstance()->signalOutPutLoggerAddInformation(msg);

			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalBatchCloseRun, this, &batchProcess::closeBatchRun);
			// 对于处理std::string参数的版本
			void (SignalInstance::*signalSetJsonString)(int, const std::string&, const std::string&) = &SignalInstance::signalSetJson;
			void (batchProcess::*slotSetJsonString)(int, const std::string&, const std::string&) = &batchProcess::onSetJson;
			connect(SignalInstance::getInstance(), signalSetJsonString, this, slotSetJsonString);

			// 对于处理double参数的版本
			void (SignalInstance::*signalSetJsonDouble)(int, const std::string&, double) = &SignalInstance::signalSetJson;
			void (batchProcess::*slotSetJsonDouble)(int, const std::string&, double) = &batchProcess::onSetJson;
			connect(SignalInstance::getInstance(), signalSetJsonDouble, this, slotSetJsonDouble);

			// 对于处理int参数的版本
			void (SignalInstance::*signalSetJsonInt)(int, const std::string&, int) = &SignalInstance::signalSetJson;
			void (batchProcess::*slotSetJsonInt)(int, const std::string&, int) = &batchProcess::onSetJson;
			connect(SignalInstance::getInstance(), signalSetJsonInt, this, slotSetJsonInt);

			// 对于处理bool参数的版本
			void (SignalInstance::*signalSetJsonBool)(int, const std::string&, bool) = &SignalInstance::signalSetJson;
			void (batchProcess::*slotSetJsonBool)(int, const std::string&, bool) = &batchProcess::onSetJson;
			connect(SignalInstance::getInstance(), signalSetJsonBool, this, slotSetJsonBool);
		}
		void batchProcess::closeProcess(int threadId) {
			qDebug() << "threadId=" << threadId;
			qDebug() << " m_threadIndex[threadId]=" << m_threadIndex[threadId];
			if (m_threadIndex[threadId] == -1) {
				if (currentId != threadId) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前进程未运行");
				}
				else emit SignalInstance::getInstance()->signalPrintInfo(u8"当前进程正处于批量运行队列，请终止批量运行队列");
				return;
			}
			m_processPool[m_threadIndex[threadId]]->close();
			setJson(threadId, "status", processStatus::IS_FREE);
			updateFileData();
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			m_threadIndex[threadId] = -1;

		}
		int batchProcess::findNextBatch(bool& isHead) {
			for (int threadId = batchIndex; threadId < m_size; threadId++) {
				int status = getJson<int>(threadId, "status");
				if (status) continue;
				bool headFinish = getJson<bool>(threadId, "finish_head");
				if (!headFinish&&nsFlag == 0) {
					isHead = true; return threadId;
				}
				bool bodyFinish = getJson<bool>(threadId, "finish_body");
				if (!bodyFinish) {
					isHead = false; return threadId;
				}
			}
			return -1;
		}
		void batchProcess::closeBatchRun() {
			if (!m_process->isOpen()) return;
			if (currentId == -1) return;
			m_process->disconnect();
			m_process->close();
			//emit SignalInstance::getInstance()->signalOutPutLoggerAddInformation(u8"程序关闭");
			//m_size = jsonData["case_number"].asInt();
			if (currentId < m_size) {
				setJson(currentId, "status", processStatus::IS_FREE);
				updateFileData();
				emit SignalInstance::getInstance()->signalUpdateTable(currentId, statusContent(currentId));
			}
			currentId = -1;
			batchIndex = 0;
		}
		void batchProcess::BatchRun() {

			if (m_process->isOpen()) {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"目前正在批量运行");
				return;
			}
			bool isHead = false;
			int threadId = findNextBatch(isHead);
			if (threadId == -1) {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"批量运行设置完成，目前没有待处理的进程");
				return;
			}
			batchIndex = threadId;
			currentId = threadId;
			QString _argPath;
			QString newPath = appComDir + "/help/";
			if (isHead) {
				qDebug() << "is head";
				_argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/head/";
				newPath = newPath + "ns.exe";
				setJson(threadId, "status", processStatus::IS_RUNNING_HEAD);
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));

			}
			else {

				qDebug() << "is body";
				_argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/body/";
				if (nsFlag == 1) {
					_argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/";
				}
				newPath = newPath + "pns3d.exe";
				setJson(threadId, "status", processStatus::IS_RUNNING_BODY);
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));

			}
			//auto argPath = stringToLPCWSTR(_argPath.toLocal8Bit().toStdString());
			//SetCurrentDirectory(argPath);
			QDir::setCurrent(_argPath);
			m_process->disconnect();
			connect(m_process, &QProcess::readyReadStandardOutput, this, [=]() {
				QString msg = m_process->readAllStandardOutput().data();
				//emit SignalInstance::getInstance()->signalOutPutLoggerAddInformation(msg);
				m_logWidget[threadId]->appendText(msg);
			});
			QMetaObject::Connection conn = connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {

				if (isHead) {

					batchFinishHead(threadId, exitCode, exitStatus);

				}
				else {

					batchFinishBody(threadId, exitCode, exitStatus);
					BatchRun();
				}

			});
			m_process->start(newPath);
		}
		void batchProcess::batchFinishHead(int threadId, int exitCode, QProcess::ExitStatus exitStatus) {
			QString lastLine = m_logWidget[threadId]->getLastMsg();
			if (exitStatus != QProcess::NormalExit || lastLine != " computation complete!           0") {
				m_process->close();
				//setJson(threadId, "status", processStatus::IS_FREE);
				setJson(threadId, "status", processStatus::HEAD_FAIL);
				m_logWidget[threadId]->appendText("process Head program failed!\n");
				updateFileData();
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
				QDate currentDate = QDate::currentDate();
				setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
				//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
				batchIndex++;
				BatchRun();
				return;
			}
			m_logWidget[threadId]->appendText("process Head program finished successfully!\n");
			QString batchPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId);
			QString path = SignalInstance::getWorkSpace();
			QThread* thr = QThread::create([=](QString batchPath, QString path) {
				createBoundata cb(path.toLocal8Bit().toStdString());
				cb.setBatch(batchPath.toLocal8Bit().toStdString());
				cb.work();
			}, batchPath, path);

			connect(thr, &QThread::finished, this, [=]() {
				batchTrueHeadEnd(threadId);
			});
			thr->start();

		}
		void batchProcess::batchTrueHeadEnd(int threadId) {
			m_process->close();
			m_logWidget[threadId]->appendText("create boundata successfully!\n");
			setJson(threadId, "finish_head", true);
			setJson(threadId, "status", processStatus::IS_FREE);
			updateFileData();
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
			BatchRun();
		}
		void batchProcess::batchFinishBody(int threadId, int exitCode, QProcess::ExitStatus exitStatus) {
			m_process->close();
			QString lastLine = m_logWidget[threadId]->getLastMsg();
			if (exitStatus != QProcess::NormalExit || lastLine != " All Missions Succeed!") {
				m_logWidget[threadId]->appendText("process body program failed!");
				QDate currentDate = QDate::currentDate();
				setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
				setJson(threadId, "status", processStatus::BODY_FAIL);
				/*if (nsFlag == 0)setJson(threadId, "status", processStatus::IS_FREE);
				else {
					setJson(threadId, "status", processStatus::BODY_FAIL);
				}*/

				updateFileData();
				//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
				
				batchIndex++;
				return;
			}
			m_logWidget[threadId]->appendText("process body program finished successfully!");

			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
			setJson(threadId, "finish_body", true);
			setJson(threadId, "status", processStatus::IS_FREE);
			updateFileData();
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			
			batchIndex++;

		}
		void batchProcess::judgeRunning() {
			bool flag = false;
			if (m_process->state() == QProcess::Running) flag = true;
			for (int i = 0; i < m_processPool.size(); i++) {
				if (m_processPool[i]->state() == QProcess::Running) {
					flag = true;
				}
			}
			SignalInstance::batchFlag = flag;
		}
		bool batchProcess::checkRunning() {
			return isBatchRunning;
		}
		bool batchProcess::checkSingleRunning() {
			return isSingleRunnning;
		}
		QString batchProcess::getPath(int threadId) {
			QString dirName;
			QStringList argName;
			argName << "Ma" << "H" << "AOA" << "AOB";
			for (int j = 0; j < argName.size(); j++)
			{
				dirName += argName[j] + QString::number(getJson<float>(threadId, argName[j].toLocal8Bit().toStdString())) + "_";
			}
			QString dirNameTemplate = "Ma%1_H%2_AOA%3_AOB%4";
			dirName = dirNameTemplate;
			int decimal = 1;
			for (int j = 0; j < argName.size(); j++) {
				// 将表格内容转换为数值，并按格式填充：宽度0，浮点数格式，保留5位小数
				double value = getJson<float>(threadId, argName[j].toLocal8Bit().toStdString());
				dirName = dirName.arg(value, 0, 'f', decimal);
			}
			return dirName;
		}
		void batchProcess::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
			/*if (m_currentFlag==0) processHeadFinished(exitCode, exitStatus);
			else processBodyFinished(exitCode, exitStatus);*/
		}
		void batchProcess::onSetJson(int index, const std::string& jsonKey, double value) {
			setJson(index, jsonKey, value);
			emit SignalInstance::getInstance()->signalSetJsonFinished();
		}

		void batchProcess::onSetJson(int index, const std::string& jsonKey, const std::string& value) {
			setJson(index, jsonKey, value);
			emit SignalInstance::getInstance()->signalSetJsonFinished();
		}

		void batchProcess::onSetJson(int index, const std::string& jsonKey, bool value) {
			setJson(index, jsonKey, value);
			emit SignalInstance::getInstance()->signalSetJsonFinished();
		}

		void batchProcess::onSetJson(int index, const std::string& jsonKey, int value) {
			setJson(index, jsonKey, value);
			emit SignalInstance::getInstance()->signalSetJsonFinished();
		}
		// 在batchProcess类中添加以下两个成员函数实现

		// 检查单个线程的头部配置，未通过返回threadId，通过返回-1
		int batchProcess::checkHead(int threadId) {
			// 获取头部处理的配置文件路径
			QString headConfigPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/head/headconfig.dat";
			QString restartPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/head/restart.dat";
			// 使用FileParamChecker检查配置文件有效性
			FileParamChecker paramChecker;
			if (!paramChecker.checkHeadConfigDat(headConfigPath)) {
				return threadId; // 检查未通过，返回当前threadId
			}
			//检查并修改续算标记
			paramChecker.checkRestart(restartPath, headConfigPath);
			return -1; // 检查通过
		}

		// 检查单个线程的体部配置，未通过返回threadId，通过返回-1
		int batchProcess::checkBody(int threadId) {
			// 获取体部处理的配置文件路径
			QString bodyConfigPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/body/input.dat";
			if (nsFlag == 1) {
				bodyConfigPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/input.dat";
			}
			// 使用FileParamChecker检查配置文件有效性
			FileParamChecker paramChecker;
			if (!paramChecker.checkInputDat(bodyConfigPath)) {
				return threadId; // 检查未通过，返回当前threadId
			}
			return -1; // 检查通过
		}

		// 检查所有线程的头部和体部配置，返回第一个未通过的threadId，全部通过返回-1
		int batchProcess::checkAll() {
			// 遍历所有线程ID对应的任务
			for (int threadId = 0; threadId < m_size; ++threadId) {
				if (nsFlag == 0) {//ns+pns
					// 检查头部配置，未通过则返回当前threadId
					int headResult = checkHead(threadId);
					if (headResult != -1) {
						return headResult;
					}
				}
				
				// 检查体部配置，未通过则返回当前threadId
				int bodyResult = checkBody(threadId);
				if (bodyResult != -1) {
					return bodyResult;
				}
			}
			// 所有任务的头部和体部检查均通过
			return -1;
		}
		void batchProcess::runHead(int threadId) {

			qDebug() << "threadId=" << threadId;
			//mutex.lock();
			if (!checkAppend(threadId)) {
				//mutex.unlock();
				emit SignalInstance::getInstance()->signalPrintInfo(u8"当前进程正在运行或者无法同时创建更多进程");
				return;
			}

			m_currentSize++;
			QString _argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/head/";
			setJson(threadId, "status", processStatus::IS_RUNNING_HEAD);
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			qDebug() << "argPath=" << _argPath << endl;
			//auto argPath = stringToLPCWSTR(_argPath.toLocal8Bit().toStdString());
			//SetCurrentDirectory(argPath);
			QDir::setCurrent(_argPath);
			int processId = 0;
			if (m_processPool.size() < m_poolSize) {
				m_processPool.push_back(new QProcess());
				processId = m_processPool.size() - 1;
			}
			else {
				for (int i = 0; i < m_processPool.size(); i++) {
					if (m_processPool[i]->state() == QProcess::NotRunning) {
						processId = i; break;
					}
				}
			}
			setHeadRestart(threadId);
			m_threadIndex[threadId] = processId;
			QString newPath = appComDir + "/help/";
			m_processPool[processId]->disconnect();
			QMetaObject::Connection conn = connect(m_processPool[processId], QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
				finishHead(processId, threadId, exitCode, exitStatus);
				//QObject::disconnect(conn);
			});
			connect(m_processPool[processId], &QProcess::readyReadStandardOutput, this, [=]() {
				QString msg = m_processPool[processId]->readAllStandardOutput().data();
				m_logWidget[threadId]->appendText(msg);
				//emit SignalInstance::getInstance()->signalOutPutLoggerAddInformation(msg);
			});


			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
			//启动计算任务
			m_processPool[processId]->start(newPath + "ns.exe");
			
		}
		void batchProcess::setHeadRestart(int threadId) {
			QString path = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/head/";
			QFile file(path + "restart.dat");
			if (!file.exists()) return;
			qDebug() << "setRestart";
			//writeFile(path+"headload.dat",0);
			writeFile(path + "headconfig.dat", 1);
		}
		void batchProcess::writeFile(QString path, int flag) {
			QFile file(path);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				qDebug() << "file cant open";
				qDebug() << path;
			}
			QTextStream in(&file);
			in.setCodec("UTF-8");  // 根据文件编码设置
			QStringList lines;
			while (!in.atEnd()) {
				QString line = in.readLine();
				lines.append(line);
			}
			file.close();
			if (!flag) {
				lines[14] = "cInitComboBox 1";
			}
			else {
				qDebug() << "lines.size()=" << lines.size() << " path=" << path;
				for (int i = 0; i < lines.size(); i++) {
					qDebug() << "i=" << i << " lines=" << lines[i];
				}
				lines[18][0] = '2';
			}

			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << u8"无法打开文件进行写入";
			}
			// 使用QTextStream写入修改后的内容
			QTextStream out(&file);
			for (const QString &line : lines) {
				out << line << "\n";
			}
			// 关闭文件
			file.close();
		}
		void batchProcess::finishHead(int processId, int threadId, int exitCode, QProcess::ExitStatus exitStatus) {
			QString lastLine = m_logWidget[threadId]->getLastMsg();
			if (exitStatus != QProcess::NormalExit || lastLine != " computation complete!           0") {
				m_logWidget[threadId]->appendText("process head program failed!\n");
				//setJson(threadId, "status", processStatus::IS_FREE);
				setJson(threadId, "status", processStatus::HEAD_FAIL);
				m_processPool[processId]->close();
				m_currentSize--;
				m_threadIndex[threadId] = -1;
				updateFileData();
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
				QDate currentDate = QDate::currentDate();
				setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
				//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
				
				return;
			}
			m_logWidget[threadId]->appendText("process head program successfully!\n");
			QString batchPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId);
			QString path = SignalInstance::getWorkSpace();
			QThread* thr = QThread::create([=](QString batchPath, QString path) {
				createBoundata cb(path.toLocal8Bit().toStdString());
				cb.setBatch(batchPath.toLocal8Bit().toStdString());
				cb.work();
			}, batchPath, path);

			connect(thr, &QThread::finished, this, [=]() {
				trueHeadEnd(processId, threadId);
			});

			thr->start();
		}
		void batchProcess::trueHeadEnd(int processId, int threadId) {
			m_threadIndex[threadId] = -1;
			m_currentSize--;
			m_processPool[processId]->close();
			m_logWidget[threadId]->appendText("create boundata successfully!\n");
			setJson(threadId, "finish_head", true);
			setJson(threadId, "status", processStatus::IS_FREE);
			updateFileData();
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
		}
		bool batchProcess::checkAppend(int threadId) {

			if (threadId >= m_size) return false;
			if (m_currentSize >= m_poolSize) return false;
			int currentStatus = getJson<int>(threadId, "status");
			if (currentStatus == processStatus::IS_RUNNING_BODY|| currentStatus== processStatus::IS_RUNNING_HEAD)return false;
			return true;
		}
		void batchProcess::runBody(int threadId) {
			qDebug() << "m_currentSize=" << m_currentSize;
			for (int i = 0; i < m_processPool.size(); i++) {
				qDebug() << "i=" << i << " status=" << m_processPool[i]->state();
			}
			if (!checkAppend(threadId)) {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"当前进程正在运行或者无法同时创建更多进程");
				return;
			}
			m_currentSize++;
			QString _argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/body/";
			if (nsFlag == 1) {
				//如果是pns界面，没有body文件夹
				_argPath = SignalInstance::getWorkSpace() + "/Batch/" + getProcessName(threadId) + "/";
			}
			setJson(threadId, "status", processStatus::IS_RUNNING_BODY);
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			qDebug() << "argPath=" << _argPath << endl;
			//auto argPath = stringToLPCWSTR(_argPath.toLocal8Bit().toStdString());
			//SetCurrentDirectory(argPath);
			QDir::setCurrent(_argPath);
			int processId = 0;
			if (m_processPool.size() < m_poolSize) {
				m_processPool.push_back(new QProcess());
				processId = m_processPool.size() - 1;
			}
			else {
				for (int i = 0; i < m_processPool.size(); i++) {
					if (m_processPool[i]->state() == QProcess::NotRunning) {
						processId = i; break;
					}
				}
			}
			m_threadIndex[threadId] = processId;
			QString newPath = appComDir + "/help/";
			m_processPool[processId]->disconnect();
			QMetaObject::Connection conn = connect(m_processPool[processId], QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
				//QObject::disconnect(conn);
				finishBody(processId, threadId, exitCode, exitStatus);

			});
			connect(m_processPool[processId], &QProcess::readyReadStandardOutput, this, [=]() {
				QString msg = m_processPool[processId]->readAllStandardOutput().data();
				m_logWidget[threadId]->appendText(msg);
				//emit SignalInstance::getInstance()->signalOutPutLoggerAddInformation(msg);
			});

			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
			m_processPool[processId]->start(newPath + "pns3d.exe");

		}
		void batchProcess::logAppend(int threadId, QString context) {

		}
		void batchProcess::finishBody(int processId, int threadId, int exitCode, QProcess::ExitStatus exitStatus) {
			QString lastLine = m_logWidget[threadId]->getLastMsg();
			if (exitStatus != QProcess::NormalExit || lastLine != " All Missions Succeed!") {
				m_logWidget[threadId]->appendText("process body program failed!");
				QDate currentDate = QDate::currentDate();
				setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());
				setJson(threadId, "status", processStatus::BODY_FAIL);
				/*if (nsFlag == 0)setJson(threadId, "status", processStatus::IS_FREE);
				else {
					setJson(threadId, "status", processStatus::BODY_FAIL);
				}*/
				qDebug() << "processId=" << processId << " poolSize=" << m_processPool.size();
				m_processPool[processId]->close();
				m_currentSize--;
				m_threadIndex[threadId] = -1;
				updateFileData();
				emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
				//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));
				
				return;
			}
			m_logWidget[threadId]->appendText("process body program finished successfully!");
			m_threadIndex[threadId] = -1;
			m_processPool[processId]->close();
			m_currentSize--;
			QDate currentDate = QDate::currentDate();
			setJson(threadId, "last_run_time", currentDate.toString("yyyy-MM-dd").toStdString());

			setJson(threadId, "finish_body", true);
			setJson(threadId, "status", processStatus::IS_FREE);
			updateFileData();
			emit SignalInstance::getInstance()->signalUpdateTable(threadId, statusContent(threadId));
			//emit SignalInstance::getInstance()->signalUpdateTime(threadId, currentDate.toString("yyyy-MM-dd"));

		}
		template <typename valueType>
		void batchProcess::setJson(int index, std::string json_key, valueType json_value) {
			if (index == 0) {
				qDebug() << "index=0";
			}
			std::string caseI = "case_" + std::to_string(index);
			jsonData[caseI][json_key] = json_value;
			updateFileData();
		}

		void batchProcess::updateFileData() {
			char* pData = static_cast<char*>(pFileView);
			std::string newContent = Json::writeString(writerBuilder, jsonData);
			//qDebug() << "newContent.size()=" << newContent.size() << " fileSize=" << fileSize;
			if (newContent.size() <= fileSize) {
				memcpy(pData, newContent.c_str(), newContent.size());
				// 如果新内容小于原内容，填充多余部分为空格
				memset(pData + newContent.size(), ' ', fileSize - newContent.size());
			}
			else {
				qDebug() << u8"新内容超过文件大小，无法写入。";
				std::exit(EXIT_FAILURE);
			}
		}
		void batchProcess::closeAllWindow() {
			for (int i = 0; i < m_logWidget.size(); i++) {
				m_logWidget[i]->close();
			}
		}
		batchProcess::~batchProcess() {
			if (m_process->isOpen()) {
				m_process->disconnect();
				m_process->close();
			}
			for (int i = 0; i < m_processPool.size(); i++) {
				if (m_processPool[i]->isOpen()) {
					m_processPool[i]->disconnect();
					m_processPool[i]->close();
				}
			}
			closeAllWindow();
			// 无法持久化的原因
			/*for (int i = 0; i < m_size; i++) {
				setJson(i, "status", processStatus::IS_FREE);
			}*/
			updateFileData();

			// 解除文件映射视图
			UnmapViewOfFile(pFileView);

			// 关闭文件映射对象句柄
			CloseHandle(hFileMapping);

			// 关闭文件句柄
			CloseHandle(hFile);

		}
	}
}

