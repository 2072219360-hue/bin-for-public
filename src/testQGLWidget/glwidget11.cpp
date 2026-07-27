#include <ctime>
#include "glwidget11.h"

const float piover180 = 0.0174532925f;

GLWidget11::GLWidget11(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    wiggleCount = 0;
    xrot = yrot = zrot = 0;
    scaling = 1.0;

    setWindowTitle(tr("Æ®¶¯µÄÆìÖÄ"));
    QIcon icon(":/images/INV_Chest_Chain_14.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget11::loadGLTextures()
{
    //QImage tex, buf(":/images/tim.bmp");
    QImage tex, buf(":/images/apple2.jpg");
    tex = QGLWidget::convertToGLFormat(buf);

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void GLWidget11::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glPolygonMode(GL_BACK, GL_FILL); // back face is filled in
    //glPolygonMode(GL_FRONT, GL_LINE); // front face is drawn with lines
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int x = 0; x < 45; x++) {
        for (int y = 0; y < 45; y++) {
            points[x][y][0] = float((x / 5.0f) - 4.5f);
            points[x][y][1] = float((y / 5.0f) - 4.5f);
            points[x][y][2] = float(sin((x/ 5.0f) * 40.0f * piover180));
        }
    }

    GLint lightAmbient[4] = {rand() % 256, rand() % 256, rand() % 256, 255};
    GLint lightDiffuse[4] = {rand() % 256, rand() % 256, rand() % 256, 255};
    GLint lightPosition[4] = {-1, -1, 0, 1};

    glLightiv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightiv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightiv(GL_LIGHT0,GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
}

void GLWidget11::resizeGL(int width, int height)
{
    height = height ? height : 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, GLdouble(width) / GLdouble(height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget11::paintGL()
{
    float float_x, float_y, float_xb, float_yb; // used to break the flag into tiny quads

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen and depth buffer
    glLoadIdentity(); // reset the current matrix

    srand(time(NULL));
    glColor4ub(rand() % 256, rand() % 256, rand() % 256, 200);

    glTranslatef(0.0, 0.0, -12.0); // translate 12 units into the screen

    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture[0]); // select our texture

    glBegin(GL_QUADS);
    for (int x = 0; x < 44; x++) {
        for (int y = 0; y < 44; y++) {
            float_x = float(x) / 44.0;
            float_y = float(y) / 44.0;
            float_xb = float(x + 1) / 44.0;
            float_yb = float(y + 1) / 44.0;

            glTexCoord2f(float_x, float_y); // first texture coordinate(bottom left)
            glVertex3f(points[x][y][0], points[x][y][1], points[x][y][2]);
            glTexCoord2f(float_x, float_yb); // top left
            glVertex3f(points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2]);
            glTexCoord2f(float_xb, float_yb); // top right
            glVertex3f(points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2]);
            glTexCoord2f(float_xb, float_y); // bottom right
            glVertex3f(points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2]);
        }
    }
    glEnd();
}

void GLWidget11::timeOut()
{
    GLfloat hold; // temporarily holds a floating point value
    //if (wiggleCount == 2) { // used to slow down the wave(every 2nd frame only)
        for (int y = 0; y < 45; y++) {
            hold = points[0][y][2]; // store current value one left side of wave
            for (int x = 0; x < 44; x++) {
                points[x][y][2] = points[x + 1][y][2]; // current wave value equals value to the right
            }
            points[44][y][2] = hold; // last value becomes the far left stored value
        }
//        wiggleCount = 0;
  //  }
//    wiggleCount++;

    xrot += 0.3;
    yrot += 0.2;
    zrot += 0.4;

    updateGL();
}

void GLWidget11::keyPressEvent(QKeyEvent *e)
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

void GLWidget11::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget11::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget11::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
