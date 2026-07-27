#include <ctime>
#include "glwidget25.h"

GLWidget25::GLWidget25(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    xspeed = yspeed = zspeed = 0.0;
    cx = cy = 0.0;
    cz = -10.0;
    key = 1;
    step = 0;
    steps = 5000;
    morph = false;
    maxVertices = 10;

    setWindowTitle(tr("变形和从文件中加载3D物体"));
    QIcon icon(":/images/Spell_Fire_Incinerate.jpg");
    setWindowIcon(icon);
    showMaximized();
}

GLWidget25::~GLWidget25()
{
    freeObject1(&morph1);
    freeObject1(&morph2);
    freeObject1(&morph3);
    freeObject1(&morph4);
    freeObject1(&helper);
}

void GLWidget25::allocateObject1(Object1 *o, int n)
{
    o->points = (Vertex1 *)malloc(sizeof(Vertex1) * n);
}

void GLWidget25::freeObject1(Object1 *o)
{
    free(o->points);
}

void GLWidget25::readLine(FILE *f, char *string)
{
    do {
        fgets(string, 255, f);
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}

void GLWidget25::loadObject1(const char *fileName, Object1 *o)
{
    int ver; // 顶点个数
    float rx, ry, rz; // 模型位置
    FILE *fin;
    char oneLine[255];

    fin = fopen(fileName, "rt");
    readLine(fin, oneLine);
    sscanf(oneLine, "Vertices: %d\n", &ver);
    o->verts = ver;
    allocateObject1(o, ver);

    for (int i = 0; i < ver; i++) {
        readLine(fin, oneLine);
        sscanf(oneLine, "%f %f %f", &rx, &ry, &rz);
        o->points[i].x = rx;
        o->points[i].y = ry;
        o->points[i].z = rz;
    }
    fclose(fin);
    if (ver > maxVertices)
        maxVertices = ver;
}

Vertex1 GLWidget25::calculate(int i)
{
    Vertex1 v;
    v.x = (sour->points[i].x - dest->points[i].x) / steps;
    v.y = (sour->points[i].y - dest->points[i].y) / steps;
    v.z = (sour->points[i].z - dest->points[i].z) / steps;
    return v;
}

void GLWidget25::initializeGL()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    

    loadObject1("data/Sphere.txt", &morph1);
    loadObject1("data/Torus.txt", &morph2);
    loadObject1("data/Tube.txt", &morph3);
    allocateObject1(&morph4, 486);

    srand(time(NULL));
    for (int i = 0; i < 486; i++) {
        morph4.points[i].x = (rand() % 14000) / 1000.0 - 7;
        morph4.points[i].y = (rand() % 14000) / 1000.0 - 7;
        morph4.points[i].z = (rand() % 14000) / 1000.0 - 7;
    }
    loadObject1("data/Sphere.txt", &helper);
    sour = dest = &morph1;
}

void GLWidget25::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(cx, cy, cz);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

//    xrot += xspeed;
//    yrot += yspeed;
//    zrot += zspeed;

    GLfloat tx, ty, tz; // 顶点临时变量
    Vertex1 q; // 保存中间计算的临时顶点

    glPointSize(2);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS); {
        for (int i = 0; i < morph1.verts; i++) {
            if (morph)
                q = calculate(i); // 如果启用变形，则计算中间模型
            else
                q.x = q.y = q.z = 0;

            helper.points[i].x -= q.x;
            helper.points[i].y -= q.y;
            helper.points[i].z -= q.z;

            tx = helper.points[i].x;
            ty = helper.points[i].y;
            tz = helper.points[i].z;

            glColor3f(0.0, 1.0, 1.0);
            glPointSize(3);
            glVertex3f(tx, ty, tz); // 绘制顶点

            if (morph) {
                glColor3f(0.0, 0.5, 1.0);
                tx -= 2 * q.x;
                ty -= 2 * q.y;
                tz -= 2 * q.z;
                glVertex3f(tx, ty, tz);

                glColor3f(0.0, 0.0, 1.0);
                tx -= 2 * q.x;
                ty -= 2 * q.y;
                tz -= 2 * q.z;
                glVertex3f(tx, ty, tz);
            }
        }
        glPointSize(1);
    } glEnd();

    // 如果启用变形则把变形步数增加
    if (morph && step <= steps)
        step++;
    else {
        morph = false;
        sour = dest;
        step = 0;
    }
}

void GLWidget25::resizeGL(int width, int height)
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

void GLWidget25::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    case Qt::Key_PageUp:
        zspeed += 0.1;
        updateGL();
        break;
    case Qt::Key_PageDown:
        zspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Up:
        xspeed += 0.1;
        updateGL();
        break;
    case Qt::Key_Down:
        xspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Right:
        yspeed += 0.1;
        updateGL();
        break;
    case Qt::Key_Left:
        yspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Q:
        cz -= 0.1;
        updateGL();
        break;
    case Qt::Key_Z:
        cz += 0.1;
        updateGL();
        break;
    case Qt::Key_W:
        cy += 0.1;
        updateGL();
        break;
    case Qt::Key_S:
        cy -= 0.1;
        updateGL();
        break;
    case Qt::Key_D:
        cx += 0.1;
        updateGL();
        break;
    case Qt::Key_A:
        cx -= 0.1;
        updateGL();
        break;
    case Qt::Key_1:
        if (key != 1 && !morph) {
            key = 1;
            morph = true;
            dest = &morph1;
        }
        updateGL();
        break;
    case Qt::Key_2:
        if (key != 2 && !morph) {
            key = 2;
            morph = true;
            dest = &morph2;
        }
        updateGL();
        break;
    case Qt::Key_3:
        if (key != 3 && !morph) {
            key = 3;
            morph = true;
            dest = &morph3;
        }
        updateGL();
        break;
    case Qt::Key_4:
        if (key != 4 && !morph) {
            key = 4;
            morph = true;
            dest = &morph4;
        }
        updateGL();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget25::timeOut()
{
    //updateGL();
}

void GLWidget25::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget25::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget25::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
