#include "gl/GLU.h"
#include "glwidget8.h"

GLWidget8::GLWidget8(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    for (int i = 0; i < 4; ++i) {
        lightAmbient[i] = 0.5f;
        lightDiffuse[i] = 1.0f;
        lightPosition[i] = 0.0f;
    }
    lightAmbient[3] = 1.0f;
    lightPosition[2] = 2.0f;
    lightPosition[3] = 1.0f;

    scaling = 1.0;
    xrot = yrot = zrot = 0.0f;
    z = -5.0f;
    drotx = droty = 0.0f;
    filter = 0;
    light = true;
    blend = true;

    setWindowTitle(tr("»ìºÏ"));
    QIcon icon(":/images/war6.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget8::initializeGL()
{
    loadTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
}

void GLWidget8::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, z);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f), glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face 
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

void GLWidget8::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget8::loadTextures()
{
    QImage t, b(":/images/glass.bmp");
    /*
    if (!b.load("images/glass.bmp")) {
        b = QImage(16, 16, 32);
        b.fill(Qt::green.rgb());
    }
    */
    t = QGLWidget::convertToGLFormat(b);

    glGenTextures(3, &texture[0]);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, t.bits());

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, t.bits());

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, t.width(), t.height(), GL_RGBA,
                      GL_UNSIGNED_BYTE, t.bits());
}

void GLWidget8::timeOut()
{
        xrot += drotx;
        yrot += droty;
	updateGL();
}

void GLWidget8::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::Key_L:
		light = !light;
		if (light)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;

	case Qt::Key_B:
		blend = !blend;
		if (blend) { 
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		break;

	case Qt::Key_F:
		filter++;
		if (filter > 2)
			filter = 0;
		break;

	case Qt::Key_Left:
                droty -= 0.1f;
		break;

	case Qt::Key_Right:
                droty += 0.1f;
		break;

	case Qt::Key_Up:
                drotx -= 0.1f;
		break;

	case Qt::Key_Down:
                drotx += 0.1f;
		break;

	case Qt::Key_PageDown:
                z -= 0.2f;
		break;

	case Qt::Key_PageUp:
                z += 0.2f;
		break;
	}
}

void GLWidget8::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget8::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget8::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
