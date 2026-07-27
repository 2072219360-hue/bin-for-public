#include "glwidget18.h"

GLWidget18::GLWidget18(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{    
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    xspeed = yspeed = 0.0;
    light = false;
    z = -5.0;

    GLfloat _lightAmbient[4] = {0.5, 0.5, 0.5, 1.0};
    GLfloat _lightDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat _lightPosition[4] = {0.0, 0.0, 2.0, 1.0};
    for (int i = 0; i < 4; i++) {
        lightAmbient[i] = _lightAmbient[i];
        lightDiffuse[i] = _lightDiffuse[i];
        lightPosition[i] = _lightPosition[i];
    }

    filter = 0;
    part1 = part2 = 0.0;
    p1 = 0;
    p2 = 1;
    object = 0;
    fullScreen = false;

//    setWindowTitle(QString::fromLocal8Bit("二次几何体"));
    setWindowTitle(tr("二次几何体"));
    QIcon icon(":/images/INV_Jewelry_Ring_26.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget18::drawCube()
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

void GLWidget18::loadGLTextures()
{
    QImage tex, buf(":/images/Wall.bmp");
    tex = convertToGLFormat(buf);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void GLWidget18::initializeGL()
{
    loadGLTextures();

    quadratic = gluNewQuadric(); // 创建二次几何体
    gluQuadricNormals(quadratic, GL_SMOOTH); // 使用平滑法线
    gluQuadricTexture(quadratic, GL_TRUE); // 使用纹理

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget18::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, z);

    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    switch (object) {
    case 0:
        drawCube(); // 绘制立方体
        break;
    case 1:
        glTranslatef(0.0, 0.0, -1.5);
        gluCylinder(quadratic, 1.0, 1.0, 3.0, 32, 32); // 绘制圆柱体
        break;
    case 2:
        gluDisk(quadratic, 0.5, 1.5, 32, 32); // 绘制圆盘
        break;
    case 3:
        gluSphere(quadratic, 1.5, 32, 32); // 绘制球
        break;
    case 4:
        glTranslatef(0.0, 0.0, -1.5);
        gluCylinder(quadratic, 1.0, 0.0, 3.0, 32, 32);
        break;
    case 5:
        part1 += p1;
        part2 += p2;

        if (part1 > 359) {
            p1 = 0;
            part1 = 0;
            p2 = 1;
            part2 = 0;
        }
        if (part2 > 359) {
            p1 = 1;
            p2 = 0;
        }

        gluPartialDisk(quadratic, 0.5, 1.5, 32, 32, part1, part2 - part1);
        break;
    }

    xrot += xspeed;
    yrot += yspeed;
}

void GLWidget18::resizeGL(int width, int height)
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

void GLWidget18::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_L:
        light = !light;
        if (light)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
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
        xspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Down:
        xspeed += 0.1;
        updateGL();
        break;
    case Qt::Key_Left:
        yspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Right:
        yspeed += 0.1;
        updateGL();
        break;
    case Qt::Key_Space:
        object = (object + 1) % 6;
        updateGL();
        break;
    case Qt::Key_F11:
        //fullScreen = !fullScreen;
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget18::timeOut()
{
    updateGL();
}

void GLWidget18::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget18::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget18::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
