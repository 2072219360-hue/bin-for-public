/* 贝塞尔曲面
 */

#ifndef GLWIDGET28_H
#define GLWIDGET28_H

#include "glwidget.h"

struct Point3D
{
    double x, y, z;
};

struct BezierPatch // 贝塞尔面片结构
{
    Point3D anchors[4][4]; // 有4x4网格组成
    GLuint dlBPatch; // 绘制面片的显示列表名称
    GLuint texture; // 面片的纹理
};

class GLWidget28 : public GLWidget
{
public:
    GLWidget28(int timeInterval = 0, QWidget *parent = 0);

protected:
    Point3D pointAdd(Point3D p, Point3D q); // 两个向量相加:p = p + q
    Point3D pointTimes(double c, Point3D p); // 向量和标量相乘:p = c * p
    Point3D makePoint(double a, double b, double c); // 创建一个3D向量
    Point3D Bernstein(float u, Point3D *p); // 计算贝塞尔方程的值，变量u的范围[0,1]
    GLuint genBezier(BezierPatch patch, int divs); // 生成贝塞尔曲面的显示列表
    void initBezier();
    void loadGLTexture(GLuint *texPtr, const QString &imgName);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    float xrot, yrot, zrot;
    int divs; // 细分精度，控制曲面的显示精度
    bool showCtrlPoints; // 是否显示控制点
    BezierPatch myBezier;
    GLfloat scaling;
    QPoint lastPos;
};

#endif // GLWIDGET28_H
