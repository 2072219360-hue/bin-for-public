#include "glwidget27.h"

GLWidget27::GLWidget27(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    xrot = yrot = zrot = xspeed = yspeed = 0.0;
    float _lightPosition[4] = {0.0, 5.0, -4.0, 1.0};
    float _lightAmbient[4] = {0.2, 0.2, 0.2, 1.0};
    float _lightDiffuse[4] = {0.6, 0.6, 0.6, 1.0};
    float _lightSpecular[4] = {-0.2, -0.2, -0.2, 1.0};
    float _materialAmbient[4] = {0.4, 0.4, 0.4, 1.0};
    float _materialDiffuse[4] = {0.2, 0.6, 0.9, 1.0};
    float _materialSpecular[4] = {0.0, 0.0, 0.0, 1.0};
    float _materialShininess[1] = {0.0};
    float _objPos[3] = {-2.0, -2.0, -5.0};
    float _spherePos[3] = {-4.0, -5.0, -6.0};
    for (int i = 0; i < 4; i++) {
        lightPosition[i] = _lightPosition[i];
        lightAmbient[i] = _lightAmbient[i];
        lightDiffuse[i] = _lightDiffuse[i];
        lightSpecular[i] = _lightSpecular[i];
        materialAmbient[i] = _materialAmbient[i];
        materialDiffuse[i] = _materialDiffuse[i];
        materialSpecular[i] = _materialSpecular[i];
        materialShininess[i] = _materialShininess[i];
        if (i < 3) {
            objPos[i] = _objPos[i];
            spherePos[i] = _spherePos[i];
        }
    }

    //QString str = QDir::currentPath();

    scaling = 1.0;
    setWindowTitle(tr("影子"));
    QIcon icon(":/images/INV_Sword_09.jpg");
    setWindowIcon(icon);
    showMaximized();
}

GLWidget27::~GLWidget27()
{

}

void GLWidget27::initializeGL()
{
    initGLObjects();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    glCullFace(GL_BACK); // 剔除背面以加快速度
    glEnable(GL_CULL_FACE); // 启用剔除
    glClearColor(0.1, 1.0, 0.5, 1.0);

    q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);
    gluQuadricTexture(q, GL_FALSE);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget27::paintGL()
{
    GLmatrix16f Minv;
    GLvector4f lp;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -20.0);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glTranslatef(spherePos[0], spherePos[1], spherePos[2]);
    gluSphere(q, 1.5, 32, 32);
    glLoadIdentity();
    glRotatef(-yrot, 0.0, 1.0, 0.0);
    glRotatef(-xrot, 1.0, 0.0, 0.0);
    glTranslatef(-objPos[0], -objPos[1], -objPos[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, Minv); // 计算从世界坐标系变化到物体坐标系中的坐标

    for (int i = 0; i < 4; i++)
        lp[i] = lightPosition[i];
    VMatMult(Minv, lp);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -20.0);
    drawGLRoom();

    glTranslatef(objPos[0], objPos[1], objPos[2]);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    drawGLObject(obj);
    castShadow(&obj, lp);
    glColor4f(0.7, 0.4, 0.0, 1.0);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);;
    glTranslatef(lp[0], lp[1], lp[2]);
    gluSphere(q, 0.2, 16, 16);
    glEnable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
    xrot += xspeed;
    yrot += yspeed;
}

void GLWidget27::resizeGL(int width, int height)
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

void GLWidget27::VMatMult(GLmatrix16f M, GLvector4f v)
{
    GLfloat result[4];
    for (int i = 0; i < 4; i++) {
        result[i] = M[i] * v[0] + M[i + 4] * v[1] + M[i + 8] * v[2] + M[i + 12] * v[3];
    }
    for (int j = 0; j < 4; j++) {
        v[j] = result[j];
    }
}

