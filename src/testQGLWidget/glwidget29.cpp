#include "glwidget29.h"

GLWidget29::GLWidget29(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{   
    z = -5.0;
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;

    setWindowTitle(tr("把一块纹理贴到另一块纹理上"));
    QIcon icon(":/images/INV_ThrowingKnife_04.jpg");
    setWindowIcon(icon);
    showMaximized();
}

TextureImage2 * GLWidget29::allocateTextureBuffer(GLint w, GLint h, GLint f)
{
    unsigned char *c = NULL;
    TextureImage2 *ti = (TextureImage2*)malloc(sizeof(TextureImage2));

    if (ti) {
        ti->width = w;
        ti->height = h;
        ti->format = f;
        c = (unsigned char*)malloc(w * h * f);
        if (c)
            ti->data = c;
        else {
            QMessageBox::warning(0, tr("分配内存失败!"), "内存不足!");
            return NULL;
        }
    } else {
        QMessageBox::warning(0, tr("内存不足"), tr("分配内存失败!"));
        return NULL;
    }
    return ti; // 返回指向图像数据的指针
}

void GLWidget29::releaseTextureBuffer(TextureImage2 *t)
{
    if (t) {
        if (t->data) {
            free(t->data);
            t->data = NULL;
        }
        free(t);
        t = NULL;
    }
}

int GLWidget29::readTextureData(const char *fileName, TextureImage2 *buffer)
{
    int i, j, k, done = 0;
    int stride = buffer->width * buffer->format; // 记录每一行的宽度，以字节为单位
    unsigned char *p = NULL;
    FILE *f = fopen(fileName, "rb");

    if (f) {
        for (i = buffer->height - 1; i >= 0; i--) {
            p = buffer->data + (i * stride);
            for (j = 0; j < buffer->width; j++) {
                for (k = 0; k < buffer->format - 1; k++, p++, done++) {
                    *p = fgetc(f);
                }
                *p = 255;
                p++;
            }
        }
        fclose(f);
    } else
        QMessageBox::warning(0, tr("Error!"), tr("打开文件%1失败！").arg(fileName));
    return done; // 返回读取的字节数
}

void GLWidget29::buildTexture(TextureImage2 *tex)
{
    glGenTextures(2, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);

    QImage tex2, buf(":/images/apple2.jpg");
    tex2 = convertToGLFormat(buf);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());
}

void GLWidget29::Blit(TextureImage2 *src, TextureImage2 *dst, int src_xstart, int src_ystart,
                      int src_width, int src_height, int dst_xstart, int dst_ystart, int blend, int alpha)
{
    int i, j, k;
    unsigned char *s, *d;
    alpha = qBound(0, alpha, 255);
    blend = qBound(0, blend, 1);

    d = dst->data + (dst_ystart * dst->width * dst->format); // 要复制的像素在目标图像数据中的开始位置
    s = src->data + (src_ystart * src->width * src->format);

    for (i = 0; i < src_height; i++) {
        s = s + (src_xstart * src->format); // 移动到下一个像素
        d = d + (dst_xstart * dst->format);

        for (j = 0; j < src_width; j++) { // 循环复制一行
            for (k = 0; k < src->format; k++, d++, s++) { // 复制每一个字节
                if (blend)
                    *d = ((*s * alpha) + (*d * (255 - alpha))) >> 8; // 根据混合复制颜色
                else
                    *d = *s;
            }
        }
        d = d + (dst->width - (src_width + dst_xstart)) * dst->format; // 移动到下一行
        s = s + (src->width - (src_width + src_xstart)) * src->format;
    }
}

void GLWidget29::drawCube()
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

void GLWidget29::initializeGL()
{
    t1 = allocateTextureBuffer(256, 256, 4);
    if (readTextureData("data/Monitor.raw", t1) == 0) {
        QMessageBox::warning(0, tr("Error!"), tr("Can't read file 'Monitor.raw'"));
        exit(-1);
    }
    t2 = allocateTextureBuffer(256, 256, 4);
    if (readTextureData("data/GL.raw", t2) == 0) {
        QMessageBox::warning(0, tr("Error!"), tr("Can't read file 'GL.raw'"));
        exit(-1);
    }

    Blit(t2, t1, 127, 127, 128, 128, 64, 64, 1, 127);
    buildTexture(t1);

    releaseTextureBuffer(t1);
    releaseTextureBuffer(t2);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void GLWidget29::resizeGL(int width, int height)
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

void GLWidget29::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, z);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    drawCube();
}

void GLWidget29::keyPressEvent(QKeyEvent *e)
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

void GLWidget29::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget29::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget29::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
