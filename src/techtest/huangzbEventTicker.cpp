#include "QtGui/QtGui"

#include "huangzbEventTicker.h"

EventTicker::EventTicker(QWidget *parent):QWidget(parent)
{
	offset =0;
	myTimerId =0;
}

void EventTicker::setText(const QString &newText)
{
	myText = newText;
	update();
	updateGeometry();
}

QSize EventTicker::sizeHint() const
{
	return fontMetrics().size(0,text());
}

void EventTicker::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	int textWidth=fontMetrics().width(text());
	if (textWidth<1)
		return;
	int x=-offset;
	while(x<width())
	{
		painter.drawText(x,0,textWidth,height(),Qt::AlignLeft|Qt::AlignVCenter,text());
		x+=textWidth;
	}
}

void EventTicker::showEvent(QShowEvent *event)
{
	myTimerId=startTimer(30);
}

void EventTicker::timerEvent(QTimerEvent *event)
{
	if (event->timerId()==myTimerId)
	{
		++offset;
		if(offset>=fontMetrics().width(text()))
			offset=0;
		scroll(-1,0);
	}else
	{
		QWidget::timerEvent(event);
	}

}

void EventTicker::hideEvent(QHideEvent *event)
{
	killTimer(myTimerId);
	myTimerId=0;
}

