#include "WorkSpaceOp.h"
#include "ParamSetDialog.h"
#include<computeLPCWSTR.h>
extern QString appComDir;
extern statusSubject* myStatus;
extern RWThread* rwTh;
//extern int djk;

//LPCWSTR static stringToLPCWSTR(std::string orig)
//{
//	size_t origsize = orig.length() + 1;
//	const size_t newsize = 100;
//	size_t convertedChars = 0;
//	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
//	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
//
//	return wcstring;
//}
namespace platforms {
	namespace sspns {
		namespace reveal {
			WorkSpaceOp::WorkSpaceOp(int ns_flag)
			{
				m_nsFlag=ns_flag;
				m_dialog = new detail::WorkSpaceOpDialog(1, m_nsFlag);

				initWorkSpcaeUi();

				
				

			}

			WorkSpaceOp::~WorkSpaceOp()
			{
			}

			void WorkSpaceOp::initWorkSpcaeUi()
			{
				QPixmap pixmap = QPixmap(appComDir + "/images/SSPNS/filemanager.png");
				QString sheet = "QPushButton#btnAdd{border-image:url(" + appComDir + "/images/SSPNS/filemanager.png);}" +
					"QPushButton#btnAdd:pressed{border-image:url(" + appComDir + "/images/SSPNS/filemanager_down.png);}";
				QFrame* hframe = new QFrame(this);
				hframe->setFrameShape(QFrame::HLine);      // 设置水平方向
				hframe->setStyleSheet("QFrame{background:lightgrey;min-height:0.1px}");
				//appComDir = QCoreApplication::applicationDirPath();
				btnAdd = new QPushButton;
				btnAdd->setObjectName("btnAdd");
				//file.open(QFile::ReadOnly);//以只读的方式读取文件
				btnAdd->setStyleSheet(sheet);
				loadStyleSheet("workspace");
				//qDebug() << "sheet:" << sheet;
				tree = new QTreeView;
				tree->setObjectName("treeView");
				//fix:0524
				//model = new QDirModel;
				model = new QFileSystemModel;
				model->setRootPath(appComDir + "/workspace");
				

				//fix:0524
				tree->setModel(model);
				tree->setRootIndex(model->index(appComDir + "/workspace"));
				//tree->setSelectionMode(QAbstractItemView::MultiSelection);
				tree->setColumnHidden(1, true);
				tree->setColumnHidden(2, true);
				tree->setColumnHidden(3, true);
				tree->setExpandsOnDoubleClick(false);
				tree->setItemsExpandable(false);
				//add:0524
				//启用排序功能
				tree->setSortingEnabled(true);
				tree->sortByColumn(0, Qt::AscendingOrder);

				m_sceneIsWording = false;
				//tree->setAlternatingRowColors(true);
				m_mainLayout = new QVBoxLayout;
				btnAdd->setFixedSize(40, 40);
				//m_mainLayout->addWidget(btnAdd);
				btnBuild = new QPushButton(u8"新建工作区"); 
				btnLoad = new QPushButton(u8"打开工作区");
				btnDelete = new QPushButton(u8"删除工作区");
				btnLoadGrid = new QPushButton(u8"打开网格"); 
				btnLoadDir = new QPushButton(u8"打开目录");
				btnLayout = new QHBoxLayout;
				btnProject=new QPushButton(u8"打开项目");
				btnLayout->addWidget(btnLoadDir);
				btnLayout->addWidget(btnProject);

				m_currentWorkSpace = new QLabel(u8"当前工作区");
				m_currentWorkSpaceLineEdit = new QLineEdit;
				m_currentWorkSpaceLineEdit->setReadOnly(true);
				m_currentWorkSpaceLayout = new QHBoxLayout;
				m_currentWorkSpaceLayout->addWidget(m_currentWorkSpace);
				m_currentWorkSpaceLayout->addWidget(m_currentWorkSpaceLineEdit);
				m_mainLayout->addLayout(btnLayout);
				m_mainLayout->addWidget(hframe);
				m_mainLayout->addWidget(tree);
				m_mainLayout->addLayout(m_currentWorkSpaceLayout);
				m_mainLayout->setSpacing(2);
				this->setLayout(m_mainLayout);
				
				m_projectWidget = new projectWidget(m_nsFlag);

				initConnect();
			}
			void WorkSpaceOp::setBatchMode() {
				btnBuild->setParent(nullptr);
				disconnect(btnLoad, &QPushButton::clicked, this, &WorkSpaceOp::slotLoadWorkSpace);
				connect(btnLoad, &QPushButton::clicked, this, &WorkSpaceOp::slotCheckBatchStatus);
				

			}
			void WorkSpaceOp::slotCheckBatchStatus() {
				QModelIndex index = tree->currentIndex();
				if (!index.isValid()) {
					return;
				}
				if (m_sceneIsWording) {
					QMessageBox::information(this, u8"提示", u8"当前工作区有未完成的运行无法打开工作区", QMessageBox::Yes);
					return;
				}

				QString path = index.data().toString();
				QString str = appComDir + "/workSpace/" + path;
				qDebug() << "loadWorkSpace=" << str;
				bool flag = true;
				statusSubject status;
				status.readStatus(str.toLocal8Bit().toStdString() + "/status.txt");
				if (!status.getSectFinish()) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"所选工作区未完成分段程序，无法进行批量处理");
					return;
				}
				
				
				QDir batchDir(str + "/Batch");
				QFile batchConfigDir(str + "/Batch/batchConfig.json");
				qDebug() << "batchConfigDir=" << str + "/Batch/batchConfig.json";
				if (!batchConfigDir.exists()) {
					if (!batchDir.exists())batchDir.mkpath(str + "/Batch");
					emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区之前未进行批量处理");
					//emit SignalInstance::getInstance()->signalUpdateBatchTable();
				}
				else {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区之前进行过批量处理");
					//emit SignalInstance::getInstance()->signalUpdateBatchTable();
				}

			}
			void WorkSpaceOp::initConnect()
			{
				

				connect(btnProject, &QPushButton::clicked, this, [&]() {
					slotShowProject();
				});
				connect(btnLoadDir, &QPushButton::clicked, this, &WorkSpaceOp::slotShowDirDialog);

				connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, m_dialog, &QWidget::close);
				connect(btnBuild, &QPushButton::clicked, this, &WorkSpaceOp::slotShowDialog);
				connect(btnLoad, &QPushButton::clicked, this, &WorkSpaceOp::slotLoadWorkSpace);
				connect(btnDelete, &QPushButton::clicked, this, &WorkSpaceOp::slotDeleteWorkSpace);
				connect(SignalInstance::getInstance(), SIGNAL(signalUpdateWorkSpaceView()), this, SLOT(slotUpdateView()));
				connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(slotUpdateView()));
				connect(SignalInstance::getInstance(), SIGNAL(signalSceneWorked(bool)), this, SLOT(onChangedSceneWorkState(bool)));
				//connect(tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeViewDobuleClicked()));
				connect(tree, &QTreeView::doubleClicked, this, &WorkSpaceOp::onTreeViewDobuleClicked);
				connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(onUpdateWorkSpaceLineEdit(QString)));
			}
			
			void WorkSpaceOp::setCurrentWorkSpaceLineEdit() {
				m_currentWorkSpaceLineEdit->setText(SignalInstance::getWorkSpace());
			}
			void WorkSpaceOp::onTreeViewDobuleClicked()
			{
				return;
				if (!toggleHandle())return;
				//qDebug() << "double clicked!!";
				QModelIndex index = tree->currentIndex();
				QString path = index.data().toString();
				if (m_currentWorkSpaceLineEdit->text() == ("workSpace/" + path))
				{
					QMessageBox::information(this, u8"提示", u8"当前工作区为所选工作区!");
					return;
				}
				QString str = appComDir + "/workSpace/" + path;
				SignalInstance::getInstance()->setWorkSpace(str);
				emit SignalInstance::getInstance()->signalWorkSpaceChanged(str);
				emit SignalInstance::getInstance()->signalPreRead(str);
				m_currentWorkSpaceLineEdit->setText("workSpace/" + path);

			}

			void WorkSpaceOp::onChangedSceneWorkState(bool isWorking)
			{
				m_sceneIsWording = isWorking;
				return;
			}


			void WorkSpaceOp::loadStyleSheet(QString sheetName)
			{
				//m_styleName = sheetName;
				QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
				file.open(QFile::ReadOnly);//以只读的方式读取文件
				if (file.isOpen()) {
					setStyleSheet("");
					//将样式表全部读取，保存到字符串中
					QString qsstyleSheet = QLatin1String(file.readAll());

					setStyleSheet(qsstyleSheet);
					//qDebug() <<"qsstyleSheet:"<< qsstyleSheet;
				}
				file.close();
			}

			void WorkSpaceOp::slotUpdateView()
			{
				qDebug() << __FUNCTION__;
				//model->reset
				//fix:0524
				//model = new QDirModel;


				model = new QFileSystemModel;
				model->setRootPath(SignalInstance::getWorkSpace());


				tree->setModel(model);
				tree->setRootIndex(model->index(SignalInstance::getWorkSpace()));
				tree->setColumnHidden(1, true);
				tree->setColumnHidden(2, true);
				tree->setColumnHidden(3, true);
				//tree->header()->setSortIndicator(2, Qt::AscendingOrder); // 2 是列索引，通常表示文件类型
				//tree->header()->setSortIndicatorShown(true);
				//tree->header()->setSectionsClickable(true);
				tree->sortByColumn(0, Qt::AscendingOrder);
;

				tree->update();
				//this->update();
			}

			void WorkSpaceOp::slotLoadWorkSpace()
			{
				QModelIndex index = tree->currentIndex();
				if (!index.isValid()) {
					return;
				}
				if (m_sceneIsWording) {
					QMessageBox::information(this, u8"提示", u8"当前工作区有未完成的运行无法打开工作区", QMessageBox::Yes);
					return;
				}
				
				QString path = index.data().toString();
				QString str = appComDir + "/workSpace/" + path;
				qDebug() << "loadWorkSpace=" << str;
				myStatus->setStatus(Status::DEFAULT_STATUS);
				myStatus->Notify();
				rwTh->setStatusFlag(2);
				SignalInstance::getInstance()->setWorkSpace(str);
				//myStatus->readStatus(str.toStdString() + "/status.txt");
				emit SignalInstance::getInstance()->signalWorkSpaceChanged(str);
				emit SignalInstance::getInstance()->signalPreRead(str);
				m_currentWorkSpaceLineEdit->setText("workSpace/" + path);
				
				//emit SignalInstance::getInstance()->signalLoadFinish();
			}

			void WorkSpaceOp::slotDeleteWorkSpace() {
				QModelIndex index = tree->currentIndex();
				if (!index.isValid()) {
					return;
				}
				QString path = index.data().toString();
				QString str = appComDir + "/workSpace/" + path;
				QString currentWorkSpace = SignalInstance::getInstance()->getWorkSpace();
				auto res = QMessageBox::information(this, u8"提示", u8"是否要删除工作区:\""+ path +"\"?", QMessageBox::Yes | QMessageBox::No);
				if (res == QMessageBox::No)
				{
					return;
				}
				
				//qDebug() << "str=" << str;
				//qDebug() << "currentWorkSpace=" << currentWorkSpace;
				bool flag = false;
				if (currentWorkSpace == str) flag = true;
				/*qDebug() << "now flag=" << flag;
				qDebug() << "str=" << str;
				qDebug() << "currentWorkSpace=" << currentWorkSpace;*/
				
				if (flag) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"所选工作区为当前工作区，不可删除");
				}else {
					QDir dir(str);
					//SetCurrentDirectory(stringToLPCWSTR(appComDir.toLocal8Bit().toStdString() + "/workSpace"));
					QDir::setCurrent(appComDir+ "/workSpace");
					//if(removeDirectory(str)){
					if (dir.removeRecursively()) {
						emit SignalInstance::getInstance()->signalPrintInfo(u8"删除成功");
						slotUpdateView();
					}
					else {
						emit SignalInstance::getInstance()->signalPrintInfo(u8"删除失败,检查所选工作区是否被其他程序打开");
					}
				}

			}
			bool WorkSpaceOp::judgeWorksapce() {
				SignalInstance::batchFlag = false;
				emit SignalInstance::getInstance()->signalJudgeRunning();
				return SignalInstance::batchFlag;
			}
			void WorkSpaceOp::slotShowProject() {
				if (judgeWorksapce()) {
					auto res = QMessageBox::information(this, u8"提示", u8"当前正在批量运行,是否终止所有任务并打开新目录？", QMessageBox::Yes | QMessageBox::No);
					if (res == QMessageBox::No)
					{
						return;
					}
				}
				if (!toggleHandle()) {
					return;
				}
				m_projectWidget->close();
				m_projectWidget->openFile();
				m_projectWidget->show();
				connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, m_projectWidget, &QWidget::close);
			}
			void WorkSpaceOp::slotShowDirDialog() {
				
				//qDebug() <<  "当前页面：" << djk << '\n';
				
				//if (djk == 3)
				//{
				//	auto res = QMessageBox::information(this, u8"提示", u8"请勿在PNS求解器界面打开新目录");
				//	return ;
				//}
				
				if (judgeWorksapce()) {
					auto res = QMessageBox::information(this, u8"提示", u8"当前正在批量运行,是否终止所有任务并打开新目录？", QMessageBox::Yes | QMessageBox::No);
					if (res == QMessageBox::No)
					{
						return;
					}
				}
				if (!toggleHandle()) {
					return;
				}

				m_dialog->m_nsFlag = m_nsFlag;
				qDebug() << "m_dialog->m_nsFlag =" << m_dialog->m_nsFlag;
				m_dialog->m_projectWidget = m_projectWidget;
				//m_dialog = dialog;
				m_dialog->close();
				//m_dialog->clearText();
				m_dialog->show();
				
			}
			void WorkSpaceOp::slotShowDialog()
			{
				if (!toggleHandle())return;
			/*	detail::WorkSpaceOpDialog*dialog = new detail::WorkSpaceOpDialog;
				dialog->show();*/
				//emit SignalInstance::getInstance()->signalBuildFinish();
			}
			bool WorkSpaceOp::isExeStarted()
			{
				QProcess process;
				process.start("tasklist");
				process.waitForFinished();

				QByteArray result = process.readAllStandardOutput();
				QString str = result;
				if (str.contains("sect.exe") || str.contains("ns.exe") || str.contains("pns3d.exe"))
					return true;
				else
					return false;
			}
			bool WorkSpaceOp::killPrograms()
			{
				QProcess process;
				process.start("tasklist");
				process.waitForFinished();

				QByteArray result = process.readAllStandardOutput();
				QString str = result;
				QString strExe;
				if (str.contains("sect.exe"))
				{
					strExe = "sect.exe";
					QString strKill = QString("taskkill /im %1 /f").arg(strExe);
					if (0 != QProcess::execute(strKill)) {
						qInfo() << "killProcess error" << strExe;
						return false;
					}
					emit SignalInstance::getInstance()->finishSect();
				}
				if (str.contains("pns3d.exe"))
				{
					strExe = "pns3d.exe";
					QString strKill = QString("taskkill /im %1 /f").arg(strExe);
					if (0 != QProcess::execute(strKill)) {
						qInfo() << "killProcess error" << strExe;
						return false;
					}
					
				}
				if (str.contains("ns.exe"))
				{
					strExe = "ns.exe";
					QString strKill = QString("taskkill /im %1 /f").arg(strExe);
					if (0 != QProcess::execute(strKill)) {
						qInfo() << "killProcess error" << strExe;
						return false;
					}
					emit SignalInstance::getInstance()->bodyFinish();
				}
				return true;
			}
			bool WorkSpaceOp::toggleHandle()
			{
				/*if (m_sceneIsWording)
				{
					QMessageBox::information(this, u8"提示", u8"工作区中有未完成的任务：请停止染色后继续操作", QMessageBox::Yes);
					return false;
				}*/
				if (isExeStarted())
				{


					auto res = QMessageBox::information(this, u8"提示", u8"当前工作区有未完成的运行无法新建工作区，是否结束当前工作区的运行程序？", QMessageBox::Yes | QMessageBox::Cancel);
					if (res == QMessageBox::Yes)
					{
						if (!killPrograms())
						{
							QMessageBox::information(this, u8"提示", u8"结束运行程序错误！");
							return false;
						}
					}
					else {
						return false;
					}
				}
				return true;
			}
			void WorkSpaceOp::onUpdateWorkSpaceLineEdit(QString str)
			{
				/*QStringList list = str.split('/');
				QString path = list.at(list.size() - 1);
				m_currentWorkSpaceLineEdit->setText("workSpace/" + path);*/

				m_currentWorkSpaceLineEdit->setText(SignalInstance::getWorkSpace());
			}
		}/// reveal
	}/// sspns
}/// platforms

