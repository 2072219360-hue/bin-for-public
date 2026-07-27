
#ifndef QWT01_CURVDEMO_H
#define QWT01_CURVDEMO_H

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
const int Size = 27;
const int CurvCnt = 6;


class QWT01DemoMainWin : public QDialog 
{
public:
    QWT01DemoMainWin(QWidget *parent=0);
    
protected:
    virtual void paintEvent(QPaintEvent *);
    void drawContents(QPainter *p);

private:
    void shiftDown(QRect &rect, int offset) const;

    QwtPlotCurve d_curves[CurvCnt];
};


#endif
