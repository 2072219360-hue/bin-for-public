#include "WorkSpaceOpDialog.h"
#include"fileOp.h"
using platforms::sspns::reveal::SceneWidget;
extern QString appComDir;
extern SceneWidget*main_sceneWidget;
extern RWThread* rwTh;
extern statusSubject* myStatus;
#define FIlEINSTANCE platforms::detail::FileOp::getInstance()
namespace platforms {
	namespace sspns {
		namespace detail {
			QString pathToFileName(QString path)
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
			bool WorkSpaceOpDialog::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
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
			void WorkSpaceOpDialog::initWidget()
			{
				m_dirLabel=new QLabel(u8"选择目录");
				m_dirSelect=new QLineEdit;
				m_dirSelect->setText("");
				m_btnDir=new QPushButton;
				m_btnDir->setObjectName("m_btnHeadGrid");
				m_btnDir->setFixedSize(40, 40);

				m_mainLayout = new QVBoxLayout;
				m_workNameLabel = new QLabel(u8"工作区名称:");
				m_workNameLineEdit = new QLineEdit;
				m_describeLabel = new QLabel(u8"工作区描述:");
				m_describeTextEdit = new QTextEdit;
				m_headPara = new QLineEdit;
				m_bodyPara = new QLineEdit;
				m_sectPara = new QLineEdit;
				m_btnHead = new QPushButton;
				m_btnHead->setFixedSize(40, 40);
				m_btnHead->setObjectName("m_btnHead");
				m_btnBody = new QPushButton;
				m_btnBody->setFixedSize(40, 40);
				m_btnBody->setObjectName("m_btnBody");
				m_btnSect = new QPushButton;
				m_btnSect->setFixedSize(40, 40);
				m_btnSect->setObjectName("m_btnSect");
				m_labelHead = new QLabel(u8"头部初始目录");
				m_labelBody = new QLabel(u8"身部初始目录");
				m_labelSect = new QLabel(u8"分段初始目录");
				m_btnOk = new QPushButton(u8"确定");
				m_btnCancel = new QPushButton(u8"取消");
				m_gridHeadLabel = new QLabel(u8"头部网格文件选择");
				m_btnHeadGrid = new QPushButton;
				m_btnHeadGrid->setObjectName("m_btnHeadGrid");
				m_btnHeadGrid->setFixedSize(40, 40);
				m_gridHeadSelect = new QLineEdit;
				m_gridHeadSelect->setText(appComDir + "/newtemplate/head_grid.grd");
				m_gridHeadSelect->setReadOnly(true);
				m_inpHeadLabel = new QLabel(u8"头部边界文件选择");
				m_inpHeadSelect = new QLineEdit;
				m_inpHeadSelect->setText(appComDir + "/newtemplate/head_bd.inp");
				m_inpHeadSelect->setReadOnly(true);
				m_btnHeadInp = new QPushButton;
				m_btnHeadInp->setObjectName("m_btnHeadInp");
				m_btnHeadInp->setFixedSize(40, 40);
				m_gridLabel = new QLabel(u8"身部网格文件选择");
				m_btnGrid = new QPushButton;
				m_btnGrid->setObjectName("m_btnGrid");
				m_btnGrid->setFixedSize(40, 40);
				m_gridSelect = new QLineEdit;
				m_gridSelect->setText(appComDir + "/newtemplate/body_grid.grd");
				m_gridSelect->setReadOnly(true);
				m_inpLabel = new QLabel(u8"身部边界文件选择");
				m_inpSelect = new QLineEdit;
				m_inpSelect->setText(appComDir + "/newtemplate/body_genericbd.inp");
				m_inpSelect->setReadOnly(true);
				m_btnInp = new QPushButton;
				m_btnInp->setObjectName("m_btnInp");
				m_btnInp->setFixedSize(40, 40);
				m_fullGridLabel = new QLabel(u8"完整网格文件选择");
				m_fullGridSelect = new QLineEdit;
				//m_fullGridSelect->setText(appComDir + "/newtemplate/full_grid.grd");
				m_btnFullGrid = new QPushButton;
				m_btnFullGrid->setObjectName("m_btnFullGrid");
				m_btnFullGrid->setFixedSize(40, 40);

				m_fullInpLabel = new QLabel(u8"完整边界文件选择");
				m_fullInpSelect = new QLineEdit;
				//m_fullInpSelect->setText(appComDir + "/newtemplate/full_bd.inp");
				m_btnFullInp = new QPushButton;
				m_btnFullInp->setObjectName("m_btnFullInp");
				m_btnFullInp->setFixedSize(40, 40);



				m_labelInputSelect = new QLabel(u8"网格文件选择");
				m_selectButtonGroup = new QButtonGroup;
				m_btnFullSelect = new QRadioButton(u8"完整网格");
				m_btnFullSelect->setObjectName("m_btnFullSelect");
				m_btnFullSelect->setChecked(true);
				m_btnHeadBodySelect = new QRadioButton(u8"分隔网格");
				m_btnHeadBodySelect->setObjectName("m_btnHeadBodySelect");
				m_selectButtonGroup->addButton(m_btnFullSelect);
				m_selectButtonGroup->addButton(m_btnHeadBodySelect);
				m_headBodySelectWidget = new QWidget;
				m_fullSelectWidget = new QWidget;

			}
			void WorkSpaceOpDialog::initLayout()
			{
				QHBoxLayout*layout1 = new QHBoxLayout;
				if (m_versionFlag==1) {
					layout1->addWidget(m_dirLabel);
					layout1->addWidget(m_dirSelect);
					layout1->addWidget(m_btnDir);
				}
				if (!m_versionFlag)layout1->addWidget(m_workNameLabel);
				if (!m_versionFlag)layout1->addWidget(m_workNameLineEdit);
				QHBoxLayout*layout2 = new QHBoxLayout;
				QVBoxLayout*layout6 = new QVBoxLayout;
				if (!m_versionFlag)layout6->addWidget(m_describeLabel);
				if (!m_versionFlag)layout6->addStretch();
				if(!m_versionFlag)layout2->addLayout(layout6);
				if(!m_versionFlag)layout2->addWidget(m_describeTextEdit);
				
				QHBoxLayout* gridTypeLayout = new QHBoxLayout;
				if(!m_versionFlag)gridTypeLayout->addWidget(m_labelInputSelect);
				gridTypeLayout->addWidget(m_btnFullSelect);
				//gridTypeLayout->addWidget(m_btnHeadBodySelect );
				if (!m_versionFlag)gridTypeLayout->addStretch();
				
				/****head-body*****/
				QHBoxLayout*layout3 = new QHBoxLayout;
				QHBoxLayout*layout4 = new QHBoxLayout;
				QHBoxLayout*layout7 = new QHBoxLayout;
				QHBoxLayout*layout8 = new QHBoxLayout;
				if (!m_versionFlag) {
					layout8->addWidget(m_gridHeadLabel);
					layout8->addWidget(m_gridHeadSelect);
					layout8->addWidget(m_btnHeadGrid);
				}
				
				QHBoxLayout*layout9 = new QHBoxLayout;
				if (!m_versionFlag) {
					layout9->addWidget(m_inpHeadLabel);
					layout9->addWidget(m_inpHeadSelect);
					layout9->addWidget(m_btnHeadInp);
				}
				layout3->addWidget(m_gridLabel);
				layout3->addWidget(m_gridSelect);
				layout3->addWidget(m_btnGrid);
				layout4->addWidget(m_inpLabel);
				layout4->addWidget(m_inpSelect);
				layout4->addWidget(m_btnInp);
				QVBoxLayout* m_headBodySelectLayout = new QVBoxLayout;
				m_headBodySelectLayout->addLayout(layout8);
				m_headBodySelectLayout->addLayout(layout9);
				m_headBodySelectLayout->addLayout(layout3);
				m_headBodySelectLayout->addLayout(layout4);
				m_headBodySelectWidget->setLayout(m_headBodySelectLayout);
				m_headBodySelectWidget->setVisible(false);
				/****head-body*****/
				
				/******full********/
				QVBoxLayout* m_fullSelectLayout = new  QVBoxLayout;
				QHBoxLayout* m_fullGridLayout = new QHBoxLayout;
				m_fullGridLayout->addWidget(m_fullGridLabel);
				m_fullGridLayout->addWidget(m_fullGridSelect);
				m_fullGridLayout->addWidget(m_btnFullGrid);
				QHBoxLayout* m_fullInpLayout = new QHBoxLayout;
				m_fullInpLayout->addWidget(m_fullInpLabel);
				m_fullInpLayout->addWidget(m_fullInpSelect);
				m_fullInpLayout->addWidget(m_btnFullInp);
				m_fullSelectLayout->addLayout(m_fullGridLayout);
				m_fullSelectLayout->addLayout(m_fullInpLayout);
				m_fullSelectWidget->setLayout(m_fullSelectLayout);
				/******full********/

				layout7->addWidget(m_btnOk);
				layout7->addWidget(m_btnCancel);
				m_mainLayout->addLayout(layout1);
				m_mainLayout->addLayout(layout2);
				m_mainLayout->addLayout(gridTypeLayout);
				m_mainLayout->addWidget(m_headBodySelectWidget);
				m_mainLayout->addWidget(m_fullSelectWidget);
				m_mainLayout->addLayout(layout7);

			}
			void WorkSpaceOpDialog::setBathMode() {

			}
			void WorkSpaceOpDialog::initConnection()
			{
				connect(SignalInstance::getInstance(), SIGNAL(signalReClipClear()), this, SLOT(slotReClip()));

				connect(m_btnDir, SIGNAL(clicked()), this, SLOT(slotSelectDir()));
				if(!m_versionFlag)connect(m_btnOk, SIGNAL(clicked()), this, SLOT(slotLoadDirData()));
				else connect(m_btnOk, SIGNAL(clicked()), this, SLOT(slotOK()));
				connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
				connect(m_btnGrid, SIGNAL(clicked()), this, SLOT(slotSelectGrid()));
				connect(m_btnInp, SIGNAL(clicked()), this, SLOT(slotSelectInp()));
				connect(m_btnHeadGrid, SIGNAL(clicked()), this, SLOT(slotHeadSelectGrid()));
				connect(m_btnHeadInp, SIGNAL(clicked()), this, SLOT(slotHeadSelectInp()));
				/// 完整网格和边界文件进行选取
				connect(m_btnFullGrid, SIGNAL(clicked()), this, SLOT(slotFullSelectGrid()));
				connect(m_btnFullInp, SIGNAL(clicked()), this, SLOT(slotFullSelectInp()));
				connect(m_btnFullSelect, SIGNAL(clicked(bool)), this, SLOT(slotSelectedGridType()));
				connect(m_btnHeadBodySelect, SIGNAL(clicked(bool)), this, SLOT(slotSelectedGridType()));
			}
			void WorkSpaceOpDialog::loadStyleSheet(QString sheetName)
			{
				QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
				qDebug() << appComDir + "/images/SSPNS/CSS/" + sheetName + ".css";
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
			bool WorkSpaceOpDialog::copyFile(const QString &toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit)
			{
				//qDebug() << "toDir:" << toDir << "    " << "fromFile:" << fromFile << "    " << toFile << endl;
				QDir targetDir(toDir);
				if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
					if (!targetDir.mkdir(targetDir.absolutePath()))
						return false;
				}
				QFileInfo fileInfo(toFile);
				if (converFileIfExit && targetDir.exists(toFile)) {
					targetDir.remove(toFile);
				}

				/// 进行文件copy
				qDebug() << "toFile:" << toDir + "/" + toFile << endl;
				if (!QFile::copy(fromFile, toDir + "/" + toFile)) {
					return false;
				}


				return true;


			}
			WorkSpaceOpDialog::WorkSpaceOpDialog(int versionFlag ,int nsFlag, QWidget*parent ) :m_versionFlag(versionFlag),QWidget(parent)
			{
				//m_sceneWidget = main_sceneWidget;
				m_nsFlag = nsFlag;
				init();
			}
			WorkSpaceOpDialog::~WorkSpaceOpDialog()
			{
				delete m_workNameLabel;
				delete m_workNameLineEdit;
				delete m_describeLabel;
				delete m_describeTextEdit;
				delete m_headPara;
				delete m_bodyPara;
				delete m_sectPara;
				delete m_btnHead;
				delete m_btnBody;
				delete m_btnSect;
				delete m_mainLayout;
			}
			void WorkSpaceOpDialog::slotbodyDirOpen()
			{
				QString str = QFileDialog::getExistingDirectory(this, u8"身部初始目录", appComDir + "/workspace");
				m_bodyPara->setText(str);
			}
			void WorkSpaceOpDialog::slotsectDirOpen()
			{
				QString str = QFileDialog::getExistingDirectory(this, u8"分段初始目录", appComDir + "/workspace");
				m_sectPara->setText(str);
			}
			void WorkSpaceOpDialog::clearText() {
				m_fullGridSelect->setText("");
				m_fullInpSelect->setText("");
			}
			void WorkSpaceOpDialog::slotLoadDirData()
			{
				QString dirName = m_workNameLineEdit->text();
				//先创建文件夹
				QDir dir(QDir::currentPath());
				//dir.cd(appComDir + "/workspace");
				dir.cd(appComDir + "/workSpace");
				if (m_workNameLineEdit->text() == "" || m_gridHeadSelect->text() == "" || m_inpHeadSelect->text() == "" || m_gridSelect->text() == "" || m_inpSelect->text() == "")
				{
					QMessageBox::information(this, u8"提示", u8"请填写完整的工作区信息（工作区名称/头部数据/身部数据）");
					return;
				}
				std::regex regExpress("[\\/:*?\"<>|]");
				auto strWorkName = m_workNameLineEdit->text().toLocal8Bit().toStdString();
				auto isNotValueName = std::regex_search(strWorkName, regExpress);
				if (isNotValueName)
				{
					QMessageBox::information(this, u8"提示", u8"请填写合法的工作区名称");
					return;
				}
				if (!dir.exists(dirName))
				{
					dir.mkdir(dirName);

					dir.cd(dirName);
					QFile file(dir.path() + "/README.md");
					if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
					{
						QMessageBox::critical(this, "错误", "文件加载失败，工作区未生成！", "确定");
						return;
					}
					QTextStream out(&file);//写入
					out << m_describeTextEdit->toPlainText();
					file.close();
					
						myStatus->setStatus(Status::DEFAULT_STATUS);
						myStatus->Notify();
						rwTh->setStatusFlag(1);
					

					//分隔模式
					if (m_btnHeadBodySelect->isChecked()) {
						copyDirectoryFiles(appComDir + "/newtemplate/default/head", dir.path() + "/head", true);
						copyDirectoryFiles(appComDir + "/newtemplate/default/body", dir.path() + "/body", true);
						copyDirectoryFiles(appComDir + "/newtemplate/sect", dir.path() + "/sect", true);
						//拷贝头部grd以及inp
						copyFile(dir.path(), m_gridHeadSelect->text(), pathToFileName(m_gridHeadSelect->text()), true);
						copyFile(dir.path(), m_inpHeadSelect->text(), pathToFileName(m_inpHeadSelect->text()), true);
						copyFile(dir.path(), m_gridSelect->text(), pathToFileName(m_gridSelect->text()), true);
						copyFile(dir.path(), m_inpSelect->text(), pathToFileName(m_inpSelect->text()), true);
						copyFile(dir.path(), m_gridSelect->text(), pathToFileName(m_gridSelect->text()), true);
						copyFile(dir.path(), m_inpSelect->text(), pathToFileName(m_inpSelect->text()), true);
						SignalInstance::getInstance()->setWorkSpace(dir.path());
						emit SignalInstance::getInstance()->signalWorkSpaceChanged(dir.path());
						main_sceneWidget->setFullGridReadFlag(false);
						emit SignalInstance::getInstance()->signalLoadAndOut(m_gridSelect->text(), dir.path());
						this->close();
					}
					//整体模式
					else 
					{
						// 拷贝三个默认文件夹head/body/sect
						copyDirectoryFiles(appComDir + "/newtemplate/default/head", dir.path() + "/head", true);
						copyDirectoryFiles(appComDir + "/newtemplate/default/body", dir.path() + "/body", true);
						copyDirectoryFiles(appComDir + "/newtemplate/sect", dir.path() + "/sect", true);
						// 拷贝整体网格及INP文件
						copyFile(dir.path(), m_fullGridSelect->text(), pathToFileName(m_fullGridSelect->text()), true);
						copyFile(dir.path(), m_fullInpSelect->text(), pathToFileName(m_fullInpSelect->text()), true);
						SignalInstance::getInstance()->setWorkSpace(dir.path());
						emit SignalInstance::getInstance()->signalWorkSpaceChanged(dir.path());
						QString gridSelect = m_fullGridSelect->text();
						main_sceneWidget->setFullGridReadFlag(true);
						emit SignalInstance::getInstance()->signalLoadAndOut(m_fullGridSelect->text(), dir.path());
						this->close();
					}
				}
				else
				{
					QMessageBox::information(this, u8"提示", u8"工作区" + dirName + u8"已存在", QMessageBox::Yes);
				}
			}
			bool WorkSpaceOpDialog::checkFile(QString path) {
				if (path == "") return false;
				QFileInfo fileInfo(path);
				if (!fileInfo.exists() ||!fileInfo.isFile()) return false;

				return true;
			}
			bool WorkSpaceOpDialog::checkDir(QString path) {
				if (path == "") return false;
				if (!QDir(path).exists()) return false;

				return true;
			}
			void WorkSpaceOpDialog::preWork(QString& a,QString& b,QString& c){
				//qDebug()<<"fullFalg="<<fullFlag;
				m_dirSelect->setText(SignalInstance::getWorkSpace());
				m_fullGridSelect->setText(SignalInstance::getGridName());
				m_fullInpSelect->setText(SignalInstance::getInpName());
				
				QString dirName = m_dirSelect->text();
				a = dirName;
				//workContext->setDirName(dirName);
				QDir dir(dirName);
				SignalInstance::setWorkSpace(dirName);
				QString statusFilePath = dirName + "/status.txt";
				QFile statusFile(statusFilePath);
				bool recoverFlag = statusFile.exists();

				QFileInfo gridInfo(m_fullGridSelect->text());
				QString gridName = gridInfo.fileName();
				SignalInstance::setGridName(gridName);

				QFileInfo inpInfo(m_fullInpSelect->text());
				QString inpName = inpInfo.fileName();
				SignalInstance::setInpName(inpName);
				////copy body
				//// 需要改的地方是下面这行，加上一个if判断，原本copy之前没有判断
				//if (!QDir(dir.path() + "/body").exists())
				//	copyDirectoryFiles(appComDir + "/newtemplate/default/body", dir.path() +
				//		"/body", true);
				//

				//copyDirectoryFiles(appComDir + "/newtemplate/sect", dir.path() +
				//	"/sect", true);

				//向sect文件夹中拷贝整体网格及INP文件,不能删去
				QString gridPath = dirName + "/" + gridName;
				QString inpPath = dirName + "/" + inpName;
				b = gridPath;
				c = inpPath;
				QString toDirSect = dirName + "/sect";
				qDebug() << "toDirSect=" << toDirSect << " gridPath=" << gridPath;
				FIlEINSTANCE->copyFile(toDirSect, gridPath, "grid.dat");
				FIlEINSTANCE->copyFile(toDirSect, inpPath, "genericbd.inp");

				qDebug() << "gridPath=" << gridPath;
				qDebug() << "inpPath=" << inpPath;
				QFileInfo fileInfoGrid(gridPath), fileInfoInp(inpPath);

				if (!fileInfoGrid.exists()) {
					qDebug() << "grid not exitst";
					copyFile(dir.path(), m_fullGridSelect->text(), pathToFileName(m_fullGridSelect->text()), true);
				}
				if (!fileInfoInp.exists()) { 
					qDebug() << "inp not exitst";
					copyFile(dir.path(), m_fullInpSelect->text(), pathToFileName(m_fullInpSelect->text()), true); 
				}


				qDebug() << "all finish";

				emit SignalInstance::getInstance()->signalWorkSpaceChanged(dir.path());
				main_sceneWidget->setFullGridReadFlag(false);
				
				this->close();

			}

