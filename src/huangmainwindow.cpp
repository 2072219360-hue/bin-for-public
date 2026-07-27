
#pragma execution_character_set("utf-8")
#include <QtGui>

#include "huangmainwindow.h"
#pragma execution_character_set("utf-8")
#include "huangglobalconfig.h"
#include <QMessageBox>
using namespace platforms;
using namespace sspns;
using namespace echv;
using platforms::sspns::reveal::InputHeader;
using platforms::sspns::reveal::InputSet;
using platforms::sspns::reveal::TrTableShow;
Widget* generateWindow;
SceneWidget*sceneWidget;
SceneWidget *gridshowviewer;
QString appComDir;
bool boolisLoaded;


bool isSwitchPage = false;
//! [0]
enum Job{Worker,Farmer,Docter,Lawyer,Armyman};


HuangzbGlobalConfig globalcfg;

//! [1]
MainWindow::MainWindow()
{
	//indexPlt = 1;
	//sspns = nullptr;
	//postgridsimplify = nullptr;
	profilingWindow = nullptr;
	connect(SignalInstance::getInstance(), SIGNAL(signalShowProfiling()), this, SLOT(slotShowProfiling()));
	connect(SignalInstance::getInstance(), SIGNAL(signalPrintInfo(QString)), this, SLOT(showRWMessage(QString)));
	currentWidgetIndex = 0;
	sspns = nullptr;
	profilingWindow = nullptr;
	batchSSPNS = nullptr;
	pns = nullptr;
	boolisLoaded = false;
	logger()->info("Constructing Main Window......");
	//globalcfg.restoreStateFromFile();
	//logger()->info(globalcfg.author);
	//logger()->info(globalcfg.release);
	//logger()->info(globalcfg.gridbgimage);
	appComDir = QCoreApplication::applicationDirPath();
    //gridshowviewer = new SceneWidget();
	stackedWidget = new QStackedWidget();
	//SSPNS*sspns = new SSPNS;
    //setCentralWidget(sspns);
	this->setObjectName("mainWindow");
	QString style = this->styleSheet();
	qDebug() <<"style:"<< style;
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    //createDockWindows();

    setWindowTitle(tr("Dock Widgets"));

    setUnifiedTitleAndToolBarOnMac(true);
	boolgridshowOptions=false;
	boolinspParamTable=false;
	boolOrbiterForDBSqlite=false;
	boolNSAppRunParam=false;

	boolPostBasicFrameShow=false;

	boolDataResultBrowser=false;

	boolMPILocalJobBrowser=false;
	boolGenerateOp = false;
	boolMakeOp = false;
	boolArguOp = false;
	boolMeshPre = false;
	boolComputeOp = false;

	boolMPILog=false;

	boolMPIclusterJobBrowser=false;
	boolMPIclusterBaseCfg=false;

	boolMPIclusterInfoBrowser=false;

	bNetworkStatus=true;
	//显示背景图片
	MyWidget* grphWidget = new MyWidget(this);
	setCentralWidget(grphWidget);
	/*QPixmap pixmap("images/splash.PNG");
	QLabel* label = new QLabel(this);
	label->setPixmap(pixmap.scaled(QSize(geometry().width(), geometry().height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	label->resize(QSize(geometry().width(), geometry().height()));
	label->lower();
	setCentralWidget(label);
	update();*/
	//this->setStyleSheet("{background-image: url(images/splash.PNG);}");
	//this->setObjectName("mainWindow");
	//this->setStyleSheet("QMainWindow#mainWindow { background-image: url(images/splash.PNG); }");


	


}
MainWindow::~MainWindow() {
	
}
void MainWindow::generateOp()
{
	//将查询的结果显示在表格中.
	////////////////////////////////////////////////////////////////////////////////////
	if (!this->boolGenerateOp)
	{
		//////////////////////////////////////////////////////////////////////////
		QDockWidget *dockRightGraph = new QDockWidget(tr("用户操作框"), this);
		dockRightGraph->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		//THuangzbWinMPIJobBrowserClass *diag = new THuangzbWinMPIJobBrowserClass(this, dockRightGraph);
		generateWindow = new Widget(dockRightGraph);
		dockRightGraph->setWidget(generateWindow);

		addDockWidget(Qt::LeftDockWidgetArea, dockRightGraph);
		//addDockWidget(Qt::LeftDockWidgetArea, dockRightGraph);
		viewMenu->addAction(dockRightGraph->toggleViewAction());
		boolGenerateOp = true;
	}

	/////////////////////////////////////////////////////////////////////////////////////
}
void MainWindow::slotMakeOp()
{
	//
}
void MainWindow::slotMeshPre()
{
}
void MainWindow::inputFileSet()
{
		//////////////////////////////////////////////////////////////////////////
		QDockWidget *dockRightGraph = new QDockWidget(tr("属性配置窗口"), this);
		dockRightGraph->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		InputSet*diag = new InputSet(dockRightGraph);
		dockRightGraph->setWidget(diag);
		
		addDockWidget(Qt::LeftDockWidgetArea, dockRightGraph);
		//addDockWidget(Qt::LeftDockWidgetArea, dockRightGraph);
		viewMenu->addAction(dockRightGraph->toggleViewAction());
		QDockWidget *dockRightGraphTable = new QDockWidget(tr("TR数据窗口显示"), this);
		TrTableShow*table = new TrTableShow(dockRightGraphTable);
		dockRightGraphTable->setWidget(table);
		addDockWidget(Qt::BottomDockWidgetArea, dockRightGraphTable);
		viewMenu->addAction(dockRightGraphTable->toggleViewAction());

}
//void MainWindow::setCentralWidget(QWidget *widget)
//{
//	QWidget * curWidget = this->takeCentralWidget();
//	if (curWidget)
//		curWidget->setParent(NULL);
//	this->setCentralWidget(widget);
//}
void MainWindow::setCentral(QWidget* widget) {
	QWidget * curWidget = this->takeCentralWidget();
		if (curWidget)
			curWidget->setParent(NULL);
		this->setCentralWidget(widget);
}
void MainWindow::slotShowPNS() {
	if (sspns != nullptr) {
		sspns->closeAllWindow();
		delete sspns;
		sspns = nullptr;
	}
	if (pns == nullptr) {

		pns = new SSPNS(1);

	}
	this->setCentral(pns);
	//this->showNormal();
	//this->showMaximized();
	return;
}

