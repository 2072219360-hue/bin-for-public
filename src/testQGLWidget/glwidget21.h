/*
 线、反走样、计时、正投影和简单的声音。
 */

#ifndef GLWIDGET21_H
#define GLWIDGET21_H

#include "glwidget.h"

struct Object // 记录游戏中的对象
{
    int fx, fy; // 使移动变得平滑
    int x, y; // 当前游戏者的位置
    float spin; // 旋转方向
};

class GLWidget21 : public GLWidget
{
public:
    GLWidget21(int timeInterval = 0, QWidget *parent = 0);
protected:
    void resetObjects();
    void loadGLTextures();
    void buildFonts();
    void killFonts();
    void glPrint(GLint x, GLint y, int set, const char *fmt, ...);
    void movePlayer();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
private:
    bool vline[11][10]; // 保存垂直方向的11根线条中，每根线条中的10段是否被走过
    bool hline[10][11];
    bool filled; // 网格是否被填满
    bool gameOver; // 游戏是否结束
    bool antialiasing; // 是否启用反走样
    int delay; // 敌人的暂停时间
    int adjust; // 调整显示的速度
    int lives; // 玩家的生命
    int level; // 内部游戏的等级
    int level2; // 显示的游戏等级
    int stage; // 游戏的关卡
    Object player; // 玩家信息
    Object enemy[9]; // 最多9个敌人的信息
    Object hourglass; // 沙漏
    int steps[6]; // 用来调整显示的速度
    GLuint texture[2]; // 字符纹理
    GLuint base; // 字符显示列表的开始值
};

#endif // GLWIDGET21_H
