#include "btnObserver.h"

void btnObserver::Update() {
	if (chooseAction())  setEnable();
	else setDisable();
	
	
}
bool btnObserver::chooseAction() {
	Status current_Status= m_sub->getStatus();
	return current_Status & m_btnType;
}
void btnObserver::safeCall() {
	//qDebug() << "type=" << m_btnType;
	// 一级检查：基本有效性
	if (!QCoreApplication::instance()) return;

	QMutexLocker locker(&mu);
	QPointer<QWidget> btn = m_btn;

	// 二级检查：对象存在性
	if (!btn) return;

	try {
		// 三级检查：线程状态
		if (auto* t = btn->thread()) {
			if (t->isRunning()) {
				// 最终安全调用
				QMetaObject::invokeMethod(btn, [btn]() {
					if (btn) btn->setEnabled(true);
					if (btn) btn->update();
				}, Qt::QueuedConnection);
			}
		}
	}
	catch (...) {
		qCritical() << "Exception in thread access";
	}
}
void btnObserver::setEnable() {
	safeCall();
	//if (m_btn!=nullptr) {
	//	//TODO:0424
	//	mu.lock();
	//	if (m_btn != nullptr) {
	//		m_btn->setEnabled(true);
	//	}
	//	//QPalette palette = m_btn->palette();
	//	//palette.setColor(QPalette::Background, QColor(173, 216, 230));
	//	////palette.setColor(QPalette::Button, QColor(173, 216, 230));
	//	//m_btn->setPalette(palette);
	//	// 检查线程亲和性
	//	if (QThread::currentThread() != m_btn->thread()) {
	//		// 确保按钮对象仍然存在且属于有效线程
	//		if (QCoreApplication::instance() &&
	//			m_btn->thread()->isRunning()) {
	//			QMetaObject::invokeMethod(m_btn, "update", Qt::QueuedConnection);
	//		}
	//	}
	//	else {
	//		m_btn->update(); // 如果在同一线程直接调用
	//	}
	//	//if (m_btn != nullptr) { 
	//	//	// 在工作线程中
	//	//	QMetaObject::invokeMethod(m_btn, "update", Qt::QueuedConnection);
	//	//	//m_btn->update(); 
	//	//}

	//	mu.unlock();
	//}
}
void btnObserver::setDisable() {
	if (m_btn != nullptr) {
		
		m_btn->setEnabled(false);
		//QPalette palette = m_btn->palette();
		//palette.setColor(QPalette::Background, QColor(144, 238, 144));
		////palette.setColor(QPalette::Button, QColor(144, 238, 144));
		//m_btn->setPalette(palette);
		m_btn->update();
	}
}
