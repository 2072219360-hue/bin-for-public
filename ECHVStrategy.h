#pragma once
#include<iostream>
#include"runData.h"
#include"argData.h"
using namespace std;

namespace platforms {
	namespace echv {
		class ECHVStrategy
		{
		public:
			virtual void AlgorithmInterface() = 0;
			ECHVStrategy(runData* run_data, argData* arg_data) {
				this->data = run_data;
				this->arg = arg_data;
			}
		protected:
			runData* data;
			argData* arg;
		};
	}
}



