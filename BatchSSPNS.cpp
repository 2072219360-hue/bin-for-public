#include "BatchSSPNS.h"
extern QString appComDir;
extern SceneWidget*main_sceneWidget;
SceneWidget*main_sceneWidget2 = NULL;
extern RWThread* rwTh;
namespace platforms {
	namespace sspns {

		BatchSSPNS::BatchSSPNS(){
			initUI();
			initData();
			setBatchMode();
			
			//qDebug() << "workSpace=" << SignalInstance::getWorkSpace();
			double stdWidth = 2880, stdHeight = 1800;
			//stdWidth = 300, stdHeight = 500;
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
		BatchSSPNS::~BatchSSPNS() {
			delete m_dialog;
			if (m_processPool != NULL) delete m_processPool;
			m_processPool = nullptr;
			if (trWidget != nullptr) delete trWidget;
			trWidget = nullptr;
			if (widget != nullptr) delete widget;
			widget = nullptr;
		}
		void BatchSSPNS::initData() {
			m_processPool = NULL;
			SignalInstance::setWorkSpace("");
		}
		void BatchSSPNS::setBatchMode() {
			m_sceneWidget->setBatchMode();
			m_workSpaceOp->setBatchMode();
		}
		void BatchSSPNS::loadSceneWidget() {
			m_sceneWidget = new SceneWidget();
			m_sceneWidget->LoadVTKFile();
			
			setMainSceneWidget();
			
			//qDebug() << "main_sceneWidget=" << main_sceneWidget<<" this->wdiget="<< m_sceneWidget;
		}
		void BatchSSPNS::setMainSceneWidget() {
			main_sceneWidget = m_sceneWidget;
			/*if (main_sceneWidget == m_sceneWidget) return;
			if (main_sceneWidget == NULL) main_sceneWidget = m_sceneWidget;
			else if (main_sceneWidget2 == NULL) {
				main_sceneWidget2 = main_sceneWidget;
				main_sceneWidget = m_sceneWidget;
			}
			else {
				swap(main_sceneWidget, main_sceneWidget2);
			}*/
		}
		void BatchSSPNS::updateWindow() {
			m_workSpaceOp->setCurrentWorkSpaceLineEdit();
			setMainSceneWidget();
		}
		void BatchSSPNS::loadWorkSpace() {
			QLabel* labelWorkSpace = new QLabel("工作区");
			labelWorkSpace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelWorkSpace->setStyleSheet(titleSheet);
			QVBoxLayout*workSpaceLayout = new QVBoxLayout;
			m_workSpaceOp = new WorkSpaceOp;


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
		void BatchSSPNS::initLayout() {
			//m_sceneWidget vtk显示窗口
			//outPutLogger命令行窗口
			//m_workSpace工作区窗口
			//m_runWidget运行按钮窗口
			//m_progressWidget进程控制窗口
			//m_argWidget 参数显示窗口
			QSplitter* splitterVisDos = new QSplitter(Qt::Orientation::Vertical);
			//splitterVisDos->addWidget(fatherWidget);
			splitterVisDos->addWidget(m_sceneWidget);
			splitterVisDos->addWidget(outPutLogger);
			
			//右边上半部分
			QWidget*widgetHead = new QWidget;
			QHBoxLayout*layoutHead = new QHBoxLayout;
			layoutHead->addWidget(m_workSpace);
			layoutHead->addWidget(m_runWidget);
			widgetHead->setLayout(layoutHead);


			QSplitter*splitterHead = new QSplitter(Qt::Orientation::Vertical);
			splitterHead->addWidget(widgetHead);
			splitterHead->addWidget(m_progressWidget);
			splitterHead->addWidget(m_argWidget);

			/*QSplitter*splitterHead3 = new QSplitter(Qt::Orientation::Vertical);
			splitterHead2->addWidget(splitterHead2);
			splitterHead2->addWidget(m_argWidget);*/


			QSplitter*splitterLeftRight = new QSplitter(Qt::Orientation::Horizontal);
			splitterLeftRight->addWidget(splitterVisDos);
			splitterLeftRight->addWidget(splitterHead);
			QHBoxLayout* layoutMain = new QHBoxLayout;
			layoutMain->addWidget(splitterLeftRight);
			this->setLayout(layoutMain);
			int fac1 = 95, fac2 = 60, fac3 = 30, fac4 = 55;
			splitterVisDos->setStretchFactor(0, fac1);
			splitterVisDos->setStretchFactor(1, 100 - fac1);

			splitterLeftRight->setStretchFactor(0, fac2);
			splitterLeftRight->setStretchFactor(1, 100 - fac2);

			splitterHead->setStretchFactor(0, fac3);
			splitterHead->setStretchFactor(1, fac4);
			splitterHead->setStretchFactor(2, 100 - fac3-fac4);
			//this->setLayout(layoutMain);
			loadStyleSheet("SSPNS");
		}
		void BatchSSPNS::initConnection() {
			connect(btnArgBatch, SIGNAL(clicked(bool)), this, SLOT(onShowBatchWidget()));
			
			connect(SignalInstance::getInstance(), &SignalInstance::signalShowDialog, this, [=](QString arg1, QString arg2) {
				m_dialog->setTitle(arg1, arg2);
				m_dialog->show();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalCloseDialog, this, [=]() {
				m_dialog->close();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalExeSect, this, [=]() {
				outPutLogger->exeProgramSect(SignalInstance::getInstance()->m_workspace);
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalLoadTrData2, this, [=]()
			{

				QString workPath = SignalInstance::getInstance()->getWorkSpace();
				rwTh->setArg(workPath + "/", "");
				rwTh->setFunFlag(2);
				rwTh->start();
				//重写表格信息
				emit SignalInstance::getInstance()->signalRedictTableData();
			});

			connect(rwTh, SIGNAL(signalRWFinsh(QString)), this, SLOT(showRWMessage(QString)));
			connect(SignalInstance::getInstance(), SIGNAL(signalPrintInfo(QString)), this, SLOT(showRWMessage(QString)));

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

				rwTh->setArg(arg1, arg2);
				rwTh->setFunFlag(0);
				rwTh->start();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalPreRead, this, [=](QString arg1)
			{
				qDebug() << "arg1=" << arg1;
				rwTh->setArg(arg1, "");
				rwTh->setFunFlag(5);
				rwTh->start();
			});
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessPoolCreate, this, &BatchSSPNS::createProcessPool);
			connect(SignalInstance::getInstance(), &SignalInstance::signalProcessPoolDelete, this, &BatchSSPNS::deleteProcessPool);
			connect(btnRunBatch, &QPushButton::clicked, this, [=]() {
				emit SignalInstance::getInstance()->signalBatchRun();
			});
		}
		void BatchSSPNS::createProcessPool() {
			if (m_processPool != NULL) delete m_processPool;
			m_processPool = new batchProcess();
		}
		void BatchSSPNS::deleteProcessPool() {
			if (m_processPool != NULL) delete m_processPool;
			m_processPool = NULL;
		}
		void BatchSSPNS::loadOutputLoger() {
			outPutLogger = new OutPutLogger;
			outPutLogger->setBatchMode();
			
		}
		
		
		void BatchSSPNS::loadRunButton() {
			QLabel*labelLoadAndRun = new QLabel(u8"加载参数及运行");
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			labelLoadAndRun->setStyleSheet(titleSheet);
			labelLoadAndRun->setFixedHeight(40);

			btnRunBatch = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"启动批量计算");
			btnArgBatch = new QPushButton(QIcon(":/images/update.png"), u8"生成批量运行状态");
			

			QGridLayout* gridLayout = new QGridLayout;
			btnRunBatch->setFixedHeight(60);
			btnArgBatch->setFixedHeight(60);

			gridLayout->addWidget(btnRunBatch, 0, 0, 1, 1);
			gridLayout->addWidget(btnArgBatch, 1, 0, 1, 1);
			
			

			QVBoxLayout*layout = new QVBoxLayout;
			layout->addWidget(labelLoadAndRun);
			layout->addLayout(gridLayout);
			layout->addStretch();

			m_runWidget = new QWidget;
			m_runWidget->setLayout(layout);
		}
		void BatchSSPNS::loadProgressWidget() {
			m_progressWidget = new QWidget;
			m_progressWidget->setStyleSheet("background:lightyellow;");

			
			QLabel* StepCtrl = new QLabel(u8"状态信息");
			StepCtrl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			StepCtrl->setFixedHeight(40);
			StepCtrl->setStyleSheet(titleSheet);
			//StepCtrl->setStyleSheet("background-color:lightblue;font-size:25px;");
			QVBoxLayout*trLayout = new QVBoxLayout;
			trLayout->addWidget(StepCtrl);
			m_tableShow = new progressTable;
			//tableShow = m_tableShow;
			trLayout->addWidget(m_tableShow);
			//trLayout->addStretch();
			m_progressWidget->setLayout(trLayout);
		}
		bool BatchSSPNS::checkWorkSpace()
		{
			if (SignalInstance::getInstance()->getWorkSpace() == "")
			{
				QMessageBox::information(this, u8"提示", u8"工作区未加载！", QMessageBox::Ok);
				return false;
			}
			return true;
		}
		void BatchSSPNS::onShowBatchWidget()
		{

			if (!checkWorkSpace()) { 
				//emit SignalInstance::getInstance()->signalPrintInfo(u8"请先打开工作区");
				return; 
			}
			QFile batchConfigDir(SignalInstance::getWorkSpace()+ "/Batch/batchConfig.json");
			if (batchConfigDir.exists()) {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"该工作区已经配置过参数");
				return;
			}
			//展示界面
			detail::BatchConfigWidget* batchConfig = new detail::BatchConfigWidget(0);
			batchConfig->show();
		}
		void BatchSSPNS::showRWMessage(QString messageInfo) {
			QMessageBox::information(this, "提示", messageInfo);
		}
		void BatchSSPNS::loadArgWidget() {
			m_argWidget = new QWidget;
			QVBoxLayout* layout = new QVBoxLayout();
			m_argWidget2 = new publicArgWindow;
			trWidget = nullptr;
			widget = nullptr;
			QPushButton* button1 = new QPushButton(QIcon(":/images/singleMPIenv.png"), u8"点击查看分段信息");
			connect(button1, &QPushButton::clicked, this, [=]() {
				if (SignalInstance::getInstance()->getWorkSpace() == "")
				{
					QMessageBox::information(this, u8"提示", u8"工作区未加载！", QMessageBox::Ok);
					return;
				}
				//static QWidget* widget = nullptr;
				//static TrTableShow* trWidget = nullptr;
				if (widget == nullptr) {
					widget = new QWidget;
					widget->setMinimumSize(800,400);
					//widget->setWindowFlag(Qt::WindowCloseButtonHint);
					widget->setWindowFlags(widget->windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);
				}
				
				if (trWidget == nullptr) {
					qDebug() << "trWidget";
					widget->setLayout(nullptr);
					trWidget = new TrTableShow;
					trWidget->setBatchmode();
					QVBoxLayout* layout = new QVBoxLayout();
					layout->addWidget(trWidget);
					trWidget->onPreLoadTableData();
					widget->setLayout(layout);
				}
				qDebug() << "trWidget=" << trWidget;
				//emit SignalInstance::getInstance()->signalRedictTableData();
				trWidget->initSectPreData();
				trWidget->loadTrData();
				m_sceneWidget->trOnlyLoad();
				//emit SignalInstance::getInstance()->signalOnlyLoadTrData();
				/*m_sceneWidget->trOnlyLoad();
				emit SignalInstance::getInstance()->signalRedictTableData();*/
				widget->show();
				
			});
			layout->addWidget(button1);
			layout->addWidget(m_argWidget2);
			m_argWidget->setLayout(layout);
			//m_argWidget->setStyleSheet("background:lightblue;");
		}
		
		void BatchSSPNS::initUI() {
			m_dialog = new ProgressDialog("Loading...", "Please wait for finishing running...");
			m_dialog->close();
			titleSheet = u8"background:lightblue;font-family: \"Microsoft YaHei\";font-size:10pt;color:#000000;";

			loadWorkSpace();
			loadSceneWidget();
			loadOutputLoger();
			loadRunButton();
			loadArgWidget();
			loadProgressWidget();

			initConnection();
			initLayout();
			
		}
		void BatchSSPNS::loadStyleSheet(QString sheetName)
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
	}
}


