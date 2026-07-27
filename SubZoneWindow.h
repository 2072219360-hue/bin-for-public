#pragma once

#include <QWidget>
#include <inttypes.h>
#include <QString>
#include <QVector>
#include <QTableWidget>
#include <QStringList>
#include <QCheckBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "SignalInstance.h"
#define COLUMNCOUNT 3
class SubZoneWindow : public QWidget
{
	Q_OBJECT

public:
	SubZoneWindow(int32_t zoneNum, QVector<QString>zoneNames, QWidget *parent);
	~SubZoneWindow();
	void init();
	void initUi();
	void initConnection();
	void setZoneNum(int32_t zoneNum);
	void setZoneNames(QVector<QString>zoneNames);
	void update();
	int32_t getZoneNum();
	QVector<QString> getZoneNames();
	QPushButton* okBtn_;
private:
	int32_t zoneNum_;
	QVector<QString>zoneNames_;
	QTableWidget* tableWidget_;
	QStringList tableHeaderLabels_;
	QHBoxLayout* layoutBtn_;
	QVBoxLayout* layoutMain_;
	QPushButton* closeBtn_;

};
