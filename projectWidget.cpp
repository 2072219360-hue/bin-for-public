#include "projectWidget.h"
extern QString appComDir;
namespace platforms {
	namespace sspns {
		projectWidget::projectWidget(int _flag, QWidget *parent): QWidget(parent) {
			m_starter = nullptr;
			ns_flag = _flag;
			initWidget();
			initConnection();
			
			//openFile();
		}
		void projectWidget::initConnection() {
			//connect(openButton, &QPushButton::clicked, this, &projectWidget::openFile);
			connect(clearButton, &QPushButton::clicked, this, &projectWidget::clearProjects);
			connect(listView, &QListView::doubleClicked, this, &projectWidget::handleItemClick);
		}
		void projectWidget::initWidget() {
			this->setMinimumSize(1200, 400);
			this->setWindowTitle(u8"最近打开项目");
			// 创建布局
			layout = new QVBoxLayout(this);

			
			clearButton = new QPushButton(u8"清空最近所有项目",this);

			// 创建列表视图
			listView = new QListView(this);
			listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
			layout->addWidget(listView);
			layout->addWidget(clearButton);
			QFont font = listView->font();
			font.setPointSize(14); // 设置字体大小，例如12
			listView->setFont(font);

			// 创建模型
			model = new QStringListModel(this);
			listView->setModel(model);
			if(m_starter==nullptr)m_starter = new detail::workSpaceStarter(ns_flag);
			m_starter->m_nsFlag = ns_flag;
		}
		void projectWidget::clearProjects() {
			openFile();
			QString path;
			if (ns_flag == 0) {
				path = appComDir + "/help/nsPnsTable.txt";
			}
			else {
				path = appComDir + "/help/pnsTable.txt";
			}
			QFile file(path);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << u8"无法打开文件以追加文本:" << file.errorString();
				return;
			}
			QTextStream out(&file);
			file.close();
			openFile();
		}
		void projectWidget::insertInfo(QString fileName, QString gridName, QString inpName) {
			openFile();
			if (mp.count(fileName)) return;
			QString path;
			if (ns_flag == 0) {
				path = appComDir + "/help/nsPnsTable.txt";
			}
			else {
				path = appComDir + "/help/pnsTable.txt";
			}
			QFile file(path);
			if (!file.open(QIODevice::Append | QIODevice::Text)) {
				qDebug() << u8"无法打开文件以追加文本:" << file.errorString();
				return;
			}
			QTextStream out(&file);
			out << fileName << "\n";
			out << gridName << "\n";
			out << inpName << "\n";
			file.close();
		}
		void projectWidget::openFile() {
			// 打开文件对话框
			//QString fileName = QFileDialog::getOpenFileName(this, u8"选择文件", "", "Text Files (*.txt);;All Files (*)");

			//if (fileName.isEmpty()) {
			//	return; // 如果没有选择文件，返回
			//}
			this->checkAndRemoveInvalidEntries();
			QString fileName;
			if (ns_flag == 0) {
				fileName = appComDir + "/help/nsPnsTable.txt";
			}
			else {
				fileName = appComDir + "/help/pnsTable.txt";
			}
			// 读取文件内容
			QFile file(fileName);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QMessageBox::warning(this, u8"错误", u8"无法打开文件");
				return;
			}
			mp.clear();
			QStringList lines; // 存储每行文本
			QTextStream in(&file);
			while (!in.atEnd()) {
				QString line = in.readLine();
				if (in.atEnd()) break;
				QString gridName = in.readLine();
				if (in.atEnd()) break;
				QString inpName = in.readLine();
				lines << line; // 将每行添加到QStringList中
				mp[line].push_back(gridName);
				mp[line].push_back(inpName);
			}
			file.close();

