#pragma once
#include"workStrategy.h"
namespace platforms {
	namespace sspns {
		class pnsStrategy:public workStrategy {
		public:
			bool process(workContext* context);
		private:
			//
			//bool closeFlag = false;
			std::atomic<bool> closeFlag;
		};


	}
}

