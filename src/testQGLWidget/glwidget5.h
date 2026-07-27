#ifndef GLWIDGET5_H
#define GLWIDGET5_H

#include "glwidget.h"

class GLWidget5 : public GLWidget
{        
public:
    GLWidget5(int timerInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    virtual void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    GLfloat rtri, rquad;
    QPoint lastPos;
};

#endif // GLWIDGET5_H
