#ifndef QWT_03_CURVDEMO2_H
#define QWT_03_CURVDEMO2_H

#include <qframe.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_map.h>

class QWT03MainWinDockW : public QWidget 
{
public:
    enum { curveCount = 4 };

    QwtScaleMap xMap[curveCount];
    QwtScaleMap yMap[curveCount];
    QwtPlotCurve curve[curveCount];
	static const int Size = 15;
	static const int USize = 13;

	//
	//   Arrays holding the values
	//
	double xval[Size];
	double yval[Size];
	double zval[Size];
	double uval[USize];
	double vval[USize];

public:
    QWT03MainWinDockW(QWidget *parent=0);
    
protected:
    virtual void timerEvent(QTimerEvent *t);
    virtual void paintEvent(QPaintEvent *);
    virtual void drawContents(QPainter *);

private:
    void newValues();
};

#endif
