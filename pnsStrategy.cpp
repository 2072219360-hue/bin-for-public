#include "pnsStrategy.h"
static mutex muPnsClose;
namespace platforms {
	namespace sspns {

		bool pnsStrategy::process(workContext* context) {
			qDebug() << "begin pns";
			QString mess = u8"开始pns";
			QMetaObject::invokeMethod(context->main_sceneWidget,
				"showMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, mess));

			QMetaObject::invokeMethod(context->m_paramDialog,
				"redictBodyParameter", Qt::BlockingQueuedConnection);
			//fix:0518
			//static QEventLoop eventLoop;
			QEventLoop eventLoop;
			closeFlag = false;
			QMetaObject::Connection m_closeConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::signalCloseAllWindow, [&]() {
				muPnsClose.lock();
				closeFlag = true;
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
				muPnsClose.unlock();

			});

			QMetaObject::Connection m_finishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::finishEventLoop, [&]() {
				muPnsClose.lock();
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
				muPnsClose.unlock();

			});
			QMetaObject::Connection m_bodyFinishConnect = QObject::connect(SignalInstance::getInstance(), &SignalInstance::bodyFinish, [&]() {
				muPnsClose.lock();
				if (eventLoop.isRunning()) {
					qDebug() << "Exiting event loop.";
					eventLoop.quit(); // 退出事件循环
				}
				muPnsClose.unlock();
			});
			eventLoop.exec();
			QObject::disconnect(m_closeConnect);
			QObject::disconnect(m_finishConnect);
			QObject::disconnect(m_bodyFinishConnect);
			if (closeFlag) return false;

			context->myStatus->setStatus(Status::HEAD_SECT_FINISH);
			context->myStatus->Notify();
			qDebug() << "finish body";
			return true;
		}
	}
}
