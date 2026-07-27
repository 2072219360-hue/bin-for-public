#ifndef GLWIDGET9_H
#define GLWIDGET9_H

#include "glwidget.h"

const int num = 50; // number of stars to draw

struct Star
{
    int r, g, b; // Star color
    GLfloat dist; // Star distance from center
    GLfloat angle; // Star current angle
};

class GLWidget9 : public GLWidget
{
public:
    GLWidget9(int timeInterval = 0, QWidget *parent = 0);
protected:
    void LoadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
private:
    bool twinkle;
    Star stars[num];
    GLfloat zoom; // viewing distance away from Star
    GLfloat tilt; // tilt the view
    GLfloat spin; // spin twinkling Star
    GLuint texture[1]; // storage for one texture
};

#endif // GLWIDGET9_H
