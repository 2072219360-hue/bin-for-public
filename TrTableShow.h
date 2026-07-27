#pragma once
#include <qwidget.h>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include<QThread>
#include"sync_trData_input.h"
namespace platforms {
	namespace sspns {
		namespace reveal {
			class TrTableShow :
				public QWidget
			{
				Q_OBJECT
			public:
				explicit TrTableShow(QWidget*parent = NULL);
				~TrTableShow();
				void initUi();
				void selectCell(int row, int col);
				QStringList getSectNames();
				
				QTableWidget* getTable();
				void setBatchmode();
				
			public slots:
				void saveTrData();
				void initSectPreData();
				void exeBody();
				void loadTrData();
				void changeColor(int r, int g, int b, int sectId);
				void onLoadSelectedData(int sectId, QStringList list);
				void onPreLoadData(int sectNum, QStringList*list, unsigned char** colorData);
				void onPreLoadTableData();
				void reshowColor();
			private:
				QTableWidget*m_table;
				QVBoxLayout*m_mainLayout;
				QPushButton*exeBtn;
				QStringList m_colorList;
				int m_rowCount;
				int m_sectCount;
				QVector<QVector<QString>>m_sectPreData;
				bool editFlag;
			};
		}
	}
}

