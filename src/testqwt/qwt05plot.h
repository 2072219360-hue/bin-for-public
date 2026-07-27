#ifndef _QWT05_PLOT_H_
#define _QWT05_PLOT_H_

#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <qwt_series_data.h>

class QwtPlotCurve;
class QwtPlotIntervalCurve;

class QWT05Plot: public QwtPlot
{
    Q_OBJECT

public:
    enum Mode
    {
        Bars,
        Tube
    };

    QWT05Plot( QWidget * = NULL );

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

private:
    void insertCurve( const QString &title,
        const QVector<QPointF> &, const QColor & );

    void insertErrorBars( const QString &title,
        const QVector<QwtIntervalSample> &,
        const QColor &color );


    QwtScaleDiv yearScaleDiv() const;

    QwtPlotIntervalCurve *d_intervalCurve;
    QwtPlotCurve *d_curve;
};

#endif
