#include "glwidget20.h"

GLWidget20::GLWidget20(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    masking = true;
    scene = true;
    roll = 0.0;
    //showFullScreen();
    setWindowTitle(tr("蒙板"));
    QIcon icon(":/images/INV_Misc_Book_06.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget20::loadGLTextures()
{
    QString imageNames[5] = {"Logo", "Mask1", "Image1", "Mask2", "Image2"};
    QImage tex, buf;

    glGenTextures(5, &texture[0]);

    for (int i = 0; i < 5; i++) {
        buf.load(":/images/" + imageNames[i] + ".bmp");
        tex = convertToGLFormat(buf);

        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    }
}

void GLWidget20::drawQuads1()
{
    glBegin(GL_QUADS); {
        GLfloat texCoord_list[4][2] = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
        GLfloat vertex_list[4][3] = {{-1.1, -1.1, 0.0}, {1.1, -1.1, 0.0}, {1.1, 1.1, 0.0}, {-1.1, 1.1, 0.0}};
        for (int i = 0; i < 4; i++) {
            glTexCoord2fv(texCoord_list[i]);
            glVertex3fv(vertex_list[i]);
        }
    } glEnd();
}

void GLWidget20::drawQuads2()
{
    GLfloat texCoord_list[4][2] = {{roll + 0.0, 0.0}, {roll + 4.0, 0.0}, {roll + 4.0, 4.0}, {roll + 0.0, 4.0}};
    GLfloat vertex_list[4][3] = {{-1.1, -1.1, 0.0}, {1.1, -1.1, 0.0}, {1.1, 1.1, 0.0}, {-1.1, 1.1, 0.0}};

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(texCoord_list[i]); glVertex3fv(vertex_list[i]);
    }
    glEnd();
}

void GLWidget20::initializeGL()
{
    loadGLTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget20::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -2.0);

    glBindTexture(GL_TEXTURE_2D, texture[0]); // 选择Logo纹理
    glBegin(GL_QUADS); {
        glTexCoord2f(0.0, -roll + 0.0); glVertex3f(-1.1, -1.1, 0.0);
        glTexCoord2f(3.0, -roll + 0.0); glVertex3f(1.1, -1.1, 0.0);
        glTexCoord2f(3.0, -roll + 3.0); glVertex3f(1.1, 1.1, 0.0);
        glTexCoord2f(0.0, -roll + 3.0); glVertex3f(-1.1, 1.1, 0.0);
    } glEnd();

    glEnable(GL_BLEND); // 启用混合
    glDisable(GL_DEPTH_TEST); // 禁止深度测试

    if (masking)
        glBlendFunc(GL_DST_COLOR, GL_ZERO); // 使用黑白“掩模”混合屏幕颜色

    if (scene) {
        glTranslatef(0.0, 0.0, -1.0);
        glRotatef(roll * 360.0, 0.0, 0.0, 1.0);

        if (masking) {
            glBindTexture(GL_TEXTURE_2D, texture[3]); // 选择第二个“掩模”纹理
            drawQuads1();
        }

        glBlendFunc(GL_ONE, GL_ONE);
        glBindTexture(GL_TEXTURE_2D, texture[4]); // 选择第二个纹理
        drawQuads1();
    } else {
        if (masking) {
            glBindTexture(GL_TEXTURE_2D, texture[1]); // 选择第一个“掩模“纹理
            drawQuads2();
        }

        glBlendFunc(GL_ONE, GL_ONE); // 把纹理1复制到屏幕
        glBindTexture(GL_TEXTURE_2D, texture[2]); // 选择第一个纹理
        drawQuads2();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    roll += 0.002;
    if (roll > 1.0)
        roll -= 1.0;
}

void GLWidget20::resizeGL(int width, int height)
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

void GLWidget20::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Space:
        scene = !scene;
        updateGL();
        break;
    case Qt::Key_M:
        masking = !masking;
        updateGL();
        break;
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

void GLWidget20::timeOut()
{
    updateGL();
}
