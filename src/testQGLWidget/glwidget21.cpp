#include <ctime>
#include "glwidget21.h"

GLWidget21::GLWidget21(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 10; j++) {
            vline[i][j] = false;
            hline[j][i] = false;
        }
    }
    filled = false;
    gameOver = false;
    antialiasing = true;
    delay = 0;
    adjust = 3;
    lives = 5;
    level = level2 = 1;
    stage = 1;

    steps[0] = 1; steps[1] = 2; steps[2] = 4; steps[3] = 5; steps[4] = 10; steps[5] = 20;

    player.x = 0;
    player.y = 0;
    player.fx = 0;
    player.fy = 0;

    srand(time(NULL));
    for (int i = 0; i < stage * level; i++) {
        enemy[i].x = 5 + rand() % 6;
        enemy[i].y = rand() % 11;
        enemy[i].fx = enemy[i].x * 60;
        enemy[i].fy = enemy[i].y * 40;
    }

    resetObjects();

    setWindowTitle(tr("线 反走样 计时 正投影 简单的声音"));

    //showMaximized();
    //resize(500, 500);
    QIcon icon(":/images/INV_Misc_Horn_02.jpg");
    setWindowIcon(icon);
    setFixedSize(640, 480);
}

void GLWidget21::resetObjects()
{
    srand(time(NULL));
    player.x = player.y = player.fx = player.fy = 0;
    for (int i = 0; i < stage * level; i++) {
        enemy[i].x = 5 + rand() % 6;
        enemy[i].y = rand() % 11;
        enemy[i].fx = enemy[i].x * 60;
        enemy[i].fy = enemy[i].y * 40;
    }
}

void GLWidget21::loadGLTextures()
{
    QString imgNames[2] = {"font", "Image"};
    glGenTextures(2, &texture[0]);
    QImage tex, buf;

    for (int i = 0; i < 2; i++) {
        buf.load(":/images/" + imgNames[i] + ".bmp");
        tex = convertToGLFormat(buf);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    }
}

void GLWidget21::buildFonts()
{
    base = glGenLists(256);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for (int i = 0; i < 256; i++) {
        float cx = (i % 16) / 16.0;
        float cy = (i / 16) / 16.0;
        glNewList(base + i, GL_COMPILE); {
            glBegin(GL_QUADS); {
                glTexCoord2f(cx, 1 - cy - 0.0625); glVertex2d(0, 16);
                glTexCoord2d(cx + 0.0625, 1 - cy - 0.0625); glVertex2d(16, 16);
                glTexCoord2f(cx + 0.0625, 1 - cy); glVertex2d(16, 0);
                glTexCoord2f(cx, 1 - cy); glVertex2d(0, 0);
            } glEnd();
            glTranslated(15, 0, 0);
        } glEndList();
    }
}

void GLWidget21::killFonts()
{
    glDeleteLists(base, 256);
}

void GLWidget21::glPrint(GLint x, GLint y, int set, const char *fmt...)
{
    char text[256];
    va_list ap;

    if (fmt == NULL)
        return;
    va_start(ap, fmt);
    vsprintf(text, fmt, ap);
    va_end(ap);

    if (set >= 1)
        set = 1;
    else
        set = 0;

    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslated(x, y, 0);
    glListBase(base - 32 + (set * 128));

    if (set == 0)
        glScalef(1.5, 2.0, 1.0);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glDisable(GL_TEXTURE_2D);
}

void GLWidget21::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget21::movePlayer()
{
    if (player.fx < player.x * 60)
        player.fx += steps[adjust];
    if (player.fx > player.x * 60)
        player.fx -= steps[adjust];
    if (player.fy < player.y * 40)
        player.fy += steps[adjust];
    if (player.fy > player.y * 40)
        player.fy -= steps[adjust];
}

