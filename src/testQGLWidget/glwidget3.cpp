#include "glwidget3.h"

GLWidget3::GLWidget3(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;

    setWindowTitle(tr("添加颜色"));
    QIcon icon(":/images/war2.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget3::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget3::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget3::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    // 三角形采用平滑着色
    glTranslatef(-1.5f,0.0f,-6.0f); // 相对当前所在的屏幕位置移动
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES); // 开始绘制三角形
    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glVertex3f( 0.0f, 1.0f, 0.0f); // 上顶点
    glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    glVertex3f(-1.0f,-1.0f, 0.0f); // 左下顶点
    glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    glVertex3f( 1.0f,-1.0f, 0.0f); // 右下顶点
    glEnd(); // 三角形绘制结束

    /* 使用顺时针次序画正方形(左上->右上->右下->左下), 采用顺时针绘制的
       是对象的后表面.
     */
    // 四边形采用平面着色
    glTranslatef(3.0f,0.0f,0.0f); // 向x轴正方向移动3个单位
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glColor3f(0.5f, 0.5f, 1.0f);
    glBegin(GL_QUADS); // 开始绘制四边形
    glVertex3f(-1.0f, 1.0f, 0.0f); // 左上顶点
    glVertex3f( 1.0f, 1.0f, 0.0f); // 右上顶点
    glVertex3f( 1.0f,-1.0f, 0.0f); // 右下顶点
    glVertex3f(-1.0f,-1.0f, 0.0f); // 左下顶点
    glEnd(); // 四边形绘制结束
}

void GLWidget3::keyPressEvent(QKeyEvent *e)
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

void GLWidget3::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget3::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget3::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
