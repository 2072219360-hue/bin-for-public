#include "ECHV.h"


namespace platforms {
	namespace echv {
		ECHV::ECHV(QWidget *parent )
			: QWidget(parent)
		{
			
			myServer = new dataServer();
			/*myData = new runData();
			myArg = new argData();*/
			//QFrame *frame = new QFrame(this);
			QHBoxLayout *mainLayout = new QHBoxLayout(this);
			QWidget *subWidget1 = new QWidget(this);
			QWidget *subWidget2 = new QWidget(this);
			QVBoxLayout *layout1 = new QVBoxLayout;
			QVBoxLayout *layout2 = new QVBoxLayout;

			//设置窗口标题
			setWindowTitle("展示系统-showWindow");
			//重设窗口大小
			//resize(1500, 1000);
			//frame->setLayout(layout);

			//外边距
			mainLayout->setMargin(30);
			//内边距
			mainLayout->setSpacing(30);
			//int layout_gap = 50;
			graphWin1 = new GraphWindow("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/graph1.png", this);
			graphWin2 = new GraphWindow("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/graph2.png", this);
			graphWin3 = new GraphWindow("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/graph3.png", this);
			graphWin2->move(500, 500);
			
			//layout->addWidget(graphWin1, 1, 0);
			//layout->addWidget(graphWin2, 1, 1);
			//layout->addWidget(graphWin3,1,1);

			argWin = new ArgWindow(this);
			//layout->addWidget(argWin, 0, 0);
			vtkWin = new echvVTKWin(this);
			//layout->addWidget(vtkWin, 0, 1);
			
			
			
			layout1->setMargin(30);
			layout1->setSpacing(30);
			layout1->addWidget(vtkWin);
			layout1->addWidget(argWin);
			subWidget1->setLayout(layout1);
			subWidget1->setMinimumSize(500, 500);
			subWidget1->setMaximumSize(1500, 1500);

			layout2->setMargin(30);
			layout2->setSpacing(30);
			layout2->addWidget(graphWin1);
			layout2->addWidget(graphWin2);
			layout2->addWidget(graphWin3);
			subWidget2->setLayout(layout2);
			subWidget2->setMinimumSize(1000, 1000);
			subWidget2->setMaximumSize(1500, 1500);

			mainLayout->addWidget(subWidget1);
			mainLayout->addWidget(subWidget2);

			this->setLayout(mainLayout);
			connectAll();
		}

		ECHV::~ECHV()
		{
			
			delete graphWin1;
			delete graphWin2;
			delete graphWin3;
			delete argWin;
			delete vtkWin;

			delete myServer;
			
			/*delete myData;
			delete myArg;*/
		}
		void ECHV::connectAll() {
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalPrint, this, [=](QString str) {
				QMessageBox *msg = new QMessageBox(this);
				msg->setIcon(QMessageBox::Information);
				msg->setWindowTitle("print information");
				msg->setText(str);
				msg->show();
			});

			/*connect(SignalInstance::getInstance(), &SignalInstance::signalReloadVTK, this, [=] {
				thr->setFunFlag(1);
				thr->start();
			});*/
		}
	}
}
