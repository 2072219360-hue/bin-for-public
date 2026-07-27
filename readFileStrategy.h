#pragma once
#include"workStrategy.h"
#include"workContext.h"
#include"SignalInstance.h"
#include"sectStrategy.h"
#include<QDebug>
#include<QMessageBox>
namespace platforms {
	namespace sspns {
		using namespace reveal;
		class readFileStrategy:public workStrategy
		{
		public:
			readFileStrategy(int _index = 0);
			bool process(workContext* context) ;
			void readBody(workContext* context);
			void copyFiles(workContext* context);
		private:
			enum STRATE_TYPE :int {
				READ_BODY = 1,
			};
			int strategyIndex;
		};


	}
}



