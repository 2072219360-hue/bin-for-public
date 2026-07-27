#include "RWThread.h"
#include <chrono>
using platforms::sspns::reveal::SceneWidget;
extern SceneWidget* main_sceneWidget;
extern QMutex* rwMutex;
RWThread::RWThread(QString arg1, QString arg2,int funFlag) {
	this->funFlag = funFlag;
	this->arg1 = arg1;
	this->arg2 = arg2;
}
void RWThread::run() {

	if (this->arg1 == "")return;
	qDebug() << "main_sceneWidget=" << main_sceneWidget;
	// 目前只用得到0 2 5 6 7
	if(funFlag==0)main_sceneWidget->readAndOut(arg1, arg2);
	else if (funFlag == 1) main_sceneWidget->onlyReadGrid(arg1);
	else if (funFlag == 2) main_sceneWidget->onlyReadSect(arg1);
	else if (funFlag == 3) main_sceneWidget->onlyPreReadGirdSect(arg1);
	else if (funFlag == 4) main_sceneWidget->onlyOut(arg2);
	else if (funFlag == 5) main_sceneWidget->preRead(arg1);
	else if (funFlag == 6) main_sceneWidget->showClip();
	else if (funFlag == 7) main_sceneWidget->cancelShowClip();
	
	//QMessageBox::information(nullptr ,u8"提示", u8"Tr文件渲染完成");
	//emit signalRWFinsh(u8"Tr文件渲染完成");
	emit SignalInstance::getInstance()->signalPreLoad(100);
	if(funFlag==2){
		emit SignalInstance::getInstance()->signalSectFinish();
	}
	switch (statusFlag) {
		case 1:emit SignalInstance::getInstance()->signalBuildFinish();
			break;
		case 2:emit SignalInstance::getInstance()->signalLoadFinish();
			break;
		case 3:emit SignalInstance::getInstance()->signalClipShowFinish();
			break;
		case 4:emit SignalInstance::getInstance()->signalClipCancelFinish();
			break;
	}
	
	statusFlag = 0;
	//rwMutex->unlock();
}
void RWThread::setArg(QString arg1, QString arg2) {
	//rwMutex->lock();
	this->arg1 = arg1;
	this->arg2 = arg2;
	//rwMutex->unlock();
}
void RWThread::setFunFlag(int funFlag) {
	//rwMutex->lock();
	this->funFlag = funFlag;
	//rwMutex->unlock();
}
void RWThread::setStatusFlag(int statusFlag) {
	this->statusFlag = statusFlag;
}

