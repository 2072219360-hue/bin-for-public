#pragma once
#include"workContext.h"
#include"workStrategy.h"
#include"SignalInstance.h"
#include <QMetaObject>
#include <QThread>
namespace platforms {
	namespace sspns {
		class updateStrategy:public workStrategy
		{
		public:
			updateStrategy(int _value = 0,int _saveFlag=0);
			bool process(workContext* context);
		private:
			int value;
			int saveFlag;
		};


	}
}

