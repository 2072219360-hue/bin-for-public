#pragma once
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <SignalInstance.h>
#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#include"bodyPathChecker.h"
class SectInfoDialog :
	public QDialog
{
	Q_OBJECT
public:
	SectInfoDialog(int id, QDialog*parent = nullptr);
	~SectInfoDialog();
	void init();
	void loadStyleSheet(QString sheetName);
	void loadData();
public slots:
	void onLoadTrDada();
private:
	int m_sectId;
	QLabel*m_xStep;
	QLineEdit*m_xStepLineEdit;
	QLabel*m_xStepMin;
	QLineEdit*m_xStepMinLineEdit;
	QLabel*m_min1;
	QLineEdit*m_min1LineEdit;
	QLabel*m_max;
	QLineEdit*m_maxLineEdit;
	QLabel* m_kStep;
	QLineEdit*m_kStepLineEdit;
	QLabel*m_nCstp;
	QLineEdit*m_nCstpLineEdit;
	QGridLayout*m_mainGridLayout;
	QVBoxLayout*m_mainLayout;
	QHBoxLayout*m_btnLayout;

	QPushButton*m_saveBtn;
	QPushButton*m_cancelBtn;
};

