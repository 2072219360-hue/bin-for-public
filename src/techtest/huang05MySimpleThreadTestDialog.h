#ifndef TST_DIALOG_H
#define TST_DIALOG_H

#include "QtGui/QDialog"
#include "huang05MySimpleThread.h"

class QPushButton;

class TSTDialogForMySimpleThread:public QDialog
{
	Q_OBJECT

public:
	TSTDialogForMySimpleThread(QWidget *parent=0);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void startOrStopThreadA();
	void startOrStopThreadB();

private:
    MySimpleThread threadA;
    MySimpleThread threadB;

    QPushButton *threadAButton;
    QPushButton *threadBButton;
    QPushButton *quitButton;
	
};


#endif
