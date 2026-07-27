/*
 影子
 */
#ifndef GLWIDGET27_H
#define GLWIDGET27_H

#include "glwidget.h"

typedef float GLvector4f[4];
typedef float GLmatrix16f[16];

struct sPoint
{
    float x, y, z;
};

struct sPlaneEq
{
    float a, b, c, d; // 平面方程为ax+by+cz+d=0
};

struct sPlane // 描述一个模型表面的结构
{
    unsigned int p[3]; // 3个整型索引指定了模型中三角形的三个顶点
    sPoint normals[3]; // 第2个变量指定了三角形面的法线
    unsigned int neigh[3]; // 与本三角形三个边相邻的面的索引
    sPlaneEq planeEq; // 平面方程描述了三角形所在的平面
    bool visible; // 最后一个变量指出这个三角形是否投出阴影
};

struct glObject // 描述一个产生阴影的物体
{
    GLuint nPlanes, nPoints;
    sPoint points[100];
    sPlane planes[200];
};

class GLWidget27 : public GLWidget
{
public:
    GLWidget27(int timeInterval = 0, QWidget *parent = 0);
    ~GLWidget27();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void VMatMult(GLmatrix16f M, GLvector4f v);
    int initGLObjects();
    void drawGLRoom();
    void drawGLObject(glObject o);
    void doShadowPass(glObject *o, float *lp);
    void castShadow(glObject *o, float *lp);
    void loadGLTextures();
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    inline int readObject(const char *fileName, glObject *obj)
    {
        FILE *file;
        unsigned int i;
        file = fopen(fileName, "r");
        if (!file)
            return false;

        fscanf(file, "%d", &(obj->nPoints));
        for (i = 1; i <= obj->nPoints; i++) { // 读取顶点
//        for (i = 0; i < obj->nPoints; i++) { // 读取顶点
            fscanf(file, "%f", &(obj->points[i].x));
            fscanf(file, "%f", &(obj->points[i].y));
            fscanf(file, "%f", &(obj->points[i].z));
        }

        fscanf(file, "%d", &(obj->nPlanes));
        for (i = 0; i < obj->nPlanes; i++) {
            for (int j = 0; j < 3; j++) { // 读取三角形面
                fscanf(file, "%d", &(obj->planes[i].p[j]));
            }

            for (int k = 0; k < 3; k++) {
                fscanf(file, "%f", &(obj->planes[i].normals[k].x));
                fscanf(file, "%f", &(obj->planes[i].normals[k].y));
                fscanf(file, "%f", &(obj->planes[i].normals[k].z));
            }
        }
        return true;
    }

    inline void setConnectivity(glObject *obj) // 设置相邻顶点信息
    {
        unsigned int p1i, p2i, p1j, p2j;
        unsigned int P1i, P2i, P1j, P2j;
        unsigned int i, j, ki, kj;

        for (i = 0; i < obj->nPlanes - 1; i++) { // 对于模型中的每一个面A
            for (j = i + 1; j < obj->nPlanes; j++) { // 对于除了此面的其他面B
                for (ki = 0; ki < 3; ki++) { // 对于面A中的每一个相邻的顶点
                    if (!obj->planes[i].neigh[ki]) { // 如果这个相邻的顶点没有被设置
                        for (kj = 0; kj < 3; kj++) {
                            p1i = ki;
                            p1j = kj;
                            p2i = (ki + 1) % 3;
                            p2j = (kj + 1) % 3;

                            p1i = obj->planes[i].p[p1i];
                            p2i = obj->planes[i].p[p2i];
                            p1j = obj->planes[j].p[p1j];
                            p2j = obj->planes[j].p[p2j];

                            // 如果面A的边P1i->P1j和面B的边P2i->P2j为同一条边，则有下面的公式的P1i=P1j，并且P2i=P2j
                            P1i = ((p1i + p2i) - abs(p1i - p2i)) / 2;
                            P2i = ((p1i + p2i) + abs(p1i - p2i)) / 2;
                            P1j = ((p1j + p2j) - abs(p1j - p2j)) / 2;
                            P2j = ((p1j + p2j) + abs(p1j - p2j)) / 2;

                            // 记录与这个边相邻的面的索引
                            if ((P1i == P1j) && (P2i == P2j)) {
                                obj->planes[i].neigh[ki] = j + 1;
                                obj->planes[j].neigh[kj] = i + 1;
                            }
                        }
                    }
                }
            }
        }
    }

    inline void calculatePlane(glObject obj, sPlane *plane) // 计算平面方程的参数
    {
        sPoint v[4];
        int i;
        for (i = 0; i < 3; i++) {
            v[i + 1].x = obj.points[plane->p[i]].x;
            v[i + 1].y = obj.points[plane->p[i]].y;
            v[i + 1].z = obj.points[plane->p[i]].z;
        }

        plane->planeEq.a = v[1].y * (v[2].z - v[3].z) + v[2].y * (v[3].z - v[1].z) + v[3].y * (v[1].z - v[2].z);
        plane->planeEq.b = v[1].z * (v[2].x - v[3].x) + v[2].z * (v[3].x - v[1].x) + v[3].z * (v[1].x - v[2].x);
        plane->planeEq.c = v[1].x * (v[2].y - v[3].y) + v[2].x * (v[3].y - v[1].y) + v[3].x * (v[1].y - v[2].y);
        plane->planeEq.d = -(v[1].x * (v[2].y * v[3].z - v[3].y * v[2].z) + v[2].x * (v[3].y * v[1].z - v[1].y * v[3].z) + v[3].x * (v[1].y * v[2].z - v[2].y * v[1].z));
    }

private:
    glObject obj;
    GLfloat xrot, yrot, zrot;
    GLfloat xspeed, yspeed;
    float lightPosition[4];
    float lightAmbient[4];
    float lightDiffuse[4];
    float lightSpecular[4];
    float materialAmbient[4]; // 环境光的材质
    float materialDiffuse[4]; // 漫射光的材质
    float materialSpecular[4]; // 反射光的材质
    float materialShininess[4]; // 反射光亮度
    float objPos[3]; // 几何体位置
    GLUquadricObj *q; // 球体对象
    float spherePos[3]; // 球体位置
    GLuint texture[1];
    GLfloat scaling;
    QPoint lastPos;
};

#endif // GLWIDGET27_H
