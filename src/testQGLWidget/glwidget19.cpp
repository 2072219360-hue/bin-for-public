#include <ctime>
#include "glwidget19.h"

static GLfloat colors[12][3] = {
    {1.0, 0.5, 0.5}, {1.0, 0.75, 0.5}, {1.0, 1.0, 0.5}, {0.75, 1.0, 0.5},
    {0.5, 1.0, 0.5}, {0.5, 1.0, 0.75}, {0.5, 1.0, 1.0}, {0.5, 0.75, 1.0},
    {0.5, 0.5, 1.0}, {0.75, 0.5, 1.0}, {1.0, 0.5, 1.0}, {1.0, 0.5, 0.75}
};

GLWidget19::GLWidget19(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    rainbow = true;
    sp = rp = false;
    slowdown = 2.0;
    xspeed = yspeed = 0.0;
    zoom = -40.0;
    col = 0;
    delay = 0;
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    setWindowTitle(tr("粒子系统"));
    QIcon icon(":/images/INV_Jewelry_Talisman_01.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget19::loadGLTextures()
{
    QImage tex, buf(":/images/Particle.bmp");
    tex = convertToGLFormat(buf);

    glGenTextures(1, &texture[0]);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void GLWidget19::initializeGL()
{
    loadGLTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);
    glDisable(GL_DEPTH_TEST); // 禁止深度测试

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    srand(time(NULL));
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = true;
        particles[i].life = 1.0;
        particles[i].fade = (rand() % 100 ) / 1000.0 + 0.003;
        particles[i].r = colors[int(i * (12.0 / MAX_PARTICLES))][0];
        particles[i].g = colors[int(i * (12.0 / MAX_PARTICLES))][1];
        particles[i].b = colors[int(i * (12.0 / MAX_PARTICLES))][2];
        particles[i].xi = (rand() % 50 - 26) * 10.0;
        particles[i].yi = (rand() % 50 - 25) * 10.0;
        particles[i].zi = (rand() % 50 - 25) * 10.0;
        particles[i].xg = 0.0;
        particles[i].yg = -0.8;
        particles[i].zg = 0.0;
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
}

void GLWidget19::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 0.0);

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            float x = particles[i].x;
            float y = particles[i].y;
            float z = particles[i].z + zoom;

            glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].life);

            glBegin(GL_TRIANGLE_STRIP); { // 绘制三角形带
                glTexCoord2d(1, 1); glVertex3f(x + 0.5, y + 0.5, z);
                glTexCoord2d(0, 1); glVertex3f(x - 0.5, y + 0.5, z);
                glTexCoord2d(1, 0); glVertex3f(x + 0.5, y - 0.5, z);
                glTexCoord2d(0, 0); glVertex3f(x - 0.5, y - 0.5, z);
            } glEnd();

            particles[i].x += particles[i].xi / (slowdown * 1000);
            particles[i].y += particles[i].yi / (slowdown * 1000);
            particles[i].z += particles[i].zi / (slowdown * 1000);

            particles[i].xi += particles[i].xg;
            particles[i].yi += particles[i].yg;
            particles[i].zi += particles[i].zg;

            particles[i].life -= particles[i].fade;

            if (particles[i].life < 0.0) {
                particles[i].life = 1.0;
                particles[i].fade = (rand() % 100) / 1000.0 + 0.003;
                particles[i].x = 0.0;
                particles[i].y = 0.0;
                particles[i].z = 0.0;
                particles[i].xi = xspeed + rand() % 60 - 32.0;
                particles[i].yi = yspeed + rand() % 60 - 30.0;
                particles[i].zi = rand() % 60 - 30.0;
                particles[i].r = colors[col][0];
                particles[i].g = colors[col][1];
                particles[i].b = colors[col][2];
            }
        }
    }
}

void GLWidget19::resizeGL(int width, int height)
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

void GLWidget19::timeOut()
{
    col = (col + 1) % 12;
    updateGL();
}

void GLWidget19::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_8:
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].yg < 1.5)
                particles[i].yg += 0.01;
        }
        updateGL();
        break;
    case Qt::Key_2:
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].yg > -1.5)
                particles[i].yg -= 0.01;
        }
        updateGL();
        break;
    case Qt::Key_6:
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].xg < 1.5)
                particles[i].xg += 0.01;
        }
        updateGL();
        break;
    case Qt::Key_4:
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].xg > -1.5)
                particles[i].xg -= 0.01;
        }
        updateGL();
        break;
    case Qt::Key_Plus:
        if (slowdown > 1.0)
            slowdown -= 0.01;
        updateGL();
        break;
    case Qt::Key_Minus:
        if (slowdown < 4.0)
            slowdown += 0.01;
        updateGL();
        break;
    case Qt::Key_PageUp:
        zoom += 0.1;
        updateGL();
        break;
    case Qt::Key_PageDown:
        zoom -= 0.1;
        updateGL();
        break;
    case Qt::Key_Return:
        rainbow = !rainbow;
        updateGL();
        break;
    case Qt::Key_Space:
        rainbow = false;
        delay = 0;
        col++;
        if (col > 11)
            col = 0;
        updateGL();
        break;
    case Qt::Key_Up:
        if  (yspeed < 200)
            yspeed += 1.0;
        updateGL();
        break;
    case Qt::Key_Down:
        if (yspeed > -200)
            yspeed -= 1.0;
        updateGL();
        break;
    case Qt::Key_Right:
        if (xspeed < 200)
            xspeed += 1.0;
        updateGL();
        break;
    case Qt::Key_Left:
        if (xspeed > -200)
            xspeed -= 1.0;
        updateGL();
        break;
    case Qt::Key_Tab:
        for (int i = 0; i < MAX_PARTICLES; i++) {
            particles[i].x = 0.0;
            particles[i].y = 0.0;
            particles[i].z = 0.0;;
            particles[i].xi = (rand() % 50 - 26) * 10.0;
            particles[i].yi = (rand() % 50 - 25) * 10.0;
            particles[i].zi = (rand() % 50 - 25) * 10.0;
        }
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

void GLWidget19::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget19::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget19::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