			void WorkSpaceOpDialog::slotOK() {
				qDebug() << "slotOk";
				qDebug() << "nsFlag=" << m_nsFlag << " m_projectWidgetFlag=" << m_projectWidget->ns_flag;
				QString dirName = m_dirSelect->text();
				QDir dir(dirName);
				//dir.cd(dirName);
				if (m_gridHeadSelect->text() == "" || m_inpHeadSelect->text() == "" || m_gridSelect->text() == "" || m_inpSelect->text() == "")
				{
					QMessageBox::information(this, u8"提示", u8"请选择目录名称");
					return;
				}

				if (!checkDir(m_dirSelect->text())) {
					QMessageBox::information(this, u8"提示", u8"目录不存在");
					return;
				}
 				if (!checkFile(m_fullGridSelect->text())) {
					QMessageBox::information(this, u8"提示", u8"网格文件不存在");
					return;
				}
				if (!checkFile(m_fullInpSelect->text())) {
					QMessageBox::information(this, u8"提示", u8"inp文件不存在");
					return;
				}
				this->close();

				m_projectWidget->insertInfo(m_dirSelect->text(),m_fullGridSelect->text(), m_fullInpSelect->text());
				m_starter->setString(m_dirSelect->text(), m_fullGridSelect->text(), m_fullInpSelect->text());
				m_starter->run();

			}
			void WorkSpaceOpDialog::slotReClip() {
				QString dir_path = SignalInstance::getInstance()->getWorkSpace();
				QString grid_name = SignalInstance::getGridName();
				QString inp_name = SignalInstance::getInpName();
				m_starter->setString(dir_path, grid_name, inp_name);
				m_starter->clearFiles();
				m_starter->run();
			}
			void WorkSpaceOpDialog::slotSelectDir()
			{
				QString directory = QFileDialog::getExistingDirectory(this, u8"选择目录", SignalInstance::getWorkSpace(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
				if (directory == "") return;
				m_dirSelect->setText(directory);
				clearText();
				if (!directory.isEmpty()) {
					// 创建一个 QDir 对象
					QDir dir(directory);
					if (m_fullSelectWidget->isVisible()) {
						// 设置过滤器，只获取 .inp 文件
						QStringList filters;
						filters << "*.inp";
						// 获取符合过滤器的文件名列表
						QStringList inpFiles = dir.entryList(filters, QDir::Files);
						if (!inpFiles.isEmpty()) {
							QString str = directory + "/" + inpFiles[0];
							for (int i = 0; i < inpFiles.size(); i++) {
								str= directory + "/" + inpFiles[i];
								if (inpFiles[i] != "head_bd.inp"&&inpFiles[i] != "body_genericbd.inp") {
									break;
								}
							}
							m_fullInpSelect->setText(str);
						}
						filters.clear();
						filters << "*.grd";
						QStringList grdFiles = dir.entryList(filters, QDir::Files);
						if (!grdFiles.isEmpty()) {
							QString str = directory+"/"+grdFiles[0];
							for (int i = 0; i < grdFiles.size(); i++) {
								str = directory + "/" + grdFiles[i];
								if (grdFiles[i] != "body_grid.grd"&&grdFiles[i] != "head_grid.grd") {
									break;
								}
							}
							m_fullGridSelect->setText(str);
						}
					}
					

				}
			}
			void WorkSpaceOpDialog::slotSelectGrid()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载身部网格文件", dirPath, tr("grid(*.dat)"));
				if (fileName == "") return;
				m_gridSelect->setText(fileName);
			}
			void WorkSpaceOpDialog::slotSelectInp()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载身部边界网格文件", dirPath, tr("genericbd(*.inp)"));
				if (fileName == "") return;
				m_inpSelect->setText(fileName);
			}
			void WorkSpaceOpDialog::slotHeadSelectGrid()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载头部网格文件", dirPath, tr("grid(*.grd)"));
				m_gridHeadSelect->setText(fileName);
			}
			void WorkSpaceOpDialog::slotHeadSelectInp()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载头部边界网格文件", dirPath, tr("genericbd(*.inp)"));
				m_inpHeadSelect->setText(fileName);
			}

