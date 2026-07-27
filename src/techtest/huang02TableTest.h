#ifndef TABLE_TEST_H
#define TABLE_TEST_H

#include "QtGui/QtGui"

class TableTest:public QTableWidget
{
	Q_OBJECT
public:
	TableTest(QWidget * parent = 0);
	~TableTest();
};

#endif
