#pragma once
#include"myObserver.h"
class mySubject
{
	virtual void Attach(myObserver*)=0;
	virtual void Remove(myObserver*) = 0;
	virtual void Notify() = 0;
};

