#include "readFileStrategy.h"

namespace platforms {
	namespace sspns {
		readFileStrategy::readFileStrategy(int _index ) :strategyIndex(_index) {

		}
		bool readFileStrategy::process(workContext* context) {
			qDebug() << "readFile" << endl;
			if (strategyIndex == READ_BODY) {
				readBody(context);
			}
			return true;
			//qDebug() << "workSpace=" << SignalInstance::getInstance()->m_workspace;
		}
		//先运行sect程序->更新表格->渲染网格
		void readFileStrategy::readBody(workContext* context) {
			QString dirName, gridPath, inpPath;
			dirName = context->dirName;
			gridPath = context->gridPath;
			inpPath = context->inpPath;
			SceneWidget*main_sceneWidget = context->main_sceneWidget;
			SignalInstance::getInstance()->color_mutex.lock();
			if (main_sceneWidget->thread() != QThread::currentThread()) {
				qDebug() << u8"不在同一个线程中";
				QMetaObject::invokeMethod(main_sceneWidget, "ColorStop", Qt::QueuedConnection);
			}
			else {
				qDebug() << u8"直接调用";
				main_sceneWidget->ColorStop(); // 直接调用
			}
			SignalInstance::getInstance()->color_mutex.lock();
			SignalInstance::getInstance()->color_mutex.unlock();
			
			QString title = u8"提示窗口", desc = u8"正在渲染，请耐心等待......";
			QMetaObject::invokeMethod(context->m_dialog,
				"setTitle", Qt::BlockingQueuedConnection, Q_ARG(const QString&, title), Q_ARG(const QString&, desc));
			QMetaObject::invokeMethod(context->m_dialog,
				"show", Qt::BlockingQueuedConnection);
			qDebug() << "dirName=" << dirName << " gridPath=" << gridPath << " inpPath=" << inpPath;
			QMetaObject::invokeMethod(main_sceneWidget,
				"readAndOut", Qt::BlockingQueuedConnection, Q_ARG(QString, gridPath), Q_ARG(QString, dirName));

			



			QMetaObject::invokeMethod(context->m_dialog,
				"close", Qt::BlockingQueuedConnection);


			//渲染tr
			QMetaObject::invokeMethod(context->m_trTable,
				"initSectPreData", Qt::BlockingQueuedConnection);
			QMetaObject::invokeMethod(context->m_trTable,
				"loadTrData", Qt::BlockingQueuedConnection);
			QMetaObject::invokeMethod(context->main_sceneWidget,
				"onlyReadSect", Qt::BlockingQueuedConnection, Q_ARG(QString, dirName));
			QMetaObject::invokeMethod(context->m_trTable,
				"onPreLoadTableData", Qt::BlockingQueuedConnection);
			//create trData.dat
			QMetaObject::invokeMethod(context->m_trTable,
				"saveTrData", Qt::BlockingQueuedConnection);
			

			QMetaObject::invokeMethod(context->m_paramDialog,
				"updateParam", Qt::BlockingQueuedConnection);


			QMetaObject::invokeMethod(context->m_trTable,
				"reshowColor", Qt::BlockingQueuedConnection);
			
			/*QString mess = u8"网格解析完成";
			QMetaObject::invokeMethod(context->main_sceneWidget,
				"showMessage", Qt::BlockingQueuedConnection, Q_ARG(QString, mess));*/

		}

	}
}
