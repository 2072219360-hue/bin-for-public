/*
 粒子系统，用于创建爆炸、喷泉、流星之类的效果。
 */
#ifndef GLWIDGET19_H
#define GLWIDGET19_H

#include "glwidget.h"

#define MAX_PARTICLES 1000 // 最大粒子数目

struct Particle
{
    bool active; // 是否激活
    float life; // 粒子生命
    float fade; // 衰减速度
    float r, g, b; // 红、绿、蓝
    float x, y, z; // 位置
    float xi, yi, zi; // 方向
    float xg, yg, zg; // 加速度
};

class GLWidget19 : public GLWidget
{
public:
    GLWidget19(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    bool rainbow; // 是否为彩虹模式
    bool sp, rp; // 空格、回车键是否被按下
    float slowdown; // 减速粒子
    float xspeed, yspeed; // 速度
    float zoom;
    GLuint col; // 当前的颜色
    GLuint delay; // 彩虹效果延迟
    GLfloat scaling;
    QPoint lastPos;
    GLfloat xrot, yrot, zrot; // 旋转量
    Particle particles[MAX_PARTICLES];
    GLuint texture[1];
};

#endif // GLWIDGET19_H
