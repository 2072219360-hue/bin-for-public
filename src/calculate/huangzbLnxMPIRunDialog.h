#ifndef HUANGZB_WIN_MPI_RUNDIALOG_H
#define HUANGZB_WIN_MPI_RUNDIALOG_H

#include "QtGui/QtGui"

class THuangzbWinMPIRunDialog:public QDialog
{
	Q_OBJECT
public:
	THuangzbWinMPIRunDialog(QWidget *parent=0);

	void createBaseInfo();
	void createDetailInfo();

public slots:
	void slotExtension();

private:
	QWidget *baseWidget;
	QWidget *detailWidget;
public:
	QLineEdit *MPINPEdit;


};
#endif