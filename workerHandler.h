#pragma once
#include"workContext.h"
namespace platforms {
	namespace sspns {
		class workerHandler
		{
		public:
			workerHandler();
			virtual void handle(workContext* ,workerHandler* preWorker) = 0;
			//virtual void updateNextNode() = 0;
			//virtual void updateCurrentNode() = 0;
			virtual void setNextNode(workerHandler* nextNode) = 0;
		};
	}
}


