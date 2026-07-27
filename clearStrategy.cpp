#include "clearStrategy.h"


namespace platforms {
	namespace sspns {
		clearStrategy::clearStrategy() {

		}
		bool clearStrategy::process(workContext* context) {
			QMetaObject::invokeMethod(context->main_sceneWidget,
				"clearAll", Qt::BlockingQueuedConnection);
			qDebug() << "clearFinish";
			return true;
		}


	}
}