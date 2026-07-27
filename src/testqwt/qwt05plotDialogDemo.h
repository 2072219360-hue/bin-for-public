#ifndef _PLOT_DIALOG_DEMO_H_
#define _PLOT_DIALOG_DEMO_H_

#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <qwt_series_data.h>

#include "qwt05plot.h"

class QwtPlotCurve;
class QwtPlotIntervalCurve;

class PlotDialogDemo: public QWidget
{
    Q_OBJECT
public:
	PlotDialogDemo(QWidget *parent=0);

private:
	QWT05Plot *d_plot;

};

#endif
