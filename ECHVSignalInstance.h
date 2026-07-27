#pragma once
#include <qwidget.h>
#include<string>
#include"runData.h"
#include"argData.h"

namespace platforms {
	namespace echv {
		//该类用来控制信号量
		class ECHVSignalInstance :
			public QWidget
		{
			Q_OBJECT
		public:
			static ECHVSignalInstance*instance;
			static ECHVSignalInstance*getInstance();

			/*static void setWorkSpace(QString path)
			{
				instance->m_workspace = path;
			}
			static QString getWorkSpace()
			{
				return instance->m_workspace;
			}
			QString m_workspace;*/
		signals:
			void signalReloadVTK(argData *arg);
			void signalLoadVTK(argData *arg, runData *data);
			void signalUpdateArg(argData *arg);
			void signalPrint(QString str);
			void signalTransGrid();
			void signalCreateFullDataset();
			void signalCreateTrees();
			void signalCountActor();
			void signalRelevance();
		private:
			ECHVSignalInstance();
			~ECHVSignalInstance();
		};
		
	}
}