void MainWindow::slotShowSSPNS()
{

	if (pns != nullptr) {
		pns->closeAllWindow();
		delete pns;
		pns = nullptr;
	}
	if (sspns == nullptr) sspns = new SSPNS;
	this->setCentral(sspns);
}


void MainWindow::slotShowBatchSSPNS() {

	
	QWidget* widget = this->centralWidget();
	if (widget != batchSSPNS) {
		if (widget == sspns) {
			delete sspns;
			sspns = nullptr;
		}
		if (batchSSPNS != nullptr) delete batchSSPNS;

		batchSSPNS = new BatchSSPNS;
		setCentralWidget(batchSSPNS);
	}
	return;

	
	if ( batchSSPNS== nullptr)
	{
		//batchSSPNS = new BatchSSPNS(nullptr);
		batchSSPNS = new BatchSSPNS();
		viewMenu->addAction(u8"批量SSPNS窗口", this, SLOT(slotShowSSPNS()));
		stackedWidget->addWidget(batchSSPNS);
		widgetIndex["batchSSPNS"]=currentWidgetIndex++;
	}
	if (stackedWidget->currentIndex() != widgetIndex["batchSSPNS"]) SignalInstance::swapWorkSpace();
	batchSSPNS->updateWindow();
	//emit SignalInstance::getInstance()->signalPrintInfo(SignalInstance::getWorkSpace());
	qDebug() << "batchSspns index=" << widgetIndex["batchSSPNS"];
	stackedWidget->setCurrentIndex(widgetIndex["batchSSPNS"]);
}
void MainWindow::showRWMessage(QString messageInfo) {
	
	QMessageBox::information(this, "提示", messageInfo);
}
void MainWindow::slotShowECHV()
{	
	QWidget* widget = this->takeCentralWidget();
	if (widget)
	{
		//widget->setParent(nullptr);
		//widget->hide();
	}

	static ECHV* echv = nullptr;
	if (echv == nullptr)
	{
		echv = new ECHV;
		viewMenu->addAction(u8"ECHV窗口", this, SLOT(slotShowSSPNS()));
	}
	setCentralWidget(echv);
	echv->show();
}
void MainWindow::slotShowPostProcessGridSimplify()
{
	static PostGridSimplify* postgridsimplify = nullptr;
	if (postgridsimplify == nullptr)
	{
		postgridsimplify = new PostGridSimplify;
		viewMenu->addAction(u8"网格数据简化窗口", this, SLOT(slotShowPostProcessGridSimplify()));
	}
	setCentralWidget(postgridsimplify);
	
}
void MainWindow::slotClearProcessRes()
{
	emit SignalInstance::getInstance()->signalCloseProcess();
}
//! [1]

