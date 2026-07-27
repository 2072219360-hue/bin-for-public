/*
蒙板:
到目前为止你已经学会如何使用alpha混合，把一个透明物体渲染到屏幕上了，
但有的使用它看起来并不是那么的复合你的心意。使用蒙板技术，将会按照你蒙板的位置精确的绘制。
 */

#ifndef GLWIDGET20_H
#define GLWIDGET20_H

#include "glwidget.h"

class GLWidget20 : public GLWidget
{
public:
    GLWidget20(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void drawQuads1();
    void drawQuads2();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
private:
    bool masking; // 是否使用“掩模”
    bool scene; // 绘制哪一个场景
    GLuint texture[5];
    GLfloat roll; // 滚动纹理
};

#endif // GLWIDGET20_H
