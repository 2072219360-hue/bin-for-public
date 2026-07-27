/*
剪裁平面，蒙板缓存和反射:
在这一课中你将学会如何创建镜面显示效果，它使用剪裁平面，蒙板缓存等OpenGL中一些高级的技巧。
 */
#ifndef GLWIDGET26_H
#define GLWIDGET26_H

#include "glwidget.h"

class GLWidget26 : public GLWidget
{
public:
    GLWidget26(int timeInterval = 0, QWidget *parent = 0);
    ~GLWidget26();
protected:
    void loadGLTextures();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void drawObject();
    void drawFloor();
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLfloat lightAmbient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightPosition[4];
    GLUquadricObj *q;
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    GLfloat xrotspeed, yrotspeed;
    GLfloat zoom;
    GLfloat height;
    QPoint lastPos;
    GLuint texture[3];
};

#endif // GLWIDGET26_H