void MainWindow::closeEvent(QCloseEvent *event)
{

	globalcfg.saveStateToFile();
	switch(QMessageBox::question(this,tr("警告！"),tr("你确定要退出主程序窗口吗？"),
		QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
	{
	case QMessageBox::Ok:
		//label->setText(" Question button / Ok ");
	{/*if (profilingWindow != nullptr) {
		profilingWindow->close();
		delete profilingWindow;
		profilingWindow = nullptr;
	}*/
		if (sspns != nullptr) {
			sspns->closeAllWindow();
		}
		if (pns != nullptr) {
			pns->closeAllWindow();
		}
		if (profilingWindow != nullptr) {
			profilingWindow->close();

		}
	event->accept();
	break; }
	case QMessageBox::Cancel:
		//label->setText(" Question button / Cancel ");
		event->ignore();
		break;
	default:
		break;
	}

}

//设置主窗口背景
void MainWindow::SetBgImage(const QString &strPath)
{
	logger()->info("MainWindow.SetBgImage");
	QPixmap objPixmap(strPath);
	QPalette palette = this->palette();
	if(strPath.isEmpty())
	{
		palette.setBrush(QPalette::Base, QBrush(QColor(0,0,255)));
	}
	else
	{
		palette.setBrush(QPalette::Base, QBrush(objPixmap.scaled(width(),height())));
	}
	setPalette(palette);
}

void MainWindow::loadStyleSheet(QString sheetName)
{
	//m_styleName = sheetName;
	QFile file(sheetName);
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

void MainWindow::paintEvent (QPaintEvent * event)
{
	//SetBgImage(QString(":/images/bg.jpg"));
}

void MainWindow::about()
{
   QSplashScreen *splash = new QSplashScreen;
   splash->setPixmap(QPixmap("images/splash.jpg"));//设置图片
   Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
   splash->show();

}

void MainWindow::createActions()
{
    aboutAct = new QAction(u8"关于...", this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	/*********************************************************************************/
	vtkFileOpen = new QAction(u8"vtk打开窗口", this);
	//connect(vtkFileOpen, SIGNAL(triggered()), this, SLOT(vtkShow()));
	/*********************************************************************************/
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	diagModalTstAct = new QAction(tr("测试模态对话框"),this);
	diagModalTstAct->setStatusTip(tr("测试对话框的调用"));
	//connect(diagModalTstAct, SIGNAL(triggered()), this, SLOT(diagModalTst()));

	post3DAct=new QAction(QIcon(":/images/3Dpost.png"),tr("三维后处理分析"),this);
	post3DAct->setShortcuts(QList<QKeySequence>()<< QKeySequence(tr("Ctrl+P")));
	connect(post3DAct, SIGNAL(triggered()), this, SLOT(PostBasicFrameShow()));

	post2DCanvasAct=new QAction(QIcon(":/images/2Dpost.png"),tr("二维后处理分析"),this);
	post2DCanvasAct->setShortcuts(QList<QKeySequence>()<< QKeySequence(tr("Ctrl+T")));
	connect(post2DCanvasAct, SIGNAL(triggered()), this, SLOT(post2DCanvasDo()));


	gridLoadAct=new QAction(QIcon(":/images/gridpre.png"),tr("选定网格文件..."),this);
	gridLoadAct->setShortcuts(QList<QKeySequence>()<< QKeySequence(tr("Ctrl+G")));
	connect(gridLoadAct, SIGNAL(triggered()), this, SLOT(gridLoad()));

	quitAct = new QAction(QIcon(":/images/Exit.png"),tr("退出(&Q)"), this);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	fileSaveLayoutAct = new QAction(tr("保存窗体布局"), this);
	fileSaveLayoutAct->setStatusTip(tr("保存窗体布局大小和位置，方便下次启动。"));
	connect(fileSaveLayoutAct, SIGNAL(triggered()), this, SLOT(close()));

	fileLoadLayoutAct = new QAction(tr("加载窗体布局..."), this);
	fileLoadLayoutAct->setStatusTip(tr("加载窗体布局大小和位置，方便下次启动。"));
	connect(fileLoadLayoutAct, SIGNAL(triggered()), this, SLOT(close()));

	gblconfigAct = new QAction(QIcon(":/images/option.png"),tr("选项(&O)"),this);
	gblconfigAct->setStatusTip(tr("查看和维护系统的选项参数."));
	//connect(gblconfigAct, SIGNAL(triggered()), this, SLOT(preference()));

	inspParamAct = new QAction(QIcon(":/images/gridInsp.png"),tr("边界条件维护(&B)"),this);
	inspParamAct->setStatusTip(tr("边界条件维护."));
	connect(inspParamAct, SIGNAL(triggered()), this, SLOT(inspParamDo()));

	runParamAct = new QAction(QIcon(":/images/runParam.png"),tr("运行参数维护(&R)"),this);
	runParamAct->setStatusTip(tr("运行参数维护."));
	connect(runParamAct, SIGNAL(triggered()), this, SLOT(runParamDo()));

	basedataAct= new QAction(QIcon(":/images/basedata.png"),tr("数据库(&D)"),this);
	basedataAct->setStatusTip(tr("运行参数维护."));
	connect(basedataAct, SIGNAL(triggered()), this, SLOT(basedataDo()));

	runBrowserAct= new QAction(QIcon(":/images/runBrowser.png"),tr("运行情况浏览(&D)"),this);
	connect(runBrowserAct, SIGNAL(triggered()), this, SLOT(runBrowserDo()));

	runLocalMPIAct= new QAction(QIcon(":/images/singleMPIrun.png"),tr("本地并行运行(&D)"),this);
	connect(runLocalMPIAct, SIGNAL(triggered()), this, SLOT(runLocalMPIDo()));
	
	runClusterMPIAct= new QAction(QIcon(":/images/clusterMPIrun.png"),tr("集群并行运行(&D)"),this);
	connect(runClusterMPIAct, SIGNAL(triggered()), this, SLOT(runClusterMPIDo()));
	
	mpiLocalJobBrowserAct = new QAction(QIcon(":/images/clusterMPIrun.png"),tr("本地MPI作业情况浏览"),this);
	connect(mpiLocalJobBrowserAct,SIGNAL(triggered()),this,SLOT(mpiO5MPILocalJobBrowser()));
	
	mpiClusterJobBrowserAct = new QAction(QIcon(":/images/basedata.png"),tr("3.集群MPI作业情况浏览"),this);
	connect(mpiClusterJobBrowserAct,SIGNAL(triggered()),this,SLOT(clusterMPIJobBrowser()));

}


void MainWindow::changeStyle()
{
	QAction * a = qobject_cast<QAction *>(sender());
	if (!a) return;
	qApp->setStyle(a->text());
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(u8"系统(&S)");
	/*
	fileMenu->addAction(u8"输入属性设置", this, SLOT(inputFileSet()));
	fileMenu->addAction(u8"1.登录远程机群", this, SLOT(generateOp()));
	fileMenu->addSeparator();
	fileMenu->addAction(u8"2.系统软件编译", this, SLOT(slotMakeOp()));
	fileMenu->addSeparator();
	
	fileMenu->addSeparator();

	filesubStyleMenu=fileMenu->addMenu(tr("界面风格"));
	// Style menu
	QStringList styleList = QStyleFactory::keys();
	foreach (QString stylename, styleList)
	{
		filesubStyleMenu->addAction(stylename, this, SLOT(changeStyle()));
	}

    fileMenu->addSeparator();
	*/
    fileMenu->addAction(quitAct);

 


	calcMenu = menuBar()->addMenu( tr("业务操作(&C)"));
	//calcMenu->addAction(u8"SSPNS", this, SLOT(slotShowSSPNS()));
	calcMenu->addAction(QIcon(":/images/EditInsp.png"), u8"NS+PNS计算", this, SLOT(slotShowSSPNS()));
	calcMenu->addSeparator();
	
	calcMenu->addAction(QIcon(":/images/EditInsp.png"), u8"PNS计算", this, SLOT(slotShowPNS()));
	calcMenu->addSeparator();

	//calcMenu->addAction(QIcon(":/images/EditInsp.png"), u8"后处理", this, SLOT(slotShowProfiling()));
	//calcMenu->addSeparator();

	//calcMenu->addAction(QIcon(":/images/EditInsp.png"), u8"批量SSPNS", this, SLOT(slotShowBatchSSPNS()));
	//calcMenu->addSeparator();
	//calcMenu->addAction(u8"ECHV", this, SLOT(slotShowECHV()));

	/*
	calcMenu->addSeparator();
	calcMenu->addAction(u8"3.计算", this, SLOT(slotComputeOp()));//触发运行命令，并下载文件boundary+modelcheck.txt（nohup）
	*/
	
	//postProMenu = menuBar()->addMenu( tr("后处理(&A)"));
	//postProMenu->setIcon(QIcon(":/images/EditInsp.png"));
	//postProMenu->addAction(u8"流场数据简化分析方法",this,SLOT(slotShowPostProcessGridSimplify()));
	//postProMenu->addSeparator();
	//postProMenu->addAction(u8"基于体绘制分析方法");
	//postProMenu->addSeparator();

	viewMenu = menuBar()->addMenu( tr("窗口(&W)"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu( tr("帮助(&H)"));
    helpMenu->addAction(aboutAct);
	/*添加测试vtk按钮*/
	//vtkTestMenu = menuBar()->addMenu(tr("vtk测试(&V)"));
	//vtkTestMenu->addAction(vtkFileOpen);
	
	

#ifdef HUANGZB
	testMenu= menuBar()->addMenu(tr("测试(QT)"));
	testMenu->addAction(diagModalTstAct);
	testMenu->addAction(tr("非模态对话框"),this,SLOT(nondiagModalTst()));
	testMenu->addSeparator();
	testMenu->addAction(tr("动态生成dockwidget"),this,SLOT(dyCreateDockWidget()));
	testMenu->addAction(tr("动态生成dockwidget,改变配色方案"),this,SLOT(dyCreateDockWidgetPallete()));

	testMenu->addAction(tr("动态加载UI"),this,SLOT(dyCreateDialogUi()));
	testMenu->addSeparator();
	testMenu->addAction(tr("进度条例子"),this,SLOT(processTst()));
	testMenu->addSeparator();
	testMenu->addAction(tr("jambiPlotter使用例子"),this,SLOT(jambiPlotterDemo()));
	testMenu->addAction(tr("线程测试"),this,SLOT(threadTest()));
	testMenu->addSeparator();
	testMenu->addAction(tr("动态生成dockwidget,Group Boxes的实用案例"),this,SLOT(dyCreateDockWidgetGroupBox()));
	testMenu->addAction(tr("动态生成dockwidget,Line Edit的实用实例"),this,SLOT(dyCreateDockWidgetLineEdit()));
	testMenu->addSeparator();
	testMenu->addAction(tr("StyleSheet案例"),this,SLOT(dyCreateDockWidgetStyleSheet()));
	testMenu->addSeparator();
	testMenu->addAction(tr("Windows Flag案例"),this,SLOT(dyCreateDockWidgetWindowsFlag()));
	testMenu->addAction(tr("基本的布局方案对话框(带菜单)"),this,SLOT(basiclayoutdemo()));
	testMenu->addAction(tr("向导类的案例"),this,SLOT(wizardDemo()));
	testMenu->addAction(tr("licenseWizard的案例"),this,SLOT(wizardlicenseDemo()));
	testMenu->addSeparator();
	testMenu->addAction(tr("高级复杂的配置对话框"),this,SLOT(complexconfigdialogdemo()));
	testMenu->addAction(tr("动态生成dockwidget,Find File Dialog Demo"),this,SLOT(findfiledialogdemo()));
	testMenu->addAction(tr("动态生成dockwidget,简单的2栏列表框"),this,SLOT(dyCreateDockWidgetCoordinatesetterDemo()));

	testMenu->addSeparator();
	testMenu->addAction(tr("Tab Widget的案例"),this,SLOT(TabWidgetDemo()));
	testMenu->addSeparator();
	testMenu->addAction(tr("动态生成dockwidget,Sqlite数据库简单实例一"),this,SLOT(dyCreateDockWidgetDBSqliteSimpleDemo()));
	testMenu->addAction(tr("动态生成dockwidget,Sqlite数据库主从表实例"),this,SLOT(dyCreateDockWidgetDBSqliteMasterSlaveDemo()));

	testMenu->addSeparator();
	testMenu->addAction(tr("动态生成dockwidget,setting的例子;每个Widget有独立的Pallete"),this,SLOT(dyCreateDockWidgetSimpleSettingDemo()));
	testMenu->addAction(tr("动态生成dockwidget,自定义tableview的案例???有问题"),this,SLOT(dyCreateDockWidgetTableViewDemo()));
	testMenu->addSeparator();
	testMenu->addAction(tr("测试多个Splitter，在对话框中"),this,SLOT(testQSplitterInDialog()));
	testMenu->addSeparator();
	testMenu->addAction(tr("动态生成dockwidget,OpenGL-Hello,World."),this,SLOT(dyCreateDockWidgetOpenGLSimpleDemo()));
	testMenu->addAction(tr("动态生成dockwidget,OpenGL-Teapots."),this,SLOT(dyCreateDockWidgetOpenGLTeapotsDemo()));
	testMenu->addAction(tr("测试QGLWidget----"),this,SLOT(dyCreateDockWidgetQtGL01Demo()));


	testQtGLMenu=menuBar()->addMenu(tr("测试QGLWidget/NEHE"));
	testQtGLMenu->addAction(tr("测试NEHE--1"),this,SLOT(QGLW01Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--1/FOR DockWidget"),this,SLOT(dyCreateDockWidgetQGLW01Demo()));
	testQtGLMenu->addSeparator();
	testQtGLMenu->addAction(tr("测试NEHE--9/FOR DockWidget"),this,SLOT(dyCreateDockWidgetQGLW09Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--9"),this,SLOT(QGLW09Demo()));
	testQtGLMenu->addSeparator();
	testQtGLMenu->addAction(tr("测试NEHE--6/For DockWidget"),this,SLOT(dyCreateDockWidgetQGLW06Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--6"),this,SLOT(QGLW06Demo()));
	testQtGLMenu->addSeparator();
	testQtGLMenu->addAction(tr("测试NEHE--16/For DockWidget"),this,SLOT(dyCreateDockWidgetQGLW16Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--16"),this,SLOT(QGLW16Demo()));
	testQtGLMenu->addSeparator();
	//testQtGLMenu->addAction(tr("测试NEHE--20/For DockWidget"),this,SLOT(dyCreateDockWidgetQGLW20Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--20"),this,SLOT(QGLW20Demo()));

	testQtGLMenu->addSeparator();
	//testQtGLMenu->addAction(tr("测试NEHE--29/For DockWidget"),this,SLOT(dyCreateDockWidgetQGLW29Demo()));
	testQtGLMenu->addAction(tr("测试NEHE--29"),this,SLOT(QGLW29Demo()));

	testQwtMenu= menuBar()->addMenu(tr("测试QWT"));
	testQwtMenu->addAction(tr("qwt-6.0.1--最简单的使用/对话框"),this,SLOT(QWT01DialogDemo()));
	testQwtMenu->addAction(tr("qwt-6.0.1--最简单的使用/DockWidget"),this,SLOT(dyCreateDockWidget01QWTDemo()));
	testQwtMenu->addAction(tr("qwt-6.0.1--最简单的使用TWO/DockWidget"),this,SLOT(dyCreateDockWidget03QWTDemo()));
	testQwtMenu->addSeparator();
	testQwtMenu->addAction(tr("qwt-6.0.1--条带曲线ONE/DockWidget"),this,SLOT(dyCreateDockWidget04QWTDemo()));
	testQwtMenu->addSeparator();
	testQwtMenu->addAction(tr("qwt-6.0.1--曲线好案例/DockWidget"),this,SLOT(dyCreateDockWidget05QWTDemo()));
	testQwtMenu->addAction(tr("qwt-6.0.1--曲线好案例TWO/DockWidget"),this,SLOT(dyCreateDockWidget06QWTDemo()));
	testQwtMenu->addSeparator();
	testQwtMenu->addAction(tr("qwt-6.0.1--曲线好案例--spectrogram--THREE/DockWidget"),this,SLOT(dyCreateDockWidget07QWTDemo()));
	testQwtMenu->addSeparator();
	testQwtMenu->addAction(tr("qwt-6.0.1--曲线复杂的案例FOUR/DockWidget"),this,SLOT(dyCreateDockWidget08QWTDemo()));

	testQwtPlot3DMenu=menuBar()->addMenu(tr("测试QWTPlot3d"));
	testQwtPlot3DMenu->addAction(tr("qwtplot3d-0.2.7--最简单的使用/对话框"),this,SLOT(QWTPLOT3D01DialogDemo()));
	testQwtPlot3DMenu->addAction(tr("qwtplot3d-0.2.7--最简单的使用/嵌入DockWidget"),this,SLOT(dyCreateDockWidget01QWTPLOT3DDemo()));
	testQwtPlot3DMenu->addSeparator();
	testQwtPlot3DMenu->addAction(tr("qwtplot3d-0.2.7--mesh2"),this,SLOT(Mesh2Demo()));

	testQGLVMenu=menuBar()->addMenu(tr("测试LibQGLViewer2.3.17"));
	testQGLVMenu->addAction(tr("最简单的对话框----libQGLViewer"),this,SLOT(QGLV01Demo()));
	testQGLVMenu->addAction(tr("最简单的使用/嵌入DockWidget----libQGLViewer"),this,SLOT(dyCreateDockWidgetQGLV01Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("场景对象的选择/嵌入DockWidget----libQGLViewer"),this,SLOT(dyCreateDockWidgetQGLV02Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("场景摄像机的案例对话框---libQGLViewer"),this,SLOT(QGLV03Demo()));
	testQGLVMenu->addAction(tr("场景摄像机的案例/嵌入DockWidget---libQGLViewer"),this,SLOT(dyCreateDockWidgetQGLV03Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("基于回调的绘画/对话框---"),this,SLOT(QGLV04Demo()));
	testQGLVMenu->addAction(tr("基于回调的绘画/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV04Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("鼠标移动对象/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV05Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("多个视图/对话框"),this,SLOT(QGLV06Demo()));
	testQGLVMenu->addAction(tr("多个视图/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV06Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("Corner Axis/对话框"),this,SLOT(QGLV07Demo()));
	testQGLVMenu->addAction(tr("Corner Axis/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV07Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("有背景图的GLViewer/对话框"),this,SLOT(QGLV08Demo()));
	testQGLVMenu->addAction(tr("有背景图的GLViewer/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV08Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("多重选择场景对象/对话框"),this,SLOT(QGLV09Demo()));
	testQGLVMenu->addAction(tr("多重选择场景对象/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV09Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("多重场景对象+多种灯光/对话框"),this,SLOT(QGLV10Demo()));
	testQGLVMenu->addAction(tr("多重场景对象+多种灯光/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV10Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("fastDraw/对话框"),this,SLOT(QGLV11Demo()));
	testQGLVMenu->addAction(tr("fastDraw/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV11Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("QGLViewer接口测试/对话框"),this,SLOT(QGLV12Demo()));
	testQGLVMenu->addAction(tr("QGLViewer接口测试/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV12Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("2维和3维结合显示/对话框"),this,SLOT(QGLV13Demo()));
	testQGLVMenu->addAction(tr("2维和3维结合显示/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV13Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("screenCoordSystem/对话框"),this,SLOT(QGLV14Demo()));
	testQGLVMenu->addAction(tr("screenCoordSystem/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV14Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("keyFrames/对话框"),this,SLOT(QGLV15Demo()));
	testQGLVMenu->addAction(tr("keyFrames/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV15Demo()));
	testQGLVMenu->addSeparator();
	testQGLVMenu->addAction(tr("constrainedFrame/对话框"),this,SLOT(QGLV16Demo()));
	testQGLVMenu->addAction(tr("constrainedFrame/嵌入DockWidget---"),this,SLOT(dyCreateDockWidgetQGLV16Demo()));




#endif


}

//void MainWindow::createToolBars()
//{
//    fileToolBar = addToolBar(tr("File"));
//	//fileToolBar->addAction(gridLoadAct);
//
//
//    editToolBar = addToolBar(tr("Edit"));
//	//editToolBar->addAction(mpiLocalJobBrowserAct);
//	//editToolBar->addAction(mpiClusterJobBrowserAct);
//	//editToolBar->addAction(quitAct);
//    //editToolBar->addAction(undoAct);
//}
void MainWindow::slotShowProfiling() {
	//static PostProcessProfiling* profilingWindow = nullptr;
	if (profilingWindow == nullptr) {
		profilingWindow = new PostProcessProfiling(this);
		profilingWindow->resize(this->size()*0.7);
		profilingWindow->move(100,100);
	}
	
	profilingWindow->show();
	//this->setCentral(profilingWindow);
	return;
}
void MainWindow::createToolBars()
{

	QToolBar * toolbar = new QToolBar();

	//创建快捷项等同于菜单项
	QAction * actionQuit = new QAction("quit");
	QAction * actionSSPNS = new QAction(u8"NS+PNS计算");
	QAction * actionPNS = new QAction(u8"PNS计算");
	QAction * actionBatchSSPNS = new QAction("BatchSSPNS");
	QAction * actionVolume = new QAction("VolumeRendering");
	QAction * actionProfiling = new QAction(u8"后处理");
	actionQuit->setIcon(QIcon(":/img/PNG/application-exit.png"));
	actionSSPNS->setIcon(QIcon(":/images/gridInsp.png"));
	actionPNS->setIcon(QIcon(":/images/Mask2.bmp"));
	actionBatchSSPNS->setIcon(QIcon(":/images/Mask2.bmp"));
	actionVolume->setIcon(QIcon(":/images/Mask2.bmp"));
	actionProfiling->setIcon(QIcon(":/images/Mask2.bmp"));
	toolbar->addAction(actionQuit);
	toolbar->addAction(actionSSPNS);
	toolbar->addAction(actionPNS);
	toolbar->addAction(actionProfiling);
	//toolbar->addAction(actionBatchSSPNS);
	//toolbar->addAction(actionVolume);
	//toolbar->setFixedWidth(64);
	//toolbar->setFixedHeight(1980);
	toolbar->setIconSize(QSize(64, 64));
	//this->addToolBar(Qt::TopToolBarArea, toolbar);
	//this->addToolBar(Qt::LeftToolBarArea, toolbar);
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(actionSSPNS, SIGNAL(triggered()), this, SLOT(slotShowSSPNS()));
	connect(actionPNS, SIGNAL(triggered()), this, SLOT(slotShowPNS()));
	connect(actionBatchSSPNS, SIGNAL(triggered()), this, SLOT(slotShowBatchSSPNS()));
	connect(actionProfiling, SIGNAL(triggered()), this, SLOT(slotShowProfiling()));

	//connect(actionVolume, SIGNAL(triggered()), this, SLOT(showVolumeRendering()));
}
//! [8]
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}




void MainWindow::gridLoad()
{
	;//QMessageBox::information(this, "INFORMATION", tr("You clicked gridload."));
	//QString name = QFileDialog::getOpenFileName(".", "Images (*.png *.xpm *.jpg)", this, "Choose", "Select an image"); 
	QString fileName = QFileDialog::getOpenFileName(this, tr("选择网格文件"),QApplication::applicationDirPath(),
		tr("格式化Plot3D文件(*.plot3d);;二进制非格式化Plot3D文件(*.plot3db)"));
	// In case of Cancel
	if (fileName.isEmpty())
		return; 
	qApp->processEvents();
	QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 

	QFileInfo tmpfileinfo;
	tmpfileinfo.setFile(fileName);
	bool bGrid=false;
	bool bGridInsp=false;
	if (!tmpfileinfo.exists())
	{
		QMessageBox::information(this, "警告", tr("网格文件不存在！"));
		//return;
	}
	else
		bGrid=true;

	tmpfileinfo.setFile(globalcfg.s2q(globalcfg.change_file_ext(globalcfg.q2s(fileName),".inp")));
	if (!tmpfileinfo.exists())
	{

		QMessageBox::information(this, "警告", QString("与网格文件匹配的边界文件不存在！").append(fileName));
		//return;
	}
	else
		bGridInsp=true;

	if (!bGridInsp)
	{
		int r = QMessageBox::warning(this, tr("警告"),
                    tr("与网格文件匹配的边界文件不存在！是否继续？\n如果继续，则只能显示网格，运行环境将不再存在，\n其他操作只有在网格与边界正确匹配选择之后才能进行！"),
                    QMessageBox::Yes | QMessageBox::No);
		if (r==QMessageBox::Yes)
		{
			globalcfg.gridInspfilename="9999";
			globalcfg.gridfilename = fileName;	
			gridshowviewer->LoadPlot3DFile(fileName);
		}

	}
	else
	{
		globalcfg.gridfilename = fileName;
		globalcfg.gridInspfilename = globalcfg.s2q(globalcfg.change_file_ext(globalcfg.q2s(fileName),".inp"));	
	
		gridshowviewer->LoadPlot3DFile(fileName);
	}
}

void MainWindow::mpi00EnvSetup()
{
	

}

void MainWindow::mpiO1CreateMPD()
{
	QProgressDialog *pb=new QProgressDialog();
	pb->setRange(0,1000);
	QString strprompt("正在构建并行环境...\n检测并行后台服务MPD!");
	pb->setLabelText(strprompt);

	pb->setValue(10);

	QProcess prun;
	char * str= (char*)("01mpd.exe");
	int pid=HuangzbGlobalConfig::GetProcessidFromName(str);
	strprompt="并行后台服务MPD已经启动！不需要初始化!";
	if (pid)
	{
		QMessageBox::information(this, "信息", tr("并行后台服务MPD已经启动！不需要初始化!"));
		delete pb;
		return;
	}

	QString strFileBasePath=QCoreApplication::applicationDirPath()+"/mpisupport/";
	pb->setValue(20);

	////////////////////////////////////////////////////////////////////
	QString exefilename=strFileBasePath+"01mpd.exe -remove";
	prun.start(exefilename);
	// 等待进程启动
    if (!prun.waitForStarted())
    {
        logger()->debug( "01mpd.exe -remove启动失败");
		delete pb;
        return;
    }
	// 关闭写通道,因为没有向进程写数据,没用到 
    prun.closeWriteChannel();
    // 等待进程结束
    while (false == prun.waitForFinished()) qApp->processEvents();
	int ec=prun.exitCode();
	logger()->debug(QString("%1，进程返回值：%2").arg(exefilename).arg(ec));
	pb->setValue(100);

	////////////////////////////////////////////////////////////////////////
	exefilename=strFileBasePath+"01mpd.exe -install";
	prun.start(exefilename);
	// 等待进程启动
    if (!prun.waitForStarted())
    {
        logger()->debug( "01mpd.exe -install启动失败");
		delete pb;
        return;
    }
	// 关闭写通道,因为没有向进程写数据,没用到 
    prun.closeWriteChannel();
    // 等待进程结束
    while (false == prun.waitForFinished()) qApp->processEvents();
	ec=prun.exitCode();
	logger()->debug(QString("%1，进程返回值：%2").arg(exefilename).arg(ec));
	pb->setValue(300);

	bool bFlag=false;
	int nCount=0;
	while(!bFlag)
	{
		qApp->processEvents();
		char * str1 = (char*)("01mpd.exe");
		pid=HuangzbGlobalConfig::GetProcessidFromName(str1);
		if (nCount>1000) break;
		if (pid)
			bFlag=true;
		nCount++;
		pb->setValue(300+nCount);
	}
	if (nCount<1001)
		QMessageBox::information(this, " 信息", tr("建立MPI并行服务完成！"));
	else
		QMessageBox::information(this, " 信息", tr("并行后台服务启动不成功！请检查并行环境配置！"));
	delete pb;
}

void MainWindow::mpiO2MPIRegister()
{
	;//QMessageBox::information(this, "INFORMATION", tr("You clicked mpiO2MPIRegister."));
	QString strFileBasePath=QCoreApplication::applicationDirPath()+"/mpisupport/";
	QString exefilename=strFileBasePath+"02MPIRegister.exe";

	QProcess prun;
	QString cmd="cmd";
	QStringList arguments;
	arguments<<"/k"<<exefilename;
	prun.startDetached(cmd,arguments);
	// 等待进程启动
    if (!prun.waitForStarted())
    {
        logger()->debug( "02MPIRegister.exe启动失败");
        return;
    }
}

void MainWindow::mpiO3MPIConfig()
{
	;//QMessageBox::information(this, "INFORMATION", tr("You clicked mpiO3MPIConfig."));
	QString strFileBasePath=QCoreApplication::applicationDirPath()+"/mpisupport/";
	QString exefilename=strFileBasePath+"03MPIConfig.exe";

	QProcess prun;
	//QString cmd="cmd";
	QStringList arguments;
	arguments<<"/k"<<exefilename;
	prun.startDetached(exefilename);
	// 等待进程启动
    if (!prun.waitForStarted())
    {
        logger()->debug( "02MPIRegister.exe启动失败");
        return;
    }

}

void MainWindow::mpiO4MPITest()
{
	;//QMessageBox::information(this, "INFORMATION", tr("You clicked mpiO4MPITest."));

}

void MainWindow::mpiO5MPILocalJobBrowser()
{
	//将查询的结果显示在表格中.
	////////////////////////////////////////////////////////////////////////////////////
	if (!this->boolMPILocalJobBrowser)
	{	
		generateOp();
		
		boolMPILocalJobBrowser = true;
	}

	/////////////////////////////////////////////////////////////////////////////////////
}

void MainWindow::clusterMPIJobBrowser()
{
	
}

void MainWindow::vtkShow()
{
	//将查询的结果显示在表格中.
	////////////////////////////////////////////////////////////////////////////////////
	if (!this->boolVtkBrowser)
	{
		//////////////////////////////////////////////////////////////////////////
		QDockWidget *dockRightGraph = new QDockWidget(tr("vtk展示"), this);
		dockRightGraph->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		//THuangzbWinMPIJobBrowserClass *diag = new THuangzbWinMPIJobBrowserClass(this, dockRightGraph);
		centralwidget = new QWidget(this);
		sceneWidget = new SceneWidget(0,centralwidget);
		centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
		QGridLayout* gridLayout = new QGridLayout(centralwidget);
		gridLayout->setSpacing(0);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		gridLayout->setContentsMargins(0, 0, 0, 0);
		sceneWidget = new SceneWidget(0,centralwidget);
		sceneWidget->setObjectName(QString::fromUtf8("sceneWidget"));

		gridLayout->addWidget(sceneWidget, 0, 0, 1, 1);
		dockRightGraph->setWidget(centralwidget);

		//addDockWidget(Qt::RightDockWidgetArea, dockRightGraph);
		addDockWidget(Qt::LeftDockWidgetArea, dockRightGraph);
		//viewMenu->addAction(dockRightGraph->toggleViewAction());
		boolVtkBrowser = true;
	}
	
	//打开文件对话框
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "",
		"VTK Files (*.vtk)");

	// Open file，取得文件句柄
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);

	// Return on Cancel
	if (!file.exists()) {
		QMessageBox::information(this, "提示", "文件不存在!!");
		return;
	}
	//清除已经有的图片和初始化背景
	sceneWidget->removeDataSet();

}

void MainWindow::clusterconfig()
{
	

}

void MainWindow::clusternetwork()
{
	bNetworkStatus=true;
	QString pyexepath= QCoreApplication::applicationDirPath() +"/pyengine/pythonw.exe ";
	QString exeParam1 =  QCoreApplication::applicationDirPath() +"/scripts/clientEnvProbe.py ";
	QString exeParam2 =  QCoreApplication::applicationDirPath()+"/logs/pythonenv.log";
	
	QProcess myproc(0);
	myproc.start(pyexepath,QStringList()<<exeParam1<<exeParam2);
	myproc.waitForStarted();
	myproc.waitForFinished();
	QString outputmsg=myproc.readAllStandardOutput();
	logger()->debug(outputmsg);
}

void MainWindow::clusterbrowser()
{
	


}

void MainWindow::inspParamDo()
{
	
}

void MainWindow::runParamDo()
{
	

}

void MainWindow::basedataDo()
{
	

}

void MainWindow::post2DCanvasDo()
{
	

}

void MainWindow::runBrowserDo()
{
	


}

void MainWindow::runLocalMPIDo()
{

	

}

void MainWindow::runClusterMPIDo()
{
	


}

void MainWindow::resultBrowserDo()
{
	;//QMessageBox::information(this, "INFORMATION", tr("You clicked resultBrowserDo."));

}

void MainWindow::gridshowOptions()
{

	
}

void MainWindow::slotComputeOp()
{
	
}


void MainWindow::PostBasicFrameShow()
{
	if (!this->boolPostBasicFrameShow)
	{	
		//////////////////////////////////////////////////////////////////////////
		QDockWidget *dockRightGraph = new QDockWidget(tr("三维后处理浏览"), this);
		dockRightGraph->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea|Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

		//ThuangzbPostBasicFrameWidget *diag=new ThuangzbPostBasicFrameWidget(dockRightGraph);
		//dockRightGraph->setWidget(diag);

		addDockWidget(Qt::RightDockWidgetArea, dockRightGraph);
		viewMenu->addAction(dockRightGraph->toggleViewAction());
		boolPostBasicFrameShow = true;
	}


}

void MainWindow::OndataresultBrowser()
{
}