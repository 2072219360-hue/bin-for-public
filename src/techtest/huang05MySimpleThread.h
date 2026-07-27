#ifndef MY_SIMPLETHREAD_H
#define MY_SIMPLETHREAD_H

#include "QtCore/QThread"

class MySimpleThread:public QThread
{
	Q_OBJECT

public:
	MySimpleThread();
	void setMessage(const QString &message);
	void stop();

protected:
	void run();

private:
	QString messageStr;
	volatile bool stopped;

};

#endif
