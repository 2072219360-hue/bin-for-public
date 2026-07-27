#include "glwidget4.h"

GLWidget4::GLWidget4(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    rtri = rquad = 0.0f;

    setWindowTitle(tr("旋转"));
    QIcon icon(":/images/war3.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget4::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget4::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget4::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    // 三角形采用平滑着色
    glTranslatef(-1.5f,0.0f,-6.0f); // 相对当前所在的屏幕位置移动
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glRotatef(rtri, 0.0f, 1.0f, 0.0f); // 绕y轴从左向右旋转三角形
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
    glLoadIdentity(); // 重置模型观察矩阵
    glTranslatef(1.5f,0.0f,-6.0f); // 右移1.5单位，并移入屏幕6.0单位
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glRotatef(rquad, 1.0f, 0.0f, 0.0f);    
    glBegin(GL_QUADS); // 开始绘制四边形    
    glColor3f(0.5f, 0.5f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f); // 左上顶点
    glColor3f(0.1f, 0.2f, 0.3f);
    glVertex3f( 1.0f, 1.0f, 0.0f); // 右上顶点
    glColor3f(0.2f, 0.3f, 0.4f);
    glVertex3f( 1.0f,-1.0f, 0.0f); // 右下顶点
    glColor3f(0.4f, 0.1f, 0.3f);
    glVertex3f(-1.0f,-1.0f, 0.0f); // 左下顶点
    glEnd(); // 四边形绘制结束
}

void GLWidget4::timeOut()
{
    rtri += 0.5f;
    rquad -= 0.25f;
    updateGL();
}

void GLWidget4::keyPressEvent(QKeyEvent *e)
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

void GLWidget4::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget4::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget4::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
