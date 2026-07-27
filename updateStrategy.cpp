#include "updateStrategy.h"
namespace platforms {
	namespace sspns {

		updateStrategy::updateStrategy(int _value,int _saveFlag):value(_value),saveFlag(_saveFlag) {

		}
		bool updateStrategy::process(workContext* context) {
			QMetaObject::invokeMethod(context->m_outPutLogger,
				"onChangeProgressBar", Qt::BlockingQueuedConnection, Q_ARG(int, value) );
			return true;
		}

	}
}
