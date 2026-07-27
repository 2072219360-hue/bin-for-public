#pragma once
#include <qwidget.h>
#include"runData.h"
#include"argData.h"
#include "ECHVSignalInstance.h"
#include"transGrid.h"
#include "CreateFullDataset.h"
#include"CreateTrees.h"
#include"relevance.h"
#include<QThread>
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()
namespace platforms {
	namespace echv {
		class dataServer :
			public QWidget
		{
			Q_OBJECT

		public:
			dataServer();
			~dataServer();
			void updateArg(argData *arg);
			void connectAll();
			argData* getMyArg();
			runData* getMyData();
		public slots:

		private:
			runData *myData;
			argData *myArg;
		};
	}
}





