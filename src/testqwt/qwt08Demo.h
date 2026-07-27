#include <qmainwindow.h>

class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class QWT08Plot;
class QPolygon;


class QWT08Demo : public QWidget
{
    Q_OBJECT

public:
    QWT08Demo(QWidget *parent = 0);

private Q_SLOTS:
    void moved(const QPoint &);
    void selected(const QPolygon &);
    
#ifndef QT_NO_PRINTER
    void print();
#endif

    void exportDocument();
    void enableZoomMode(bool);

private:
    void showInfo(QString text = QString::null);

    QWT08Plot *d_plot;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};
