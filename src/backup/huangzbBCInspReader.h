/****************************************************************************

 Copyright (C) 2002-2011 Zhidaofangyuan . All rights reserved.

*****************************************************************************/

#include <qglviewer.h>

class gridshowQGLViewer : public QGLViewer
{
protected :
  virtual void init();
  virtual void draw();
  virtual void drawBackground();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

  void loadImage();

private :
  float ratio, u_max, v_max;

  bool background_;
};


class gridshowQGLViewerDialog:public QDialog
{
public:
	gridshowQGLViewerDialog();
private:
	gridshowQGLViewer *v;
};

class gridshowQGLViewerWidget:public QWidget
{
public:
	gridshowQGLViewerWidget(QWidget *parent=0);
private:
	gridshowQGLViewer *v;
};


