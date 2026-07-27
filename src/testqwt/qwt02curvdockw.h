
#ifndef QWT02_CURVDEMO_DOCKW_H
#define QWT02_CURVDEMO_DOCKW_H

#include <qwt_scale_map.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qframe.h>
#include <qdialog>

//------------------------------------------------------------
//      curvdemo1
//
//  This example program features some of the different
//  display styles of the QwtPlotCurve class
//------------------------------------------------------------
//
//   Array Sizes
//
const int Size02 = 27;
const int CurvCnt02 = 6;


class QWT01DemoDockW : public QWidget 
{
public:
    QWT01DemoDockW(QWidget *parent=0);
	double xval[Size02];
	double yval[Size02];
	QwtScaleMap xMap; 
	QwtScaleMap yMap;
   
protected:
    virtual void paintEvent(QPaintEvent *);
    void drawContents(QPainter *p);

private:
    void shiftDown(QRect &rect, int offset) const;

    QwtPlotCurve d_curves[CurvCnt02];
};


#endif