			// 更新模型
			model->setStringList(lines);
		}
		void projectWidget::checkAndRemoveInvalidEntries() {
			// 用于存储需要删除的key
			QStringList keysToRemove;

			// 遍历mp检查每个条目对应的文件/目录是否存在
			for (auto it = mp.begin(); it != mp.end(); ++it) {
				QString fileName = it.key();
				QVector<QString> values = it.value();

				// 检查主文件/目录是否存在
				QFileInfo fileInfo(fileName);
				if (!fileInfo.exists()) {
					keysToRemove.append(fileName);
					continue;
				}

				// 检查gridName和inpName对应的文件是否存在
				if (values.size() >= 2) {
					QFileInfo gridInfo(values[0]);
					QFileInfo inpInfo(values[1]);

					// 如果任一关联文件不存在，也删除该条目
					if (!gridInfo.exists() || !inpInfo.exists()) {
						keysToRemove.append(fileName);
					}
				}
				else {
					// 数据不完整，也删除该条目
					keysToRemove.append(fileName);
				}
			}

			// 如果没有需要删除的条目，直接返回
			if (keysToRemove.isEmpty()) {
				return;
			}

			// 从mp中删除无效条目
			for (const QString &key : keysToRemove) {
				mp.remove(key);
			}

			// 更新txt文件
			QString path;
			if (ns_flag == 0) {
				path = appComDir + "/help/nsPnsTable.txt";
			}
			else {
				path = appComDir + "/help/pnsTable.txt";
			}

			QFile file(path);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << u8"无法打开文件以写入文本:" << file.errorString();
				QMessageBox::warning(this, u8"错误", u8"无法保存修改，请检查文件权限");
				return;
			}

			QTextStream out(&file);
			// 写入剩余的有效条目
			for (auto it = mp.begin(); it != mp.end(); ++it) {
				if (it.value().size() >= 2) {
					out << it.key() << "\n";
					out << it.value()[0] << "\n";
					out << it.value()[1] << "\n";
				}
			}
			file.close();

			// 更新列表显示
			openFile();

			// 提示用户有多少条目被删除
			/*if (keysToRemove.size() > 0) {
				QMessageBox::information(this, u8"清理完成",
					QString(u8"已移除 %1 个不存在的项目记录").arg(keysToRemove.size()));
			}*/
		}
		void projectWidget::deleteProject(const QString &key) {
			// 检查key是否存在
			if (!mp.contains(key)) {
				QMessageBox::warning(this, u8"提示", u8"该项目不存在，无法删除");
				return;
			}

			// 从map中移除该key
			mp.remove(key);

			// 确定文件路径
			QString path;
			if (ns_flag == 0) {
				path = appComDir + "/help/nsPnsTable.txt";
			}
			else {
				path = appComDir + "/help/pnsTable.txt";
			}

			// 打开文件并写入剩余内容
			QFile file(path);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << u8"无法打开文件以写入文本:" << file.errorString();
				QMessageBox::warning(this, u8"错误", u8"无法保存修改，请检查文件权限");
				return;
			}

			QTextStream out(&file);
			// 遍历剩余的所有条目并写入文件
			for (auto it = mp.begin(); it != mp.end(); ++it) {
				if (it.value().size() >= 2) {
					out << it.key() << "\n";          // fileName
					out << it.value()[0] << "\n";     // gridName
					out << it.value()[1] << "\n";     // inpName
				}
			}
			file.close();

			// 更新列表显示
			openFile();

			QMessageBox::information(this, u8"成功", u8"项目已删除");
		}
		QString projectWidget::handleItemClick(const QModelIndex &index) {
			// 获取选中的文本
			QString selectedText = model->data(index, Qt::DisplayRole).toString();

			// 显示选中的文本
			if (!selectedText.isEmpty()) {
				QDir dir(selectedText);
				if (!dir.exists()) {  // 目录不存在时直接返回空字符串
					QMessageBox::information(this, u8"成功", u8"项目不存在");
					// 额外防护：关闭可能的编辑状态 【核心修改3】
					listView->closePersistentEditor(index);
					return "";
				}
				//QMessageBox::information(this, "选中的文本", selectedText);
				QString gridName, inpName;
				gridName = mp[selectedText][0];
				inpName = mp[selectedText][1];
				//QMessageBox::information(this, u8"选中的文本", "gridName="+ gridName+"\n inpName="+inpName);
				this->close();
				m_starter->setString(selectedText, gridName, inpName);
				SignalInstance::getInstance()->setWorkSpace(selectedText);
				SignalInstance::getInstance()->setGridName(gridName);
				SignalInstance::getInstance()->setInpName(inpName);
				m_starter->run();
				return selectedText+" "+gridName + " " + inpName;
			}
			return "";
		}
	}
}