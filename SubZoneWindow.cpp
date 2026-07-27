#include "SubZoneWindow.h"

SubZoneWindow::SubZoneWindow(int32_t zoneNum, QVector<QString>zoneNames, QWidget *parent)
	: QWidget(parent)
{
	layoutBtn_ = new QHBoxLayout;
	layoutMain_ = new QVBoxLayout;
	okBtn_ = new QPushButton("ok");
	closeBtn_ = new QPushButton("close");
	zoneNum_ = zoneNum;
	zoneNames_ = zoneNames;
	tableHeaderLabels_ << "Zone Number" << "Zone Name" << "Show Zone";
	init();
}

SubZoneWindow::~SubZoneWindow()
{}

void SubZoneWindow::init()
{
	initUi();
	initConnection();
}

void SubZoneWindow::initUi()
{
	tableWidget_ = new QTableWidget(this);
	tableWidget_->setRowCount(zoneNum_);
	tableWidget_->setColumnCount(COLUMNCOUNT);
	tableWidget_->setFixedSize(900, 350);
	tableWidget_->setAutoScroll(true);
	QFont font("song", 12);
	tableWidget_->setFont(font);
	//设置表格表头
	tableWidget_->setHorizontalHeaderLabels(tableHeaderLabels_);
	tableWidget_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableWidget_->verticalHeader()->hide();
	tableWidget_->setColumnWidth(0, 25);

	tableWidget_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	// 添加number以及name
	for (auto row = 0; row < zoneNum_; row++)
	{
		QTableWidgetItem* item = new QTableWidgetItem;
		item->setText(QString::number(row));
		tableWidget_->setItem(row, 0, item);

		QTableWidgetItem* itemZoneName = new QTableWidgetItem;
		itemZoneName->setText(zoneNames_[row]);
		tableWidget_->setItem(row, 1, itemZoneName);

		QCheckBox* checkBox = new QCheckBox;
		checkBox->setChecked(true);
		tableWidget_->setCellWidget(row, 2, checkBox);
	}

	for (int32_t row = 0; row < zoneNum_; row++)
	{
		QTableWidgetItem* item = tableWidget_->item(row, 0);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem* item1 = tableWidget_->item(row, 1);
		item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
	}
	layoutBtn_->addStretch();
	layoutBtn_->addWidget(okBtn_);
	layoutBtn_->addWidget(closeBtn_);
	layoutMain_->addWidget(tableWidget_);
	layoutMain_->addLayout(layoutBtn_);
	this->setLayout(layoutMain_);
	this->setWindowIcon(QIcon(":/images/default.png"));
	this->setWindowTitle(u8"Zone Style");
	tableWidget_->horizontalHeader()
		->setStyleSheet("background-color:#257EA2;");

}

void SubZoneWindow::initConnection()
{
	connect(okBtn_, &QPushButton::clicked, [this]() {
		QVector<int>zones;
		for (auto row = 0; row < zoneNum_; row++)
		{
			QCheckBox* checkbox;
			checkbox = qobject_cast<QCheckBox*>(tableWidget_->cellWidget(row, 2));
			if (checkbox->isChecked())
			{
				zones.push_back(row);
			}
		}
		emit SignalInstance::getInstance()->signalZonesVisbleChanged(zones);
		this->close();
	});

	connect(closeBtn_, &QPushButton::clicked, [this]() {
		this->close();
	});
}

void SubZoneWindow::setZoneNum(int32_t zoneNum)
{
	zoneNum_ = zoneNum;
	tableWidget_->setRowCount(zoneNum);
}

void SubZoneWindow::setZoneNames(QVector<QString> zoneNames)
{
	zoneNames_ = zoneNames;
}

void SubZoneWindow::update()
{
	tableWidget_->clearContents();
	// 添加number以及name
	for (auto row = 0; row < zoneNum_; row++)
	{
		QTableWidgetItem* item = new QTableWidgetItem;
		item->setText(QString::number(row));
		tableWidget_->setItem(row, 0, item);

		QTableWidgetItem* itemZoneName = new QTableWidgetItem;
		itemZoneName->setText(zoneNames_[row]);
		tableWidget_->setItem(row, 1, itemZoneName);

		QCheckBox* checkBox = new QCheckBox;
		checkBox->setChecked(true);
		tableWidget_->setCellWidget(row, 2, checkBox);
	}

	tableWidget_->viewport()->update();
	for (auto row = 0; row < zoneNum_; row++)
	{
		QTableWidgetItem* item = tableWidget_->item(row, 0);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		QTableWidgetItem* item1 = tableWidget_->item(row, 1);
		item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
	}
}

int32_t SubZoneWindow::getZoneNum()
{
	return zoneNum_;
}

QVector<QString> SubZoneWindow::getZoneNames()
{
	return zoneNames_;
}
