#ifndef NEHE16WIDGET_H
#define NEHE16WIDGET_H

#include "QtGui"
#include <qgl.h>
#include <QKeyEvent>

class NeHe16Widget : public QGLWidget
{
    Q_OBJECT

public:

  NeHe16Widget( QWidget* parent = 0,  bool fs = false );
  ~NeHe16Widget();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

  void keyPressEvent( QKeyEvent *e );
  void loadGLTextures();

protected:

  bool fullscreen;

  GLfloat xRot, yRot, zRot;
  GLfloat zoom;
  GLfloat xSpeed, ySpeed;
  GLuint texture[3];
  GLuint filter;

  bool light;

  GLuint fogFilter;

};

class NeHe16Dialog:public QDialog
{
public:
	NeHe16Dialog();

private:
	NeHe16Widget *nh;
};


#endif // NEHEWIDGET_H
