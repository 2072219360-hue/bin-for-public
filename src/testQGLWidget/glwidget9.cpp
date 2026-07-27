#include "gl/GLU.h"

#include "glwidget9.h"

GLWidget9::GLWidget9(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    twinkle = false;
    zoom = -15.0f;
    tilt = 90.0f;
    spin = 0.1f;

    setWindowTitle(tr("3D¿Õ¼äÖÐÒÆ¶¯Í¼Ïñ"));
    QIcon icon(":/images/Ability_BackStab.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget9::LoadGLTextures()
{
    QImage t, b(":/images/Particle.bmp");
    t = QGLWidget::convertToGLFormat(b);
    glGenTextures(1, &texture[0]); // create one texture
    // create linear filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
}

void GLWidget9::initializeGL()
{
    LoadGLTextures(); // jump to texture loading routine
    glEnable(GL_TEXTURE_2D); // enable texture mapping
    glShadeModel(GL_SMOOTH); // enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // black background
    glClearDepth(1.0f); // depth buffer setup
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // really nice perspective calculations
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // set the blending function for translucency
    glEnable(GL_BLEND); // enable blending

    for (int i = 0; i < num; i++) {
        stars[i].angle = 0.0f; // start all the stars at angle zero
        stars[i].dist = ((float)i / num) * 5.0f; // calculate distance from the center
        stars[i].r = rand() % 256; // give stasrs[i] a random red intensity
        stars[i].g = rand() % 256;
        stars[i].b = rand() % 256;
    }
}

void GLWidget9::resizeGL(int width, int height)
{
    height = height ? height : 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// here is where we do all the drawing
void GLWidget9::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen and the depth buffer
    glBindTexture(GL_TEXTURE_2D, texture[0]); // select our texture
    for (int loop = 0; loop < num; loop++) { // loop through all the stars
        glLoadIdentity(); // reset the view before we draw all the stars
        glTranslatef(0.0f, 0.0f, zoom); // zoom into the screen
        glRotatef(tilt, 1.0f, 0.0f, 0.0f); // tilt the view

        glRotatef(stars[loop].angle, 0.0f, 1.0f, 0.0f); // rotate to the current star angle
        glTranslatef(stars[loop].dist, 0.0f, 0.0f); // move forward on the X plane

        glRotatef(-stars[loop].angle, 0.0f, 1.0f, 0.0f); // cancel the current star angle
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f); // cancel the screen tilt

        if (twinkle) { // twinkling star enabled
            glColor4ub(stars[num - loop - 1].r, stars[num - loop - 1].g, stars[num - loop - 1].b, 255);

            glBegin(GL_QUADS); // begin drawing the textured quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
            glEnd(); // done drawing the textured quad
        }

        glRotatef(spin, 0.0f, 0.0f, 1.0f); // rotate the star on the Z axis
        glColor4ub(stars[loop].r, stars[loop].g, stars[loop].b, 255);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();
    }
}

void GLWidget9::timeOut()
{
    spin += 0.01f; // used to spin the star
    for (int loop = 0; loop < num; loop++) {
        stars[loop].angle += float(loop) / num; // change the angle of a star
        stars[loop].dist -= 0.01f; // change the distance of a star

        if (stars[loop].dist < 0.0f) { // Is the star in the middle yet?
            stars[loop].dist += 5.0f; // move the star 5 units from the center
            stars[loop].r = rand() % 256;
            stars[loop].g = rand() % 256;
            stars[loop].b = rand() % 256;
        }
    }
    updateGL();
}

void GLWidget9::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_T:
        twinkle = !twinkle;
        break;
    case Qt::Key_Up:
        tilt -= 0.1f;
        break;
    case Qt::Key_Down:
        tilt += 0.1f;
        break;
    case Qt::Key_PageDown:
        zoom -= 0.2f;
        break;
    case Qt::Key_PageUp:
        zoom += 0.2f;
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}
