#include "logWidget.h"

extern QString appComDir;
namespace platforms {
	namespace sspns {
		logWidget::logWidget(QString logpath, QString _title ):m_logpath(logpath) {
			m_label = new QLabel(_title);
			m_clearBtn = new QPushButton(u8"清除");
			m_outPutShow= new QTextEdit();
			m_initFlag = false;
			initLayout();
			initConnection();
			this->hide();
			
			this->setText();
		}
		void logWidget::initLayout() {
			m_outPutShow->setReadOnly(true);
			m_label->setFixedHeight(50);
			m_label->setAlignment(Qt::AlignCenter);

			m_clearBtn->setFixedHeight(50);
			
			QHBoxLayout* layout1 = new QHBoxLayout();
			layout1->addStretch();
			layout1->addWidget(m_clearBtn);
			QVBoxLayout* layout2 = new QVBoxLayout();
			layout2->addWidget(m_label);
			layout2->addWidget(m_outPutShow);
			layout2->addLayout(layout1);
			this->setLayout(layout2);
			this->setMinimumSize(QSize(800, 1000));
			this->setWindowTitle(u8"log日志窗口");
			loadStyleSheet("SSPNS");
		}
		void logWidget::initConnection() {
			//connect(SignalInstance::getInstance(),&SignalInstance::signalShowLogWidget,this,&logWidget::showWidget);
			//connect(m_clearBtn, &QPushButton::clicked, this, &logWidget::clearText);
			//connect(this,&QWidget::closeEvent,this,&logWidget::clearText);
			//connect(this, &QWidget::showEvent, this, &logWidget::setText);
			connect(m_clearBtn,&QPushButton::clicked,this,&logWidget::clearFileText);
		}
		void logWidget::closeEvent(QCloseEvent *event)  {
			//clearText();
			return QWidget::closeEvent(event);
		}
		void logWidget::showEvent(QShowEvent *event)  {
			//if (!m_initFlag) { setText(); m_initFlag = true; }
			return QWidget::showEvent(event);
		}
		void logWidget::clearText() {
			m_outPutShow->clear();
		}
		QString logWidget::getLastMsg() {
			QTextCursor cursor = m_outPutShow->textCursor();
			// 移动光标到文本末尾
			cursor.movePosition(QTextCursor::End);

			// 变量用于存储最后非空行的文本
			QString lastNonEmptyLine = "";
			while (true) {
				// 移动到当前块的开始
				cursor.movePosition(QTextCursor::StartOfBlock);
				// 获取当前块的文本
				QString currentLine = cursor.block().text();

				// 如果当前行不是空行，则保存并退出循环
				if (!currentLine.trimmed().isEmpty()) {
					lastNonEmptyLine = currentLine;
					break;
				}

				// 向上移动到前一个块
				if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
					// 如果已经到达文本的开头，退出循环
					break;
				}
			}
			return lastNonEmptyLine;
		}
		void logWidget::appendText(QString str) {
			QFile file(m_logpath);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
				m_outPutShow->setText("Failed to open the file.\n" + m_logpath);
				qDebug() << "m_logPath=" << m_logpath;
				return;
			}
			QTextStream out(&file);
			out << str;
			file.close();
			QString msgShow = m_outPutShow->toPlainText();
			msgShow += str;
			m_outPutShow->setText(msgShow);
			m_outPutShow->moveCursor(QTextCursor::End);
			update();

		}
		void logWidget::clearFileText() {
			QFile file(m_logpath);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				m_outPutShow->setText("Failed to open the file.\n" + m_logpath);
				qDebug() << "m_logPath=" << m_logpath;
				return;
			}
			file.close();
			m_outPutShow->clear();
		}
		void logWidget::setText() {
			QFile file(m_logpath);
			if (!file.open(QIODevice::ReadOnly| QIODevice::Text)) {
				m_outPutShow->setText("Failed to open the file.\n"+m_logpath);
				qDebug() << "m_logPath="<<m_logpath;
				return;
			}
			QTextStream in(&file);
			QString fileContent = in.readAll();
			m_outPutShow->setText(fileContent);
			m_outPutShow->moveCursor(QTextCursor::End);
			file.close();
		}
		void logWidget::showWidget() {
			//clearText();
			//setText();
			this->close();
			this->show();
		}
		void logWidget::loadStyleSheet(QString sheetName)
		{

			QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
			file.open(QFile::ReadOnly);//以只读的方式读取文件
			if (file.isOpen()) {
				setStyleSheet("");
				//将样式表全部读取，保存到字符串中
				QString qsstyleSheet = QLatin1String(file.readAll());

				setStyleSheet(qsstyleSheet);
				//qDebug() << "qsstyleSheet:" << qsstyleSheet;
			}
			file.close();
		}
	}
}