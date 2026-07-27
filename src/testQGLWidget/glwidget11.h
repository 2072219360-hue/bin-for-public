#ifndef GLWIDGET11_H
#define GLWIDGET11_H

#include "glwidget.h"

class GLWidget11 : public GLWidget
{
public:
    GLWidget11(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    float points[45][45][3]; // the array for the points on the grid of our "wave"
    int wiggleCount; // counte used to control how fast flag waves
    GLfloat xrot, yrot, zrot;
    GLuint texture[1];
    QPoint lastPos;
    GLfloat scaling;
};

#endif // GLWIDGET11_H
