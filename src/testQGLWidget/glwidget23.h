/*
 球面映射
 */
#ifndef GLWIDGET23_H
#define GLWIDGET23_H

#include "glwidget.h"

class GLWidget23 : public GLWidget
{
public:
    GLWidget23(int timeInterval = 0, QWidget *parent = 0);
protected:
    void drawCube();
    void loadGLTextures();
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
    GLuint texture[6]; // 三种纹理的存储空间
    int part1, part2; // 圆盘的起始、结束角度
    int p1, p2; // 增量
    GLUquadricObj *quadratic; // 二次几何体
    GLuint object; // 二次几何体标识符
    bool fullScreen;
};

#endif // GLWIDGET23_H
