#pragma once
#include<QProcess>
#include<vector>
#include <windows.h>
#include <iostream>
#include<string>
#include <shared_mutex>
#include<QTimer>
#include<QThread>
#include<QStringList>
#include<QDir>
#include<QVector>
#include<QMutex>
#include "json/json.h"
#include"SignalInstance.h"
#include"batchCommon.h"
#include"logWidget.h"
#include"createBoundata.h"
#include"fileParamChecker.h"
#include"bodyPathChecker.h"
namespace platforms {
	namespace sspns {
		class batchProcess :public QObject
		{
			Q_OBJECT
		public:
			batchProcess(int _nsFlag = 0);
			~batchProcess();
			void setHeadRestart(int threadId);
			void initConnection();
			void writeFile(QString path, int flag);
			//QString getPath(int threadId);
			template <typename valueType>
			void setJson(int index, std::string json_key, valueType json_value);

			template <typename valueType>
			valueType getJson(int index, std::string json_key);

			QString statusContent(int threadId);
			void updateFileData();
			bool checkRunning();
			bool checkSingleRunning();
			QString getProcessName(int threadId);
			//void processHeadFinished(int exitCode, QProcess::ExitStatus exitStatus);
			//void processBodyFinished(int exitCode, QProcess::ExitStatus exitStatus);
			//void processFinished(int, QProcess::ExitStatus);
			bool checkAppend(int threadId);
			void closeProcess(int threadId);
			QString getPath(int threadId);
			void logAppend(int threadId, QString context);
			void closeAllWindow();
			int nsFlag;
		public slots:
			void onSetJson(int index, const std::string& jsonKey, double value);
			void onSetJson(int index, const std::string& jsonKey, const std::string& value);
			void onSetJson(int index, const std::string& jsonKey, bool value);
			void onSetJson(int index, const std::string& jsonKey, int value);

			void batchTrueHeadEnd(int threadId);
			void batchFinishHead(int threadId, int exitCode, QProcess::ExitStatus exitStatus);
			void batchFinishBody(int threadId, int exitCode, QProcess::ExitStatus exitStatus);
			void judgeRunning();
			void runHead(int threadId);
			void finishHead(int processId, int threadId, int exitCode, QProcess::ExitStatus exitStatus);
			void runBody(int threadId);
			void finishBody(int processId, int threadId, int exitCode, QProcess::ExitStatus exitStatus);
			void BatchRun();
			void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
			void closeBatchRun();
			int batchProcess::findNextBatch(bool& isHead);
		private:
			int checkHead(int threadId);
			int checkBody(int threadId);
			int checkAll();
			int batchIndex;
			int m_size;
			int m_currentFlag;
			bool isBatchRunning;
			bool isSingleRunnning;
			int currentId;
			void trueHeadEnd(int processId, int threadId);
			QProcess* m_process;
			QVector<QProcess*> m_processPool;
			QVector<logWidget*> m_logWidget;
			QVector<int> m_threadIndex;
			int m_poolSize;
			int m_currentSize;
			QMutex mutex;
			LPVOID pFileView;
			HANDLE hFileMapping;
			HANDLE hFile;
			Json::Value jsonData;
			Json::StreamWriterBuilder writerBuilder;
			std::size_t fileSize;
			std::mutex m_mutex;
		};
	}
}



