#include "sectStrategy.h"
static mutex muSectClose;
namespace platforms {
	namespace sspns {
		sectStrategy::sectStrategy() {

		}

		//fix:0525
		bool sectStrategy::process(workContext* context) {
			QString dirName, gridPath, inpPath;

			if(context->fullFlag)
			QMetaObject::invokeMethod(context->m_workSpaceOp->m_dialog,
				"preWork", Qt::BlockingQueuedConnection, Q_ARG(QString&, dirName), Q_ARG(QString&, gridPath), Q_ARG(QString&, inpPath));
			else{
				QMetaObject::invokeMethod(context->m_workSpaceOp->m_dialog,
					"preWork", Qt::BlockingQueuedConnection, Q_ARG(QString&, dirName), Q_ARG(QString&, gridPath), Q_ARG(QString&, inpPath));
				// pns界面
				/*QMetaObject::invokeMethod(context->m_workSpaceOp->m_dialog,
					"preWork", Qt::BlockingQueuedConnection, Q_ARG(QString&, dirName), Q_ARG(QString&, gridPath), Q_ARG(QString&, inpPath), Q_ARG(int, 0));*/
			}
			context->setDirName(dirName);
			context->setGirdPath(gridPath);
			context->setInpPath(inpPath);
			qDebug() << "dirName=" <<dirName << " gridPath=" << gridPath << " inpPath=" << inpPath;

			qDebug() << "runSect" << endl; 
			QMetaObject::invokeMethod(context->m_outPutLogger,
				"exeProgramSect", Qt::BlockingQueuedConnection, Q_ARG(QString, context->dirName));
			//qDebug() << "workSpace=" << SignalInstance::getInstance()->m_workspace;
			qDebug() << "begin sect and wait";
			QEventLoop eventLoop;
			closeFlag = false;
			QMetaObject::Connection m_closeConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, [&]() {
				muSectClose.lock();
				closeFlag = true;
				muSectClose.unlock();
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}

			});
			QMetaObject::Connection m_finishConnect=QObject::connect(SignalInstance::getInstance(), &SignalInstance::finishEventLoop, [&]() {
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}

			});

			QMetaObject::Connection m_sectFinishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::finishSect, [&]() {
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
					qDebug() << "isRunning=" << eventLoop.isRunning();
				}
				qDebug() << "isRunning=" << eventLoop.isRunning();
		
			});

			eventLoop.exec();
			QObject::disconnect(m_closeConnect);
			QObject::disconnect(m_finishConnect);
			QObject::disconnect(m_sectFinishConnect);
			qDebug() << "closeFlag=" << closeFlag;
			if (closeFlag) return false;
			qDebug() << "finish sect";
			return true;
		}

	}
}

