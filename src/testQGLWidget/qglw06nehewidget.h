#ifndef NEHE06WIDGET_H
#define NEHE06WIDGET_H


#include "QtGui"
#include <qgl.h>

class NeHe06Widget : public QGLWidget
{
     Q_OBJECT
public:

  NeHe06Widget( QWidget* parent = 0,  bool fs = false );
  ~NeHe06Widget();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
  void loadGLTextures();

  void keyPressEvent( QKeyEvent *e );

protected:

  bool fullscreen;
  GLfloat xRot, yRot, zRot;  
  GLfloat zoom;
  GLfloat xSpeed, ySpeed;
  GLuint texture[1];
};


class NeHe06Dialog:public QDialog
{
public:
	NeHe06Dialog();

private:
	NeHe06Widget *nh;
};


#endif // NEHEWIDGET_H
