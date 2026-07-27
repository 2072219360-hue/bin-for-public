#ifndef GLWIDGET8_H
#define GLWIDGET8_H

#include "glwidget.h"

class GLWidget8 : public GLWidget
{
public:
    GLWidget8(int timerInterval = 0, QWidget *parent = 0);
private:
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    GLfloat z;
    GLfloat drotx, droty;
    QPoint lastPos;

    GLfloat lightAmbient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightPosition[4];

    bool light, blend;
    GLuint filter;
    GLuint texture[3];
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void loadTextures();
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // GLWIDGET8_H
