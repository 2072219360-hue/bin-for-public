#include "glwidget.h"
#include "gl/GLU.h"

GLWidget::GLWidget(int timerInterval, QWidget *parent)
    : QGLWidget(parent)
{
    if (timerInterval == 0) {
        m_timer = 0;
    } else {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        m_timer->start(timerInterval);
    }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        close();
    }
}

void GLWidget::timeOut()
{
}

void GLWidget::timeOutSlot()
{
    timeOut();
}
