#include "progressTable.h"
#include "ParamEditDialog.h"
#define TRINITCOLUMNCOUNT 4
#define TRINITROWCOUNT 20
namespace platforms {
	namespace sspns {
		progressTable::progressTable(int _nsFlag,QWidget*parent)
		{
			nsFlag = _nsFlag;
			initUi();
			initConnection();
		}
		void progressTable::initUi() {
			QString qssTV = "QTableWidget::item:hover{background-color:rgb(92,188,227,200)}"
				"QTableWidget::item:selected{background-color:#1B89A1}"
				"QHeaderView::section,QTableCornerButton:section{ \
            padding:3px; margin:0px; color:#696969;  border:1px solid #242424; \
    border-left-width:0px; border-right-width:1px; border-top-width:0px; border-bottom-width:1px; \
background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #C1FFC1,stop:1 #B4EEB4); }"
"QTableWidget{background-color:white;border:none;}";
			m_row=TRINITROWCOUNT;
			m_column = TRINITCOLUMNCOUNT;
			m_table = new QTableWidget;
			m_mainLayout = new QVBoxLayout(this);
			m_title = new QLabel;
			
			QStringList strList;
			strList << u8"马赫数Ma" << u8"高度H" << u8"攻角AOA"<<u8"侧滑角AOB" << u8"操作" << u8"运行状态" << u8"上次运行时间";
			m_column = strList.size();

			m_table->setColumnCount(m_column);
			m_table->setRowCount(m_row);
			m_table->setStyleSheet(qssTV);
			m_table->setHorizontalHeaderLabels(strList);
			m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			m_table->horizontalHeader()->setMinimumSectionSize(100);
			m_table->verticalHeader()->setMinimumSectionSize(100);

			m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_table->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
			m_table->setColumnWidth(4, 300);
			for (int i = 0; i < 4; i++) {
				m_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
				m_table->setColumnWidth(i, 130);
			}
			m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
			m_table->setColumnWidth(5, 130);
			m_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
			m_table->setColumnWidth(6, 150);


			loadButton();
			m_title->setText(u8"当前未加载批量工作区");
			m_title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			m_title->setFixedHeight(40);
			QString titleSheet = u8"background:lightblue;font-family: \"Microsoft YaHei\";font-size:10pt;color:#000000;";
			m_title->setStyleSheet(titleSheet);
			m_mainLayout->addWidget(m_title);
			m_mainLayout->addWidget(m_table);
			this->setLayout(m_mainLayout);
			
		}
		void progressTable::loadButton() {
			
			int index = 0;
			for (int i = 0; i < m_table->columnCount(); ++i) {
				QTableWidgetItem *headerItem = m_table->horizontalHeaderItem(i);
				if (headerItem) {
					if (headerItem->text() == u8"操作") index = i;
				}
			}
			for (int i = 0; i < m_row; i++) {
				if (m_table->cellWidget(i, index) != nullptr) {
					m_table->removeCellWidget(i, index);
				}
			}
			deleteAllButtons(opBtn);
			opBtn.resize(m_row);
			for (int i = 0; i < m_row; i++) {
				opBtn[i] = new btnWigdet(i, 4, {u8"运行头部",u8"运行身部",u8"终止进程",u8"查看日志"});
				if (nsFlag == 1) {
					opBtn[i]->setBodyMode();
				}
				m_table->setCellWidget(i, index, opBtn[i]);
				
			}
			
		}
		void progressTable::connectAllButton() {

			for (int i = 0; i < opBtn.size(); i++) {
				auto f1 = std::bind(&progressTable::op1, this, std::placeholders::_1);
				auto f2 = std::bind(&progressTable::op2, this, std::placeholders::_1);
				auto f3 = std::bind(&progressTable::op3, this, std::placeholders::_1);
				auto f4 = std::bind(&progressTable::op4, this, std::placeholders::_1);
				opBtn[i]->dynamicConnect(0, this, f1);
				opBtn[i]->dynamicConnect(1, this, f2);
				opBtn[i]->dynamicConnect(2, this, f3);
				opBtn[i]->dynamicConnect(3, this, f4);
			}

		}
		void progressTable::initConnection() {
			connectAllButton();

			connect(SignalInstance::getInstance(), SIGNAL(signalUpdateBatchTable()), this, SLOT(updateTable()));
			connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(updateTable()));
			connect(SignalInstance::getInstance(), &SignalInstance::signalUpdateTable, this,&progressTable::updateStatus);
			//connect(SignalInstance::getInstance(), &SignalInstance::signalUpdateTime, this, &progressTable::updateTime);

