#pragma once
#include<QProgressBar>
#include "OutPutLogger.h"
#include"WorkSpaceOp.h"
#include"SceneWidget.h"
#include"ProgressDialog.h"
#include"ParamSetDialog.h"
#include"TrTableShow.h"
#include"mySubject.h"
namespace platforms {
	namespace sspns {
		using namespace reveal;
		using namespace detail;
		
		class workContext
		{
		public:
			workContext(bool fullFlag=true);
			void setOutPutLogger(OutPutLogger* _outPutLogger) { m_outPutLogger = _outPutLogger; }
			void setQProgressBar(QProgressBar*_dataProgressBar) { m_dataProgressBar = _dataProgressBar; }
			void setDirName(QString _dirName) { dirName = _dirName; }
			void setGirdPath(QString _gridPath) { gridPath = _gridPath; }
			void setInpPath(QString _inpPath) { inpPath = _inpPath; }
			void setWorkSpaceOp(WorkSpaceOp* _workSpaceOp) {  m_workSpaceOp = _workSpaceOp; }
			void setSceneWidget(SceneWidget*_sceneWidget) { main_sceneWidget = _sceneWidget; }
			void setProgressDialog(ProgressDialog* _dialog) { m_dialog = _dialog; }
			void setTrTableShow(TrTableShow* _trTable) { m_trTable = _trTable; }
			void setParamSetDialog(ParamSetDialog* _paramDialog) { m_paramDialog = _paramDialog; }
			QProgressBar* m_dataProgressBar;
			OutPutLogger* m_outPutLogger;
			QString dirName,gridPath,inpPath;
			WorkSpaceOp* m_workSpaceOp;
			SceneWidget*main_sceneWidget;
			ProgressDialog*m_dialog;
			TrTableShow* m_trTable;
			ParamSetDialog* m_paramDialog;
			statusSubject* myStatus;
			bool fullFlag;
		private:

		};
	}
}


