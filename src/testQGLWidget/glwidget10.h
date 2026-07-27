#ifndef GLWIDGET10_H
#define GLWIDGET10_H

#include "glwidget.h"

const int textureNum = 6;

struct Vertex
{
    GLfloat x, y, z;
    GLfloat u, v; // 纹理坐标
};

struct Triangle
{
    Vertex vertices[3];
};

class GLWidget10 : public GLWidget
{
public:
    GLWidget10(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadTriangles();
    void loadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:    
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    QPoint lastPos;
    GLfloat heading;
    GLfloat xpos;
    GLfloat zpos;
    //GLfloat yrot; // 旋转角度
    GLfloat walkbias;
    GLfloat walkbiasangle;
    GLfloat lookupdown;
//    GLuint filter; // 滤波类型
    GLuint texture[textureNum];
    QList<Triangle> triangles;
    bool blend; // 是否混合
    bool lightEnabled; // 光源开关
    GLfloat lightAmbient[4]; // 环境光
    GLfloat lightDiffuse[4]; // 漫射光
    GLfloat lightPosition[4]; // 光源位置
};

#endif // GLWIDGET10_H
