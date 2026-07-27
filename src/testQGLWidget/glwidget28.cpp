#include "glwidget28.h"

GLWidget28::GLWidget28(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    xrot = yrot = zrot = 0.0;
    showCtrlPoints = true;
    divs = 7;
    scaling = 1.0;

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowTitle(tr("贝塞尔曲面"));
    QIcon icon(":/images/INV_Sword_25.jpg");
    setWindowIcon(icon);
    showMaximized();
}

Point3D GLWidget28::pointAdd(Point3D p, Point3D q)
{
    p.x += q.x;
    p.y += q.y;
    p.z += q.z;
    return p;
}

Point3D GLWidget28::pointTimes(double c, Point3D p)
{
    p.x *= c;
    p.y *= c;
    p.z *= c;
    return p;
}

Point3D GLWidget28::makePoint(double a, double b, double c)
{
    Point3D p;
    p.x = a;
    p.y = b;
    p.z = c;
    return p;
}

Point3D GLWidget28::Bernstein(float u, Point3D *p)
{
    Point3D a, b, c, d, r;
    a = pointTimes(pow(u, 3), p[0]);
    b = pointTimes(3 * pow(u, 2) * (1 - u), p[1]);
    c = pointTimes(3 * u * pow((1 - u), 2), p[2]);
    d = pointTimes(pow((1 - u), 3), p[3]);
    r = pointAdd(pointAdd(a, b), pointAdd(c, d));
    return r;
}

GLuint GLWidget28::genBezier(BezierPatch patch, int divs)
{
    int u = 0, v;
    float px, py, pyold;
    GLuint drawlist = glGenLists(1); // 创建显示列表
    Point3D temp[4];
    Point3D *last = (Point3D*)malloc(sizeof(Point3D) * (divs + 1)); // 根据每一条曲线的细分数，分配响应的内存

    if (patch.dlBPatch != 0) // 如果显示列表存在则删除
        glDeleteLists(patch.dlBPatch, 1);

    for (int i = 0; i < 4; i++) // 获取u方向的四个控制点
        temp[i] = patch.anchors[i][3];

    for (v = 0; v <= divs; v++) {
        px = float(v) / divs;
        last[v] = Bernstein(px, temp); // 求分割点的坐标
    }

    glNewList(drawlist, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, patch.texture);

    for (u = 1; u <= divs; u++) {
        py = float(u) / divs; // 计算v方向上细分点的参数
        pyold = (float(u) - 1.0) / divs; // 上一个v方向上细分点的参数
        for (int j = 0; j < 4; j++)
            temp[j] = Bernstein(py, patch.anchors[j]); // 计算每个细分点v方向上贝塞尔曲面的控制点

        glBegin(GL_TRIANGLE_STRIP); // 开始绘制三角形带
        for (v = 0; v <= divs; v++) {
            px = float(v) / divs; // 沿着u轴方向顺序绘制
            glTexCoord2f(pyold, px); // 设置纹理坐标
            glVertex3d(last[v].x, last[v].y, last[v].z);

            last[v] = Bernstein(px, temp); // 创建下一个顶点
            glTexCoord2f(py, px);
            glVertex3d(last[v].x, last[v].y, last[v].z);
        }
        glEnd();
    }
    glEndList();
    free(last);
    return drawlist;
}

void GLWidget28::initBezier()
{
    static const double points[][3] = {
        {-0.75, -0.75, -0.50}, {-0.25, -0.75,  0.00}, { 0.25, -0.75,  0.00}, { 0.75, -0.75, -0.50},
        {-0.75, -0.25, -0.75}, {-0.25, -0.25,  0.50}, { 0.25, -0.25,  0.50}, { 0.75, -0.25, -0.75},
        {-0.75,  0.25,  0.00}, {-0.25,  0.25, -0.50}, { 0.25,  0.25, -0.50}, { 0.75,  0.25,  0.00},
        {-0.75,  0.75, -0.50}, {-0.25,  0.75, -1.00}, { 0.25,  0.75, -1.00}, { 0.75,  0.75, -0.50}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            // 设置贝塞尔曲面的控制点
            myBezier.anchors[i][j] = makePoint(points[i * 4 + j][0], points[i * 4 + j][1], points[i * 4 + j][2]);
    }
    myBezier.dlBPatch = 0; // 默认的显示列表为0
}

void GLWidget28::loadGLTexture(GLuint *texPtr, const QString &imgName)
{
    glGenTextures(1, texPtr);
    QImage tex, buf(":/images/" + imgName);
    tex = convertToGLFormat(buf);

    glBindTexture(GL_TEXTURE_2D, *texPtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void GLWidget28::initializeGL()
{
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.05, 0.05, 0.05, 0.5);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    initBezier();
    loadGLTexture(&(myBezier.texture), "apple2.jpg");
    myBezier.dlBPatch = genBezier(myBezier, divs);
}

void GLWidget28::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width >= height)
        gluPerspective(45.0, GLdouble(width) / height, 0.1, 100.0);
    else
        gluPerspective(45.0, GLdouble(height) / width, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget28::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -4.0);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glCallList(myBezier.dlBPatch);

    if (showCtrlPoints) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0, 0.0, 0.0);
        int i, j;
        for (i = 0; i < 4; i++) {
            glBegin(GL_LINE_STRIP);
            for (j = 0; j < 4; j++)
                glVertex3d(myBezier.anchors[i][j].x, myBezier.anchors[i][j].y, myBezier.anchors[i][j].z);
            glEnd();
        }
        for (i = 0; i < 4; i++) {
            glBegin(GL_LINE_STRIP);
            for (j = 0; j < 4; j++)
                glVertex3d(myBezier.anchors[j][i].x, myBezier.anchors[j][i].y, myBezier.anchors[j][i].z);
            glEnd();
        }
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
    }
}

void GLWidget28::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Left:
        zrot -= 1.0;
        updateGL();
        break;
    case Qt::Key_Right:
        zrot += 1.0;
        updateGL();
        break;
    case Qt::Key_Up:
        xrot += 1.0;
        updateGL();
        break;
    case Qt::Key_Down:
        xrot -= 1.0;
        updateGL();
        break;
    case Qt::Key_PageUp:
        divs++;
        myBezier.dlBPatch = genBezier(myBezier, divs);
        updateGL();
        break;
    case Qt::Key_PageDown:
        if (divs > 1) {
            divs--;
            myBezier.dlBPatch = genBezier(myBezier, divs);
            updateGL();
        }
        break;
    case Qt::Key_Space:
        showCtrlPoints = !showCtrlPoints;
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

void GLWidget28::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget28::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget28::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
