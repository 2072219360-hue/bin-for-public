#include "glwidget24.h"

GLWidget24::GLWidget24(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scroll = 0;
    maxTokens = 0;
    swidth = sheight = 0;
    base = 0;
    setWindowTitle(tr("TGA文件和扩展名显示"));
    QIcon icon(":/images/INV_Stone_05.jpg");
    setWindowIcon(icon);
    showMaximized();
}

bool GLWidget24::loadTGA(TextureImage *texture, const char *fileName)
{
    GLubyte TGAHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 无压缩的TGA文件头
    GLubyte TGACompare[12]; // 保存读入的文件头信息
    GLubyte header[6]; // 保存最有用的图像信息，宽、高、位深
    GLuint bytesPerPixel; // 记录每个颜色所占字节数
    GLuint imageSize; // 记录文件大小
    GLuint temp;
    GLuint type = GL_RGBA; // 设置默认的格式为GL_RGBA,即32为图像

    FILE *file = fopen(fileName, "rb");
    if (file == NULL || // 文件是否存在?
        fread(TGACompare, 1, sizeof(TGACompare), file) != sizeof(TGACompare) || // 是否包含12个字节的文件头?
        memcmp(TGAHeader, TGACompare, sizeof(TGAHeader)) != 0 || // 是否是需要的格式?
        fread(header, 1, sizeof(header), file) != sizeof(header)) {
        if (file == NULL)
            return false;
        else {
            fclose(file);
            return false;
        }
    }

    texture->width = header[1] * 256 + header[0];
    texture->height = header[3] * 256 + header[2];

    if (texture->width <= 0 || texture->height <= 0 || (header[4] != 24 && header[4] != 32)) {
        fclose(file);
        return false;
    }

    texture->bpp = header[4]; // 记录文件的位深
    bytesPerPixel = texture->bpp / 8; // 记录每个像素所占的字节数
    imageSize = texture->width * texture->height * bytesPerPixel; // 计算TGA文件所需要的内存大小
    texture->imageData = (GLubyte *)malloc(imageSize);

    if (texture->imageData == NULL || fread(texture->imageData, 1, imageSize, file) != imageSize) {
        if (texture->imageData != NULL)
            free(texture->imageData);
        fclose(file);
        return false;
    }

    for (GLuint i = 0; i < imageSize; i += bytesPerPixel) {
        temp = texture->imageData[i];
        texture->imageData[i] = texture->imageData[i + 2];
        texture->imageData[i + 2] = temp;
    }

    fclose(file);

    glGenTextures(1, &texture[0].texID);
    glBindTexture(GL_TEXTURE_2D, texture[0].texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (texture[0].bpp == 24)
        type = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

    return true;
}

void GLWidget24::buildFonts()
{
    base = glGenLists(256);
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);

    for (int i = 0; i < 256; i++) {
        float cx = (i % 16) / 16.0;
        float cy = (i / 16) / 16.0;

        glNewList(base + i, GL_COMPILE); {
            glBegin(GL_QUADS); {
                glTexCoord2f(cx, 1 - cy - 0.0625); glVertex2d(0, 16);
                glTexCoord2f(cx + 0.0625, 1 - cy - 0.0625); glVertex2d(16, 16);
                glTexCoord2f(cx + 0.0625, 1 - cy - 0.001); glVertex2d(16, 0);
                glTexCoord2f(cx, 1 - cy - 0.001); glVertex2d(0, 0);
            } glEnd();
            glTranslated(14, 0, 0);
        } glEndList();
    }
}

void GLWidget24::killFonts()
{
    glDeleteLists(base, 256);
}

void GLWidget24::glPrint(GLint x, GLint y, int set, const char *fmt...)
{
    char text[1024];
    va_list ap; // 指向第一个参数
    if (fmt == NULL)
        return;

    va_start(ap, fmt); // 开始分析参数，并把结果写入到text中
    vsprintf(text, fmt, ap);
    va_end(ap);

    if (set >= 1)
        set = 1;
    else
        set = 0;

    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslated(x, y, 0);
    glListBase(base - 32 + (128 * set));

    glScalef(1.0, 2.0, 1.0);

    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glDisable(GL_TEXTURE_2D);
}

void GLWidget24::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);

    if (!loadTGA(&textures[0], "data/Font.tga")) {
        QMessageBox::warning(NULL, "Warning", tr("无法打开文件Font.tga!"), QMessageBox::Ok);
        exit(-1);
    }

    buildFonts();
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget24::paintGL()
{
    char *token;
    int cnt = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.5, 0.5);
    glPrint(50, 16, 1, "Renderer");
    glPrint(80, 48, 1, "Vendor");
    glPrint(66, 80, 1, "Version");
    glColor3f(1.0, 0.7, 0.4);
    glPrint(200, 16, 1, (char *)glGetString(GL_RENDERER));
    glPrint(200, 48, 1, (char *)glGetString(GL_VENDOR));
    glPrint(200, 80, 1, (char *)glGetString(GL_VERSION));

    glColor3f(0.5, 0.5, 1.0);
    glPrint(192, 432, 1, "NeHe Productions");
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_STRIP); {
        glVertex2d(639, 417);
        glVertex2d(0, 417);
        glVertex2d(0, 480);
        glVertex2d(639, 480);
        glVertex2d(639, 128);
    } glEnd();
    glBegin(GL_LINE_STRIP); {
        glVertex2d(0, 128);
        glVertex2d(639, 128);
        glVertex2d(639, 1);
        glVertex2d(0, 1);
        glVertex2d(0, 417);
    } glEnd();
    glScissor(1, int(0.135416 * sheight), swidth - 2, int(0.597916 * sheight));
    glEnable(GL_SCISSOR_TEST);

    char *text = (char *)malloc(strlen((char *)glGetString(GL_EXTENSIONS)) + 1);
    strcpy(text, (char *)glGetString(GL_EXTENSIONS));
    token = strtok(text, " ");
    while (token != NULL) {
        cnt++;
        if (cnt > maxTokens)
            maxTokens = cnt;
        glColor3f(0.5, 1.0, 0.5);
        glPrint(0, 96 + (cnt * 32) - scroll, 0, "%i", cnt);

        glColor3f(1.0, 1.0, 0.5);
        glPrint(50, 96 + cnt * 32 - scroll, 0, token);

        token = strtok(NULL, " ");
    }
    glDisable(GL_SCISSOR_TEST);
    free(text);
}

void GLWidget24::resizeGL(int width, int height)
{
    swidth = width;
    sheight = height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 640.0, 480.0, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget24::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Up:
        if (scroll > 0)
            scroll -= 5;
        updateGL();
        break;
    case Qt::Key_Down:
        if (scroll < 32 * (maxTokens - 9))
            scroll += 5;
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

void GLWidget24::timeOut()
{
    updateGL();
}
