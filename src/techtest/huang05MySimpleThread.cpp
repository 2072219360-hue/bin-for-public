#include "QtCore/QtCore"
#include "iostream"

#include "huang05MySimpleThread.h"

MySimpleThread::MySimpleThread()
{
	stopped = false;
}

void MySimpleThread::setMessage(const QString &message)
{
	messageStr=message;
}

void MySimpleThread::run()
{
	while (!stopped)
		std::cerr<<qPrintable(messageStr);

	stopped = false;
	std::cerr<<std::endl;
}

void MySimpleThread::stop()
{
	stopped = true;
}


