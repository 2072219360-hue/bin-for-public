#pragma once

#include"workStrategy.h"
#include"workContext.h"
#include"SignalInstance.h"
#include<QDebug>
#include<QMessageBox>
namespace platforms {
	namespace sspns {
		using namespace reveal;
		class sectStrategy :public workStrategy
		{
		public:
			sectStrategy();
			bool process(workContext* context);

		private:
			//fix:0525
			//bool closeFlag = false;
			std::atomic<bool> closeFlag;
		};


	}
}