int GLWidget27::initGLObjects()
{
    if (!readObject("data/Object2.txt", &obj))
        return false;

    setConnectivity(&obj); // 设置相邻顶点的信息

    for (unsigned int i = 0; i < obj.nPlanes; i++)
        calculatePlane(obj, &obj.planes[i]); // 计算每个面的平面参数

    return true;
}

void GLWidget27::drawGLRoom()
{
    GLfloat vertex_list[8][3] = {{-10.0, -10.0, 20.0}, {10.0, -10.0, 20.0}, {10.0, 10.0, 20.0}, {-10.0, 10.0, 20.0},
                                 {-10.0, -10.0, -20.0}, {10.0, -10.0, -20.0}, {10.0, 10.0, -20.0}, {-10.0, 10.0, -20.0}};
    GLint vertex_index_list[6][4] = {{4, 0, 1, 5}, // 地面
                                     {3, 7, 6, 2}, // 天花板
                                     {7, 4, 5, 6}, // 前面
                                     {2, 1, 0, 3}, // 后面
                                     {3, 0, 4, 7}, // 左面
                                     {6, 5, 1, 2}}; // 右面
    GLfloat normal_list[6][3] = {{0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0},
                                 {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}};

    glBegin(GL_QUADS); {
        for (int i = 0; i < 6; i++) {
            glNormal3fv(normal_list[i]);
            for (int j = 0; j < 4; j++)
                glVertex3fv(vertex_list[vertex_index_list[i][j]]);
        }
    } glEnd();
}

void GLWidget27::drawGLObject(glObject o)
{
    unsigned int i, j;
    glBegin(GL_TRIANGLES); {
        for (i = 0; i < o.nPlanes; i++) {
            for (j = 0; j < 3; j++) {
                glNormal3f(o.planes[i].normals[j].x, o.planes[i].normals[j].y, o.planes[i].normals[j].z);
                glVertex3f(o.points[o.planes[i].p[j]].x, o.points[o.planes[i].p[j]].y, o.points[o.planes[i].p[j]].z);
            }
        }
    } glEnd();
}

void GLWidget27::doShadowPass(glObject *o, float *lp)
{
    unsigned int i, j, k, jj, p1, p2;
    sPoint v1, v2;
    for (i = 0; i < o->nPlanes; i++) { // 对模型中的每一个面
        if (o->planes[i].visible) { // 如果面在灯光的前面
            for (j = 0; j < 3; j++) { // 对于被灯光照射的面的每一个相邻的面
                k = o->planes[i].neigh[j];
                if (!k || !o->planes[k - 1].visible) { // 如果面不存在，或不被灯光照射，那么这个边是边界
                    // 获取面的两个顶点
                    p1 = o->planes[i].p[j];
                    jj = (j + 1) % 3;
                    p2 = o->planes[i].p[jj];

                    // 计算边的顶点到灯光的方向，并放大100倍
                    v1.x = (o->points[p1].x - lp[0]) * 100;
                    v1.y = (o->points[p1].y - lp[1]) * 100;
                    v1.z = (o->points[p1].z - lp[2]) * 100;

                    v2.x = (o->points[p2].x - lp[0]) * 100;
                    v2.y = (o->points[p2].y - lp[1]) * 100;
                    v2.z = (o->points[p2].z - lp[2]) * 100;

                    // 绘制构成阴影体边界的面
                    glBegin(GL_TRIANGLE_STRIP); {
                        glVertex3f(o->points[p1].x, o->points[p1].y, o->points[p1].z);
                        glVertex3f(o->points[p1].x + v1.x, o->points[p1].y + v1.y, o->points[p1].z + v1.z);
                        glVertex3f(o->points[p2].x, o->points[p2].y, o->points[p2].z);
                        glVertex3f(o->points[p2].x + v2.x, o->points[p2].y + v2.y, o->points[p2].z + v2.z);
                    } glEnd();
                }
            }
        }
    }
}

