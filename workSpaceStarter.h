#pragma once
#include <qwidget.h>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDir>
#include <QMessageBox>
#include <regex>
#include"inputFileEditor.h"
#include "SignalInstance.h"
#include"RWThread.h"
#include"scenewidget.h"
#include"fileOp.h"
namespace platforms {

	namespace sspns {

		namespace detail {
			class workSpaceStarter
			{
			public:
				int m_nsFlag;
				workSpaceStarter(int _nsFlag=0);
				void run();
				void clearFiles();
				void setString(QString _dirName, QString _gridName, QString _inpName);
				QString pathToFileName(QString path);
				bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
				void changeConfig(QString configFile,QString inpDir, QString grdDir,int flag);
				bool copyFile(const QString &toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit);
			private:
				
				QString dirPath, gridPath, inpPath;
				inputFileEditor fileEditor;
			};
		}
	}
}


