#pragma once
#include <qwidget.h>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include<QPushButton>
#include<QVector>
#include<qDebug>
#include<QString>
#include <initializer_list>
namespace platforms {
	namespace sspns {
		class btnWigdet :
			public QWidget
		{
			Q_OBJECT
		public:
			btnWigdet(int _columnIndex = 0, int _btnNum = 0, std::initializer_list<QString> args = {});
			~btnWigdet();
			void dynamicConnect(int index, QObject* reciver, std::function<void(int)> f);
			void setBodyMode();
		private:
			//第几行
			int m_columnIndex;
			//按钮数量
			int m_btnNum;
			QVector<QPushButton*> m_btn;
			QGridLayout* m_layout;
			
		};

	}
}

