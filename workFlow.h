#pragma once
#include"workerHandler.h"
#include"workStrategy.h"
#include<vector>
#include<list>
#include<unordered_map>
#include<QMutex>
#include <QThreadPool>

#include <QDebug>

using namespace std;
namespace platforms {
	namespace sspns {
		class workFlow:public workerHandler
		{
		public:
			class myRunnable:public QRunnable {
			public:
				myRunnable(workContext* _context, workerHandler* _preWorker, workerHandler* _thisWorker):
					context(_context),preWorker(_preWorker),thisWorker(_thisWorker){}
				void run()override {
					thisWorker->handle(context, preWorker);
				}

			private:
				workContext* context;
				workerHandler* preWorker;
				workerHandler* thisWorker;
			};
			//workFlow();
			workFlow(workStrategy* _strategy=nullptr);
			~workFlow();
			void handle(workContext* context,workerHandler* preWorker);
			void setNextNode(workerHandler* nextNode);
			void setLastNodes(vector<workerHandler*> nodes);
			void setLastNode(workerHandler* node);
			void recoverStatus();
		private:
			workStrategy* m_strategy;
			typedef int workStatus;
			workStatus currentStatus,initStatus;
			list<workerHandler*> nextNodes;
			int offset = 0;
			unordered_map<workerHandler*, int> preNodeIndex;
			QMutex mu;
		};



	}
}


