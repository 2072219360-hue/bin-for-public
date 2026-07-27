#include "TrTableShow.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include "OutPutLogger.h"
#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>
#include "SignalInstance.h"
using platforms::sspns::reveal::OutPutLogger;
extern OutPutLogger* outPutLogger;
#define TRINITCOLUMNCOUNT 7
#define TRINITROWCOUNT 3
namespace platforms {
	namespace sspns {
		namespace reveal {
			TrTableShow::TrTableShow(QWidget*parent)
			{
				initUi();
			}
			TrTableShow::~TrTableShow()
			{
			}

			void TrTableShow::initUi()
			{
				editFlag = false;
				QString qssTV = "QTableWidget::item:hover{background-color:rgb(92,188,227,200)}"
					"QTableWidget::item:selected{background-color:#1B89A1}"
					"QHeaderView::section,QTableCornerButton:section{ \
            padding:3px; margin:0px; color:#696969;  border:1px solid #242424; \
    border-left-width:0px; border-right-width:1px; border-top-width:0px; border-bottom-width:1px; \
background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #C1FFC1,stop:1 #B4EEB4); }"
"QTableWidget{background-color:white;border:none;}";
				m_table = new QTableWidget;
				m_mainLayout = new QVBoxLayout(this);
				//QWidget*tableShowWidget = new QWidget;
				exeBtn = new QPushButton(u8"运行");
				connect(exeBtn, SIGNAL(clicked(bool)), this, SLOT(exeBody()));
				exeBtn->setFixedHeight(30);
				QStringList strList;

				strList << u8"分段"
					<< u8"初始步长"
					<< u8"初始阶段步长限制"
					<< u8"终止阶段步长限制"
					<< u8"最大步长限制"
					<< u8"步长增长因子"
					<< u8"初始步长计算步数";

				m_table->setColumnCount(TRINITCOLUMNCOUNT);
				m_table->setRowCount(TRINITROWCOUNT);
				m_rowCount = TRINITROWCOUNT;
				m_table->setStyleSheet(qssTV);
				//m_table->insertRow(0);
				m_table->setHorizontalHeaderLabels(strList);
				m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
				//设置整行选中
				m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
				m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
				m_table->setFont(QFont("微软雅黑", 10));
				exeBtn->setStyleSheet("QPushButton{font-size:20px;background:lightblue;color:\
			#ffffff;border:none;border-radius:4px;\
		}QPushButton:hover, QPushButton:pressed{\
			background:rgb(7, 189, 253);\
		}");
				
				connect(SignalInstance::getInstance(), &SignalInstance::signalLoadTrData, this,[=](){
					QThread * thr = QThread::create([=]() {
						initSectPreData();
						loadTrData();
					});
					connect(thr, &QThread::finished, this, [=]() {
						emit SignalInstance::getInstance()->signalLoadTrData2();
					});
					thr->start();
				});
				connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, [=]() {
					m_table->clearContents();
					m_table->setRowCount(TRINITCOLUMNCOUNT);
					m_colorList.clear();
				});
				connect(m_table, &QTableWidget::cellClicked, this, [=](int row, int column) {
					//给某段高亮
					emit SignalInstance::getInstance()->signalHighlight(row);
				});
				
				QObject::connect(m_table, &QTableWidget::itemChanged, [&](QTableWidgetItem *item) {
					if (editFlag) {
						editFlag = false;
						saveTrData();
					}
					
					
				});
				connect(m_table, &QTableWidget::cellDoubleClicked, this, [=](int row, int column) {
					selectCell(row, column);


				});
				connect(SignalInstance::getInstance(), &SignalInstance::signaleSelectCell, this, [=](int row) {
					m_table->selectRow(row);

				});
				connect(SignalInstance::getInstance(), SIGNAL(signalChooseColor(int, int, int, int)), this, SLOT(changeColor(int, int, int, int)));
				connect(SignalInstance::getInstance(), SIGNAL(signalSelectTrData(int, QStringList)), this, SLOT(onLoadSelectedData(int, QStringList)));
				connect(SignalInstance::getInstance(), SIGNAL(signalTrDataLoad(int, QStringList*, unsigned char**)), this, SLOT(onPreLoadData(int, QStringList*, unsigned char**)));
				connect(SignalInstance::getInstance(), SIGNAL(signalRedictTableData()), this, SLOT(onPreLoadTableData()));
				m_mainLayout->addWidget(m_table);
				this->setLayout(m_mainLayout);

			}
			void TrTableShow::setBatchmode() {
				m_table->disconnect();
				SignalInstance::getInstance()->disconnect(this);
				connect(SignalInstance::getInstance(), &SignalInstance::signalWorkSpaceChanged, this, [=]() {
					m_table->clearContents();
					m_table->setRowCount(TRINITCOLUMNCOUNT);
					m_colorList.clear();
				});
				connect(SignalInstance::getInstance(), SIGNAL(signalOnlyTrDataLoad(int, QStringList*, unsigned char**)), this, SLOT(onPreLoadData(int, QStringList*, unsigned char**)));

			}
			
