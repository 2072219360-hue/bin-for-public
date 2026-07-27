#include <cmath>
#include "glwidget17.h"

using namespace std;

GLWidget17::GLWidget17(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    cnt1 = cnt2 = 0.0;
    fullScreen = false;

    setWindowTitle(tr("2D图像文字"));
    QIcon icon(":/images/INV_Jewelry_Amulet_04.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget17::loadGLTextures()
{
    QImage texImg[2], buf1(":/images/font.bmp"), buf2(":/images/bumps.bmp");
//    buf2.scaled(256, 256);
    texImg[0] = QGLWidget::convertToGLFormat(buf1);
    texImg[1] = QGLWidget::convertToGLFormat(buf2);

    glGenTextures(2, &texture[0]);

    for (int i = 0; i < 2; i++) {
        // 生成所有纹理
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, texImg[i].width(), texImg[i].height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, texImg[i].bits());
    }
}

/* 创建字符显示列表 */
void GLWidget17::buildFont()
{
    float cx, cy; // 字符坐标
    base = glGenLists(256); // 创建256个显示列表
    glBindTexture(GL_TEXTURE_2D, texture[0]); // 选择字符图像

    for (int i = 0; i < 256; i++) {
        cx = (i % 16) / 16.0;
        cy = (i / 16) / 16.0;

        glNewList(base + i, GL_COMPILE); {
            glBegin(GL_QUADS); {
                glTexCoord2f(cx, 1 - cy - 0.0625); glVertex2i(0, 0);
                glTexCoord2f(cx + 0.0625, 1 - cy - 0.0625); glVertex2i(16, 0);
                glTexCoord2f(cx + 0.0625, 1 - cy); glVertex2i(16, 16);
                glTexCoord2f(cx, 1 - cy); glVertex2i(0, 16);
            } glEnd();
            glTranslated(10, 0, 0);
        } glEndList();
    }
}

void GLWidget17::killFont()
{
    glDeleteLists(base, 256);
}

void GLWidget17::glPrint(GLint x, GLint y, const char *string, int set)
{
    if (set >= 1)
        set = 1;
    else
        set = 0;

    glBindTexture(GL_TEXTURE_2D, texture[0]); // 绑定为字体纹理
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, 1024, 0, 768, -1, 1); // 设置正投影的可视区域
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslated(x, y, 0); // 把字符原点移动到(x, y)位置
    glListBase(base - 32 + (128 * set)); // 选择字符集
    glCallLists(strlen(string), GL_BYTE, string); // 把字符串写到屏幕

    glMatrixMode(GL_PROJECTION); // 选择投影矩阵
    glPopMatrix(); // 设置为保存的矩阵
    glMatrixMode(GL_MODELVIEW); // 选择模型矩阵
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void GLWidget17::drawQuad()
{
   //glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS); {
        glTexCoord2d(0.0, 0.0); glVertex2f(1.0, -1.0);
        glTexCoord2d(1.0, 0.0); glVertex2f(-1.0, -1.0);
        glTexCoord2d(1.0, 1.0); glVertex2f(-1.0, 1.0);
        glTexCoord2d(0.0, 1.0); glVertex2f(1.0, 1.0);
    } glEnd();
}

void GLWidget17::initializeGL()
{
    loadGLTextures(); // 载入纹理

    buildFont(); // 创建字符显示列表

    glEnable(GL_TEXTURE_2D); // 启用纹理映射
    glShadeModel(GL_SMOOTH); // 启用阴影平滑
    glClearColor(0.0, 0.0, 0.0, 0.5); //黑色背景

    glClearDepth(1.0); // 设置深度缓存
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 所作深度测试的类型

    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 设置混合系数
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // 告诉系统对透视进行修正
}

void GLWidget17::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵

    glBindTexture(GL_TEXTURE_2D, texture[1]); // 设置为图像纹理
    glTranslatef(0.0, 0.0, -4.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glRotatef(cnt1 * 30.0, 1.0, 1.0, 0.0);

    glDisable(GL_BLEND);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    drawQuad();

    glRotatef(90.0, 1.0, 1.0, 0.0);

    //glScalef(scaling, scaling, scaling);
    drawQuad();

    glEnable(GL_BLEND);
    glLoadIdentity();

    glColor3f(cos(cnt1), sin(cnt2), 1.0 - 0.5 * cos(cnt1 + cnt2));
    glPrint(int(280 + 250 * cos(cnt1)), int(235 + 200 * sin(cnt2)), "NeHe", 0);

    glColor3f(sin(cnt2), 1.0 - 0.5 * cos(cnt1 + cnt2), cos(cnt1));
    glPrint(int(280 + 230 * cos(cnt2)), int(235 + 200 * sin(cnt1)), "OpenGL", 1);

    glColor3f(0.0, 0.0, 1.0);
    glPrint(int(240 + 200 * cos((cnt1 + cnt2) / 5)), 2, "Giuseppe D'Agata", 0);
    glColor3f(1.0, 1.0, 1.0);
    glPrint(int(242 + 200 * cos((cnt1 + cnt2) / 5)), 2, "Giuseppe D'Agata", 0);

    glColor3f(cos(cnt1), sin(cnt2), 1.0 - 0.5 * cos(cnt1 + cnt2));
    glPrint(int(500 + 250 * cos(cnt1)), int(400 + 250 * sin(cnt1)), "CrazyWind comfanter@qq.com", 0);
}

void GLWidget17::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width >= height)
        gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
    else
        gluPerspective(45.0, (GLdouble)height / (GLdouble)width, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget17::timeOut()
{
    cnt1 += 0.01;
    cnt2 += 0.0081;
    updateGL();
}

void GLWidget17::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        fullScreen = !fullScreen;
        if (fullScreen)
            showFullScreen();
        else
            showNormal();
        break;
    }
}

void GLWidget17::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget17::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget17::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
