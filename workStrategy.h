#pragma once
#include"workContext.h"
namespace platforms {
	namespace sspns {
		class workStrategy
		{
		public:
			workStrategy();
			virtual bool process(workContext* context)=0;
		};


	}
}


