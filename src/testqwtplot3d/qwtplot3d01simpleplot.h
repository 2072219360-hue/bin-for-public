#ifndef QWTPLOT3D_01_SIMPLEPLOT_H
#define QWTPLOT3D_01_SIMPLEPLOT_H

//-----------------------------------------------------------------
  //              simpleplot.cpp
  //
  //      A simple example which shows how to use SurfacePlot
  //-----------------------------------------------------------------
#include <QDialog>
#include <QtGui>

#include <math.h>
#include <qapplication.h>
#include <qwt3d_surfaceplot.h>
#include <qwt3d_function.h>
  

  using namespace Qwt3D;

  class Rosenbrock : public Function
  {
  public:

    Rosenbrock(SurfacePlot& pw)
    :Function(pw)
    {
    }

    double operator()(double x, double y)
    {
      return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
    }
  };


  class SimplePlot : public SurfacePlot
  {
  public:
      SimplePlot();
  };

  class SimplePlotDialog:public QDialog
  {
  public:
	  SimplePlotDialog();
  private:
	  SimplePlot plot;
  };

  class SimplePlotWidget:public QWidget
  {
  public:
	  SimplePlotWidget(QWidget *parent=0);
  private:
	  SimplePlot plot;
  };

#if 0


  int main(int argc, char **argv)
  {
      QApplication a(argc, argv);
      Plot plot;
#if QT_VERSION < 0x040000
      a.setMainWidget(&plot);
#endif
      plot.resize(800,600);
      plot.show();
      return a.exec();
  }
#endif

#endif

