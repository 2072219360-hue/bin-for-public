#pragma once
#include <qwidget.h>
#include <QTableWidget>
#include<QVBoxLayout>
#include<QHeaderView>
#include<QPushButton>
#include<QVector>
#include<qDebug>
#include <functional>
#include <fstream>
#include <iostream>
#include<QMessageBox>
#include<QLabel>
#include<QDate>
#include "json/json.h"
#include"btnWiget.h"
#include"SignalInstance.h"
#include"batchCommon.h"
#include "StyleWidgetFactory.h"
#include"batchCommon.h"
namespace platforms {
	namespace sspns {
		class progressTable:
			public QWidget
		{
			Q_OBJECT
		public:
			explicit progressTable(int _nsFlag = 0,QWidget*parent = NULL);
			void initUi();
			void initConnection();
			void loadButton();
			void connectAllButton();
			void deleteAllButtons(QVector<btnWigdet*>& opBtn);
			int num_case = 0;
		public slots:
			void setRowBackgroundColor(int row, const QColor& color);
			void op1(int threadId);
			void op2(int threadId);
			void op3(int threadId);
			void op4(int threadId);
			void updateStatus(int threadId, QString str);
			void updateTable();
			void updateTime(int threadId, QString str);
			// 处理双击事件
			void onCellDoubleClicked(int row, int column);
		private:
			QTableWidget*m_table;
			QLabel* m_title;
			QVBoxLayout*m_mainLayout;
			int m_row;
			int m_column;
			QVector<btnWigdet*> opBtn;
			int nsFlag;
			//QVector<QVector<QPushButton*>> opBtn;
		};
	}
}


