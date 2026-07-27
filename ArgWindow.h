#pragma once

#include <QWidget>
#include<QPushButton>
#include<qlineedit.h>
#include<qlabel.h>
#include<QSpinBox>
#include<QDoubleSpinBox>
#include<QCombobox>
#include<QRadioButton>
#include<QButtonGroup>
#include "ECHVSignalInstance.h"
#include "ArgWindow.h"
#include <QVBoxLayout>
#include<qfiledialog.h>
#include<QMessageBox>
#include"argData.h"
#include<QDebug>
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
namespace platforms {
	namespace echv {

		//该类用来显示并设置各种参数
		class ArgWindow : public QWidget
		{
			Q_OBJECT

		public:
			ArgWindow(QWidget *parent = nullptr);
			~ArgWindow();
			void connectAll();
			void selectFile();
			void checkArg();
		public slots:

		private:
			
			QPushButton *selectFileButton, *determineArgButton, *algorithmButton1,*algorithmButton2, *algorithmButton3;
			QLineEdit *fileText;
			QLabel *m_label0, *m_label1, *m_label2, *m_label3;
			QDoubleSpinBox *arg1Box;
			QComboBox *arg2Box;
			QButtonGroup *arg3Box;
			QRadioButton *arg3Op1, *arg3Op2;


			QLabel *label_xSize, *label_ySize, *label_zSize,*label_interval;
			
			QSpinBox *box_xSize,*box_ySize,*box_zSize;
			QDoubleSpinBox *box_interval;
		};
	}
}



