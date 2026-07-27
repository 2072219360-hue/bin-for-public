#ifndef PLOTTERCC_H
#define PLOTTERCC_H

#include <QtCore/QMap>
#include <QtGui/QPixmap>
#include <QtCore/QVector>
#include <QtGui/QWidget>

class QToolButton;
class jambiPlotterSettings;

class jambiPlotter:public QWidget
{
	Q_OBJECT

public:
	jambiPlotter(QWidget *parent=0);

    void setPlotSettings(const jambiPlotterSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
	void zoomIn();
	void zoomOut();

protected:
	void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum { Margin = 50 };

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QMap<int, QVector<QPointF> > curveMap;
    QVector<jambiPlotterSettings> zoomStack;
    int curZoom;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;

};

class jambiPlotterSettings
{
public:
	jambiPlotterSettings();

	void scroll(int dx,int dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);

};


#endif