			// 添加双击事件连接
			connect(m_table, &QTableWidget::cellDoubleClicked, this, &progressTable::onCellDoubleClicked);

			//设置某行高亮
			//connect(SignalInstance::getInstance(), &SignalInstance::signalUpdateRow, this, &progressTable::setRowBackgroundColor);
			
		}
		void progressTable::setRowBackgroundColor(int row,const QColor& color) {
			if (row < 0 || row >= m_table->rowCount()) {
				return; // 无效行索引直接返回
			}
			
			// 定义浅红色（RGB: 255, 230, 230），该颜色较浅不影响黑色字体显示
			

			// 遍历行中所有列，设置单元格背景色
			for (int col = 0; col < m_table->columnCount(); ++col) {
				QTableWidgetItem* item = m_table->item(row, col);
				if (!item) {
					// 为空单元格创建默认item并设置背景
					
					item = new QTableWidgetItem();
					m_table->setItem(row, col, item);
					
				}
				item->setBackground(color);
			}
		}
		// 处理单元格双击事件
		void progressTable::onCellDoubleClicked(int row, int column) {
			// 只处理前四列（参数列）的双击事件
			if (column < 0 || column >= 4) return;

			// 检查表格项是否存在
			if (!m_table->item(row, 0) || !m_table->item(row, 1) ||
				!m_table->item(row, 2) || !m_table->item(row, 3)) {
				return;
			}

			// 获取当前行的参数值
			double ma = m_table->item(row, 0)->text().toDouble();
			double h = m_table->item(row, 1)->text().toDouble();
			double aoa = m_table->item(row, 2)->text().toDouble();
			double aob = m_table->item(row, 3)->text().toDouble();

			QString path = SignalInstance::getInstance()->getWorkSpace();
			// 创建并显示参数编辑对话框
			ParamEditDialog dialog(ma, h, aoa, aob, path, row, this);
			// 显示对话框并处理结果
			if (dialog.exec() == QDialog::Accepted) {

				if (dialog.updateJsonConfig()) {
					// 更新表格显示
					m_table->setItem(row, 0, new QTableWidgetItem(QString::number(dialog.getMa())));
					m_table->setItem(row, 1, new QTableWidgetItem(QString::number(dialog.getH())));
					m_table->setItem(row, 2, new QTableWidgetItem(QString::number(dialog.getAOA())));
					m_table->setItem(row, 3, new QTableWidgetItem(QString::number(dialog.getAOB())));
					m_table->setItem(row, 5, new QTableWidgetItem(QString(u8"都未运行")));
					QColor white(255, 255, 255);
					setRowBackgroundColor(row, white);
					// 使用工厂类创建信息框
					//QMessageBox* infoMsg = StyleWidgetFactory::createInfoMessageBox(u8"成功", u8"参数已更新", this);
					//infoMsg->exec();
					//delete infoMsg;  // 手动释放
					
				}
				else {
					// 使用工厂类创建警告框
					QMessageBox* warnMsg = StyleWidgetFactory::createWarnMessageBox(
						u8"错误", u8"无法更新配置文件，请检查‘该参数配置是否已经设置’或者‘修改文件夹正在处于打开状态’.", this);
					warnMsg->exec();
					delete warnMsg;  // 手动释放
				}
				
			}
		}
		void progressTable::updateTime(int threadId, QString str) {
			m_table->setItem(threadId, 6, new QTableWidgetItem(str));
		}
		void progressTable::updateStatus(int threadId, QString str) {
			m_table->setItem(threadId, 5, new QTableWidgetItem(str));
			QColor lightRed(255, 230, 230);
			QColor white(255,255,255);
			QDate currentDate = QDate::currentDate();
			updateTime(threadId, currentDate.toString("yyyy-MM-dd"));
			if(str== u8"头部运行失败"|| str== u8"pns身部运行失败")setRowBackgroundColor(threadId,lightRed);
			else setRowBackgroundColor(threadId, white);
		}
		void progressTable::updateTable() {

			QString batch_config_file = SignalInstance::getInstance()->getWorkSpace() + "/Batch/batchConfig.json";

			Json::Reader json_reader;
			Json::Value root;
			std::ifstream infile(batch_config_file.toLocal8Bit().toStdString());
			qDebug() << "batch_config_file=" << batch_config_file;
			if (!infile.is_open())
			{
				//QMessageBox::information(this, u8"提示", u8"文件打开失败，请检查文件是否存在!");
				m_title->setText(u8"当前无批量计算任务");
				num_case = 0;
				m_row = TRINITROWCOUNT;
				m_table->setRowCount(m_row);
				for (int j = 0; j < m_table->columnCount(); j++) {
					QTableWidgetItem *headerItem = m_table->horizontalHeaderItem(j);
					if (headerItem) {
						if (headerItem->text() == u8"操作") continue;
					}
					for (int i = 0; i < m_table->rowCount(); i++) {
						if (m_table->cellWidget(i, j) != nullptr) {
							m_table->removeCellWidget(i, j);
						}
						m_table->setItem(i, j, new QTableWidgetItem());
					}

				}
				loadButton();
				connectAllButton();
				// 重置所有行颜色为白色
				QColor white(255, 255, 255);
				for (int i = 0; i < m_table->rowCount(); ++i) {
					setRowBackgroundColor(i, white);
				}
				emit SignalInstance::getInstance()->signalProcessPoolDelete();
				emit SignalInstance::getInstance()->signalBatchDoneNot();
				return;
			}
			if (json_reader.parse(infile, root))
			{
				int case_num = root["case_number"].asInt();
				num_case = case_num;
				m_row = case_num;
				m_title->setText(u8"批量计算任务 " + QString::number(case_num) + u8" 个案例");
				m_table->setRowCount(m_row);
				loadButton();
				connectAllButton();

				std::vector<std::string> args = { "Ma","H","AOA","AOB" };
				QColor lightRed(255, 230, 230);
				QColor white(255, 255, 255);

				for (int i = 0; i < case_num; i++)
				{
					std::string caseName = "case_" + std::to_string(i);
					Json::Value _case = root[caseName];
					for (int j = 0; j < args.size(); j++) {
						std::string key = args[j];
						float value = _case[key].asFloat();
						m_table->setItem(i, j, new QTableWidgetItem(QString::number(value)));
					}

					// 设置状态并判断颜色
					int statusValue = _case["status"].asInt();
					QString statusStr;
					if (statusValue) {
						statusStr = QString(statusName[statusValue]);
						m_table->setItem(i, 5, new QTableWidgetItem(statusStr));
					}
					else {
						bool flagHead = _case["finish_head"].asBool(), flagBody = _case["finish_body"].asBool();
						//qDebug() << "flagHead=" << flagHead << " flagBody=" << flagBody;
						if(flagHead&&flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"全部运行完成")));
						else if (!flagHead&&flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"身部运行完成")));
						else if (flagHead&&!flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"头部运行完成")));
						else if (!flagHead && !flagBody) {
							
							if (statusValue == processStatus::BODY_FAIL)m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"身部运行失败")));
							else if (statusValue == processStatus::HEAD_FAIL)m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"头部运行失败")));
							else m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"都未运行")));
						}
					}
					
					std::string lastTime = _case["last_run_time"].asString();
					m_table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(lastTime)));
					// 根据状态设置行颜色
					if (statusValue == processStatus::BODY_FAIL || statusValue == processStatus::HEAD_FAIL) {
						setRowBackgroundColor(i, lightRed);
					}
					else {
						setRowBackgroundColor(i, white);
					}

					
				}

				emit SignalInstance::getInstance()->signalBatchDone();
				emit SignalInstance::getInstance()->signalProcessPoolCreate();
			}
			infile.close();
		}
		//void progressTable::updateTable() {

		//	QString batch_config_file = SignalInstance::getInstance()->getWorkSpace() + "/Batch/batchConfig.json";

		//	Json::Reader json_reader;
		//	Json::Value root;
		//	std::ifstream infile(batch_config_file.toLocal8Bit().toStdString());
		//	qDebug() << "batch_config_file=" << batch_config_file;
		//	if (!infile.is_open())
		//	{
		//		//QMessageBox::information(this, u8"提示", u8"文件打开失败，请检查文件是否存在!");
		//		m_title->setText(u8"当前未加载批量工作区");
		//		num_case = 0;
		//		m_row = TRINITROWCOUNT;
		//		m_table->setRowCount(m_row);
		//		for (int j = 0; j < m_table->colorCount(); j++) {
		//			QTableWidgetItem *headerItem = m_table->horizontalHeaderItem(j);
		//			if (headerItem) {
		//				if (headerItem->text() == u8"操作") continue;
		//			}
		//			for (int i = 0; i < m_table->rowCount(); i++) {
		//				if (m_table->cellWidget(i, j) != nullptr) {
		//					m_table->removeCellWidget(i, j);
		//				}
		//				m_table->setItem(i, j, new QTableWidgetItem());
		//			}

		//		}
		//		loadButton();
		//		connectAllButton();
		//		emit SignalInstance::getInstance()->signalProcessPoolDelete();
		//		emit SignalInstance::getInstance()->signalBatchDoneNot();
		//		return;
		//	}
		//	if (json_reader.parse(infile, root))
		//	{
		//		int case_num = root["case_number"].asInt();
		//		num_case = case_num;
		//		m_row = case_num;
		//		m_title->setText(u8"工作区中有 " + QString::number(case_num) + u8" 个实例");
		//		//m_inputExampleNum->setText(QString::number(case_num));
		//		//m_batchTabel->clearContents();
		//		//m_batchTabel->setRowCount(case_num);
		//		m_table->setRowCount(m_row);
		//		loadButton();
		//		connectAllButton();
		//		std::vector<std::string> args = { "Ma","H","AOA","AOB" };
		//		for (int i = 0; i < case_num; i++)
		//		{
		//			std::string caseName = "case_" + std::to_string(i);
		//			Json::Value _case = root[caseName];
		//			for (int j = 0; j < args.size(); j++) {
		//				std::string key = args[j];
		//				float value = _case[key].asFloat();
		//				m_table->setItem(i, j, new QTableWidgetItem(QString::number(value)));
		//			}
		//			int statusValue = _case["status"].asInt();
		//			if (statusValue) {
		//				m_table->setItem(i, 5, new QTableWidgetItem(QString(statusName[statusValue])));
		//			}
		//			else {
		//				bool flagHead = _case["finish_head"].asBool(), flagBody = _case["finish_body"].asBool();
		//				if(flagHead&&flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"全部运行完成")));
		//				else if (!flagHead&&flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"身部运行完成")));
		//				else if (flagHead&&!flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"头部运行完成")));
		//				else if (!flagHead&&!flagBody) m_table->setItem(i, 5, new QTableWidgetItem(QString(u8"都未运行")));
		//			}
		//			
		//			std::string lastTime = _case["last_run_time"].asString();
		//			m_table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(lastTime)));
		//			
		//		}
		//		emit SignalInstance::getInstance()->signalBatchDone();
		//		//emit SignalInstance::getInstance()->signalPrintInfo(u8"成功加载" + QString::number(case_num) + u8"个实例");
		//		emit SignalInstance::getInstance()->signalProcessPoolCreate();
		//		//QMessageBox::information(this, u8"提示", u8"成功加载" + QString::number(case_num) + u8"个实例");
		//	}
		//	infile.close();
		//}
		//执行头部
		void progressTable::op1(int threadId) {
			
			QString str = QString("op1 threadId=%1").arg(threadId);
			///emit SignalInstance::getInstance()->signalPrintInfo(str);
			emit SignalInstance::getInstance()->signalProcessOp1(threadId);
		}
		//执行身部
		void progressTable::op2(int threadId) {
			QString str = QString("op2 threadId=%1").arg(threadId);
			//emit SignalInstance::getInstance()->signalPrintInfo(str);
			emit SignalInstance::getInstance()->signalProcessOp2(threadId);
		}
		//终止进程
		void progressTable::op3(int threadId) {

			QString str = QString("op3 threadId=%1").arg(threadId);
			//emit SignalInstance::getInstance()->signalPrintInfo(str);
			emit SignalInstance::getInstance()->signalProcessOp3(threadId);
		}
		//重新运行
		void progressTable::op4(int threadId) {
			QString str = QString("op4 threadId=%1").arg(threadId);
			//emit SignalInstance::getInstance()->signalPrintInfo(str);
			emit SignalInstance::getInstance()->signalProcessOp4(threadId);
		}
		void progressTable::deleteAllButtons(QVector<btnWigdet*>& opBtn) {
			for (btnWigdet*& innerWidget : opBtn) {
				delete innerWidget;
			}
			opBtn.clear();
		}
	}
}