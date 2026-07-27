#ifndef GLWIDGET3_H
#define GLWIDGET3_H

#include "glwidget.h"

class GLWidget3 : public GLWidget
{        
public:
    GLWidget3(int timerInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    QPoint lastPos;
};

#endif // GLWIDGET3_H
