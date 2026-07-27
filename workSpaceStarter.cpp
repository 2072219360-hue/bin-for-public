#include "workSpaceStarter.h"
using platforms::sspns::reveal::SceneWidget;
extern QString appComDir;
extern SceneWidget*main_sceneWidget;
extern RWThread* rwTh;
extern statusSubject* myStatus;
#define FIlEINSTANCE platforms::detail::FileOp::getInstance()

namespace platforms {

	namespace sspns {

		namespace detail {
			workSpaceStarter::workSpaceStarter(int _nsFlag) {
				m_nsFlag = _nsFlag;
				qDebug() << "init m_nsFlag=" << m_nsFlag;
			}
			void workSpaceStarter::setString(QString _dirName, QString _gridName, QString _inpName) {
				dirPath = _dirName;
				gridPath = _gridName;
				inpPath = _inpName;
			}
			bool workSpaceStarter::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
			{
				QDir sourceDir(fromDir);
				QDir targetDir(toDir);
				if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
					if (!targetDir.mkdir(targetDir.absolutePath()))
						return false;
				}

				QFileInfoList fileInfoList = sourceDir.entryInfoList();
				foreach(QFileInfo fileInfo, fileInfoList) {
					if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
						continue;

					if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */
						if (!copyDirectoryFiles(fileInfo.filePath(),
							targetDir.filePath(fileInfo.fileName()),
							coverFileIfExist))
							return false;
					}
					else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
						if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
							targetDir.remove(fileInfo.fileName());
						}

