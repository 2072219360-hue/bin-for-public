#include "glwidget2.h"

GLWidget2::GLWidget2(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    setWindowTitle(tr("第一个多边形"));
    //showMaximized();
    QIcon icon(":/images/war1.png");
    setWindowIcon(icon);
}

void GLWidget2::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget2::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    glTranslatef(-1.5f,0.0f,-5.0f); // 相对当前所在的屏幕位置移动
    glBegin(GL_TRIANGLES); // 开始绘制三角形
    glVertex3f( 0.0f, 1.0f, 0.0f); // 上顶点
    glVertex3f(-1.0f,-1.0f, 0.0f); // 左下顶点
    glVertex3f( 1.0f,-1.0f, 0.0f); // 右下顶点
    glEnd(); // 三角形绘制结束

    /* 使用顺时针次序画正方形(左上->右上->右下->左下), 采用顺时针绘制的
       是对象的后表面.
     */
    glTranslatef(3.0f,0.0f,0.0f); // 向x轴正方向移动3个单位
    glBegin(GL_QUADS); // 开始绘制四边形
    glVertex3f(-1.0f, 1.0f, 0.0f); // 左上顶点
    glVertex3f( 1.0f, 1.0f, 0.0f); // 右上顶点
    glVertex3f( 1.0f,-1.0f, 0.0f); // 右下顶点
    glVertex3f(-1.0f,-1.0f, 0.0f); // 左下顶点
    glEnd(); // 四边形绘制结束
}
