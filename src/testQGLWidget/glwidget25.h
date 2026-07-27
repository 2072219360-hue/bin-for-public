/*
变形和从文件中加载3D物体:
从文件加载3D模型，并且平滑的从一个模型变换为另一个模型。
 */

#ifndef GLWIDGET25_H
#define GLWIDGET25_H

#include "glwidget.h"

struct Vertex1
{
    float x, y, z;
};

struct Object1
{
    int verts; // 物体中顶点的个数
    Vertex1 *points; // 包含顶点数据的指针
};

class GLWidget25 : public GLWidget
{
public:
    GLWidget25(int timeInterval = 0, QWidget *parent = 0);
    ~GLWidget25();
protected:
    void allocateObject1(Object1 *o, int n);
    void freeObject1(Object1 *o);
    void readLine(FILE *f, char *string);
    void loadObject1(const char *fileName, Object1 *o);
    Vertex1 calculate(int i);
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
    GLfloat xrot, yrot, zrot; // 旋转角度
    GLfloat xspeed, yspeed, zspeed; // 旋转速度
    GLfloat cx, cy, cz; // 物体位置
    int key; // 物体标识符
    int step, steps; // 变换的步数
    bool morph; // 是否使用变形
    int maxVertices; // 最大定点数
    Object1 morph1, morph2, morph3, morph4;
    Object1 helper, *sour, *dest;
};

#endif // GLWIDGET25_H
