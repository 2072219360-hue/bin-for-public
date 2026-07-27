#include "dataServer.h"

namespace platforms {
	namespace echv {
		dataServer::dataServer() {
			myData = new runData();
			myArg = new argData();

			connectAll();
		}

		dataServer::~dataServer() {
			delete myData;
			delete myArg;
		}
		void dataServer::updateArg(argData *arg) {
			myArg->copy(arg);
			//emit SignalInstance::getInstance()->signalPrint("更新参数成功");
			//emit ECHVINSTANCE->signalReloadVTK(myArg);
		}
		void dataServer::connectAll() {
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalUpdateArg, [=](argData *arg) {
				auto* thr = QThread::create([=](argData *argments) {
					updateArg(arg);
					transGrid *al = new transGrid(getMyData(), getMyArg());
					al->AlgorithmInterface();
					
				}, arg);
				connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始转换网格"); });
				connect(thr, &QThread::finished, this, [=]() { 
					//emit ECHVINSTANCE->signalPrint(u8"转化完成"); 
					emit ECHVINSTANCE->signalLoadVTK(getMyArg(), getMyData());
				});
				thr->start();
				
				

				//emit ECHVINSTANCE->signalReloadVTK(myArg);
			});
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalCreateFullDataset, [=]() {
				auto* thr = QThread::create([=]() {
					CreateFullDataset *al = new CreateFullDataset(getMyData(), getMyArg());
					al->AlgorithmInterface();

				});
				connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始生成指标"); });
				connect(thr, &QThread::finished, this, [=]() {
					emit ECHVINSTANCE->signalPrint(u8"生成指标完成"); 
					//emit ECHVINSTANCE->signalLoadVTK(getMyArg(), getMyData());
				});
				thr->start();
			});
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalCreateTrees, [=]() {
				auto* thr = QThread::create([=]() {
					CreateTrees *al = new CreateTrees(getMyData(), getMyArg());
					al->AlgorithmInterface();

				});
				connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始轮廓树"); });
				connect(thr, &QThread::finished, this, [=]() {
					emit ECHVINSTANCE->signalPrint(u8"轮廓树构建完成");
					//emit ECHVINSTANCE->signalLoadVTK(getMyArg(), getMyData());
				});
				thr->start();
			});
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalRelevance, [=]() {
				auto* thr = QThread::create([=]() {
					relevance *al = new relevance(getMyData(), getMyArg());
					al->AlgorithmInterface();

				});
				connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始涡识别"); });
				connect(thr, &QThread::finished, this, [=]() {
					emit ECHVINSTANCE->signalPrint(u8"涡识别完毕");
					//emit ECHVINSTANCE->signalLoadVTK(getMyArg(), getMyData());
				});
				thr->start();
			});
			
		}
		argData* dataServer::getMyArg() {
			return this->myArg;
		}
		runData* dataServer::getMyData() {
			return this->myData;
		}
	}
}

