#ifndef _QWT06_SINUPLOT_H
#define _QWT06_SINUPLOT_H

#include <qapplication.h>
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <math.h>

//-----------------------------------------------------------------
//              simple.cpp
//
//      A simple example which shows how to use QwtPlot connected 
//      to a data class without any storage, calculating each values 
//      on the fly.
//-----------------------------------------------------------------

class FunctionData: public QwtSyntheticPointData
{
public:
    FunctionData(double(*y)(double)):
        QwtSyntheticPointData(100),
        d_y(y)
    {
    }

    virtual double y(double x) const
    {
        return d_y(x);
    }

private:
    double(*d_y)(double);
};

class PlotFunctionDemo : public QwtPlot
{
public:
    PlotFunctionDemo( QWidget *parent = NULL);

protected:
    virtual void resizeEvent( QResizeEvent * );

private:
    void populate();
    void updateGradient();
};

int initQWT06Sinusplot(QWidget *window);

#endif
