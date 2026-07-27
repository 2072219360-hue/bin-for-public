#ifndef EVENT_TICKER_H
#define EVENT_TICKER_H

#include "QtGui/QWidget"

class EventTicker:public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText)

public:
	EventTicker(QWidget *parent=0);

	void setText(const QString &newText);
	QString text() const {return myText;}
	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);

private:
	QString myText;
	int offset;
	int myTimerId;
};


#endif