void GLWidget21::initializeGL()
{
    loadGLTextures();
    buildFonts();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget21::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]); // 选择字符纹理
    glColor3f(1.0, 0.5, 1.0);
    glPrint(207, 24, 0, "GRID CRAZY");

    glColor3f(1.0, 1.0, 0.0);
    glPrint(20, 20, 1, "Level: %2i", level2);
    glPrint(20, 40, 1, "Stage: %2i", stage);

    if (gameOver) {
        glColor3ub(rand() % 256, rand() % 256, rand() % 256);
        glPrint(472, 20, 1, "GAME OVER");
        glPrint(456, 40, 1, "PRESS SPACE");
    }

    for (int i = 0; i < lives - 1; i++) { // 绘制玩家的剩余生命
        glLoadIdentity();
        glTranslatef(490 + i * 40.0, 40.0, 0.0); // 移动到屏幕右上角
        glRotatef(-player.spin, 0.0, 0.0, 1.0);
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES); {
            glVertex2d(-5, -5);
            glVertex2d(5, 5);
            glVertex2d(5, -5);
            glVertex2d(-5, 5);
        } glEnd();

        glRotatef(-player.spin * 0.5, 0.0, 0.0, 1.0);
        glColor3f(0.0, 0.75, 0.0);
        glBegin(GL_LINES); {
            glVertex2d(-7, 0);
            glVertex2d(7, 0);
            glVertex2d(0, -7);
            glVertex2d(0, 7);
        } glEnd();
    }

    filled = true;
    glLineWidth(2.0);
    glDisable(GL_LINE_SMOOTH);
    glLoadIdentity();

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            glColor3f(0.0, 0.5, 1.0);

            if (i < 10) {
                if (hline[i][j])
                    glColor3f(1.0, 1.0, 1.0);

                if (!hline[i][j])
                    filled = false;

                glBegin(GL_LINES); {
                    glVertex2d(20 + i * 60, 70 + j * 40);
                    glVertex2d(80 + i * 60, 70 + j * 40);
                } glEnd();
            }
            glColor3f(0.0, 0.5, 1.0);


            if (j < 10) {
                if (vline[i][j])
                    glColor3f(1.0, 1.0, 1.0);

                if (!vline[i][j])
                    filled = false;

                glBegin(GL_LINES); {
                    glVertex2d(20 + i * 60, 70 + j * 40);
                    glVertex2d(20 + i * 60, 110 + j * 40);
                } glEnd();
            }

            glEnable(GL_TEXTURE_2D);
            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            if ((i < 10) && (j < 10)) {
                if (hline[i][j] && hline[i][j + 1] && vline[i][j] && vline[i + 1][j]) {
                    glBegin(GL_QUADS); {
                        glTexCoord2f(i / 10.0 + 0.1, 1 - j / 10.0); glVertex2d(20 + i * 60 + 59, 70 + j * 40 + 1);
                        glTexCoord2f(i / 10.0, 1 - j / 10.0); glVertex2d(20 + i * 60 + 1, 70 + j * 40 + 1);
                        glTexCoord2f(i / 10.0, 1 - j / 10.0 - 0.1); glVertex2d(20 + i * 60 + 1, 70 + j * 40 + 39);
                        glTexCoord2f(i / 10.0 + 0.1, 1 - j / 10.0 - 0.1); glVertex2d(20 + i * 60 + 59, 70 + j * 40 + 39);
                    } glEnd();
                }
            }
            glDisable(GL_TEXTURE_2D);
        }
    }

    glLineWidth(1.0);
    if (antialiasing)
        glEnable(GL_LINE_SMOOTH);

    if (hourglass.fx == 1) {
        glLoadIdentity();
        glTranslatef(20.0 + hourglass.x * 60, 70.0 + hourglass.y * 40, 0.0);
        glRotatef(hourglass.spin, 0.0, 0.0, 1.0);
        srand(time(NULL));
        glColor3ub(rand() % 256, rand() % 256, rand() % 256);
        glBegin(GL_LINES); {
            glVertex2d(-5, -5);
            glVertex2d(5, 5);
            glVertex2d(5, -5);
            glVertex2d(-5, 5);
            glVertex2d(-5, 5);
            glVertex2d(5, 5);
            glVertex2d(-5, -5);
            glVertex2d(5, -5);
        } glEnd();
    }

    glLoadIdentity();
    glTranslatef(player.fx + 20.0, player.fy + 70.0, 0.0);
    glRotatef(player.spin, 0.0, 0.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_LINES); {
        glVertex2d(-5, -5);
        glVertex2d(5, 5);
        glVertex2d(5, -5);
        glVertex2d(-5, 5);
    } glEnd();

    glRotatef(player.spin * 0.5, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.75, 0.0);
    glBegin(GL_LINES); {
        glVertex2d(-7, 0);
        glVertex2d(7, 0);
        glVertex2d(0, -7);
        glVertex2d(0, 7);
    } glEnd();

    for (int i = 0; i < stage * level; i++) {
        glLoadIdentity();
        glTranslatef(enemy[i].fx + 20.0, enemy[i].fy + 70.0, 0.0);
        glColor3f(1.0, 0.5, 0.5);
        glBegin(GL_LINES); {
            glVertex2d(0, -7);
            glVertex2d(-7, 0);
            glVertex2d(-7, 0);
            glVertex2d(0, 7);
            glVertex2d(0, 7);
            glVertex2d(7, 0);
            glVertex2d(7, 0);
            glVertex2d(0, -7);
        } glEnd();
        glRotatef(enemy[i].spin, 0.0, 0.0, 1.0);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES); {
            glVertex2d(-7, -7);
            glVertex2d(7, 7);
            glVertex2d(-7, 7);
            glVertex2d(7, -7);
        } glEnd();
    }

    srand(time(NULL));
    if (!gameOver) {
        for (int i = 0; i < stage * level; i++) {
            if ((enemy[i].x < player.x) && (enemy[i].fy == enemy[i].y * 40))
                enemy[i].x++;
            if ((enemy[i].x > player.x) && (enemy[i].fy == enemy[i].y * 40))
                enemy[i].x--;
            if ((enemy[i].y < player.y) && (enemy[i].fx == enemy[i].x * 60))
                enemy[i].y++;
            if ((enemy[i].y > player.y) && (enemy[i].fx == enemy[i].x * 60))
                enemy[i].y--;

            if (delay > (3 - level) && hourglass.fx != 2) {
                delay =0;
                for (int j = 0; j < stage * level; j++) {
                    if (enemy[j].fx < enemy[j].x * 60) {
                        enemy[j].fx += steps[adjust];
                        enemy[j].spin += steps[adjust];
                    }
                    if (enemy[j].fx > enemy[j].x * 60) {
                        enemy[j].fx -= steps[adjust];
                        enemy[j].spin -= steps[adjust];
                    }
                    if (enemy[j].fy < enemy[j].y * 40) {
                        enemy[j].fy += steps[adjust];
                        enemy[j].spin += steps[adjust];
                    }
                    if (enemy[j].fy > enemy[j].y * 40) {
                        enemy[j].fy -= steps[adjust];
                        enemy[j].fy -= steps[adjust];
                    }
                }
            }

            if ((enemy[i].fx == player.fx) && (enemy[i].fy == player.fy)) {
                lives--;
                if (lives == 0)
                    gameOver = true;
                resetObjects();
                QSound::play("sound/Die.wav");
            }
        }
    }

    if (filled) {
        QSound::play("sound/Complete.wav");
        stage++;
        if (stage > 3) {
            stage = 1;
            level++;
            level2++;
            if (level > 3) {
                level = 3;
                lives++;
                if (lives > 5)
                    lives = 5;
            }
        }

        resetObjects();
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                if (i < 10)
                    hline[i][j] = false;
                if (j < 10)
                    vline[i][j] = false;
            }
        }
    }

    if ((player.fx == hourglass.x * 60) && (player.fy == hourglass.y * 40) && (hourglass.fx == 1)) {
        QSound::play("sound/Freeze.wav");
        hourglass.fx = 2; // 表示吃到时间停止器
        hourglass.fy = 0;
    }

    player.spin += 0.5 * steps[adjust];
    if (player.spin > 360.0)
        player.spin -= 360.0;
    hourglass.spin -= 0.25 * steps[adjust];
    if (hourglass.spin < 0.0)
        hourglass.spin += 360.0;
    hourglass.fy += steps[adjust];

    srand(time(NULL));
    if ((hourglass.fx == 0) && (hourglass.fy > 6000 / level)) {
        QSound::play("sound/Hourglass.wav");
        hourglass.x = rand() % 10 + 1;
        hourglass.y = rand() % 11;
        hourglass.fx = 1;
        hourglass.fy = 0;
    }
    if ((hourglass.fx == 1) && (hourglass.fy > 6000 / level)) {
        hourglass.fx = 0;
        hourglass.fy = 0;
    }
    if ((hourglass.fx == 2) && (hourglass.fy > 500 + 500 * level)) {
        hourglass.fx = 0;
        hourglass.fy = 0;
    }
    delay++;
}