			void WorkSpaceOpDialog::slotFullSelectGrid()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载的完整网格文件", dirPath ,tr("genericbd(*.grd)"));
				if (fileName == "") return;
				m_fullGridSelect->setText(fileName);
			}
			void WorkSpaceOpDialog::slotFullSelectInp()
			{
				QString dirPath = m_dirSelect->text();
				if (dirPath == "") dirPath = appComDir;
				QString fileName = QFileDialog::getOpenFileName(this, u8"选择加载的完整边界文件", dirPath, tr("genericbd(*.inp)"));
				if (fileName == "") return;
				m_fullInpSelect->setText(fileName);
			}

			void WorkSpaceOpDialog::slotSelectedGridType()
			{
				QRadioButton* radioBtn = qobject_cast<QRadioButton*>(sender());
				if (radioBtn->objectName() == "m_btnFullSelect")
				{
					m_fullSelectWidget->setVisible(true);
					m_headBodySelectWidget->setVisible(false);
				}
				else if (radioBtn->objectName() == "m_btnHeadBodySelect")
				{
					m_fullSelectWidget->setVisible(false);
					m_headBodySelectWidget->setVisible(true);
				}
			}
			void WorkSpaceOpDialog::slotCancel()
			{
				close();
			}

			void WorkSpaceOpDialog::init()
			{
				initUi();
				initConnection();
			}
			void WorkSpaceOpDialog::initUi()
			{
				if (!m_versionFlag)this->setFixedSize(QSize(1200, 800));
				if (m_versionFlag == 1) {
					this->setFixedWidth(800);
				}
				initWidget();
				initLayout();
				this->setLayout(m_mainLayout);
				loadStyleSheet("workspaceDialog");

				//m_projectWidget = new projectWidget(m_nsFlag);
				m_starter = new workSpaceStarter(m_nsFlag);
			}
			void WorkSpaceOpDialog::slotheadDirOpen()
			{
				QString str = QFileDialog::getExistingDirectory(this, u8"头部初始目录", appComDir + "/workspace");
				m_headPara->setText(str);
			}
		}
	}
}