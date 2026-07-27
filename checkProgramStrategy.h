#pragma once
#include "workStrategy.h"
namespace platforms {
	namespace sspns {
		class checkProgramStrategy :
			public workStrategy
		{
		public:
			bool process(workContext* context);
		public slots:
			//void slotCloseAllWindow();
			//void slotFinishEventLoop();
			//void slotHeadFinish();
		private:
			bool closeFlag = false;
			bool checkFlag = false;
		};
	}
}

