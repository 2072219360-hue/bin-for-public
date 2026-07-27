#ifndef NEHE09WIDGET_H
#define NEHE09WIDGET_H

#include <qgl.h>
#include <qtimer.h>

#include "QtGui"

const GLuint num = 50;

typedef struct
{
  int r, g, b;
  GLfloat dist;
  GLfloat angle;
}stars;

class NeHe09Widget : public QGLWidget
{
    Q_OBJECT

public:

  NeHe09Widget( QWidget* parent = 0, bool fs = false );
  ~NeHe09Widget();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

  void keyPressEvent( QKeyEvent *e );
  void loadGLTextures();
  void timerEvent( QTimerEvent * );

protected:

  bool fullscreen;

  GLfloat xRot, yRot, zRot;
  GLfloat zoom;
  GLfloat tilt;
  GLfloat spin;
  GLuint loop;
  GLuint texture[1];

  bool twinkle;

  stars star[num];
};


class NeHe09Dialog:public QDialog
{
public:
	NeHe09Dialog();

private:
	NeHe09Widget *nh;
};

#endif // NEHEWIDGET_H