			void TrTableShow::initSectPreData()
			{	
				auto fileExists=[&](const QString &filePath) ->bool{
					QFile file(filePath);
					return file.exists();
				};

				QString path = QString(SignalInstance::getInstance()->m_workspace + "/body/trData.dat");
				BodyPathChecker tempChecker;
				path = tempChecker.checkAndGetPath(path);

				bool existFlag = fileExists(path);
				m_sectPreData.resize(0);
				if (existFlag) {
					std::ifstream inTrLoad;
					inTrLoad.open(path.toLocal8Bit().toStdString());
					int p = 0;
					string str;
					int index = 0;
					
					while (getline(inTrLoad, str)) {
						if (str == "#Section Xstep0 xstepmin min1  max kstep  ncstep") {
							while (getline(inTrLoad, str)) {
								if (str == "") break;
								QVector<QString> rowData;
								stringstream ss;
								ss << str;
								for (int i = 0; i < 7; i++) {
									string info;
									ss >> info;
									rowData.push_back(QString::fromStdString(info));
								}
								ss.str("");
								ss.clear();
								m_sectPreData.push_back(rowData);
							};
						}
						
					}

					qDebug() << "m_sectPreData.size()=" << m_sectPreData.size() << endl;

				}
				else {
					int sectCount = getSectNames().size();
					for (int i = 0; i < sectCount; i++) {
						QVector<QString> rowData;
						// 修改分段编号的地方
						rowData.push_back(QString::number(i+1));
						rowData.push_back(QString("1.D-6"));
						rowData.push_back(QString("1.D-4"));
						rowData.push_back(QString("5.D-5"));
						rowData.push_back(QString("1.D-3"));
						rowData.push_back(QString("1.005"));
						rowData.push_back(QString("200"));
						m_sectPreData.push_back(rowData);
					}
				}
			}
			void TrTableShow::reshowColor() {
				for (int i = 0; i < m_sectCount; i++)
				{
					qDebug() << "i=" << i << " color=" << m_table->item(i, 0)->backgroundColor();
					//m_table->item(i, 0)->setBackground();
					qDebug() << "i=" << i << "colorList="<<m_colorList[i];
					QStringList numbers = m_colorList[i].split(' ');
					int r, g, b;
					r=numbers[0].toInt();
					g = numbers[1].toInt();
					b = numbers[2].toInt();
					qDebug() << "r g b =" << r << " " << g << " " << b;
					m_table->item(i, 0)->setBackground(QColor(r, g, b));
					
				}

				update();
			}
			void TrTableShow::changeColor(int r, int g, int b, int sectId) {
				int columnCount = m_table->columnCount();
				for (int i = 0; i < 1; i++) {

					m_table->item(sectId, i)->setBackground(QColor(r, g, b));
					update();
					qDebug() <<"sectId="<<sectId<< "change color="<<m_table->item(sectId, i)->backgroundColor();
				}
				qDebug() << "m_colorList size=" << m_colorList.size();
				for (int i = 0; i < m_colorList.size(); i++) {
					qDebug() << "i=" << i << " m_colorList.size=" << m_colorList[i];
				}
				if (sectId >= m_colorList.size()) {
					QString colorStr = QString::number(r) + " " + QString::number(g) + " " + QString::number(b);
					m_colorList << colorStr;
				}
				else
				{
					QString colorStr = QString::number(r) + " " + QString::number(g) + " " + QString::number(b);
					m_colorList[sectId] = colorStr;
				}
				saveTrData();
			}
			void TrTableShow::selectCell(int row, int col) {
				if (col == 0) {
					emit SignalInstance::getInstance()->signalCancelHighlight();;
					emit SignalInstance::getInstance()->signalChooseSect(row);
				}
				else {
					QTableWidgetItem *item =m_table->item(row,col);
					if (item == nullptr) return;
					editFlag = true;
					m_table->editItem(item);
					//item->setFlags(item->flags() | Qt::ItemIsEditable);
					/*saveTrData();
					update();*/
				}
			}
			void TrTableShow::onLoadSelectedData(int sectId, QStringList list)
			{
				for (int i = 1; i < list.size(); i++)
				{
					m_table->setItem(sectId, i, new QTableWidgetItem(list[i]));
					m_table->item(sectId, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				}
				//将参数进行写入（用于下次加载以及写入到身部程序中）
				saveTrData();
				update();
			}
			
			void TrTableShow::onPreLoadData(int sectNum, QStringList * list, unsigned char** colorData)
			{
				qDebug() << "---";
				qDebug() << m_table->rowCount() << " " << sectNum;
				if (m_table->rowCount() < sectNum)
				{
					m_table->setRowCount(sectNum);
					m_rowCount = sectNum;
					//m_table->insertRow(0);
				}
				qDebug() << "list.size=" << list->size();
				for (int i = 0; i < sectNum; i++)
				{
					qDebug() << "list[i].size=" << list[i].size();
					for (int j = 0; j < list[i].size(); j++)
					{
						//qDebug() << list[i][j];
						m_table->setItem(i, j, new QTableWidgetItem(list[i][j]));

						m_table->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
						if (j == 0)
						{
							int r = colorData[i][0];
							int g = colorData[i][1];
							int b = colorData[i][2];
							QString colorStr = QString::number(r) + " " + QString::number(g) + " " + QString::number(b);
							m_colorList << colorStr;
							m_table->item(i, j)->setBackground(QColor(r, g, b));
						}
					}
				}
				delete[]list;
				delete[]colorData;
			}

			void TrTableShow::onPreLoadTableData()
			{
			
				for (int i = 0; i < m_sectCount; i++)
				{
					qDebug() << "i=" << i << " color=" << m_table->item(i, 0)->backgroundColor();
					for (int j = 0; j < m_table->columnCount(); j++)
					{
						m_table->setItem(i, j, new QTableWidgetItem(m_sectPreData[i][j]));
						m_table->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
						
					}
				}
				update();
			}

			void TrTableShow::loadTrData()
			{
				QStringList sectNames = getSectNames();
				m_sectCount = sectNames.size();
				if (m_table->rowCount() < sectNames.count())
				{
					m_table->setRowCount(sectNames.count());
				}
				for (int i = 0; i < sectNames.size(); i++)
				{
					m_table->setItem(i, 0, new QTableWidgetItem(sectNames[i]));
					m_table->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

				}

				update();
			}
			void TrTableShow::exeBody()
			{
				//qDebug() << "exeBody！！";
				QString bodyPath = "/SSPNS/ui/ui/body/";
				emit SignalInstance::getInstance()->signalBodyParameter(bodyPath);
			}

			QStringList TrTableShow::getSectNames()
			{
				QStringList res;
				QString path = SignalInstance::getInstance()->m_workspace + "/TR.dat";
				QFile file(path);
				if (file.open(QIODevice::ReadOnly))
				{

					QTextStream in(&file);
					QString line;
					int indexLine = 0;
					while (!in.atEnd())
					{
						line = in.readLine();
						//将多重空格变为一个空格
						line.replace(QRegExp("[\\s]+"), " ");
						QStringList list = line.split(" ");
						//将空格去除
						for (int i = 0; i < list.size(); i++)
						{
							if (list.at(i) == " ")
							{
								list.removeAt(i);
								continue;
							}
							i++;
						}
						if (list.size() == 2)
						{
							if (list.at(0) == "#Sect")
							{
								res.append(list.at(1));
							}
						}
					}
				}
				else {

				}
				return res;
			}

			void TrTableShow::saveTrData()
			{	
				if (m_table->rowCount() < 1) return;
				if (m_table->item(0, 0)->text().toStdString() == "") return;
				int count = 0;
				count = m_colorList.size();
				for (int i = 0; i < m_colorList.size(); i++) {
					qDebug() << "saveTrData i=" << i << " m_colorList=" << m_colorList[i];
				}
				auto judge = [&]()->bool {
					for (int i = 0; i < count; i++)
					{
						for (int j = 0; j < m_table->columnCount(); j++)
						{
							if (!m_table->item(i, j)) {
								return false;
							}

						}

					}
					return true;
				};

				

				if (!judge()) return;

				std::ofstream outTrLoad;
				BodyPathChecker tempChecker;
				QString truePath = tempChecker.checkAndGetPath(QString(SignalInstance::getInstance()->m_workspace + "/body/trData.dat"));

				outTrLoad.open(truePath.toLocal8Bit().toStdString());
				if (!outTrLoad.is_open()) return;

				for (int i = 0; i < count; i++)
				{
				
					outTrLoad << m_colorList[i].toStdString() << std::endl;
				}
				//outTrLoad << std::endl;
				QVector<QString> rowData;
				rowData.push_back(QString::number(0));
				rowData.push_back(QString("1.D-6"));
				rowData.push_back(QString("1.D-4"));
				rowData.push_back(QString("5.D-5"));
				rowData.push_back(QString("1.D-3"));
				rowData.push_back(QString("1.005"));
				rowData.push_back(QString("200"));
				qDebug() << "save TRData";
				outTrLoad << "#Section " << "Xstep0 " << "xstepmin " << "min1  " << "max " << "kstep  " << "ncstep" << std::endl;
				for (int i = 0; i < count; i++)
				{
					rowData[0] = QString::number(i);
					QString tempStr = "info i=" + i;
					for (int j = 0; j < m_table->columnCount(); j++)
					{
						if (!m_table->item(i, j)) {
							tempStr += " info=empty";
							outTrLoad << rowData[j].toStdString() << " ";
							continue;
						}
						tempStr += " info=" + m_table->item(i, j)->text();
						outTrLoad << m_table->item(i, j)->text().toStdString() << " ";
					}
					outTrLoad << std::endl;
					qDebug() << tempStr;
				}
				outTrLoad.close();

				sync_trData_input*updateInput = new sync_trData_input(SignalInstance::getInstance()->m_workspace.toLocal8Bit().toStdString());
				updateInput->run();
			}
			QTableWidget* TrTableShow::getTable()
			{
				return m_table;
			}
		}
	}
}