void GLWidget21::timeOut()
{
    updateGL();
}

void GLWidget21::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_A:
        antialiasing = !antialiasing;
        updateGL();
        break;
    case Qt::Key_Right:
        if ((player.x < 10) && (player.fx == player.x * 60) && (player.fy == player.y * 40)) {
            hline[player.x][player.y] = true;
            player.x++;
        }
        movePlayer();
        updateGL();
        break;
    case Qt::Key_Left:
        if ((player.x > 0) && (player.fx == player.x * 60) && (player.fy == player.y * 40)) {
            hline[player.x][player.y] = true;
            player.x--;
        }
        movePlayer();
        updateGL();
        break;
    case Qt::Key_Down:
        if ((player.y < 10) && (player.fx == player.x * 60) && (player.fy == player.y * 40)) {
            vline[player.x][player.y] = true;
            player.y++;
        }
        movePlayer();
        updateGL();
        break;
    case Qt::Key_Up:
        if ((player.y > 0) && (player.fx == player.x * 60) && (player.fy == player.y * 40)) {
            vline[player.x][player.y] = true;
            player.y--;
        }
        movePlayer();
        updateGL();
        break;
    case Qt::Key_Space:
        if (gameOver) {
            gameOver = false;
            filled = true;
            level = 1;
            level2 = 1;
            stage = 0;
            lives = 5;
        }
        updateGL();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}
