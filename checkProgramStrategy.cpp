#include "checkProgramStrategy.h"
#include <mutex>
static mutex muNsClose;
namespace platforms {
	namespace sspns {
		bool checkProgramStrategy::process(workContext* context) {
			
			checkFlag = false;
			closeFlag = false;
			checkFlag =context->m_outPutLogger->checkRunning();
			static QEventLoop eventLoop;

			QMetaObject::Connection m_closeConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, [&]() {
				qDebug() << "QObject::connect(SignalInstance::getInstance() + 1";
				closeFlag = true;
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
			});

			QMetaObject::Connection m_finishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::finishEventLoop, [&]() {
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
			});

			QMetaObject::Connection m_headFinishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::headFinish, [&]() {
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
			});

			eventLoop.exec();


			if (checkFlag == true) {
				closeFlag = true;
				QString mess = u8"当前有其他程序正在运行，请先停止当前运行程序！";
				QMetaObject::invokeMethod(context->main_sceneWidget,
					"showMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, mess));
			}

			if (closeFlag) {
				return false;
			}
			qDebug() << "finish head";
			return true;
		}
	}
}
