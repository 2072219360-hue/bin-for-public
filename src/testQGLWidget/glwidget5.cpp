#include "glwidget5.h"

GLWidget5::GLWidget5(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    rtri = rquad = 0.0f;

    setWindowTitle(tr("3D空间"));
    QIcon icon(":/images/war4.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget5::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget5::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget5::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    glTranslatef(-1.5f,0.0f,-6.0f); // 相对当前所在的屏幕位置移动
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glRotatef(rtri, 0.0f, 1.0f, 0.0f); // 绕y轴旋转金字塔

    glBegin(GL_TRIANGLES); // 开始绘制金字塔的各个面

    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(0.0f, 1.0f, 0.0f); // 三角形的上顶点（前侧面）

    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(-1.0f, -1.0f, 1.0f); // 三角形的左下顶点（前侧面）

    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(1.0f, -1.0f, 1.0f); // 三角形的右下顶点（前侧面）

    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(0.0f, 1.0f, 0.0f); // 三角形的上顶点（右侧面）

    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(1.0f, -1.0f, 1.0f); // 三角形的左下顶点（右侧面）

    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(1.0f, -1.0f, -1.0f); // 三角形的右下顶点（右侧面）

    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(0.0f, 1.0f, 0.0f); // 三角形的上顶点（后侧面）

    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(1.0f, -1.0f, -1.0f); // 三角形的左下顶点（后侧面）

    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(-1.0f, -1.0f, -1.0f); // 三角形的右下顶点（后侧面）

    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(0.0f, 1.0f, 0.0f); // 三角形的上顶点（左侧面）

    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(-1.0f, -1.0f, -1.0f); // 三角形的左下顶点（左侧面）

    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(-1.0f, -1.0f, 1.0f); // 三角形的右下顶点（左侧面）

    glEnd();

    glLoadIdentity(); // 重置模型观察矩阵
    glTranslatef(1.5f, 0.0f, -6.0f); // 右移1.5单位，并移入屏幕6.0单位
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glRotatef(rquad, 1.0f, 1.0f, 1.0f);

    /* 开始画立方体，它由六个四边形组成，所有的四边形都以逆时针顺序绘制
       （右上角->左上角->左下角->右下角)
     */
    glBegin(GL_QUADS);

    glColor3f(0.5f, 0.5f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f); // 四边形的右上顶点（顶面）
    glColor3f(0.1f, 0.0f, 0.2f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // 四边形的左上顶点（顶面）
    glColor3f(0.2f, 0.2f, 0.3f);
    glVertex3f(-1.0f, 1.0f, 1.0f); // 四边形的左下顶点（顶面）
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(1.0f, 1.0f, 1.0f); // 四边形的右下顶点（顶面）

    glColor3f(1.0f, 0.5f, 0.0f); // 橙色
    glVertex3f(1.0f, -1.0f, 1.0f); // 四边形的右上顶点（底面）
    glColor3f(0.2f, 0.4f, 0.4f);
    glVertex3f(-1.0f, -1.0f, 1.0f); // 四边形的左上顶点（底面）
    glColor3f(0.3f, 0.4f, 0.1f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // 四边形的左下顶点（底面）
    glColor3f(0.5f, 0.2f, 0.4f);
    glVertex3f(1.0f, -1.0f, -1.0f); // 四边形的右下顶点（底面）

    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f(1.0f, 1.0f, 1.0f); // 四边形的右上顶点（前面）
    glColor3f(0.6f, 0.8f, 0.9f);
    glVertex3f(-1.0f, 1.0f, 1.0f); // 四边形的左上顶点（前面）
    glVertex3f(-1.0f, -1.0f, 1.0f); // 四边形的左下顶点（前面）
    glVertex3f(1.0f, -1.0f, 1.0f); // 四边形的右下顶点（前面）

    glColor3f(1.0f, 1.0f, 0.0f); // 黄色
    glVertex3f(1.0f, -1.0f, -1.0f); // 四边形的右上顶点（后面）
    glColor3f(0.6f, 0.2f, 0.1f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // 四边形的左上顶点（后面）
    glVertex3f(-1.0f, 1.0f, -1.0f); // 四边形的左下顶点（后面）
    glVertex3f(1.0f, 1.0f, -1.0f); // 四边形的右下顶点（后面）

    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f(-1.0f, 1.0f, 1.0f); // 四边形的右上顶点（左面）
    glColor3f(0.7f, 0.5f, 0.9f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // 四边形的左上顶点（左面）
    glVertex3f(-1.0f, -1.0f, -1.0f); // 四边形的左下顶点（左面）
    glVertex3f(-1.0f, -1.0f, 1.0f); // 四边形的右下顶点（左面）
/*
    glColor3f(1.0f, 0.0f, 1.0f); // 紫罗兰色
    glVertex3f(1.0f, 1.0f, -1.0f); // 四边形的右上顶点（右面）
    glVertex3f(1.0f, 1.0f, 1.0f); // 四边形的左上顶点（右面）
    glVertex3f(1.0f, -1.0f, 1.0f); // 四边形的左下顶点（右面）
    glVertex3f(1.0f, -1.0f, -1.0f); // 四边形的右下顶点（右面）
*/
    glEnd();
}

void GLWidget5::timeOut()
{
    rtri += 0.5f;
    rquad -= 0.25f;
    updateGL();
}

void GLWidget5::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget5::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget5::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat(e->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(e->y() - lastPos.y()) / height();
    if (e->buttons() & Qt::LeftButton) {
        xrot -= 180 * dy;
        yrot -= 180 * dx;
        updateGL();
    } else if (e->buttons() & Qt::RightButton) {
        xrot += 180 * dy;
        zrot += 180 * dx;
        updateGL();
    }
    lastPos = e->pos();
}

void GLWidget5::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
