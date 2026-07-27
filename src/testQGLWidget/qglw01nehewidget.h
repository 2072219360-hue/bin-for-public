#ifndef NEHE01WIDGET_H
#define NEHE01WIDGET_H

#include <qgl.h>
#include <QKeyEvent>
#include "QtGui"

class NeHe01Widget : public QGLWidget
{
    Q_OBJECT

public:
    NeHe01Widget(QWidget *parent = 0, bool fs = false);
    ~NeHe01Widget();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

  void keyPressEvent( QKeyEvent *e );

protected:

  bool fullscreen;
  GLfloat rTri;
  GLfloat rQuad;

};

class NeHe01Dialog:public QDialog
{
public:
	NeHe01Dialog();

private:
	NeHe01Widget *nh;
};

#endif // NEHEWIDGET_H
