#include "gl/glew.h"
#include "qglw01nehewidget.h"


NeHe01Widget::NeHe01Widget( QWidget *parent, bool fs)
    : QGLWidget(parent)
{
  rTri = 0.0;
  rQuad = 0.0;

  fullscreen = fs;
  setGeometry( 100, 100, 640, 480 );
  setWindowTitle( "NeHe's OpenGL Framework" );

  if ( fullscreen )
    showFullScreen();
}

NeHe01Widget::~NeHe01Widget()
{
}

void NeHe01Widget::initializeGL()
{
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.0, 0.0, 0.0, 0.0 );
  glClearDepth( 1.0 );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void NeHe01Widget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  glTranslatef( -1.5,  0.0, -6.0 );
  glRotatef( rTri,  0.0,  1.0,  0.0 );

  glBegin( GL_TRIANGLES );
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.0,  1.0,  0.0 );
    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
  glEnd();

  glLoadIdentity();
  glTranslatef(  1.5,  0.0, -7.0 );
  glRotatef( rQuad,  1.0,  1.0,  1.0 );

  glBegin( GL_QUADS );
    glColor3f( 0.0, 1.0, 0.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glVertex3f(  1.0,  1.0,  1.0 );

    glColor3f( 1.0, 0.5, 0.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f(  1.0, -1.0, -1.0 );

    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );

    glColor3f( 1.0, 1.0, 0.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );

    glColor3f( 0.0, 0.0, 1.0 );
    glVertex3f( -1.0,  1.0,  1.0 );
    glVertex3f( -1.0,  1.0, -1.0 );
    glVertex3f( -1.0, -1.0, -1.0 );
    glVertex3f( -1.0, -1.0,  1.0 );

    glColor3f( 1.0, 0.0, 1.0 );
    glVertex3f(  1.0,  1.0, -1.0 );
    glVertex3f(  1.0,  1.0,  1.0 );
    glVertex3f(  1.0, -1.0,  1.0 );
    glVertex3f(  1.0, -1.0, -1.0 );
  glEnd();

  rTri += 0.2;
  rQuad -= 0.15;
}

void NeHe01Widget::resizeGL( int width, int height )
{
  if ( height == 0 )
  {
    height = 1;
  }
  glViewport( 0, 0, (GLint)width, (GLint)height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void NeHe01Widget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Qt::Key_F2:
    fullscreen = !fullscreen;
    if ( fullscreen )
    {
      showFullScreen();
    }
    else
    {
      showNormal();
      setGeometry( 0, 0, 640, 480 );
    }
    updateGL();
    break;
  case Qt::Key_Escape:
    close();
  }
}

NeHe01Dialog::NeHe01Dialog()
{
	nh=new NeHe01Widget();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(nh);
	this->setLayout(mainLayout);
}

