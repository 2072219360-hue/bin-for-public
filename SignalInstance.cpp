#include "SignalInstance.h"
SignalInstance* SignalInstance::instance = nullptr;
bool SignalInstance::batchFlag = false;
int SignalInstance::nsFlag = 0;
SignalInstance * SignalInstance::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SignalInstance();
	}
	return instance;
}



SignalInstance::SignalInstance()
{
	//qDebug() << __FUNCTION__;
}

SignalInstance::~SignalInstance()
{
	//qDebug() << __FUNCTION__;
}
