#pragma once
#include"workStrategy.h"
namespace platforms {
	namespace sspns {
		class nsStrategy :public workStrategy {
		public:
			nsStrategy();
			bool process(workContext* context);
		public slots:
			//void slotCloseAllWindow();
			//void slotFinishEventLoop();
			//void slotHeadFinish();
		private:
			std::atomic<bool> closeFlag;
			//bool closeFlag = false;
			//QEventLoop eventLoop;
		};


	}
}

