#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(int timeInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void timeOut();

protected slots:
    virtual void timeOutSlot();

private:
    QTimer *m_timer;
};

#endif
