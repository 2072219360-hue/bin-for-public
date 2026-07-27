#include "nsStrategy.h"
#include <mutex>
static mutex muNsClose;
namespace platforms {
	namespace sspns {
		nsStrategy::nsStrategy() {
			//QMetaObject::Connection m_closeConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, [&]() {
			//	qDebug() << "QObject::connect(SignalInstance::getInstance() + 1";
			//	closeFlag = true;
			//	if (eventLoop.isRunning()) {
			//		qDebug() << "Exiting event loop.";
			//		eventLoop.quit(); // 退出事件循环
			//	}
			//});

			//QMetaObject::Connection m_finishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::finishEventLoop, [&]() {
			//	if (eventLoop.isRunning()) {
			//		qDebug() << "Exiting event loop.";
			//		eventLoop.quit(); // 退出事件循环
			//	}
			//});

			//QMetaObject::Connection m_headFinishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::headFinish, [&]() {
			//	if (eventLoop.isRunning()) {
			//		qDebug() << "Exiting event loop.";
			//		eventLoop.quit(); // 退出事件循环
			//	}
			//});
		}
		bool nsStrategy::process(workContext* context) {
			qDebug() << "begin pns";
			QString mess = u8"开始ns";
			//mutex muNsClose;
			QMetaObject::invokeMethod(context->main_sceneWidget,
				"showMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, mess));

			QMetaObject::invokeMethod(context->m_paramDialog,
				"redictHeadParameter", Qt::BlockingQueuedConnection);
			QEventLoop eventLoop;
			//fix:0518
			//static QEventLoop eventLoop;
			closeFlag = false;

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

			QMetaObject::Connection m_headFinishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::headFinish, [&](bool success) {
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
					qDebug() << "isRunning=" << eventLoop.isRunning();
					
				}
				qDebug() << "isRunning="<<eventLoop.isRunning();
				if (success) {
					closeFlag = false;
				}
				else closeFlag = true;
			});

			eventLoop.exec();
			QObject::disconnect(m_closeConnect);
			QObject::disconnect(m_finishConnect);
			QObject::disconnect(m_headFinishConnect);
			qDebug() << "closeFlag=" << closeFlag;
			if (closeFlag) return false;
			qDebug() << "finish head";
			return true;
		}

		//void nsStrategy::slotCloseAllWindow() {
		//	qDebug() << "QObject::connect(SignalInstance::getInstance() + 1";
		//	closeFlag = true;
		//	if (eventLoop.isRunning()) {
		//		qDebug() << "Exiting event loop.";
		//		eventLoop.quit(); // 退出事件循环
		//	}
		//}

		//void nsStrategy::slotFinishEventLoop(){

		//}

		//void nsStrategy::slotHeadFinish(){
		//}

	}
}
