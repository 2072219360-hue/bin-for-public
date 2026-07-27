#ifndef TEAPOTS_H
#define TEAPOTS_H

#include <QGLFramebufferObject>
#include <QGLWidget>

class TeapotsGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    TeapotsGLWidget(QWidget *parent = 0);
    ~TeapotsGLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void createGLTeapotObject();
    void drawTeapot(GLfloat x, GLfloat y, GLfloat ambientR,
                    GLfloat ambientG, GLfloat ambientB,
                    GLfloat diffuseR, GLfloat diffuseG,
                    GLfloat diffuseB, GLfloat specularR,
                    GLfloat specularG, GLfloat specularB,
                    GLfloat shininess);
    void drawTeapots();

    QGLFramebufferObject *fbObject;
    GLuint glTeapotObject;
    QPoint rubberBandCorner1;
    QPoint rubberBandCorner2;
    bool rubberBandIsShown;
};

#endif
