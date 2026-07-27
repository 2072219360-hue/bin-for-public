#include "ECHVSignalInstance.h"

namespace platforms {
	namespace echv {
		ECHVSignalInstance* ECHVSignalInstance::instance = nullptr;
		ECHVSignalInstance * ECHVSignalInstance::getInstance()
		{
			if (instance == nullptr)
			{
				/*volatile auto temp = new (std::nothrow) SignalInstance();
				instance = temp;*/
				instance = new ECHVSignalInstance();
			}
			return instance;
		}

		//void SignalInstance::signalSendFun(QString path)
		//{
		//	emit signalBodyParameter(path);
		//}

		ECHVSignalInstance::ECHVSignalInstance()
		{
			//qDebug() << __FUNCTION__;
		}

		ECHVSignalInstance::~ECHVSignalInstance()
		{
			//qDebug() << __FUNCTION__;
		}
	}
}

