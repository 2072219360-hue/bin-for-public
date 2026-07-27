#include "workFlow.h"

namespace platforms {
	namespace sspns {
		workFlow::workFlow(workStrategy* _strategy) {
			m_strategy = _strategy;
			currentStatus = initStatus = 0;
			offset = 0;
		}

		workFlow::~workFlow() {
			if (m_strategy != nullptr) {
				delete m_strategy;
				m_strategy = nullptr;
			}
			
		}
		void workFlow::recoverStatus() {
			currentStatus = initStatus;
		}
		void workFlow::handle(workContext* context, workerHandler* preWorker) {
			bool runFlag = false;
			mu.lock();
			int mask = preNodeIndex[preWorker];
			qDebug() << "currentStatu=" << currentStatus << " mask=" << mask << endl;
			if (currentStatus&mask) currentStatus ^= mask;
			
			if (!currentStatus) runFlag = true;
			mu.unlock();
			if (!runFlag) return;

			bool processFlag = true;
			if (m_strategy != nullptr) {
				processFlag =m_strategy->process(context);
			}
			if (!processFlag) return;


			QThreadPool *threadPool = QThreadPool::globalInstance();
			for (auto nextNode : nextNodes) {
				myRunnable* myRun = new myRunnable(context, this, nextNode);
				threadPool->start(myRun);
			}

		}
		void workFlow::setNextNode(workerHandler* nextNode) {
			nextNodes.push_back(nextNode);
			
		}
		void workFlow::setLastNode(workerHandler* node) {
			int index = 1<<offset;
			offset++;
			initStatus += index;
			currentStatus += index;
			preNodeIndex[node]=index;
		}
		void workFlow ::setLastNodes(vector<workerHandler*> nodes) {
			int index = 1<<offset;
			initStatus += index;
			currentStatus += index;
			offset++;
			for (auto node : nodes) {
				preNodeIndex[node] = index;
			}
		}
	}
}

