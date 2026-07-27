#include "glwidget12.h"

static GLfloat boxcol[5][3] = {
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {1.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0}
};

static GLfloat topcol[5][3] = {
    {0.5, 0.0, 0.0},
    {0.5, 0.25, 0.0},
    {0.5, 0.5, 0.0},
    {0.0, 0.5, 0.0},
    {0.0, 0.5, 0.5}
};

GLWidget12::GLWidget12(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;

    setWindowTitle(tr("œ‘ æ¡–±Ì"));
    QIcon icon(":/images/INV_Gauntlets_03.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget12::buildLists()
{
    static const GLfloat vertex_list[8][3] = {
        {-1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0},
        {-1.0, 1.0, 1.0},
        {-1.0, -1.0, -1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, 1.0, -1.0},
    };

    static const GLint vertex_index_list[6][4] = {
        {4, 5, 1, 0}, // bottom
        {0, 1, 2,  3}, // front
        {4, 7, 6, 5}, // back
        {5, 6, 2, 1}, // right
        {4, 0, 3, 7}, // left
        {7, 6, 2, 3} // top
    };

    static const GLfloat texCoord_list[4][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0}
    };

    static const GLint texCoord_index_list[6][4] = {
        {2, 3, 0, 1},
        {0, 1, 2, 3},
        {1, 2, 3, 0},
        {1, 2, 3, 0},
        {0, 1, 2, 3},
        {3, 0, 1, 2}
    };

    static const GLfloat normal_list[6][3] = {
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0},
        {1.0, 0.0, 0.0},
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}
    };

    box = glGenLists(2);
    glNewList(box, GL_COMPILE);
    glBegin(GL_QUADS);
    for (int i = 0; i < 5; i++) {
        glNormal3fv(normal_list[i]);
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texCoord_list[texCoord_index_list[i][j]]);
            glVertex3fv(vertex_list[vertex_index_list[i][j]]);
        }
    }
    glEnd();
    glEndList();

    top = box + 1;
    glNewList(top, GL_COMPILE);
    glBegin(GL_QUADS);
    glNormal3fv(normal_list[5]);
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(texCoord_list[texCoord_index_list[5][i]]);
        glVertex3fv(vertex_list[vertex_index_list[5][i]]);
    }
    glEnd();
    glEndList();
}

void GLWidget12::loadGLTextures()
{
    QImage tex, buf(":/images/cube.bmp");
    tex = QGLWidget::convertToGLFormat(buf);

    glGenTextures(1, &texture[0]);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void GLWidget12::initializeGL()
{
    loadGLTextures();
    buildLists();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget12::resizeGL(int width, int height)
{
    //height = height ? height : 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, GLdouble(width) / GLdouble(height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget12::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    GLuint xloop, yloop;
    for (yloop = 1; yloop < 6; yloop++) {
        for (xloop =  0; xloop < yloop; xloop++) {
            glLoadIdentity();
            glTranslatef(1.4 + xloop * 2.8 - yloop * 1.4, (6.0 - yloop) * 2.4 - 7.0, -20.0);
            glScalef(scaling, scaling, scaling);
            glRotatef(45.0 - yloop * 2.0 + xrot, 1.0, 0.0, 0.0);
            glRotatef(45.0 + yrot, 0.0, 1.0, 0.0);
            glColor3fv(boxcol[yloop - 1]);
            glCallList(box);
            glColor3fv(topcol[yloop - 1]);
            glCallList(top);
        }
    }
}

void GLWidget12::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F:
        showFullScreen();
        break;
    case Qt::Key_Escape:
        showNormal();
        break;
    case Qt::Key_Up:
        xrot -= 1.5;
        updateGL();
        break;
    case Qt::Key_Down:
        xrot += 1.5;
        updateGL();
        break;
    case Qt::Key_Left:
        yrot -= 1.5;
        updateGL();
        break;
    case Qt::Key_Right:
        yrot += 1.5;
        updateGL();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget12::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget12::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget12::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
