#include "QtGui/QtGui"

#include "huang05MySimpleThreadTestDialog.h"


TSTDialogForMySimpleThread::TSTDialogForMySimpleThread(QWidget *parent):QDialog(parent)
{
    threadA.setMessage("A");
    threadB.setMessage("B");

    threadAButton = new QPushButton(tr("Start A"));
    threadBButton = new QPushButton(tr("Start B"));
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setDefault(true);

    connect(threadAButton, SIGNAL(clicked()),
            this, SLOT(startOrStopThreadA()));
    connect(threadBButton, SIGNAL(clicked()),
            this, SLOT(startOrStopThreadB()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(threadAButton);
    mainLayout->addWidget(threadBButton);
    mainLayout->addWidget(quitButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Threads"));

}

void TSTDialogForMySimpleThread::startOrStopThreadA()
{
    if (threadA.isRunning()) {
        threadA.stop();
        threadAButton->setText(tr("Start A"));
    } else {
        threadA.start();
        threadAButton->setText(tr("Stop A"));
    }

}

void TSTDialogForMySimpleThread::startOrStopThreadB()
{
    if (threadB.isRunning()) {
        threadB.stop();
        threadBButton->setText(tr("Start B"));
    } else {
        threadB.start();
        threadBButton->setText(tr("Stop B"));
    }
}

void TSTDialogForMySimpleThread::closeEvent(QCloseEvent *event)
{
	threadA.stop();
	threadB.stop();
	threadA.wait();
	threadB.wait();
	event->accept();
}
