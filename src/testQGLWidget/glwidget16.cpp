#include "glwidget16.h"

GLWidget16::GLWidget16(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    xspeed = yspeed = 0.0;
    light = false;
    filter = 0;
    fogFilter = 0;
    z = -5.0;

    GLfloat _lightAmbient[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat _lightDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat _lightPosition[4] = {0.0, 0.0, 2.0, 1.0};
    GLfloat _fogColor[4] = {0.5, 0.5, 0.5, 1.0};

    for (int i = 0; i < 4; i++) {
        lightAmbient[i] = _lightAmbient[i];
        lightDiffuse[i] = _lightDiffuse[i];
        lightPosition[i] = _lightPosition[i];
        fogColor[i] = _fogColor[i];
    }

    fogMode[0] = GL_EXP;
    fogMode[1] = GL_EXP2;
    fogMode[2] = GL_LINEAR;

    fullScreen = false;
    setWindowTitle(tr("雾"));
    QIcon icon(":/images/INV_Shield_02.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget16::loadGLTextures()
{
    QImage tex, buf(":/images/crate.bmp");
    tex = QGLWidget::convertToGLFormat(buf);

    glGenTextures(3, &texture[0]);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void GLWidget16::drawCube()
{
    GLfloat vertex_list[8][3] = {
        {-1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0},
        {-1.0, 1.0, 1.0},
        {-1.0, -1.0, -1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, 1.0, -1.0}
    };

    GLint vertex_index_list[6][4] = {
        {0, 1, 2, 3}, // front
        {4, 7, 6, 5}, // back
        {7, 3, 2, 6}, // top
        {4, 5, 1, 0}, // bottom
        {5, 6, 2, 1}, // right
        {4, 0, 3, 7} // left
    };

    static const GLfloat texCoord_list[4][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0}
    };

    static const GLint texCoord_index_list[6][4] = {
        {0, 1, 2, 3},
        {1, 2, 3, 0},
        {3, 0, 1, 2},
        {2, 3, 0, 1},
        {1, 2, 3, 0},
        {0, 1, 2, 3}
    };

    static const GLfloat normal_list[6][3] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0},
        {0.0, 1.0, 0.0},
        {0.0, -1.0, 0.0},
        {1.0, 0.0, 0.0},
        {-1.0, 0.0, 0.0}
    };

    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        glNormal3fv(normal_list[i]);
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texCoord_list[texCoord_index_list[i][j]]);
            glVertex3fv(vertex_list[vertex_index_list[i][j]]);
        }
    }
    glEnd();
}

void GLWidget16::initializeGL()
{
    loadGLTextures();

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH); // 启用阴影平滑

    glClearColor(0.5, 0.5, 0.5, 1.0); // 设置背景的颜色为雾气的颜色
    glFogi(GL_FOG_MODE, fogMode[fogFilter]);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35);
    glHint(GL_FOG_HINT, GL_DONT_CARE); // 设置系统如何计算雾气
    glFogf(GL_FOG_START, 1.0); // 雾气的开始位置
    glFogf(GL_FOG_END, 5.0);
    glEnable(GL_FOG);

    glClearDepth(1.0); // 设置深度缓存
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL); // 所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget16::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清楚屏幕和深度缓存
    glLoadIdentity(); // 重置当前的模型观察矩阵
    glTranslatef(0.0, 0.0, z);

    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture[filter]);
    drawCube(); // 绘制立方体

    xrot += xspeed;
    yrot += yspeed;
}

void GLWidget16::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget16::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        fullScreen = !fullScreen;
        if (fullScreen)
            showFullScreen();
        else
            showNormal();
        break;
    case Qt::Key_L:
        light = !light;
        if (!light)
            glDisable(GL_LIGHTING);
        else
            glEnable(GL_LIGHTING);
        updateGL();
        break;
    case Qt::Key_F:
        filter = (filter + 1) % 3;
        updateGL();
        break;
    case Qt::Key_PageUp:
        z -= 0.2;
        updateGL();
        break;
    case Qt::Key_PageDown:
        z += 0.2;
        updateGL();
        break;
    case Qt::Key_Up:
        xspeed -= 0.05;
        updateGL();
        break;
    case Qt::Key_Down:
        xspeed += 0.05;
        updateGL();
        break;
    case Qt::Key_Right:
        yspeed += 0.05;
        updateGL();
        break;
    case Qt::Key_Left:
        yspeed -= 0.05;
        updateGL();
        break;
    case Qt::Key_G:
        fogFilter = (fogFilter + 1) % 3;
        glFogi(GL_FOG_MODE, fogMode[fogFilter]);
        updateGL();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
    //updateGL();
}

void GLWidget16::timeOut()
{
    updateGL();
}

void GLWidget16::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget16::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget16::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
