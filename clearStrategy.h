#pragma once

#include"workStrategy.h"
#include"workContext.h"
#include"SignalInstance.h"
#include<QDebug>
#include<QMessageBox>
namespace platforms {
	namespace sspns {
		using namespace reveal;
		class clearStrategy :public workStrategy
		{
			clearStrategy();
			bool process(workContext* context);
		};

	}
}
