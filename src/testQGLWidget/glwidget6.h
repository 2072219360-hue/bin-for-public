#ifndef GLWIDGET6_H
#define GLWIDGET6_H

#include "glwidget.h"

class GLWidget6 : public GLWidget
{        
public:
    GLWidget6(int timerInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    virtual void timeOut();
    void loadGLTextures();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:    
    GLfloat scaling;
    GLfloat xrot, yrot, zrot; // 旋转量
    GLuint texture[6]; // 存储6个纹理
    QPoint lastPos;
};

#endif // GLWIDGET6_H
