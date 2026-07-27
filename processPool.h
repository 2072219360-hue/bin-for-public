//#pragma once
//#include<QProcess>
//#include<QVector>
//#include<QMutex>
//#include"SignalInstance.h"
//
//namespace platforms {
//	namespace sspns {
//		struct processInfo {
//			QProcess* m_process;
//			int index; bool flag;
//		};
//		class processPool
//		{
//			Q_OBJECT
//		public slots:
//			bool append(QString _path, bool flag, int index);
//
//		public:
//			processPool(int _corePoolSize=1);
//			~processPool();
//			void initConnection();
//			
//		private:
//			int m_corePoolSize;
//			int m_maximumPoolSize;
//			QProcess* m_batchProcess;
//			//QMutex m_mutex;
//			QVector<processInfo> m_processPool;
//			
//		};
//	}
//}
//
//
//
