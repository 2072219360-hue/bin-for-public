#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "ui_findwidget.h"

class FindWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(FindWidget)
public:
	explicit FindWidget(QWidget * parent = 0);

signals:
	void prevClicked();
	void nextClicked();
	void searchedTextChanged(const QString & text);

private:
	Ui::FindWidget m_ui;
};

#endif // FINDWIDGET_H
