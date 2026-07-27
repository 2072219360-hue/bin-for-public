#ifndef LINECOLUMNWIDGET_H
#define LINECOLUMNWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
    class LineColumnWidget;
}

class LineColumnWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(LineColumnWidget)
public:
    explicit LineColumnWidget(QWidget *parent = 0);
    virtual ~LineColumnWidget();

	void setLine(int line);
	void setColumn(int column);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::LineColumnWidget *m_ui;
};

#endif // LINECOLUMNWIDGET_H
