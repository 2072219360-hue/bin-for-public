#include "ArgWindow.h"

namespace platforms {
	namespace echv {
		ArgWindow::ArgWindow(QWidget *parent)
			: QWidget(parent)
		{
			
			setMinimumSize(400, 400);
			setMaximumSize(2000, 2000);
			//setFixedSize(500, 500);
			int buttonSize_x = 200, buttonSize_y = 40;
			QPalette palette = this->palette();
			palette.setColor(QPalette::Window, Qt::lightGray);
			this->setPalette(palette);
			this->setAutoFillBackground(true);

			QGridLayout *layout = new QGridLayout(this);
			//外边距
			layout->setMargin(30);
			//内边距
			layout->setSpacing(10);
			QFont font;
			font.setPointSize(15);  // 设置字体大小
			this->setFont(font);

			selectFileButton = new QPushButton(u8"选择文件夹", this);
			//selectFileButton->setFixedSize(buttonSize_x, buttonSize_y);
			determineArgButton = new QPushButton(u8"完成参数设置", this);
			//determineArgButton->setFixedSize(buttonSize_x, buttonSize_y);
			fileText = new QLineEdit(this);
			fileText->setText("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/flat_14_input.vtk");
			//设置只读
			fileText->setReadOnly(false);
			m_label0 = new QLabel(u8"请选择文件夹(vtk文件):", this);

			m_label1 = new QLabel(u8"网格维度", this);
			/*arg1Box = new QDoubleSpinBox(this);
			arg1Box->setValue(1.0);
			arg1Box->setSingleStep(0.1);
			arg1Box->setMinimum(0);
			arg1Box->setMaximum(1000);*/
			/*int defaultDim = 30;
			label_xSize=new QLabel(u8"xDimension:", this); 
			label_ySize = new QLabel(u8"yDimension:", this);
			label_zSize = new QLabel(u8"zDimension:", this);
			box_xSize = new QSpinBox(this);
			box_xSize->setValue(1);
			box_xSize->setMinimum(2);
			box_xSize->setMaximum(300);
			box_xSize->setValue(defaultDim);
			box_ySize = new QSpinBox(this);
			box_ySize->setValue(1);
			box_ySize->setMinimum(2);
			box_ySize->setMaximum(300);
			box_ySize->setValue(defaultDim);
			box_zSize = new QSpinBox(this);
			box_zSize->setValue(1);
			box_zSize->setMinimum(2);
			box_zSize->setMaximum(300);
			box_zSize->setValue(defaultDim);*/
			label_interval = new QLabel(u8"interval:", this);
			box_interval = new QDoubleSpinBox(this);
			box_interval->setValue(0.3);
			box_interval->setSingleStep(0.1);
			box_interval->setMinimum(0.01);
			box_interval->setMaximum(100);
			
			

			m_label2 = new QLabel("arg2:", this);
			arg2Box = new QComboBox(this);
			arg2Box->addItem("option1");
			arg2Box->addItem("option2");
			m_label3 = new QLabel("arg3:", this);
			arg3Op1 = new QRadioButton("op1", this);
			arg3Op2 = new QRadioButton("op2", this);
			arg3Box = new QButtonGroup(this);
			arg3Box->addButton(arg3Op1, 0);
			arg3Box->addButton(arg3Op2, 1);
			arg3Op1->setChecked(true);
			algorithmButton1 = new QPushButton(u8"计算指标", this);
			algorithmButton2 = new QPushButton(u8"构建轮廓树", this);
			algorithmButton3 = new QPushButton(u8"发夹涡识别", this);
			//组件布局
			layout->addWidget(m_label0, 0, 0);
			layout->addWidget(fileText, 0, 1,1,4);
			layout->addWidget(selectFileButton, 0, 5,1,2);

			layout->addWidget(m_label1, 1, 0);
			layout->addWidget(label_interval, 1, 1); layout->addWidget(box_interval, 1, 2);
			/*layout->addWidget(label_xSize, 1, 1); layout->addWidget(box_xSize, 1, 2);
			layout->addWidget(label_ySize, 1, 3); layout->addWidget(box_ySize, 1, 4);
			layout->addWidget(label_zSize, 1, 5); layout->addWidget(box_zSize, 1, 6);*/

			layout->addWidget(m_label2, 2, 0);
			layout->addWidget(arg2Box, 2, 1);

			layout->addWidget(m_label3, 3, 0);
			layout->addWidget(arg3Op1, 3, 1);
			layout->addWidget(arg3Op2, 3, 2);

			layout->addWidget(determineArgButton, 4, 0);
			layout->addWidget(algorithmButton1, 4, 1,1,2);
			layout->addWidget(algorithmButton2, 4, 3,1,2);
			layout->addWidget(algorithmButton3, 4, 5,1,2);
			connectAll();
		}

		ArgWindow::~ArgWindow()
		{
			delete selectFileButton;
			delete fileText;
			delete determineArgButton;
			delete algorithmButton1;
			delete algorithmButton2;
			delete algorithmButton3;
			delete m_label0;
			delete m_label1;
			delete m_label2;
			delete m_label3;
			//delete arg1Box;
			delete arg2Box;
			delete arg3Box;
			delete arg3Op1;
			delete arg3Op2;

			/*delete label_xSize;
			delete label_ySize;
			delete label_zSize;*/
			delete label_interval;
			/*delete box_xSize;
			delete box_ySize;
			delete box_zSize;*/
			delete box_interval;
		}
		void ArgWindow::connectAll() {
			connect(selectFileButton, &QPushButton::clicked, this, &ArgWindow::selectFile);
			connect(determineArgButton, &QPushButton::clicked, this, &ArgWindow::checkArg);
			connect(algorithmButton1, &QPushButton::clicked, []() {
				emit ECHVINSTANCE->signalCreateFullDataset();
			});
			connect(algorithmButton2, &QPushButton::clicked, []() {
				emit ECHVINSTANCE->signalCreateTrees();
			});
			connect(algorithmButton3, &QPushButton::clicked, []() {
				emit ECHVINSTANCE->signalRelevance();
			});
		}
		void ArgWindow::selectFile() {
			QString dir = QFileDialog::getOpenFileName(this, tr("Open File"), ".", u8"vtk文件(*.vtk);;图片文件(*.jpg)");
			fileText->setText(dir);
		}
		void ArgWindow::checkArg() {
			bool flag = true;
			QString filePath = fileText->text();
			QFileInfo fileInfo(filePath);
			int xSize = 100,  ySize = 100, zSize = 100;
			//int xSize = box_xSize->value(), ySize = box_ySize->value(), zSize = box_zSize->value();
			double interval = box_interval->value();
			//qDebug() << " xSize=" << xSize << " ySize=" << ySize << " zSize=" << zSize;
			if (!fileInfo.isFile()) flag = false;
			if (flag == false) {
				QMessageBox *msg = new QMessageBox(this);
				msg->setIcon(QMessageBox::Information);
				msg->setWindowTitle("warning!");
				msg->setText("arg invalible!");
				msg->show();
			}
			else {
				argData *curruntArg = new argData(filePath.toLocal8Bit().toStdString(),xSize,ySize,zSize, interval);
				emit ECHVINSTANCE->signalUpdateArg(curruntArg);
				//emit SignalInstance::getInstance()->signalReloadVTK();
			}
		}
	}
}

