#ifndef GLWIDGET12_H
#define GLWIDGET12_H

#include "glwidget.h"

class GLWidget12 : public GLWidget
{
public:
    GLWidget12(int timeInterval = 0, QWidget *parent = 0);
protected:
    void buildLists();
    void loadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLuint texture[1];
    GLuint box;
    GLuint top;
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    QPoint lastPos;
};

#endif // GLWIDGET12_H
