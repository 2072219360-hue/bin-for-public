#include "SSPNS.h"
OutPutLogger* outPutLogger=nullptr;
SceneWidget*main_sceneWidget = NULL;
TrTableShow* tableShow=nullptr;
WorkSpaceOp* m_workSpaceOp=nullptr;
RWThread* rwTh =new RWThread();
//RWThread* rwTh2 = new RWThread();
QMutex* rwMutex = new QMutex();
statusSubject* myStatus= new statusSubject();
extern QString appComDir;
extern SceneWidget*main_sceneWidget2;
namespace platforms {
	namespace sspns {
		//nsFlag=0: ns+pns       nsFlag=1:pns
		SSPNS::SSPNS(int nsFlag)
		{	

			if (myStatus == nullptr) {
				myStatus = new statusSubject();
			}
			myStatus->RemoveAll();
			myStatus->setStatus(Status::DEFAULT_STATUS);
			airButton = nullptr;
			batchConfig = nullptr;

			m_nsFlag = nsFlag;
			SignalInstance::nsFlag = nsFlag;

			initUi();

			addObserver();
			
			myStatus->Notify();
			initData();
			setSingalMode();

			m_workContext = nullptr;
			initContext();
			m_workFlow = nullptr;
			if(m_nsFlag==1)init_workFlow();
			else init_computeFlow();


			double stdWidth = 2880, stdHeight = 1800;
			QScreen *screen = QGuiApplication::primaryScreen();
			int currentScreenWid = screen->size().width();
			int currentScreenHei = screen->size().height();
			double factorX = currentScreenWid / stdWidth, factorY = currentScreenHei / stdHeight;
			auto scaleWidgets = [&](auto&& scaleWidgets, QWidget *parent, qreal factorx, qreal factory)->void
			{
				// 遍历所有子控件
				foreach(QObject *child, parent->children()) {
					QWidget *widget = qobject_cast<QWidget *>(child);
					if (widget) {
						// 获取控件的原始大小
						QSize originalSize = widget->sizeHint();

						// 设置新的大小
						/*widget->setFixedSize(
							originalSize.width() * widthScaleFactor,
							originalSize.height() * heightScaleFactor
						);*/
						int widgetX = widget->x();
						int widgetY = widget->y();
						int widgetWid = widget->width();
						int widgetHei = widget->height();
						int nWidgetX = (int)(widgetX*factorx);
						int nWidgetY = (int)(widgetY*factory);
						int nWidgetWid = (int)(widgetWid*factorx);
						int nWidgetHei = (int)(widgetHei*factory);
						widget->setGeometry(nWidgetX, nWidgetY, nWidgetWid, nWidgetHei);
						// 递归地调整子控件
						scaleWidgets(scaleWidgets, widget, factorx, factory);
					}
				}
			};
			scaleWidgets(scaleWidgets, this, factorX, factorY);
				
			
		}
		SSPNS::~SSPNS()
		{
			airButton = nullptr; // 显式置空，防止野指针
			if (m_processPool != NULL) delete m_processPool;
			if (batchConfig != nullptr) {
				delete batchConfig;
				batchConfig = nullptr;
			}
			m_processPool = nullptr;
			m_workSpaceOp->killPrograms();
			if (outPutLogger != nullptr) {
				delete outPutLogger;
				outPutLogger = nullptr;
			}
			if (m_workSpaceOp != nullptr) {
				delete m_workSpaceOp;
				m_workSpaceOp = nullptr;
			}
			if (tableShow != nullptr) {
				delete tableShow;
				tableShow = nullptr;
			}

			myStatus->RemoveAll();
			//TODO:0423
			/*if (myStatus != nullptr) {
				myStatus->RemoveAll();
				delete myStatus;
				myStatus = nullptr;
			}*/
		}
		void SSPNS::closeAllWindow() {
			if (m_processPool != NULL) {
				m_processPool->closeAllWindow();
			}
			//
			
			emit SignalInstance::getInstance()->signalCloseAllWindow();// ns.exe -> pns.exe
			emit SignalInstance::getInstance()->finishEventLoop();

			outPutLogger->closeProcess();
			
		}
		void SSPNS::initData() {
			SignalInstance::setWorkSpace("");
		}
		void SSPNS::setSingalMode() {

		}
		void SSPNS::initContext() {
			m_workContext = new workContext();
			m_workContext->setOutPutLogger(outPutLogger);
			m_workContext->setWorkSpaceOp(m_workSpaceOp);
			m_workContext->setSceneWidget(m_sceneWidget);
			m_workContext->setProgressDialog(m_dialog);
			m_workContext->setTrTableShow(m_tableShow);
			m_workContext->setParamSetDialog(m_paramDialog);
			m_workContext->myStatus = myStatus;
			//m_workContext->setQProgressBar(outPutLogger->m_dataProgressBar);
		}
		//ns + pns
		void SSPNS::init_computeFlow() {
			//m_workContext->fullFlag = true;
			auto getNode = [](string typeName)->workFlow* {
				workStrategy*  strategy = nullptr;
				if (typeName == "readGrid") {
					strategy = new readFileStrategy(1);
				}
				else if (typeName == "update30") {
					strategy = new updateStrategy(30);
				}
				else if (typeName == "update100") {
					strategy = new updateStrategy(100, 1);
				}
				else if (typeName == "computeHead") {
					strategy = new nsStrategy();
				}else if (typeName == "computeBody") {
					strategy = new pnsStrategy();
				}
				else if (typeName == "checkProgram") {
					strategy = new checkProgramStrategy();
				}
				workFlow* res = new workFlow(strategy);

				return res;
			};
			nodeAdr.clear();
			unordered_map<workFlow*, int>  nodeId;
			unordered_map<int, string> nodeType = {
				{1,"computeHead"},{2,"update30"},{3,"computeBody"} ,{4,"update100"},{5,"checkProgram"}

			};
			vector<vector<int>> edges = {
				{0,1}, {1,2},{2,3},{3,4},
			};
			m_workFlow = new workFlow();

			nodeAdr[0] = m_workFlow;
			nodeId[m_workFlow] = 0;

			for (auto &edge : edges) {
				int id1 = edge[0];
				if (!nodeAdr.count(id1)) {
					workFlow* node = getNode(nodeType[id1]);
					nodeAdr[id1] = node;
					nodeId[node] = id1;
				}
				workFlow* preNode = nodeAdr[id1];
				for (int i = 1; i < edge.size(); i++) {
					int id = edge[i];
					if (!nodeAdr.count(id)) {
						workFlow* node = getNode(nodeType[id]);
						nodeAdr[id] = node;
						nodeId[node] = id;
					}
					workFlow* nextNode = nodeAdr[id];
					preNode->setNextNode(nextNode);
					nextNode->setLastNode(preNode);
				}
			}

			connect(btnRunCompute, &QPushButton::clicked, this, [&]() {
				workFlow* node = nodeAdr[0];
				bool checkFlag = outPutLogger->checkRunning();
				if (checkFlag) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前有其他程序正在运行，请先停止当前运行程序！");
					return;
				}
				QString dirPath = SignalInstance::getWorkSpace();
				FileParamChecker checker;
				bool checkParam = checker.checkFileParam(dirPath, 1);
				if (!checkParam) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"参数设置不合法，请设置正确参数后再重新尝试！");
					return;
				}
				if (node != nullptr)node->handle(m_workContext, nullptr);
			});


		}
		void platforms::sspns::SSPNS::closeAllProcess()
		{
		
		}
		// pns界面
		void SSPNS::init_workFlow() {
			m_workContext->fullFlag = false;
			auto getNode = [](string typeName)->workFlow* {
				workStrategy*  strategy = nullptr;
				if (typeName == "readGrid") {
					 strategy = new readFileStrategy(1);
				}
				else if (typeName == "update30") {
					strategy = new updateStrategy(30);
				}
				else if (typeName == "update100") {
					strategy = new updateStrategy(100,1);
				}
				else if (typeName == "beginCompute") {
					strategy = nullptr;
				}
				else if (typeName == "beginBody") {
					strategy = new pnsStrategy();
				}
				else if (typeName == "runSect") {
					strategy = new sectStrategy();
				}
				workFlow* res = new workFlow(strategy);

				return res;
			};


			nodeAdr.clear();
			unordered_map<workFlow*, int>  nodeId;
			unordered_map<int, string> nodeType = {
				{1,"readGrid"},{2,"update30"},{3,"update100"},{4,"beginCompute"},{5,"beginBody"},{6,"runSect"},
				
			};
			vector<vector<int>> edges = {
				{0,6},{6,1}, {1,2},{2,3},{3,5},{4,5}
			};
			m_workFlow = new workFlow();

			nodeAdr[0] = m_workFlow;
			nodeId[m_workFlow] = 0;
			
			for (auto &edge : edges) {
				int id1 = edge[0];
				if (!nodeAdr.count(id1)) {
					workFlow* node = getNode(nodeType[id1]);
					nodeAdr[id1] = node;
					nodeId[node] = id1;
				}
				workFlow* preNode = nodeAdr[id1];
				for (int i = 1; i < edge.size(); i++) {
					int id = edge[i];
					if (!nodeAdr.count(id)) {
						workFlow* node = getNode(nodeType[id]);
						nodeAdr[id] = node;
						nodeId[node] = id;
					}
					workFlow* nextNode = nodeAdr[id];
					preNode->setNextNode(nextNode);
					nextNode->setLastNode(preNode);
				}
			}
			connect(SignalInstance::getInstance(), &SignalInstance::signalStartWorkFlow, this, [&]() {
				for (auto it : nodeAdr) {
					workFlow* currentNode = it.second;
					currentNode->recoverStatus();
				}
				m_workFlow->handle(m_workContext,nullptr);
			});
			connect(btnRunCompute, &QPushButton::clicked, this, [&]() {
				workFlow* node = nodeAdr[4];
				bool checkFlag = outPutLogger->checkRunning();
				if (checkFlag) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"当前有其他程序正在运行，请先停止当前运行程序！");
					return;
				}
				QString dirPath = SignalInstance::getWorkSpace();
				FileParamChecker checker;
				bool checkParam = checker.checkFileParam(dirPath, 0);
				if (!checkParam) {
					emit SignalInstance::getInstance()->signalPrintInfo(u8"参数设置不合法，请设置正确参数后再重新尝试！");
					return;
				}
				if(node!=nullptr)node->handle(m_workContext, nullptr);
			});

		}
		void SSPNS::addObserver() {
			//btnObserver* loadObserver = new btnObserver(myStatus, btnLoadDefault, btnType::DEFAULT_ARGMENTS);
			btnObserver* headObserver = new btnObserver(myStatus, btnRunHead, btnType::RUN_HEAD);
			btnObserver* bodyObserver = new btnObserver(myStatus, btnRunBody, btnType::RUN_BODY);
			btnObserver* sectObserver = new btnObserver(myStatus, btnRunSect, btnType::RUN_SEGMENT);
			btnObserver* batchCalConfigObserver = new btnObserver(myStatus, btnArgBatch, btnType::BATCH);
			btnObserver* btnBatchCalObserver = new btnObserver(myStatus, btnRunBatch, btnType::BATCH); 
			btnObserver* btnComputeObserver = new btnObserver(myStatus, btnRunCompute, btnType::COMPUTE);
			btnObserver* btnProfilingObserver = new btnObserver(myStatus, btnRunProfiling, btnType::PROFILING);
			

			QVector<myObserver*> v = { btnComputeObserver,batchCalConfigObserver };

			myStatus->AttachVector(v);
		}
		void SSPNS::updateWindow() {
			m_workSpaceOp->setCurrentWorkSpaceLineEdit();
			setMainSceneWidget();
		}
		SceneWidget* SSPNS::getSceneWidget() {
			return m_sceneWidget;
		}
		void SSPNS::reshow() {

		}
		
		void SSPNS::initUi()
		{
			m_dialog = new ProgressDialog("Loading...","Please wait for finishing running...");
			m_dialog->close();
			if(outPutLogger==nullptr)outPutLogger = new OutPutLogger(m_nsFlag);
			outPutLogger->nsFlag = m_nsFlag;
			m_mainLayout = new QGridLayout;
			titleSheet = u8"background:lightblue;font-family: \"Microsoft YaHei\";font-size:10pt;color:#000000;";

			loadWorkSpace();
			loadSceneWidget();
			loadOutputLoger();
			//暂时需要这两个类，等之后写好了参数窗口就取消掉这两个类
			//loadInputSet1();
			//loadInputSet2();
			m_paramDialog = new ParamSetDialog(m_nsFlag);
			loadTrTable();
			loadRunButton();
			loadProgressWidget();
			loadArgButton();
			initConnect();
			
			QSplitter* splitterVisDos = new QSplitter(Qt::Orientation::Vertical);
			//splitterVisDos->addWidget(fatherWidget);
			splitterVisDos->addWidget(m_sceneWidget);
			splitterVisDos->addWidget(outPutLogger);
			
			
			//上半部分参数部分
			QWidget*widgetHead = new QWidget;
			QHBoxLayout*layoutHead = new QHBoxLayout;
			layoutHead->addWidget(m_workSpace);
			layoutHead->addWidget(m_runWidget);
			layoutHead->addWidget(m_argRunWidget);
			widgetHead->setLayout(layoutHead);

			QVBoxLayout* layoutHeadTitle = new QVBoxLayout;
			QString str = u8"完整网格操作界面";
			if (m_nsFlag == 1) str = u8"分离网格操作界面";
			QLabel* labelTitle = new QLabel(str);
			labelTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelTitle->setStyleSheet(titleSheet);

			layoutHeadTitle->addWidget(labelTitle);
			layoutHeadTitle->addWidget(widgetHead);


			QWidget*widgetHead2 = new QWidget;
			widgetHead2->setLayout(layoutHeadTitle);
			/*QWidget*widgetRight = new QWidget;
			QVBoxLayout*layoutRight = new QVBoxLayout;
			layoutRight->addWidget(widgetHead);
			layoutRight->addWidget(m_progressWidget);
			widgetRight->setLayout(layoutRight);*/
			stackedWidget = new QStackedWidget;
			m_rightChangeWidget = new QWidget;
			//change
			QSplitter* rightChangeSplitter = new QSplitter(Qt::Orientation::Vertical);
			//批量窗口
			rightChangeSplitter->addWidget(m_progressWidget);
			//分段信息窗口
			//rightChangeSplitter->addWidget(m_trTableWidget);
			QLayout* rightChangeLayout = new QVBoxLayout;
			rightChangeLayout->addWidget(rightChangeSplitter);
			m_rightChangeWidget->setLayout(rightChangeLayout);
			//右下角窗口
			stackedWidget->addWidget(m_rightChangeWidget);
			stackedWidget->addWidget(m_paramDialog);
			//stackedWidget->addWidget(new ParamSetDialog());


			QWidget *customWidget = new QWidget();
			QLabel *label = new QLabel("", customWidget);
			label->setAlignment(Qt::AlignCenter);  // 设置文本居中

			// 设置字体大小和样式
			QFont font = label->font();
			font.setPointSize(100);  // 设置字体大小为20
			font.setBold(true);     // 设置字体为粗体
			label->setFont(font);   // 应用字体设置
			// 使用QVBoxLayout来确保标签水平和垂直居中
			QVBoxLayout *layout = new QVBoxLayout(customWidget);
			layout->addStretch();   // 添加一个拉伸因子在标签之前
			layout->addWidget(label);
			layout->addStretch();   // 添加一个拉伸因子在标签之后
			customWidget->setLayout(layout);
			stackedWidget->addWidget(customWidget);
			stackedWidget->setCurrentIndex(2);
			//QSplitter*widgetRight = new QSplitter(Qt::Orientation::Vertical);
			//widgetRight->addWidget(widgetHead);
			//widgetRight->addWidget(m_progressWidget);

			///*int fac = 30;
			//widgetRight->setStretchFactor(0, fac);
			//widgetRight->setStretchFactor(1, 100-fac);*/

			////下半部分交互部分
			//QWidget*widgetBody = new QWidget;
			//QVBoxLayout*layoutBody= new QVBoxLayout;
			//layoutBody->addWidget(m_trTableWidget);
			////layoutBody->addWidget(m_loadAndRunData);
			//widgetBody->setLayout(layoutBody);

			QSplitter*splitterBodyHead= new QSplitter(Qt::Orientation::Vertical);
			splitterBodyHead  ->addWidget(widgetHead2);
			splitterBodyHead  ->addWidget(stackedWidget);
			splitterBodyHead->addWidget(m_trTableWidget);
			m_trTableWidget->setVisible(false);
			///*QWidget* widgetRight = new QWidget;
			//QVBoxLayout* layoutRight = new QVBoxLayout;
			//layoutRight->addWidget(widgetHead);
			//layoutRight->addWidget(widgetBody);
			//widgetRight->setLayout(layoutRight);*/

			QSplitter*splitterLeftRight = new QSplitter(Qt::Orientation::Horizontal);
			splitterLeftRight->addWidget(splitterVisDos);
			//splitterLeftRight->addWidget(widgetRight);
			splitterLeftRight->addWidget(splitterBodyHead);


			
			QHBoxLayout* layoutMain = new QHBoxLayout;
			layoutMain->addWidget(splitterLeftRight);
			this->setLayout(layoutMain);
			
			
			int fac1=95,fac2 =90,fac3=30;
			splitterVisDos->setStretchFactor(0, fac1);
			splitterVisDos->setStretchFactor(1, 100-fac1);

			splitterLeftRight->setStretchFactor(0, fac2);
			splitterLeftRight->setStretchFactor(1, 100-fac2);

			splitterBodyHead->setStretchFactor(0, fac3);
			splitterBodyHead->setStretchFactor(1, 100 - fac3);


			loadStyleSheet("SSPNS");
			

		}
		void SSPNS::changeToArgWindow(int i) {
			if (i == -1) {
				
				
				//qDebug() << m_processTable->num_case << '\n';
				
				if (SignalInstance::getInstance()->getWorkSpace() == "" || m_processTable->num_case == 0)
				{
					stackedWidget->setCurrentIndex(2);
					m_trTableWidget->setVisible(false);
					return ;
				}

				stackedWidget->setCurrentIndex(0);
				m_trTableWidget->setVisible(false);
				return;
			}
			// 添加这行
			if (SignalInstance::getInstance()->getWorkSpace() == "") return;
			stackedWidget->setCurrentIndex(1);
			int index = i;
			index = argButtonIndex[ argButton[i]->text()];
			ParamSetDialog* currentWidget= dynamic_cast<ParamSetDialog*>(stackedWidget->currentWidget());
			if (currentWidget == nullptr) return;
			// 添加这行
			if (argButton[i]->text() == "气动力计算设置" && !currentWidget->returnWallForcestatus()) {
				emit SignalInstance::getInstance()->signalPrintInfo("已经在输出控制中关闭气动力相关设置!");
				return;
			}
			currentWidget->changeView(index);
			bool pnsFlag = false;
			if (m_nsFlag == 0 && i == 3) pnsFlag = true;
			if (m_nsFlag == 1 && i == 2) pnsFlag = true;
			if (pnsFlag) {
				m_trTableWidget->setVisible(true);
			}else m_trTableWidget->setVisible(false);
		}
		void SSPNS::changeWallForcestatus(bool flag) {
			if (airButton == nullptr ) {
				return;  // 确保指针有效
			}
			airButton->setEnabled(flag);  // 简化逻辑
		}
		void SSPNS::loadArgButton() {
			QLabel*labelLoadAndRun = new QLabel(u8"参数设置");
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			//labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setStyleSheet(titleSheet);
			labelLoadAndRun->setFixedHeight(40);
			//pns需要的窗口：气体属性 pns求解器参数 输出控制
			//QVector<QString> idList = {u8"Ns求解器参数",u8"边界条件",u8"输出控制",u8"气动力计算设置",u8"气体属性",u8"计算网格",u8"PNS求解器参数"};
			QVector<QString> idList = { u8"计算网格",u8"边界条件",u8"Ns求解器参数",u8"PNS求解器参数",u8"输出控制",u8"气动力计算设置",u8"气体属性" };
			for (int i = 0; i < idList.size(); i++) {
				argButtonIndex[idList[i]] = i;
			}
			if (m_nsFlag == 1) {
				idList = { u8"计算网格",u8"边界条件",u8"PNS求解器参数",u8"输出控制",u8"气动力计算设置",u8"气体属性" };
			}
			argButton.resize(idList.size());
			for (int i = 0; i < idList.size(); i++) {
				argButton[i]= new QPushButton( idList[i]);
				if (idList[i] == u8"气动力计算设置") {
					airButton = argButton[i];
					
				}
			}
			for (auto it : argButton) {
				it->setFixedHeight(60);
			}
			QGridLayout* gridLayout = new QGridLayout;
			for (int i = 0; i < argButton.size(); i++) {
				gridLayout->addWidget(argButton[i], i, 0, 1, 1);
				
			}

			QVBoxLayout* layout = new QVBoxLayout;
			layout->addWidget(labelLoadAndRun);
			layout->addLayout(gridLayout);
			layout->addStretch();


			m_argRunWidget = new QWidget;
			m_argRunWidget->setLayout(layout);
			

		}
		void SSPNS::loadRunButton() {
			QLabel*labelLoadAndRun = new QLabel(u8"加载参数及运行");
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			//labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setStyleSheet(titleSheet);
			labelLoadAndRun->setFixedHeight(40);

			btnLoadDefault = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"显示批量运行窗口");
			btnLoadDefault->setEnabled(false);

			QString compute_name = u8"启动计算";
			if (m_nsFlag == 1) compute_name = u8"启动PNS计算";
			btnRunCompute = new QPushButton(QIcon(":/images/update.png"), compute_name);
			btnRunProfiling = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"后处理");
			btnRunHead = new QPushButton(QIcon(":/images/update.png"), u8"运行头部程序");
			btnRunBody = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"运行身部程序");
			btnRunSect = new QPushButton(QIcon(":/images/runParam.png"), u8"运行分段程序");
			btnRunAll = new QPushButton(u8"一键运行");
			btnBatchCalConfig = new QPushButton(u8"批量运行配置");
			btnBatchCal = new QPushButton(u8"批量运行");
			btnShowBatch = new QPushButton(u8"");
			btnRunBatch = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"启动批量计算");
			btnRunBatch->setEnabled(false);
			btnArgBatch = new QPushButton(QIcon(":/images/update.png"), u8"生成批量运行状态");


			QVector<QPushButton*> v = { btnLoadDefault ,btnRunCompute ,btnRunProfiling,btnArgBatch,btnRunBatch };

			QGridLayout* gridLayout = new QGridLayout;
			for (auto it : v) {
				it->setFixedHeight(60);
			}
			
			for (int i = 0; i < v.size(); i++) {
				gridLayout->addWidget(v[i], i, 0, 1, 1);
			}




			QVBoxLayout*layout = new QVBoxLayout;
			layout->addWidget(labelLoadAndRun);
			layout->addLayout(gridLayout);
			layout->addStretch();

			m_runWidget = new QWidget;
			m_runWidget->setLayout(layout);
		}

		void SSPNS::loadStyleSheet(QString sheetName)
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
		void SSPNS::loadProgressWidget() {
			m_progressWidget = new QWidget;
			m_progressWidget->setStyleSheet("background:lightyellow;");


			QLabel* StepCtrl = new QLabel(u8"状态信息");
			StepCtrl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			StepCtrl->setFixedHeight(40);
			StepCtrl->setStyleSheet(titleSheet);
			//StepCtrl->setStyleSheet("background-color:lightblue;font-size:25px;");
			QVBoxLayout*trLayout = new QVBoxLayout;
			trLayout->addWidget(StepCtrl);
			m_processTable = new progressTable(m_nsFlag);
			
			m_processPool = NULL;
			//tableShow = m_tableShow;
			trLayout->addWidget(m_processTable);
			//trLayout->addStretch();
			m_progressWidget->setLayout(trLayout);
		}
		void SSPNS::loadWorkSpace()
		{
			QLabel* labelWorkSpace = new QLabel("工作区");
			labelWorkSpace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelWorkSpace->setStyleSheet(titleSheet);
			//labelWorkSpace->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
			QVBoxLayout*workSpaceLayout = new QVBoxLayout;
			m_workSpaceOp = new WorkSpaceOp(m_nsFlag);


			labelWorkSpace->setFixedHeight(30);
			labelWorkSpace->setFixedWidth(480);
			m_workSpace = new QWidget;
			m_workSpace->setFixedWidth(500);

			workSpaceLayout->addWidget(labelWorkSpace);
			workSpaceLayout->addWidget(m_workSpaceOp);
			workSpaceLayout->setSpacing(0);
			m_workSpace->setLayout(workSpaceLayout);
			QString qssStyleSheet = m_workSpace->styleSheet();
		}
		
		void SSPNS::loadSceneWidget()
		{

			m_sceneWidget = new SceneWidget(m_nsFlag);
			m_sceneWidget->LoadVTKFile();
			setMainSceneWidget();
			
		}
		void SSPNS::setMainSceneWidget() {
			main_sceneWidget = m_sceneWidget;

		}
		void SSPNS::loadOutputLoger()
		{
			m_outputWidget = new QWidget;
			m_outputWidget->setFixedWidth(500);
			m_outputWidget->setStyleSheet("border: 2px solid black");
		}
		
		void SSPNS::loadInputSet1()
		{

		}
		
		void SSPNS::loadInputSet2()
		{


		}
		
		void SSPNS::loadTrTable()
		{
			m_trTableWidget = new QWidget;

			QLabel* StepCtrl = new QLabel("分段信息");
			StepCtrl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			StepCtrl->setFixedHeight(40);
			StepCtrl->setStyleSheet(titleSheet);
			//StepCtrl->setStyleSheet("background-color:lightblue;font-size:25px;");
			QVBoxLayout*trLayout = new QVBoxLayout;
			trLayout->addWidget(StepCtrl);
			m_tableShow = new TrTableShow;
			tableShow = m_tableShow;
			trLayout->addWidget(tableShow);
			//trLayout->addStretch();
			m_trTableWidget->setLayout(trLayout);

		}

		void SSPNS::loadAndRun()
		{
			QLabel*labelLoadAndRun = new QLabel(u8"加载参数及运行");
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setStyleSheet(titleSheet);
			//labelLoadAndRun->setStyleSheet("background:lightblue;font-size:25px;color:#000000;");
			labelLoadAndRun->setFixedHeight(40);

			btnLoadDefault = new QPushButton(QIcon(":/images/singleMPIenv.png"),u8"加载默认参数");
			btnRunHead = new QPushButton(QIcon(":/images/update.png"), u8"运行头部程序");
			btnRunBody = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"运行身部程序");
			btnRunSect = new QPushButton(QIcon(":/images/runParam.png"), u8"运行分段程序");
			btnRunAll = new QPushButton(u8"一键运行");
			btnBatchCalConfig = new QPushButton(u8"批量运行配置");
			btnBatchCal = new QPushButton(u8"批量运行");

			QGridLayout* gridLayout = new QGridLayout;
			btnRunHead->setFixedHeight(60);
			btnRunBody->setFixedHeight(60);
			btnRunSect->setFixedHeight(60);
			//gridLayout->addWidget(btnLoadDefault, 0, 0, 1, 1);
			gridLayout->addWidget(btnRunHead, 0, 0, 1, 1);
			gridLayout->addWidget(btnRunBody, 0, 2, 1, 1);
			gridLayout->addWidget(btnRunSect, 0, 1, 1, 1);
			//gridLayout->addWidget(btnRunAll, 1, 1, 1, 1);
			//gridLayout->addWidget(btnBatchCalConfig, 1, 2, 1, 1);
			//gridLayout->addWidget(btnBatchCal, 1, 3, 1, 1);

			QVBoxLayout*layout = new QVBoxLayout;
			layout->addWidget(labelLoadAndRun);
			layout->addLayout(gridLayout);
			/*layout->addWidget(btnLoadDefault);
			layout->addWidget(btnRunHead);
			layout->addWidget(btnRunBody);
			layout->addWidget(btnRunSect);
			layout->addWidget(btnRunAll);
			layout->addWidget(btnBatchCalConfig);
			layout->addWidget(btnBatchCal);
			layout->addStretch();*/

			m_loadAndRunData = new QWidget;
			m_loadAndRunData->setLayout(layout);
		}
		void SSPNS::showRWMessage(QString messageInfo) {
			QMessageBox::information(this, "提示", messageInfo);
		}
		void SSPNS::onShowBatchWidget()
		{
			
			if (!checkWorkSpace()) return;	///< 如果没有加载工作区则失效
			//展示界面
			
			if (batchConfig == nullptr) { 
				batchConfig = new detail::BatchConfigWidget(m_nsFlag); 
				batchConfig->setParamSetDialog(m_paramDialog);
				//connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, batchConfig, &QWidget::close);
			}
			batchConfig->nsFlag = m_nsFlag;
			batchConfig->close();
			batchConfig->show();
			
		}
		void SSPNS::runHeaderPrograme()
		{
			if (!checkWorkSpace())return;
			if (m_paramDialog->getRestartFlag()&& !checkHeadRestart()  ) {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"没有restart文件,无法续算");
				return;
			}
			if (!m_paramDialog->getRestartFlag() && checkHeadRestart()) {
				auto res = QMessageBox::information(this, u8"提示", u8"当前工作区已经有头部续算文件，是否重新运行头部程序", QMessageBox::Yes | QMessageBox::Cancel);
				if (res == QMessageBox::Yes)
				{
					emit SignalInstance::getInstance()->signalRedictHeadParameter();
				}
				return;
			}
			emit SignalInstance::getInstance()->signalRedictHeadParameter();
		}
		bool SSPNS::checkHeadRestart() {
			
			QFile file(SignalInstance::getInstance()->getWorkSpace() + "/head/restart.dat");
			qDebug() << " file.size()=" << file.size();
			if (file.exists() && file.size()) return true;
			return false;
		}
		void SSPNS::runSectPrograme() {

			if (!checkWorkSpace())return;
			if (myStatus->getBothClip()) {
				emit SignalInstance::getInstance()->signalClearBothClip();

				return;
			}
			outPutLogger->exeProgramSect(SignalInstance::getInstance()->m_workspace);

			
		}
		void SSPNS::loadDefaultParameter()
		{
			if (!checkWorkSpace())return;
			emit SignalInstance::getInstance()->signalLoadDefaultParameter();
			//emit SignalInstance::getInstance()->
		}
		bool SSPNS::checkWorkSpace()
		{
			if (SignalInstance::getInstance()->getWorkSpace() == "")
			{
				QMessageBox::information(this, u8"提示", u8"工作区未加载！", QMessageBox::Ok);
				return false;
			}
			return true;
		}
		void SSPNS::createProcessPool() {
			if (m_processPool != NULL) delete m_processPool;
			m_processPool = new batchProcess(m_nsFlag);
		}
		void SSPNS::deleteProcessPool() {
			if (m_processPool != NULL) delete m_processPool;
			m_processPool = NULL;
			qDebug() << "About to emit signalProcessPoolDeleted";

			emit SignalInstance::getInstance()->signalProcessPoolDeleted();
			qDebug() << "Emitted signalProcessPoolDeleted";
		}
		void SSPNS::initConnect()
		{	
			for (int i = 0; i < argButton.size(); i++) {
				connect(argButton[i], &QPushButton::clicked, this, [=]() {
					changeToArgWindow(i);
				});
			}
			connect(btnRunProfiling, &QPushButton::clicked, this, [=]() {
				qDebug() << "click";
				emit SignalInstance::getInstance()->signalShowProfiling();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalChangeAirParam, this,&SSPNS::changeWallForcestatus);
			connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, [=]() {
				
				if (batchConfig != nullptr) {
					batchConfig->close();
					delete batchConfig;
					batchConfig = nullptr;
				}
				batchConfig = new detail::BatchConfigWidget(m_nsFlag);
				batchConfig->setParamSetDialog(m_paramDialog);
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, this, [=]() {
				if (airButton != nullptr) {
					airButton = nullptr;
				}
				if (batchConfig != nullptr) {
					batchConfig->close();
					delete batchConfig;
					batchConfig = nullptr;
				}
				batchConfig = new detail::BatchConfigWidget(m_nsFlag);
				batchConfig->setParamSetDialog(m_paramDialog);
			});
			//connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, batchConfig, &QWidget::close);
			
			connect(SignalInstance::getInstance(), &SignalInstance::signalClipCancelFinish, this, [=]() {
				myStatus->setStatus(Status::NEW_WORKSPACE);
				myStatus->Notify();
			});

			connect(btnRunBatch, &QPushButton::clicked, this, [=]() {
				emit SignalInstance::getInstance()->signalBatchRun();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessPoolCreate, this, &SSPNS::createProcessPool);
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessPoolDelete, this, &SSPNS::deleteProcessPool);
			connect(btnArgBatch, SIGNAL(clicked(bool)), this, SLOT(onShowBatchWidget()));
			connect(SignalInstance::getInstance(), &SignalInstance::signalShowDialog, this, [=](QString arg1,QString arg2) {
				m_dialog->setTitle(arg1, arg2);
				m_dialog->show();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalCloseDialog, this, [=]() {
				m_dialog->close();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalExeSect, this, [=]() {
				outPutLogger->exeProgramSect(SignalInstance::getInstance()->m_workspace);
			});
			//statusConnect
			connect(SignalInstance::getInstance(), &SignalInstance::signalBuildFinish, this, [=]() {
				myStatus->setStatus(Status::NEW_WORKSPACE);
				myStatus->Notify();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadFinish, this, [=]() {
				myStatus->readStatus(SignalInstance::getInstance()->getWorkSpace().toLocal8Bit().toStdString() + "/status.txt");
				
				/*myStatus->setStatus(Status::NEW_WORKSPACE);
				myStatus->Notify();*/
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalClipShowFinish, this, [=]() {
				myStatus->setStatus(Status::CLIP_WINDOW_SHOW);
				myStatus->Notify();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalClipCancelFinish, this, [=]() {
				myStatus->setStatus(Status::NEW_WORKSPACE);
				myStatus->Notify();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalBatchDoneNot, this, [=]() {
				//m_paramDialog->show();
				btnLoadDefault->setEnabled(false);
				btnRunBatch->setEnabled(false);
				//stackedWidget->setCurrentIndex(0);
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalBatchDone, this, [=]() {
				//m_paramDialog->show();
				btnLoadDefault->setEnabled(true);
				btnRunBatch->setEnabled(true);
				//stackedWidget->setCurrentIndex(0);
			});
			connect(btnLoadDefault, &QPushButton::clicked, this, [=]() {
				//m_paramDialog->show();
				changeToArgWindow(-1);
				//stackedWidget->setCurrentIndex(0);
			});
			connect(btnShowBatch, &QPushButton::clicked, this, [=]() {
				//m_paramDialog->show();
				changeToArgWindow(-1);
				//stackedWidget->setCurrentIndex(0);
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadTrData2, this, [=]()
			{

				QString workPath = SignalInstance::getInstance()->getWorkSpace();
				rwTh->setArg(workPath , "");
				rwTh->setFunFlag(2);
				rwTh->start();
				//重写表格信息
				emit SignalInstance::getInstance()->signalRedictTableData();
			});

			connect(rwTh, SIGNAL(signalRWFinsh(QString)), this, SLOT(showRWMessage(QString)));

			connect(btnRunHead, SIGNAL(clicked(bool)), this, SLOT(runHeaderPrograme()));
			connect(btnRunBody, SIGNAL(clicked(bool)), this, SLOT(runBodyPrograme()));
			connect(btnRunSect, SIGNAL(clicked(bool)), this, SLOT(runSectPrograme()));


			connect(btnBatchCalConfig, SIGNAL(clicked(bool)), this, SLOT(onShowBatchWidget()));
			connect(SignalInstance::getInstance(), &SignalInstance::signalChooseSect, this, [=](int sectId)
			{
				if (main_sceneWidget->sectInfo == NULL)return;
				QColor color = QColorDialog::getColor(Qt::white, this, u8"选择颜色");
				if (!color.isValid())return;
				int r, g, b;
				color.getRgb(&r, &g, &b);
				emit SignalInstance::getInstance()->signalChooseColor(r, g, b, sectId);
			});

			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadAndOut, this, [=](QString arg1, QString arg2)
			{
				
				SignalInstance::getInstance()->color_mutex.lock();
					if (main_sceneWidget->thread() != QThread::currentThread()) {
						qDebug() << u8"不在同一个线程中";
						QMetaObject::invokeMethod(main_sceneWidget, "ColorStop", Qt::QueuedConnection);
					}
					else {
						qDebug() << u8"直接调用";
						main_sceneWidget->ColorStop(); // 直接调用
					}
				SignalInstance::getInstance()->color_mutex.lock();
				SignalInstance::getInstance()->color_mutex.unlock();

				
				rwTh->setArg(arg1, arg2);
				rwTh->setFunFlag(0);
				rwTh->setStatusFlag(1);
				rwTh->start();
				
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadOld, this, [=](QString _gridPath, QString _inpPath, int _index, int _dire)
			{
				SignalInstance::getInstance()->color_mutex.lock();
				if (main_sceneWidget->thread() != QThread::currentThread()) {
					qDebug() << u8"不在同一个线程中";
					QMetaObject::invokeMethod(main_sceneWidget, "ColorStop", Qt::QueuedConnection);
				}
				else {
					qDebug() << u8"直接调用";
					main_sceneWidget->ColorStop(); // 直接调用
				}
				SignalInstance::getInstance()->color_mutex.lock();
				SignalInstance::getInstance()->color_mutex.unlock();

				bool bothFalg = myStatus->getBothClip();
				rwTh->setStatusFlag(0);
				QString bodyGrid= SignalInstance::getWorkSpace()+ "/body_grid.grd";
				QString bodyInp = SignalInstance::getWorkSpace() + "/body_genericbd.inp";
				
				//load old
				auto* thr = QThread::create([=](QString gridPath, QString inpPath, int index) {
					emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");
					emit SignalInstance::getInstance()->signalPreLoad(10);
					qDebug() << "slot begin="<<gridPath;
					main_sceneWidget->slotLoadGridData(gridPath, 0, 1);
					emit SignalInstance::getInstance()->signalPreLoad(100);
					emit SignalInstance::getInstance()->signalCloseDialog();

				}, bodyGrid, bodyInp, _index);

				//
				connect(thr, &QThread::finished, this, [=]() {
					myStatus->setStatus(Status::SEGMENT_FINISH);
					myStatus->Notify();
					qDebug() << "grid=" << SignalInstance::getGridName();
					qDebug() << "inp=" << SignalInstance::getInpName();

				});
				thr->start();
				stackedWidget->setCurrentIndex(2);
				m_trTableWidget->setVisible(false);

			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadAndClip, this, [=](QString _gridPath, QString _inpPath,int _index,int _dire)
			{
				SignalInstance::getInstance()->color_mutex.lock();
				if (main_sceneWidget->thread() != QThread::currentThread()) {
					qDebug() << u8"不在同一个线程中";
					QMetaObject::invokeMethod(main_sceneWidget, "ColorStop", Qt::QueuedConnection);
				}
				else {
					qDebug() << u8"直接调用";
					main_sceneWidget->ColorStop(); // 直接调用
				}
				SignalInstance::getInstance()->color_mutex.lock();
				SignalInstance::getInstance()->color_mutex.unlock();

				bool bothFalg = myStatus->getBothClip();
				rwTh->setStatusFlag(0);
				//load
				auto* thr = QThread::create([=](QString gridPath, QString inpPath, int index) {
					emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");
					emit SignalInstance::getInstance()->signalPreLoad(10);
					main_sceneWidget->slotLoadGridData(gridPath, 0,0);
					emit SignalInstance::getInstance()->signalPreLoad(20);
					
				}, _gridPath,_inpPath,_index);
				//clip
				auto* thr2 = QThread::create([=]() {
					//emit SignalInstance::getInstance()->signalPrintInfo("正在按照上次切割位置切割...");
					if (bothFalg)main_sceneWidget->slotBothClip();
					else main_sceneWidget->slotClip();
				});
				//sect
				connect(thr, &QThread::finished, this, [=]() {
					myStatus->setStatus(Status::DEFAULT_STATUS);
					myStatus->Notify();
					thr2->start();
					
				});
				connect(thr2, &QThread::finished, this, [=]() {
					qDebug() << "myStatus->getLastStatus()=" << myStatus->getLastStatus();
					/*if(myStatus->getLastStatus()!=NEW_WORKSPACE) myStatus->setStatus(myStatus->getLastStatus());
					else */
					myStatus->setStatus(Status::CLIP_FINISH);
					myStatus->Notify();
					
				});
				thr->start();
				stackedWidget->setCurrentIndex(2);
				m_trTableWidget->setVisible(false);
				
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalPreRead, this, [=](QString arg1)
			{
				qDebug() << "arg1=" << arg1;
				rwTh->setArg(arg1, "");
				rwTh->setFunFlag(5);
				rwTh->start();
			});
			connect(btnRunAll, SIGNAL(clicked(bool)), this, SLOT(runAllPrograme()));

			
		}
		void SSPNS::runBodyPrograme()
		{
			if (!checkWorkSpace())return;
			emit SignalInstance::getInstance()->signalRedictBodyParameter();
		}
		void SSPNS::runAllPrograme()
		{
			qDebug() << "begin runAllPrograme";
			if (!checkWorkSpace())return;
			//先判断是否已经运行过分段程序
			BodyPathChecker tempChecker;
			QString truePath = tempChecker.checkAndGetPath(SignalInstance::getInstance()->m_workspace + "/body/trData.dat");

			QFile file(truePath);
			if (!file.exists())
			{
				QMessageBox::information(this, u8"提示", u8"还未分段无法运行身部程序!");
				return;
			}
			outPutLogger->exeRunAll(SignalInstance::getInstance()->m_workspace);
		}
	}// sspns
}// platforms
