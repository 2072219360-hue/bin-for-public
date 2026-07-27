#ifndef DYN_LOADER_UI_TEST_H
#define DYN_LOADER_UI_TEST_H

#include "QtGui/QtGui"
#include "ui_first.h"
#include "ui_second.h"

class DynLoaderUiTest:public QDialog
{
	Q_OBJECT
public:
	DynLoaderUiTest(QWidget *parent=0);

public slots:
	void slotChild();

private:
	Ui::First firstUi;
	Ui::Second secondUi;
};


#endif
