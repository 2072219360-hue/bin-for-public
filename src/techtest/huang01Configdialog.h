#ifndef CONFIG_DIALOG_H
#define CONFIG_DIALOG_H

#include "QtGui/QDialog"
#include "QtCore/QPointer"
#include "huangzbFader.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class ConfigDialog:public QWidget
{
	Q_OBJECT
public:
	ConfigDialog(QWidget *parent);

public slots:
	void changePage(QListWidgetItem *current, QListWidgetItem *previous);
	
private:
	QListWidget *contentsWidget;
	QStackedWidget *pagesWidget;
	QPointer<FaderWidget> faderWidget;

private slots:
	void faderInWidget(int index);
};
#endif