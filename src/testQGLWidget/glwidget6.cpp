#include "glwidget6.h"

GLWidget6::GLWidget6(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0f;

    setWindowTitle(tr("纹理映射"));
    QIcon icon(":/images/war5.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget6::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D); // 启用纹理映射
    glShadeModel(GL_SMOOTH); // 启用阴影平滑
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 黑色背景
    glClearDepth(1.0f); // 设置深度缓存
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // 真正精细的透视修正
}

void GLWidget6::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget6::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    glTranslatef(0.0f,0.0f,-6.0f); // 相对当前所在的屏幕位置移动
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    // Back face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    // Top face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, -1.0f);
    glEnd();

    // Bottom face
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    // Right face
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,  1.0f);
    glEnd();

    // Left face
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void GLWidget6::timeOut()
{
    xrot += 0.3f;
    yrot += 0.2f;
    zrot += 0.4f;

    updateGL();
}

void GLWidget6::loadGLTextures()
{
    QString images[6] = {"apple1.jpg", "apple.jpg", "FreeBSD.png","Hacker2.png", "nehe.bmp", "w2.jpg"};
    glGenTextures(6, &texture[0]); // 创建纹理
    for (int i = 0; i < 6; ++i) {
        QImage t;
        //! 注意: 图片的宽度和高度必须为2的N次方           
        QImage b(":/images/" + images[i]);
        t = QGLWidget::convertToGLFormat(b);

        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 线性滤波
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void GLWidget6::keyPressEvent(QKeyEvent *e)
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

void GLWidget6::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget6::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget6::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