						/// 进行文件copy
						if (!QFile::copy(fileInfo.filePath(),
							targetDir.filePath(fileInfo.fileName()))) {
							return false;
						}
					}
				}
				return true;
			}
			bool workSpaceStarter::copyFile(const QString &toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit)
			{
				//qDebug() << "toDir:" << toDir << "    " << "fromFile:" << fromFile << "    " << toFile << endl;
				QDir targetDir(toDir);
				if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
					if (!targetDir.mkdir(targetDir.absolutePath()))
						return false;
				}
				if (converFileIfExit && targetDir.exists(toFile)) {
					targetDir.remove(toFile);
				}
				qDebug() << "fromFile:" << fromFile;
				/// 进行文件copy
				qDebug() << "toFile:" << toDir + "/" + toFile << endl;
				if (!QFile::copy(fromFile, toDir + "/" + toFile)) {
					return false;
				}


				return true;


			}
			QString workSpaceStarter::pathToFileName(QString path)
			{
				QString fileName = "";
				int i;
				for (i = path.size() - 1; i >= 0; i--)
				{
					if (path[i] == '/')
					{
						break;
					}
				}
				auto str = path.toLocal8Bit().toStdString();
				auto strfileName = str.substr(i + 1, str.size() - i - 1);
				fileName = QString::fromStdString(strfileName);
				qDebug() << fileName << endl;
				return fileName;

			}
			void workSpaceStarter::run() {
				
				QString currentDir = QDir::currentPath();
				qDebug() << "old cuurentDir=" << currentDir;

				QDir::setCurrent(appComDir);

				currentDir = QDir::currentPath();
				qDebug() << "new cuurentDir=" << currentDir;
				int m_nsFlag = SignalInstance::nsFlag;
				qDebug() << "run m_nsFlag=" << m_nsFlag;
				// pns界面
				if (m_nsFlag == 1) {

					SignalInstance::setWorkSpace(dirPath);

					SignalInstance::setGridName(gridPath);

					SignalInstance::setInpName(inpPath);
					QString statusFilePath = dirPath + "/status.txt";
					QFile statusFile(statusFilePath);
					bool recoverFlag = statusFile.exists();
					if (!recoverFlag) {
						// 拷贝默认文件夹body/sect
						// 需要改的是这三行，和上面一样加if判断，原本没加
						bool copyFlag=copyFile(dirPath, appComDir + "/newtemplate/default/body/input.dat", "input.dat", true);
						qDebug() << "copyFlag=" << copyFlag;
						if (!QDir(dirPath + "/sect").exists()) copyDirectoryFiles(appComDir +
							"/newtemplate/sect", dirPath + "/sect", true);
					}
					//拷贝整体网格及INP文件
					QFileInfo gridInfo(gridPath);
					QString gridName = gridInfo.fileName();
					SignalInstance::setGridName(gridName);

					QFileInfo inpInfo(inpPath);
					QString inpName = inpInfo.fileName();
					SignalInstance::setInpName(inpName);
					QString gridPath = dirPath + "/" + gridName;
					QString inpPath = dirPath + "/" + inpName;
					qDebug() << "gridPath=" << gridPath;
					qDebug() << "inpPath=" << inpPath;
					QFileInfo fileInfoGrid(gridPath), fileInfoInp(inpPath);


					if (!fileInfoGrid.exists()) {

						copyFile(dirPath, this->gridPath, pathToFileName(gridPath), true);
					}
					if (!fileInfoInp.exists())copyFile(dirPath, this->inpPath, pathToFileName(inpPath), true);
					//修改input.dat
					if (!recoverFlag) {
						QString bodyFile = dirPath + "/input.dat";
						QString trueGridPath = gridPath;
						QString trueInpPath = inpPath;
						changeConfig(bodyFile, trueInpPath, trueGridPath,  1);
					}
					emit SignalInstance::getInstance()->signalStartWorkFlow();

					return;
				}else{//ns+pns界面
					SignalInstance::setWorkSpace(dirPath);
					QString statusFilePath = dirPath + "/status.txt";
					QFile statusFile(statusFilePath);
					bool recoverFlag = statusFile.exists();

						QFileInfo gridInfo(gridPath);
						QString gridName = gridInfo.fileName();
						SignalInstance::setGridName(gridName);

						QFileInfo inpInfo(inpPath);
						QString inpName = inpInfo.fileName();
						SignalInstance::setInpName(inpName);
						if (!recoverFlag) {
							// 拷贝三个默认文件夹head/body/sect
							// 需要改的是这三行，和上面一样加if判断，原本没加
							if (!QDir(dirPath + "/head").exists()) copyDirectoryFiles(appComDir +
								"/newtemplate/default/head", dirPath + "/head", true);
							if (!QDir(dirPath + "/body").exists()) copyDirectoryFiles(appComDir +
								"/newtemplate/default/body", dirPath + "/body", true);
							if (!QDir(dirPath + "/sect").exists()) copyDirectoryFiles(appComDir +
								"/newtemplate/sect", dirPath + "/sect", true);
						}

						//拷贝整体网格及INP文件
						QString gridPath = dirPath + "/" + gridName;
						QString inpPath = dirPath + "/" + inpName;
						qDebug() << "gridPath=" << gridPath;
						qDebug() << "inpPath=" << inpPath;
						QFileInfo fileInfoGrid(gridPath), fileInfoInp(inpPath);

						if (!fileInfoGrid.exists()) { 
							qDebug() << "fileInfoGrid not exist";
							qDebug() << "dirPath=" << dirPath << " girdPath=" << gridPath << 
								" pathToFileName=" << pathToFileName(gridPath);
							qDebug() << "this->gridPath=" << this->gridPath <<
								" this->inpPath=" << this->inpPath;
							copyFile(dirPath, this->gridPath, pathToFileName(gridPath), true); 
						}
						if (!fileInfoInp.exists())copyFile(dirPath, this->inpPath, pathToFileName(inpPath), true);

						//修改input.dat和headconfig.dat
						if (!recoverFlag) {
							QString headFile = dirPath + "/head/headConfig.dat";
							QString bodyFile = dirPath + "/body/input.dat";
							//完整网格界面中参数应该是运行后的网格
							QString headInpPath = dirPath + "/" + "head_bd.inp";
							QString headGridPath = dirPath + "/" + "head_grid.grd";
							QString bodyInpPath = dirPath + "/" + "body_genericbd.inp";
							QString bodyGridPath = dirPath + "/" + "body_grid.grd";
							changeConfig(headFile, headInpPath, headGridPath,  0);
							changeConfig(bodyFile, bodyInpPath, bodyGridPath,  1);
						}
						emit SignalInstance::getInstance()->signalWorkSpaceChanged(dirPath);
						//QString gridSelect = m_fullGridSelect->text();
						main_sceneWidget->setFullGridReadFlag(true);
						if (recoverFlag) emit SignalInstance::getInstance()->signalRecover();
						else emit SignalInstance::getInstance()->signalLoadAndOut(gridPath, dirPath);
				}

			}
			void workSpaceStarter::changeConfig(QString configFile, QString inpDir, QString grdDir, int flag) {
				fileEditor.changeConfig(configFile, inpDir, grdDir, flag);
			}
			void workSpaceStarter::clearFiles() {
				// step1. 发送删除进程池的信号，并等待槽函数执行完毕
				QEventLoop loop;
				// 连接回执信号：当signalProcessPoolDeleted触发时，退出事件循环
				QMetaObject::Connection conn = QObject::connect(
					SignalInstance::getInstance(),
					&SignalInstance::signalProcessPoolDeleted, // 需在SignalInstance中新增此信号
					&loop,
					&QEventLoop::quit,
					Qt::QueuedConnection
				);
				if (!conn) {
					qDebug() << "Failed to connect signalProcessPoolDeleted to loop.quit()";
				}
				else {
					qDebug() << "Connected signalProcessPoolDeleted to loop.quit() successfully";
				}
				// 发送触发删除的信号
				emit SignalInstance::getInstance()->signalProcessPoolDelete();
				qDebug() << "Emitted signalProcessPoolDelete, wait for signalProcessPoolDeleted";

				// 启动事件循环，阻塞等待回执信号（槽函数执行完毕）
				loop.exec();
				qDebug() << "Loop exec exited";
				// 断开连接，避免内存泄漏
				QObject::disconnect(conn);

				//step2. 设置界面状态，保证后续重新打开网格后状态是NEW_WORKSPACE
				myStatus->setStatus(Status::NEW_WORKSPACE);

				//step3. 执行删除操作
				// 检查目录是否有效
				QDir targetDir(dirPath);
				if (!targetDir.exists()) {
					qDebug() << "Directory " << dirPath << " does not exist, skip clearFiles";
					return;
				}

				// 1. 删除Batch文件夹（递归删除整个文件夹及内容）
				QString batchDirPath = targetDir.filePath("Batch");
				QDir batchDir(batchDirPath);
				if (batchDir.exists()) {
					// 递归删除文件夹，先删内容再删文件夹本身
					if (!batchDir.removeRecursively()) {
						qDebug() << "Failed to delete directory: " << batchDirPath;
					}
					else {
						qDebug() << "Successfully deleted directory: " << batchDirPath;
					}
				}

				// 2. 定义需要删除的文件列表
				QStringList filesToDelete = {
					"body_genericbd.inp",
					"body_grid.grd",
					"body_xyz_ijk.dat",
					"head_bd.inp",
					"head_grid.grd",
					"head_xyz_ijk.dat",
					"TR.dat",
					"status.txt",
					"body/trData.dat"
				};

				// 3. 遍历删除每个文件
				foreach(const QString& fileName, filesToDelete) {
					QString filePath = targetDir.filePath(fileName);
					QFile file(filePath);
					if (file.exists()) {
						// 先确保文件未被占用，再删除
						if (file.remove()) {
							qDebug() << "Successfully deleted file: " << filePath;
						}
						else {
							qDebug() << "Failed to delete file: " << filePath << ", error: " << file.errorString();
						}
					}
					else {
						qDebug() << "File does not exist, skip deletion: " << filePath;
					}
				}
			}
		}
	}

}
