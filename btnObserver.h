#pragma once
#include"myObserver.h"
#include"statusSubject.h"
#include<QPushButton>
#include<QAbstractButton>
#include<QWidget>
#include<QDebug>
#include<QThread>
#include<QCoreApplication>
#include<QPointer>
class btnObserver:public myObserver
{
public:
	btnObserver(statusSubject* sub, QWidget* btn, btnType type) :m_sub(sub), m_btn(btn), m_btnType(type) {};
	void Update();
	void safeCall();
private:
	bool chooseAction();
	void setEnable();
	void setDisable();
	statusSubject* m_sub;
	btnType m_btnType;
	QWidget* m_btn;
	QMutex mu;
};

