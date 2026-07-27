#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include "glwidget.h"

class GLWidget2 : public GLWidget
{         
public:
    GLWidget2(int timerInterval = 0, QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
};

#endif // GLWIDGET2_H
