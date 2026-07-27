/* 把一块纹理贴到另一块纹理上 */
#ifndef GLWIDGET29_H
#define GLWIDGET29_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include "glwidget.h"

struct TextureImage2
{
    int width;
    int height;
    int format; // 像素格式
    unsigned char *data; // 纹理数据
};

class GLWidget29 : public GLWidget
{
public:
    GLWidget29(int timeInterval = 0, QWidget *parent = 0);

protected:
    TextureImage2 *allocateTextureBuffer(GLint w, GLint h, GLint f);
    void releaseTextureBuffer(TextureImage2 *t);
    // 读取*.raw文件，并把图像文件上下翻转以符合OpenGL的使用格式。
    int readTextureData(const char *fileName, TextureImage2 *buffer);
    void buildTexture(TextureImage2 *tex);
    void Blit(TextureImage2 *src, TextureImage2 *dst, int src_xstart, int src_ystart,
              int src_width, int src_height, int dst_xstart, int dst_ystart, int blend, int alpha);
    void drawCube();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    GLfloat z;
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    GLuint texture[2];
    TextureImage2 *t1, *t2;
    QPoint lastPos;
};

#endif // GLWIDGET29_H
