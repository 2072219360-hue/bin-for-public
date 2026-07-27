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
#include "SignalInstance.h"
#include"RWThread.h"
#include"scenewidget.h"
#include"projectWidget.h"
#include"workSpaceStarter.h"

namespace platforms {

	namespace sspns {
		
		namespace detail {
			
			class WorkSpaceOpDialog :
				public QWidget
			{
				Q_OBJECT
			public:
				void init();
				void initUi();
				void initWidget();
				void initLayout();
				void initConnection();
				void loadStyleSheet(QString sheetName);
			
				//bool WorkSpaceOpDialog::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
				bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
				bool copyFile(const QString &toDir, const QString&fromFile, const QString&toFile, bool converFileIfExit);
				WorkSpaceOpDialog(int versionFlag = 0, int nsFlag =0,QWidget*parent = nullptr);
				~WorkSpaceOpDialog();
				void setBathMode();
				bool checkDir(QString path);
				bool checkFile(QString path);
				projectWidget* m_projectWidget;
				int m_nsFlag;
				void clearText();
			public slots:
				void preWork(QString& a, QString& b, QString& c);
				void slotheadDirOpen();
				void slotbodyDirOpen();
				void slotsectDirOpen();
				void slotLoadDirData();
				
				void slotReClip();

				void slotSelectDir();
				void slotSelectGrid();
				void slotSelectInp();
				void slotHeadSelectGrid();
				void slotHeadSelectInp();
				void slotFullSelectGrid();
				void slotFullSelectInp();
				void slotOK();

				void slotSelectedGridType();
				void slotCancel();
			private:
				int m_versionFlag;
				
				QLabel*m_workNameLabel;
				QLineEdit*m_workNameLineEdit;
				QLabel*m_describeLabel;
				QTextEdit*m_describeTextEdit;
				QLineEdit*m_headPara;
				QLineEdit*m_bodyPara;
				QLineEdit*m_sectPara;
				
				QPushButton*m_btnHead;
				QPushButton*m_btnBody;
				QPushButton*m_btnSect;
				QLabel*m_labelHead;
				QLabel*m_labelBody;
				QLabel*m_labelSect;
				QVBoxLayout* m_mainLayout;

				//Ñ¡ÔñÄ¿Â¼
				QLabel*m_dirLabel;
				QLineEdit*m_dirSelect;
				QPushButton*m_btnDir;

				QLabel*m_gridHeadLabel;
				QLineEdit*m_gridHeadSelect;
				QPushButton*m_btnHeadGrid;

				QLabel*m_inpHeadLabel;
				QLineEdit*m_inpHeadSelect;
				QPushButton*m_btnHeadInp;

				workSpaceStarter* m_starter;


				QLabel*m_gridLabel;
				QLineEdit*m_gridSelect;
				QPushButton*m_btnGrid;

				QLabel*m_inpLabel;
				QLineEdit*m_inpSelect;
				QPushButton*m_btnInp;

				QLabel*m_fullGridLabel;
				QLineEdit* m_fullGridSelect;
				QPushButton* m_btnFullGrid;

				QLabel*m_fullInpLabel;
				QLineEdit* m_fullInpSelect;
				QPushButton* m_btnFullInp;
				

				QPushButton*m_btnOk;
				QPushButton*m_btnCancel;

				QLabel* m_labelInputSelect;
				QButtonGroup* m_selectButtonGroup;
				QRadioButton* m_btnFullSelect;
				QRadioButton* m_btnHeadBodySelect;
				QWidget* m_headBodySelectWidget;
				QWidget* m_fullSelectWidget;


				
				//SceneWidget*m_sceneWidget;
			};
		}
	}
}

