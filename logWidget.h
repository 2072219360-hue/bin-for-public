#pragma once
#include<QWidget>
#include <QVBoxLayout>
#include<QHBoxLayout>
#include <QTextEdit>
#include<QLabel>
#include<QPushButton>
#include<QString>
#include<QTextEdit>
#include<QFile>
#include<QTextCursor>
#include<QTextBlock>
#include"SignalInstance.h"
namespace platforms {
	namespace sspns {
		class logWidget:public QWidget
		{
			Q_OBJECT
		public:
			logWidget(QString logpath,QString _title=u8"输出日志窗口");
			void initConnection();
			void initLayout();
			void loadStyleSheet(QString sheetName);
			QString getLastMsg();
			
		public slots:
			void showWidget();
			void clearText();
			void setText();
			void clearFileText();
			void appendText(QString str);
			//void closeWidget();
			//void clearText();
		protected:
			void closeEvent(QCloseEvent *event) override;
			void showEvent(QShowEvent *event) override;
		private:
			QLabel*m_label;
			QPushButton*m_clearBtn;
			QTextEdit*m_outPutShow;
			QString m_logpath;
			bool m_initFlag;
		};
	}
}



