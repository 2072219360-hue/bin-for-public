  //-----------------------------------------------------------------
  //              simpleplot.cpp
  //
  //      A simple example which shows how to use SurfacePlot
  //-----------------------------------------------------------------

  #include <math.h>
  #include <qapplication.h>
  #include <qwt3d_surfaceplot.h>
  #include <qwt3d_function.h>

#include "qwtplot3d01simpleplot.h"
  

  using namespace Qwt3D;

  SimplePlot::SimplePlot()
  {
    setTitle("A Simple SurfacePlot Demonstration");
    
    Rosenbrock rosenbrock(*this);

    rosenbrock.setMesh(41,31);
    rosenbrock.setDomain(-1.73,1.5,-1.5,1.5);
    rosenbrock.setMinZ(-10);

    rosenbrock.create();

    setRotation(30,0,15);
    setScale(1,1,1);
    setShift(0.15,0,0);
    setZoom(0.2);

    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
      coordinates()->axes[i].setMajors(7);
      coordinates()->axes[i].setMinors(4);
    }


    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    //coordinates()->axes[Z1].setLabelString(QChar(0x38f)); // Omega - see http://www.unicode.org/charts/


    //setCoordinateStyle(BOX);
	setCoordinateStyle(FRAME);

    updateData();
    updateGL();
  }

  SimplePlotDialog::SimplePlotDialog()
  {
      QVBoxLayout *mainLayout = new QVBoxLayout;
      //mainLayout->setColumnStretch(0, 1);
      //mainLayout->setColumnStretch(1, 3);
	  mainLayout->addWidget(&plot);
	  this->resize(400,400);

	  setLayout(mainLayout);
	  return;
  }

  SimplePlotWidget::SimplePlotWidget(QWidget *parent)
  {
      QVBoxLayout *mainLayout = new QVBoxLayout;
      //mainLayout->setColumnStretch(0, 1);
      //mainLayout->setColumnStretch(1, 3);
	  mainLayout->addWidget(&plot);
	  //this->resize(400,600);

	  setLayout(mainLayout);
	  return;

  }


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
