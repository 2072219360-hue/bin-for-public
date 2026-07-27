#ifndef PROGRESS_TEST_H
#define PROGRESS_TEST_H

#include "QtGui/QtGui"

class ProgressTest:public QDialog
{
	Q_OBJECT
public:
	ProgressTest(QWidget *parent =0,Qt::WindowFlags f=0);
	~ProgressTest();

public:
	QLabel *numLabel;
	QLineEdit *numLineEdit;
	QLabel *typeLabel;
	QComboBox *typeComboBox;

	QProgressBar *progressBar;

	QPushButton *startPushButton;

private slots:
	void slotStart();

};


#endif
