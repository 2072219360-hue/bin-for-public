/*
2D图像文字
在这一课中，你将学会如何使用四边形纹理贴图把文字显示在屏幕上。
你将学会如何把256个不同的文字从一个256x256的纹理图像中分别提取出来，并为每一个文字创建一个显示列表，
接着创建一个输出函数来创建任意你希望的文字。
*/

#ifndef GLWIDGET17_H
#define GLWIDGET17_H

#include "glwidget.h"

class GLWidget17 : public GLWidget
{
public:
    GLWidget17(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures(); // 载入图片并转换成纹理
    void buildFont(); // 创建字符显示列表
    void killFont();
    void glPrint(GLint x, GLint y, const char *string, int set);
    void drawQuad();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLuint base; // 绘制字体的显示列表的开始位置
    GLuint texture[2]; // 保存字体纹理
    GLfloat cnt1, cnt2; // 字体移动计数器
    bool fullScreen;
    GLfloat xrot, yrot, zrot;
    GLfloat scaling;
    QPoint lastPos;
};

#endif // GLWIDGET17_H
