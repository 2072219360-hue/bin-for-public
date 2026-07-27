#pragma once
#include<QWidget>
#include<QString>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringListModel>
#include <QListView>
#include<SignalInstance.h>
#include<QMap>
#include<QVector>
#include"workSpaceStarter.h"
namespace platforms {
	namespace sspns {
		class projectWidget :
			public QWidget
		{
			Q_OBJECT
		public:
			projectWidget(int _flag=0, QWidget *parent = nullptr);
			void initWidget();
			void initConnection();
			int ns_flag;
		public slots:
			void checkAndRemoveInvalidEntries();
			void deleteProject(const QString &key);
			void openFile();
			QString handleItemClick(const QModelIndex &index);
			void insertInfo(QString fileName, QString gridName, QString inpName);
			void projectWidget::clearProjects();
		private:
			
			QTextEdit *textEdit; // 文本编辑框
			QVBoxLayout *layout;
			QPushButton *openButton;
			QPushButton *clearButton;
			QListView *listView; // 列表视图
			QStringListModel *model; // 字符串列表模型
			QMap<QString, QVector<QString>> mp;
			//detail::WorkSpaceOpDialog* dialog;
			detail::workSpaceStarter* m_starter;
		};
	}
}