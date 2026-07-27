#pragma once
#include <qwidget.h>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QDirModel>
#include <QTreeView>
#include <QListView>
#include <QTableView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCoreApplication>
#include<QMessageBox>
#include<QProcess>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <iostream>
#include <string>
#include <Windows.h>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
//#include "huangmainwindow.h"
#include "SignalInstance.h"
#include "WorkSpaceOpDialog.h"
//#include "scenewidget.h"
#include"statusSubject.h"
#include"projectWidget.h"
namespace platforms {
	namespace sspns {
		namespace reveal {
			class CustomSortModel : public QSortFilterProxyModel {
			public:
				CustomSortModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

			protected:
				bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
					QFileSystemModel *model = qobject_cast<QFileSystemModel *>(sourceModel());
					if (!model) return QSortFilterProxyModel::lessThan(left, right);

					bool leftIsDir = model->isDir(left);
					bool rightIsDir = model->isDir(right);

					// 优先按文件夹排序
					if (leftIsDir && !rightIsDir) {
						return true;
					}
					else if (!leftIsDir && rightIsDir) {
						return false;
					}
					else {
						// 如果两者都是文件夹或都是文件，则按名称排序
						return QSortFilterProxyModel::lessThan(left, right);
					}
				}
			};
			class WorkSpaceOp :
				public QWidget
			{
				Q_OBJECT
			public:


				detail::WorkSpaceOpDialog* m_dialog;
				projectWidget* m_projectWidget;
				WorkSpaceOp(int ns_flag=0);
				~WorkSpaceOp();
				void initWorkSpcaeUi();
				void initConnect();
				void loadStyleSheet(QString sheetName);
				//判断头部身部Sect程序是否正在执行
				bool isExeStarted();
				bool killPrograms();
				/// @brief 是否继续完成切换工作
				bool toggleHandle();
				void setBatchMode();
				void setCurrentWorkSpaceLineEdit();
			public slots:
				bool judgeWorksapce();
				void slotCheckBatchStatus();
				void slotShowDialog();
				void slotUpdateView();
				void slotLoadWorkSpace();
				void onTreeViewDobuleClicked();
				void onChangedSceneWorkState(bool isWorking);
				void onUpdateWorkSpaceLineEdit(QString str);
				void slotDeleteWorkSpace();
				void slotShowProject();
				//
				void slotShowDirDialog();
			private:
				int m_nsFlag;
				QPushButton*btnAdd;
				QPushButton*btnBuild;
				QPushButton*btnLoad;
				QPushButton*btnDelete;
				QPushButton*btnProject;
				//打开网格
				QPushButton*btnLoadGrid;
				//打开目录
				QPushButton*btnLoadDir;
				QHBoxLayout*btnLayout;
				QTreeView*tree;
				//QDirModel*model;
				QFileSystemModel *model;
				CustomSortModel *proxyModel;

				QVBoxLayout*m_mainLayout;
				QModelIndex m_currentModelIndex;
				bool m_sceneIsWording;
				QLabel* m_currentWorkSpace;
				QLineEdit*m_currentWorkSpaceLineEdit;
				QHBoxLayout*m_currentWorkSpaceLayout;
				
			};
		}/// reveal
	}/// sspns
}/// platforms

