#include "DynLoaderUiTest.h"
#include "QtUiTools/QtUiTools"
#include "QtGui/QtGui"

DynLoaderUiTest::DynLoaderUiTest(QWidget *parent):QDialog(parent)
{
	QTabWidget *tabW=new QTabWidget(this);

	QWidget *w1=new QWidget;
	firstUi.setupUi(w1);

	QWidget *w2=new QWidget;
	secondUi.setupUi(w2);

	tabW->addTab(w1,"First");
	tabW->addTab(w2,"Second");

	tabW->resize(300,300);

    connect(firstUi.childPushButton,SIGNAL(clicked()),this,SLOT(slotChild())); 
    connect(secondUi.closePushButton,SIGNAL(clicked()),this,SLOT(close()));   

}

void DynLoaderUiTest::slotChild()
{
	QUiLoader loader;
	QFile file(":/forms/third.ui");
	file.open(QFile::ReadOnly);
	QWidget *third=loader.load(&file);
	file.close();

	third->show();
}

