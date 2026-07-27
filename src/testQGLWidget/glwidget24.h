/*
扩展，剪裁和TGA图像文件的加载:
读取显卡支持的OpenGL的扩展，并在指定的剪裁区域把它显示出来。
 */
#ifndef GLWIDGET24_H
#define GLWIDGET24_H

#include "glwidget.h"

struct TextureImage // 创建加载TGA图像文件的结构
{
    GLubyte *imageData; // 图像数据指针
    GLuint bpp; // 每个数据所占的位数(必须为24或32)
    GLuint width, height; // 图像宽/高度
    GLuint texID; // 纹理的ID值
};

class GLWidget24 : public GLWidget
{
public:
    GLWidget24(int timeInterval = 0, QWidget *parent = 0);
protected:
    bool loadTGA(TextureImage *texture, const char *fileName);
    void buildFonts();
    void killFonts();
    void glPrint(GLint x, GLint y, int set, const char *fmt, ...);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
private:
    int scroll; // 用来滚动屏幕
    int maxTokens; // 保存扩展的个数
    int swidth, sheight; // 剪裁宽/高度
    GLuint base; // 字符显示列表的开始值
    TextureImage textures[1];
};

#endif // GLWIDGET24_H
