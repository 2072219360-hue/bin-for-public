#ifndef DIAG_EXT_DEMO_H
#define DIAG_EXT_DEMO_H

#include "QtGui/QtGui"

class DiagExtDemo:public QDialog
{
	Q_OBJECT
public:
	DiagExtDemo(QWidget *parent=0);

	void createBaseInfo();
	void createDetailInfo();

public slots:
	void slotExtension();

private:
	QWidget *baseWidget;
	QWidget *detailWidget;


};
#endif