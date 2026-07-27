#ifndef GLWIDGET4_H
#define GLWIDGET4_H

#include "glwidget.h"

class GLWidget4 : public GLWidget
{    
    // Q_OBJECT //! 注意：此处不能有Q_OBJECT    
public:
    GLWidget4(int timerInterval = 0, QWidget *parent = 0);

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

#endif // GLWIDGET4_H