void GLWidget27::castShadow(glObject *o, float *lp) // 绘制阴影
{
    float side;
    // 根据平面方程，设置那些在灯光的前方的面为可见
    for (unsigned int i = 0; i < o->nPlanes; i++) {
        side = o->planes[i].planeEq.a * lp[0] + o->planes[i].planeEq.b * lp[1] + o->planes[i].planeEq.c * lp[2] + o->planes[i].planeEq.d * lp[3];
        if (side > 0)
            o->planes[i].visible = true;
        else
            o->planes[i].visible = false;
    }
    glDisable(GL_LIGHTING);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // 关闭颜色缓存的写入
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE); // 关闭深度缓存的写入
    glEnable(GL_STENCIL_TEST); // 使用蒙板缓存
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);

    // 如果是逆时针（即面向视点）的多边形，通过了蒙板和深度测试，则把蒙板的值增加1
    glFrontFace(GL_CCW);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    doShadowPass(o, lp);

    // 如果是顺时针（即背向视点）的多边形，通过了蒙板和深度测试，则把蒙板的值减少1
    glFrontFace(GL_CW);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
    doShadowPass(o, lp);

    glFrontFace(GL_CCW);
    glColorMask(1, 1, 1, 1);;

    // 把阴影绘制上颜色
    glColor4f(0.0, 0.0, 0.0, 0.4);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glPushMatrix(); {
        glLoadIdentity();
        glBegin(GL_TRIANGLE_STRIP); {
            glVertex3f(-0.1, 0.1, -0.1);
            glVertex3f(-0.1, -0.1, -0.1);
            glVertex3f(0.1, 0.1, -0.1);
            glVertex3f(0.1, -0.1, -0.1);
        } glEnd();
    } glPopMatrix();

    glDisable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);
    glShadeModel(GL_SMOOTH);
}

void GLWidget27::loadGLTextures()
{
    QImage tex, buf(":/images/apple2.jpg");
    tex = convertToGLFormat(buf);
    glGenTextures(1, &texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void GLWidget27::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Left:
        yspeed -= 0.1;
        updateGL();
        break;
    case Qt::Key_Right:
        yspeed += 0.1;
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
    case Qt::Key_L:
        lightPosition[0] += 0.05;
        updateGL();
        break;
    case Qt::Key_J:
        lightPosition[0] -= 0.05;
        updateGL();
        break;
    case Qt::Key_I:
        lightPosition[1] += 0.05;
        updateGL();
        break;
    case Qt::Key_K:
        lightPosition[1] -= 0.05;
        updateGL();
        break;
    case Qt::Key_O:
        lightPosition[2] += 0.05;
        updateGL();
        break;
    case Qt::Key_U:
        lightPosition[2] -= 0.05;
        updateGL();
        break;
    case Qt::Key_6:
        objPos[0] += 0.1;
        updateGL();
        break;
    case Qt::Key_4:
        objPos[0] -= 0.1;
        updateGL();
        break;
    case Qt::Key_8:
        objPos[1] += 0.1;
        updateGL();
        break;
    case Qt::Key_5:
        objPos[1] -= 0.1;
        updateGL();
        break;
    case Qt::Key_9:
        objPos[2] += 0.1;
        updateGL();
        break;
    case Qt::Key_7:
        objPos[2] -= 0.1;
        updateGL();
        break;
    case Qt::Key_D:
        spherePos[0] += 0.1;
        updateGL();
        break;
    case Qt::Key_A:
        spherePos[0] -= 0.1;
        updateGL();
        break;
    case Qt::Key_W:
        spherePos[1] += 0.1;
        updateGL();
        break;
    case Qt::Key_S:
        spherePos[1] -= 0.1;
        updateGL();
        break;
    case Qt::Key_E:
        spherePos[2] += 0.1;
        updateGL();
        break;
    case Qt::Key_Q:
        spherePos[2] -= 0.1;
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

void GLWidget27::timeOut()
{
    updateGL();
}

void GLWidget27::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget27::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget27::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
