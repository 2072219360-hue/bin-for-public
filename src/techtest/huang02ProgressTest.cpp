#include "huang02ProgressTest.h"
#include "QtTest/QtTest"

ProgressTest::ProgressTest(QWidget *parent,Qt::WindowFlags f):QDialog(parent,f)
{
	setWindowTitle(tr("Progress"));

	QGridLayout *layout=new QGridLayout(this);

	numLabel=new QLabel(tr("File Num:"));
	numLineEdit=new QLineEdit;
	numLineEdit->setText(tr("10"));

	typeLabel=new QLabel(tr("Progress Type:"));
	typeComboBox=new QComboBox;
	typeComboBox->addItem(tr("ProgressBar"));
	typeComboBox->addItem(tr("ProgressDialog"));


	progressBar=new QProgressBar;
	startPushButton=new QPushButton(tr("Start"));

	layout->addWidget(numLabel,0,0);
	layout->addWidget(numLineEdit,0,1);
	layout->addWidget(typeLabel,1,0);
	layout->addWidget(typeComboBox,1,1);
	layout->addWidget(progressBar,2,0,1,2);
	layout->addWidget(startPushButton,3,1);
	layout->setMargin(15);
	layout->setSpacing(10);

	connect(startPushButton,SIGNAL(clicked()),this,SLOT(slotStart()));


}

ProgressTest::~ProgressTest()
{

}


void ProgressTest::slotStart()
{
	int num=numLineEdit->text().toInt();

	if (typeComboBox->currentIndex()==0)
	{
		progressBar->setRange(0,num);
		for(int i=1;i<num+1;i++)
		{
			progressBar->setValue(i);
			//QTest::qWait(1000);
		}

	}
	else if (typeComboBox->currentIndex()==1)
	{
		QProgressDialog *pd=new QProgressDialog(this);
		QFont font("ZYSong18030",12);
		pd->setFont(font);

		pd->setWindowModality(Qt::WindowModal);

		pd->setMinimumDuration(5);
		pd->setWindowTitle(tr("Please Wait"));
		pd->setLabelText(tr("Copying..."));
		pd->setCancelButtonText(tr("Cancel"));
		pd->setRange(0,num);

		for(int i=1;i<num+1;i++)
		{
			pd->setValue(i);
			qApp->processEvents();
			//QTest::qWait(1000);
			if (pd->wasCanceled())
				return;
		}

	}

}

