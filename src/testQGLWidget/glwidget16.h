/* fog */
#ifndef GLWIDGET16_H
#define GLWIDGET16_H

#include "glwidget.h"

class GLWidget16 : public GLWidget
{
public:
    GLWidget16(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void drawCube();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLfloat scaling;
    QPoint lastPos;
    GLfloat xrot, yrot, zrot; // 旋转量
    GLfloat xspeed, yspeed; // 旋转速度
    bool light; // 光源开关
    GLfloat z; // 深入屏幕的距离
    GLfloat lightAmbient[4]; // 环境光
    GLfloat lightDiffuse[4]; // 漫射光
    GLfloat lightPosition[4]; // 光源位置
    GLuint filter; // 滤波类型
    GLuint texture[3]; // 纹理的存储空间
    GLuint fogMode[3]; // 雾气模式
    GLuint fogFilter;
    GLfloat fogColor[4];
    bool fullScreen;
};

#endif // GLWIDGET16_H
